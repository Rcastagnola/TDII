/*
 * io.h
 *
 *  Created on: 2 sep. 2020
 *      Author: hanib
 */

#ifndef SRC_APLICACION_IO_IO_H_
#define SRC_APLICACION_IO_IO_H_

#include "main.h"
#include "Drivers/gpio/gpio.h"

#define PuertoSalidas PuertoA


#define PIN_COMPRESOR Pin1
#define PIN_DESCARGA Pin0
#define PIN_ALARMA Pin2
#define PIN_BUZZER Pin3

//WCET 1us
//BCET <1us
void CambiarEstadoAlarma(uint8_t nuevoEstado);
void CambiarEstadoCompresor(uint8_t nuevoEstado);
void CambiarEstadoDescarga(uint8_t nuevoEstado);

//WCET 3us
//BCET 1us
void tarea_actualizar_salidas();

enum{
	inactivo = 0,
	activo = 1
};


#endif /* SRC_APLICACION_IO_IO_H_ */
