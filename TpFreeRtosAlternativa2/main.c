/*Includes del FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include<stdio.h>
#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE

/*CMSIS*/
#include "LPC17xx.h"

/*Includes de la C-API*/
#include "GPIO.h"
#include "lpc_176X_PinNames.h"
#include "lpc_176X_PeriphNames.h"


/*Defino el LED y el pulsador de la placa*/
GPIO 	ledStick3;
GPIO 	ledStick1;
GPIO 	ledStick2;
GPIO    Pulsador;


#define 	TRUE  			1
#define 	FALSE  			0

#define BOTON_NO_PRESIONADO 0
#define BOTON_PRESIONADO    1

#define NO_OPRIMIDO			0
#define DEBOUNCE            1
#define VALIDAR             2
#define OPRIMIDO            3



int contador = 0;

portTickType xMeDesperte;



xTaskHandle led1 = NULL;
xTaskHandle led2 = NULL;
xTaskHandle led3 = NULL;

int A = 0;
int B = 0;
int C = 0;

int j = 0;

uint32_t random[20];

uint32_t binario[20];

xSemaphoreHandle mutex;

static void setupHardware(void)
{
	SystemInit();

	GPIO_Init(&ledStick1, SALIDA, ALTO, (int)GPIO__2_10);
	GPIO_Init(&ledStick2, SALIDA, ALTO, (int)GPIO__0_21);
	GPIO_Init(&ledStick3, SALIDA, ALTO, (int)GPIO__2_11);

	GPIO_Init(&Pulsador, ENTRADA, BAJO, (int)GPIO__2_12);

int i = 0;
	while(i<20)
	{

		static uint32_t x = 0x5555AAAA;
		static uint32_t y = 0x5555AAAA;
		static uint32_t z = 0x5555AAAA;
		static uint32_t w = 0x1494946;
		uint32_t tmp=(x^(x<<15));
		x=y;
		y=z;
		z=w;
		w = (w^(w>>21))^(tmp^(tmp>>4));
		random[i] = w;

		binario[i] = random[i] & 0x00000003;


	i++;

	}

}

void TareaLed1(void *pvParameters)
{

	while(1)
	{
				xSemaphoreTake(mutex,portMAX_DELAY);

				Activar(&ledStick1);
				Pasivar(&ledStick2);
				Pasivar(&ledStick3);

				xSemaphoreGive(mutex);

	}
}

void TareaLed2(void *pvParameters)
{

	while(1)
	{
				xSemaphoreTake(mutex,portMAX_DELAY);

				Activar(&ledStick2);
				Pasivar(&ledStick1);
				Pasivar(&ledStick3);

				xSemaphoreGive(mutex);

	}
}

void TareaLed3(void *pvParameters)
{

	while(1)
	{
				xSemaphoreTake(mutex,portMAX_DELAY);

				Activar(&ledStick3);
				Pasivar(&ledStick1);
				Pasivar(&ledStick2);

				xSemaphoreGive(mutex);

	}
}



void TareaPrior(void *pvParameters)
{
	GPIO *pin;
	unsigned int EstadoDebounce = NO_OPRIMIDO;

	xMeDesperte = xTaskGetTickCount();

	pin = (GPIO*)(pvParameters);




	while(1)
	{
		switch(EstadoDebounce)
		{
		case NO_OPRIMIDO:
			vTaskDelayUntil(&xMeDesperte,150/portTICK_RATE_MS);
			if(isActivo(pin))
				EstadoDebounce = DEBOUNCE;
			break;

		case DEBOUNCE:
			vTaskDelay(20/portTICK_RATE_MS);
				EstadoDebounce = VALIDAR;

		case VALIDAR:
			if(isActivo(pin))
			{

				EstadoDebounce = OPRIMIDO;

			}
			else
				EstadoDebounce = NO_OPRIMIDO;
			break;

		case OPRIMIDO:
			if(!isActivo(pin))
			{
				contador++;

								if (contador == 1)
												{
												vTaskPrioritySet( led1, tskIDLE_PRIORITY + 2 );
												vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
												vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );


												}
								if (contador == 2)
												{
												vTaskPrioritySet( led1, tskIDLE_PRIORITY + 1 );
												vTaskPrioritySet( led2, tskIDLE_PRIORITY + 2 );
												vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );


												}
								if (contador == 3)
												{
												vTaskPrioritySet( led1, tskIDLE_PRIORITY + 1 );
												vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
												vTaskPrioritySet( led3, tskIDLE_PRIORITY + 2 );


												}
								if (contador == 4)
												{
												if (j < 19)
												{
													switch(binario[j])
													{
														case 0:
															vTaskPrioritySet( led1, tskIDLE_PRIORITY + 2 );
															vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );

															vTaskDelay(500);

															vTaskPrioritySet( led1, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led2, tskIDLE_PRIORITY + 2 );
															vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );

															vTaskDelay(500);

															vTaskPrioritySet( led1, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led3, tskIDLE_PRIORITY + 2 );
															break;
														case 1:
															vTaskPrioritySet( led1, tskIDLE_PRIORITY + 2 );
															vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );
															break;
														case 2:
															vTaskPrioritySet( led1, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led2, tskIDLE_PRIORITY + 2 );
															vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );
															break;
														case 3:
															vTaskPrioritySet( led1, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
															vTaskPrioritySet( led3, tskIDLE_PRIORITY + 2 );
															break;
														default: break;
													}
													j++;
												}
												else
												{
													vTaskPrioritySet( led1, tskIDLE_PRIORITY + 2 );
													vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
													vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );
													j=0;
												}

												}
								if (contador == 5)
												{
												vTaskPrioritySet( led1, tskIDLE_PRIORITY + 1 );
												vTaskPrioritySet( led2, tskIDLE_PRIORITY + 1 );
												vTaskPrioritySet( led3, tskIDLE_PRIORITY + 1 );

												contador = 0;
												}

				A = uxTaskPriorityGet(led1);
				B = uxTaskPriorityGet(led2);
				C = uxTaskPriorityGet(led3);


				EstadoDebounce = NO_OPRIMIDO;
			}
			break;

		default: break;
		}

	}
}



int main(void)
{

	setupHardware();

	mutex = xSemaphoreCreateMutex();

	xTaskCreate(	TareaLed1,
					( signed portCHAR * ) "LED1",
					USERTASK_STACK_SIZE,
					NULL,
					tskIDLE_PRIORITY+1,
					&led1 );

	xTaskCreate(	TareaLed2,
						( signed portCHAR * ) "LED2",
						USERTASK_STACK_SIZE,
						NULL,
						tskIDLE_PRIORITY+1,
						&led2 );

	xTaskCreate(	TareaLed3,
						( signed portCHAR * ) "LED3",
						USERTASK_STACK_SIZE,
						NULL,
						tskIDLE_PRIORITY+1,
						&led3 );

	xTaskCreate(	TareaPrior,
						( signed portCHAR * ) "PRIOR",
						USERTASK_STACK_SIZE,
						(void*) &Pulsador,
						tskIDLE_PRIORITY+3,
						NULL );



	vTaskStartScheduler();

	return 1;
}




















