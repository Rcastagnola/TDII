/*
 * tds.c
 *
 *  Created on: 29 sep. 2020
 *      Author: hanib
 */

#include "Aplicacion/TDS/tds.h"
#include <string.h>

//uint32_t wcet = 0;
//uint32_t bcet = 0;
//uint32_t core_ticks =0;
uint32_t contador = 0;
uint32_t tics_despachador;
uint32_t wcet_todo = 0;
uint32_t ticks;
/*
 * Punteros a función de todo lo que es sistema dependiente
 * o que el planificador no puede decidir.
 */
static void (*fallar_sistema)(void);
static void (*monitor_start)(void);
static uint32_t (*monitor_stop)(void);

/*
 * Variables para gestionar el planificador
 */
static uint32_t max_len_lista_tareas;
static uint32_t tareas_a_despachar;
static TaskStat *lista_tareas;
/*
 * Lista de tareas del planificador.
 */
TaskStat lista_principal[MAX_LEN_TASK_LIST];

//Esta funcion se va a llamar una vez al comienzo de la ejecucion del programa
//Y se va a utilizar para inicializar los dispositivos, cargar la configuracion e inicializar el planificador
//Se debe agregar una llamada a esta funcion dentro del main.c en cada arquitectura
void setup(){

	dwt_init();

	//**************** CARGAR CONFIGURACIONES DE FABRICA *****************///
	// Descomentar la siguiente linea para restarurar las configuraciones de fabrica

	//Inicializar_Configuracion();
	//**************** CARGAR CONFIGURACIONES DE FABRICA *****************///

	CargarConfiguracionFromMemoria();

	inicializar_teclado();

	LCD_init();
	CambiarEstadoAlarma(inactivo);


	ADC_init();
	ticks = 0;
	PantallaIndicacion(0);

	//************** INICIALIZACION DE PLANIFICADOR **************************///

 	inicializar_despachador(lista_principal,
		MAX_LEN_TASK_LIST, MONITOR_I_Start, MONITOR_I_Stop, falla_sistema);

	agregar_tarea(lista_principal, tarea_adc, NULL, 0, 1, 0, 100);
	agregar_tarea(lista_principal, tarea_promediar, NULL, 0, 1, 0, 50);
	agregar_tarea(lista_principal, tarea_actualizar_salidas, NULL, 0, 1, 0, 10);
	agregar_tarea(lista_principal, tarea_leer_teclado, NULL, 0, 1, 0, 60);
	agregar_tarea(lista_principal, Tarea_actualizar_lcd, NULL, 0, 1, 0, 100);
	agregar_tarea(lista_principal, tarea_sistema, NULL, 0, 1, 0, 150);
	agregar_tarea(lista_principal, tarea_control_presion, NULL, 0, 1, 0, 20);
}

//Este funcion hay que llamarla dentro del while 1 del main.c
//La variable ticks hay que incrementarla en el SYSTICK cada 1 ms
void loop(){

	dwt_reset()
	if (ticks>=SYSTEM_TICK_MS)
	{
		ticks=0;
		despachar_tareas();
	}
	tics_despachador = dwt_read();
	if (tics_despachador > wcet_todo)
		wcet_todo = tics_despachador;
}


/****************** FUNCIONES DEL PLANIFICADOR ************************/
void inicializar_despachador(TaskStat *lista, uint32_t len,
		void (*start_timer)(void), uint32_t (*stop_timer)(void),
		void (*falla_sistema)(void))
{
	uint32_t i;
	monitor_start = start_timer;
	monitor_stop = stop_timer;
	fallar_sistema = falla_sistema;
	tareas_a_despachar = 0;
	max_len_lista_tareas = len;
	lista_tareas = lista;
	for (i = 0; i < len; i++)
		memset(&lista[i], 0, sizeof(TaskStat));
}

int agregar_tarea(TaskStat *lista, void (*tarea)(void *p), void *param,
		int offset, int periodo, int bcet, int wcet)
{
	// Chequeos
	if ((periodo == 0) || (tarea == NULL)
			|| tareas_a_despachar == max_len_lista_tareas)
		return -1;

	//Punteros de la tarea.
	lista[tareas_a_despachar].task = tarea;
	lista[tareas_a_despachar].param = param;

	//Periodo y offset
	lista[tareas_a_despachar].period = periodo;
	lista[tareas_a_despachar].offset = offset;

	//Cargo los tiempos.
	lista[tareas_a_despachar].et = 0;
	lista[tareas_a_despachar].bcet = bcet;
	lista[tareas_a_despachar].wcet = wcet;
	tareas_a_despachar++;
	return 0;
}

int despachar_tarea(TaskStat *estado)
{
	int ret = 0;
	int valor_us;
	if (!estado->offset)
	{
		estado->offset = estado->period - 1;
		monitor_start();
		estado->task(estado->param);
		valor_us = monitor_stop();
		estado->et = valor_us;
		if (valor_us < estado->bcet || valor_us > estado->wcet)
			ret--;
		if (estado->et_wcet < estado->et)
			estado->et_wcet = estado->et;
	}
	else
	{
		estado->offset--;
	}
	return ret;
}

void despachar_tareas(void)
{
	uint32_t i;
	int fallar = 0;
	for (i = 0; i < tareas_a_despachar; i++)
	{
		fallar = despachar_tarea(&lista_tareas[i]);
		if (fallar)
		{
			break;
		}
	}
	if (fallar)
	{
		fallar_sistema();
	}
}

void falla_sistema(void)
{
	while(1){
		for(uint32_t i = 0;i<25000;i++);
		CambiarPin(PuertoC, Pin13);
	}
}
/***************** FUNCIONES DEL MONITOR ****************/

//void MONITOR_I_Init(void)
//{
//	Chip_TIMER_Init(LPC_TIMER0);
//	Chip_Clock_SetPCLKDiv(SYSCTL_PCLK_TIMER0,SYSCTL_CLKDIV_1);
//	Chip_TIMER_PrescaleSet(LPC_TIMER0,(SystemCoreClock/1000000)-1);
//}
//
//void MONITOR_I_Start(void)
//{
//	Chip_TIMER_Reset(LPC_TIMER0);
//	Chip_TIMER_Enable(LPC_TIMER0);
//}
//
//uint32_t MONITOR_I_Stop(void)
//{
//	Chip_TIMER_Disable(LPC_TIMER0);
//	return Chip_TIMER_ReadCount(LPC_TIMER0);
//	return 0;
//}
