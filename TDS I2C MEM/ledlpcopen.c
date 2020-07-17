#include "chip.h"
#include "microplanificador.h"


#define TIC_1MS 		(1000)
#define I2C_DEVICE		(I2C1)
#define I2C_DEVICE_NUM	(1)
#define I2C_DEVICE_PORT	(0)
#define I2C_SDA_PIN		(19)
#define I2C_SCL_PIN		(20)
#define I2C_SPEED		(400000)
#define TIC_ACEL		(5)

#define dwt_init() 	{DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; DWT->CYCCNT=0;}
#define dwt_reset() {DWT->CYCCNT=0;}
#define dwt_read() 	(DWT->CYCCNT)

uint8_t buf[3];

uint8_t ejemplo[1];

int devueltos = 0;
int leidos = 0;

#define 	TRUE  			1
#define 	FALSE  			0

#define NO_KEY 		  0
#define VALIDACION    20


uint8_t Contador = 1;

uint32_t permiso = 1;
uint32_t permisoB = 1;


uint32_t LedTime = 512;



void InitHardware(void);
void TareaLed(void);
void TeclaEvent(void);
void TeclaEscribirMemoria(void);
void tarea_wwdt(void);

uint32_t et[4],wcet[4]={0,0,0,0};

int main(void)
{
	int i;
	int falla;

	InitHardware();

	while(1)
    {
    	falla = 0;
    	falla+=despacharTarea(TeclaEvent,0,3, &et[0]);
    	falla+=despacharTarea(TareaLed,0,3, &et[1]);
    	falla+=despacharTarea(TeclaEscribirMemoria,0,110, &et[2]);
    	falla+=despacharTarea(tarea_wwdt,0,2, &et[3]);

        for(i=0;i<5;i++)
        	if(wcet[i]<et[i])wcet[i]=et[i];
    	if(falla)
    	{
    		/*Pongo al sistema en falla*/
    		for(;;)
    		{
    			for(i=0;i<150000;i++);
    			Chip_GPIO_SetPinToggle(LPC_GPIO,2,10);
    		}
    	}
    	__WFI();
    }
    return 0 ;
}

uint8_t tecla_actual = NO_KEY;
void TeclaEvent (void)
{

	/*Declaracion de Variables*/

	static uint8_t tecla_anterior = NO_KEY;
	static uint8_t cont = VALIDACION;

	/*Guardo la Tecla devuleta por Barrido_Teclado*/
	tecla_actual = Chip_GPIO_GetPinState(LPC_GPIO, 2, 12);



	/*Proceso la Tecla*/
	if( tecla_actual == NO_KEY )
	{
		permiso = 1;
		tecla_anterior = NO_KEY;
		return; //Pongo return aca para no seguir con la ejecucion
	}

	/*Si la tecla actual es IGUAL a la anterior*/
	if( tecla_actual == tecla_anterior )
	{
		cont--;		// Decremento el Contador
		/*Si el contador llego a 0 valido la tecla*/
		if( cont == 0 )
		{
			cont = VALIDACION;

			if(tecla_actual != NO_KEY)
				{
				if(permiso == 1)
					{
					permiso = 0;
					Contador++;
					}
				}

		}
		return;
	}
	/*Si la tecla actual es DISTINTA a la anterior*/
	else
	{
		tecla_anterior = tecla_actual;
		cont = VALIDACION;
		return;
	}
}


void TareaLed(void)
{
	if(!--LedTime)
	{
		switch(Contador)
		{
		case 1:
				LedTime = 512;
				break;
		case 2:
				LedTime = 256;
				break;
		case 3:
				LedTime = 128;
				break;
		case 4:
				LedTime = 64;
				break;
		case 5:
				LedTime = 512;
				Contador = 1;
				break;
		default:
				break;
		}

		Chip_GPIO_SetPinToggle(LPC_GPIO,0,22);

	}
}

uint8_t tecla_actualB = NO_KEY;
void TeclaEscribirMemoria(void)
{
	/*Declaracion de Variables*/

		static uint8_t tecla_anteriorB = NO_KEY;
		static uint8_t contB = VALIDACION;

		/*Guardo la Tecla devuleta por Barrido_Teclado*/
		tecla_actualB = Chip_GPIO_GetPinState(LPC_GPIO, 2, 11);



		/*Proceso la Tecla*/
		if( tecla_actualB == NO_KEY )
		{
			permisoB = 1;
			tecla_anteriorB = NO_KEY;
			return; //Pongo return aca para no seguir con la ejecucion
		}

		/*Si la tecla actual es IGUAL a la anterior*/
		if( tecla_actualB == tecla_anteriorB )
		{
			contB--;		// Decremento el Contador
			/*Si el contador llego a 0 valido la tecla*/
			if( contB == 0 )
			{
				contB = VALIDACION;

				if(tecla_actualB != NO_KEY)
					{
					if(permisoB == 1)
						{
						permisoB = 0;

						buf[0]= 0;
						buf[1]= 0;
						buf[2] = Contador;

						devueltos = Chip_I2C_MasterSend(I2C1,0x50,buf,3);
						}
					}

			}
			return;
		}
		/*Si la tecla actual es DISTINTA a la anterior*/
		else
		{
			tecla_anteriorB = tecla_actualB;
			contB = VALIDACION;
			return;
		}

}

void tarea_wwdt(void)
{
	Chip_WWDT_Feed(LPC_WWDT);
}

void InitHardware(void)
{

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

	/*Inicializo el I2C*/
	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SDA_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_PinMux(LPC_IOCON, I2C_DEVICE_PORT, I2C_SCL_PIN, IOCON_MODE_INACT, IOCON_FUNC3);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SDA_PIN);
	Chip_IOCON_EnableOD(LPC_IOCON,I2C_DEVICE_PORT, I2C_SDA_PIN);
	Chip_I2C_Init(I2C_DEVICE_NUM);
	Chip_I2C_SetClockRate(I2C_DEVICE_NUM,I2C_SPEED);
	Chip_I2C_SetMasterEventHandler(I2C_DEVICE,Chip_I2C_EventHandlerPolling);

	/*Inicializo la memoria*/
	buf[0]= 0;
	buf[1]= 0;

	Chip_I2C_MasterSend(I2C1,0x50,buf,2);
	leidos = Chip_I2C_MasterRead(I2C1,0x50,ejemplo,1);

	if (ejemplo[0] >= 5) Contador = 1;
	else Contador = ejemplo[0];


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
