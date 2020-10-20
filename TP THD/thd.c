#include "chip.h"
#include "microplanificador.h"
#include "dac.h"
#include <math.h>

#define TIC_1MS 		(1000)
#define TIC_SISTEMA 	(10000)
#define TIC_LED_MS		(1000)
#define LED_ROJO_PORT	(0)
#define LED_ROJO_PIN	(22)
#define ADC0_PORT		(0)
#define ADC0_PIN		(23)
#define DAC_PORT		(0)
#define DAC_PIN			(26)
#define DWTINIT()		{DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk; DWT->CYCCNT=0;}
#define DWTRESET()		{DWT->CYCCNT=0;}
#define DWTREAD()		(DWT->CYCCNT)

/*
 * Hay que definir las frecuencias del los cristales del sistema y del
 * RTC por eso no funciona.
 *
 */
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

uint16_t datoADC=0;
uint16_t datoDAC=0;
static uint16_t buffer[1024];

#define dwt_init() 	{DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; DWT->CYCCNT=0;}
#define dwt_reset() {DWT->CYCCNT=0;}
#define dwt_read() 	(DWT->CYCCNT)

#define DAC_MODO_DMA

#define DAC_FREQ 		(1000000)
#define len_senial		(1024)

extern const unsigned int test[];
extern const unsigned int bufsenoDAC[];

int FFTConst = 0;
int time1= 0;
int time2= 0;
int time3= 0;
int time4= 0;


int txsenial = 0;
static int ix=0;

void InitHardware(void);
void TareaTickLed(void);
void TareaWDT(void);
void TareaADC(void);
void TareaTHD(void);
void TareaDAC(void);
void fft_radix2(float *xr, float *xi, int len);


uint32_t et[4],wcet[4]={0,0,0,0};

float parteReal[len_senial];
float parteImag[len_senial];
float potenciaFFT[len_senial];

float potencia50hz = 0;
float potenciaTotal = 0;
float THD = 0;
float fs = 0;
uint16_t indice50hz=0;

int main(void)
{
	int i;
	int falla;

	InitHardware();

	while(1)
    {
		falla = 0;
		falla+=despacharTarea(TareaWDT, 0, 5, &et[0]);
		falla+=despacharTarea(TareaTickLed,0,5,&et[1]);
		falla+=despacharTarea(TareaADC,0,15,&et[2]);
		falla+=despacharTarea(TareaTHD,0,1500000,&et[3]);

        for(i=0;i<4;i++)
        	if(wcet[i]<et[i])wcet[i]=et[i];
    	if(falla)
    	{
    		/*Pongo al sistema en falla*/
    		for(;;)
    		{
    			for(i=0;i<150000;i++);
    			Chip_GPIO_SetPinToggle(LPC_GPIO,0,22);
    		}
    	}
    	__WFI();
    }
    return 0 ;
}

void TareaTickLed(void)
{
	static int espera = TIC_LED_MS;
	if(!--espera)
	{
		espera = TIC_LED_MS;
		Chip_GPIO_SetPinToggle(LPC_GPIO,LED_ROJO_PORT,LED_ROJO_PIN);
	}
}

void TareaADC(void)
{
	if(Chip_ADC_ReadStatus(LPC_ADC,0,ADC_DR_DONE_STAT))
	{
		Chip_ADC_ReadValue(LPC_ADC,0,&datoADC);
		static int init = 0;

			if(!init)
			{
				buffer[ix++]=datoADC;

				if(ix==len_senial)
				{
					ix=2000;
					init=1;
				}
				if(ix<len_senial)
				{
				Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);
				}
			}
	}
}


