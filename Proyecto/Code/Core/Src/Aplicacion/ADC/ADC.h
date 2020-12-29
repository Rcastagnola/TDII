/*
 * ADC.h
 *
 *  Created on: 30 sep. 2020
 *      Author: Luis
 */

#ifndef SRC_APLICACION_ADC_ADC_H_
#define SRC_APLICACION_ADC_ADC_H_


#include "main.h"
#include "string.h"
#include "stdio.h"

#define DIR_ADC			0x90
#define LEN_PROMEDIO		(50)

void ADC_init(void);
void tarea_adc(void *p);
void tarea_promediar(void *p);

uint8_t flag_datos_completos;
uint32_t valor_promediado;


#endif /* SRC_APLICACION_ADC_ADC_H_ */
