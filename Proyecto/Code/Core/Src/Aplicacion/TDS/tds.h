/*
 * tds.h
 *
 *  Created on: 29 sep. 2020
 *      Author: hanib
 */

#ifndef SRC_APLICACION_TDS_TDS_H_
#define SRC_APLICACION_TDS_TDS_H_
/*
#include "Drivers/i2c/i2c.h"
#include "Aplicacion/Teclado/teclado.h"
#include "Drivers/gpio/gpio.h"
#include "Drivers/system/system.h"
#include "Aplicacion/LCD/LCD.h"
#include "Aplicacion/memoria/memoria.h"
#include "Aplicacion/config.h"
#include "Aplicacion/IO/io.h"
*/
#include "main.h"
#include <stdint.h>

uint32_t ticks;

//Definiciones
#define SYSTEM_TICK_MS 1
#define MAX_LEN_TASK_LIST	(15)
#define dwt_init() 	{DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; DWT->CYCCNT=0;}
#define dwt_reset() {DWT->CYCCNT=0;}
#define dwt_read() 	(DWT->CYCCNT)

typedef struct
{
	int bcet;
	int wcet;
	int et;
	int et_wcet;
	int offset;
	int period;
	void (*task)(void *p);
	void *param;
} TaskStat;

//Variables Externas
extern tipo_configuracion configuracion;
extern uint32_t valor_promediado;

//Prototipos

void setup();
void loop();
void Tick_TDS();

void inicializar_despachador(	TaskStat *lista,
								uint32_t len,
								void (*start_timer)(void),
								uint32_t (*stop_timer)(void),
								void (*falla_sistema)(void));

int agregar_tarea(TaskStat *lista,
				  void (*tarea)(void *p),
				  void *param,
				  int offset,
				  int periodo,
				  int bcet,
				  int wcet);

void despachar_tareas(void);
void falla_sistema(void);
#endif /* SRC_APLICACION_TDS_TDS_H_ */
