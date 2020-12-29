/*
 * config.h
 *
 *  Created on: 31 ago. 2020
 *      Author: hanib
 */

#ifndef SRC_APLICACION_CONFIG_H_
#define SRC_APLICACION_CONFIG_H_


typedef struct{
	//Configuracion de alarma
	uint16_t alarma_baja_presion;
	uint16_t alarma_alta_presion;
	//Configuracion del compresor
	uint16_t compresor_baja_presion;
	uint16_t compresor_alta_presion;
	//Configuracion para controlar el lazo del sensor
	uint16_t sensor_minimo_adc;
	uint16_t sensor_maximo_adc;
	//flags
	uint16_t  flags;

} tipo_configuracion;


tipo_configuracion configuracion;

//significado de los flags
#define estado_alarma 0x01
#define estado_lazo_sensor 0x02

#define OFFSET_ALARMA_BAJA 0
#define OFFSET_ALARMA_ALTA 2
#define OFFSET_COMPRESOR_BAJA 4
#define OFFSET_COMPRESOR_ALTA 6
#define OFFSET_SENSOR_MINIMO 8
#define OFFSET_SENSOR_MAXIMO 10
#define OFFSET_FLAGS 12




#endif /* SRC_APLICACION_CONFIG_H_ */