void TareaTHD(void)
{
	if (ix == 2000)
	{
	for(uint32_t i = 0; i<len_senial; i++)
	{
			parteReal[i] = buffer[i];
			parteImag[i] = 0;
	}

	fs = 1000;

	indice50hz = 50;

	fft_radix2(parteReal, parteImag, len_senial);

	for(uint32_t i = 0;i<len_senial/2;i++) potenciaFFT[i] = parteReal[i] * parteReal[i] +  parteImag[i] * parteImag[i];

	potencia50hz = potenciaFFT[indice50hz - 1] + potenciaFFT[indice50hz] + potenciaFFT[indice50hz+1];
	potencia50hz *= 2;

	potenciaTotal = 0;
		for(uint16_t i = 1;i<len_senial/2;i++) potenciaTotal += (potenciaFFT[i]*2);

		THD = potenciaTotal - potencia50hz;
		THD /= potencia50hz;

	}
}




void TareaWDT(void)
{
	Chip_WWDT_Feed(LPC_WWDT);
}

void InitHardware(void)
{
	DMALLIO dacconfig;

	/*Inicializo el Clock del sistema*/
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	/*Inicializo los perifericos de pines e IOCON*/
	Chip_GPIO_Init(LPC_GPIO);
	Chip_IOCON_Init(LPC_IOCON);

	/*Inicializo el led comun, led alerta, y 2 pulsadores */
	Chip_IOCON_PinMuxSet(LPC_IOCON,0,22,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,0,22);

	Chip_IOCON_PinMuxSet(LPC_IOCON,2,10,IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,2,10);

	Chip_IOCON_PinMuxSet(LPC_IOCON,2,12,IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,2,12);

	Chip_IOCON_PinMuxSet(LPC_IOCON,2,11,IOCON_FUNC0);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,2,11);

	Chip_GPIO_SetPinOutHigh(LPC_GPIO,0,22);
	Chip_GPIO_SetPinOutHigh(LPC_GPIO,2,10);

	/*Inicializo el ADC*/
	ADC_CLOCK_SETUP_T adc_setup;
	uint16_t dummy;
	Chip_IOCON_PinMuxSet(LPC_IOCON,ADC0_PORT,ADC0_PIN,IOCON_FUNC1);
	Chip_ADC_Init(LPC_ADC,&adc_setup);
	Chip_ADC_EnableChannel(LPC_ADC,ADC_CH0,ENABLE);
	Chip_ADC_ReadValue(LPC_ADC,ADC_CH0,&dummy);
	Chip_ADC_SetStartMode(LPC_ADC,ADC_START_NOW,ADC_TRIGGERMODE_RISING);

	/*Inicializo el DAC*/
	Chip_IOCON_PinMuxSet(LPC_IOCON,DAC_PORT,DAC_PIN,IOCON_FUNC2);
	Chip_DAC_Init(LPC_DAC);
	SetTableDAC((int*)bufsenoDAC, len_senial, DAC_FREQ, 0, &dacconfig);

	/*
	 * Arranco el WWDT en 1.1ms si está definida USAR_WWDT
	 */
#ifdef USAR_WDT
	uint32_t i = (4000000/1000000)*TIEMPO_WWDT_US/4;
	Chip_WWDT_SelClockSource(LPC_WWDT, WWDT_CLKSRC_IRC);
	Chip_WWDT_SetTimeOut(LPC_WWDT, i);
	Chip_WWDT_SetOption(LPC_WWDT, WWDT_WDMOD_WDRESET);
	Chip_WWDT_ClearStatusFlag(LPC_WWDT, WWDT_WDMOD_WDTOF | WWDT_WDMOD_WDINT);
	Chip_WWDT_Start(LPC_WWDT);
#endif
	dwt_init();


	/*Inicializo el systick*/
	SysTick_Config(SystemCoreClock/TIC_1MS);

	/*En tiempo de ejecución uso el timer 0 para medir tiempos de tarea*/
	MONITOR_I_Init();

	/*Apago el led rojo*/
	Chip_GPIO_SetPinOutLow(LPC_GPIO,2,10);


}

void SysTick_Handler(void)
{
}
