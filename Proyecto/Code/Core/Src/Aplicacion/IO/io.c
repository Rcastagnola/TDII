/*
 * io.c
 *
 *  Created on: 2 sep. 2020
 *      Author: hanib
 */


#include "io.h"

uint8_t estadoAlarma = inactivo;
uint8_t estadoAlarmaNuevo = inactivo;

uint8_t estadoCompresor = inactivo;
uint8_t estadoCompresorNuevo = inactivo;

uint8_t estadoDescarga = inactivo;
uint8_t estadoDescargaNuevo = inactivo;


void CambiarEstadoAlarma(uint8_t nuevoEstado){
	estadoAlarmaNuevo = nuevoEstado;

	//Chequeo si la configuracion admite el uso de alarma sonora.. si no es asi mantengo el estado de la alarma inactivo
	if(!(configuracion.flags & estado_alarma)){
		estadoAlarmaNuevo = inactivo;
	}
}

void CambiarEstadoCompresor(uint8_t nuevoEstado){
	estadoCompresorNuevo = nuevoEstado;
}

void CambiarEstadoDescarga(uint8_t nuevoEstado){
	estadoDescargaNuevo = nuevoEstado;
}

void tarea_actualizar_salidas(){

	if(estadoAlarmaNuevo != estadoAlarma){
		estadoAlarma = estadoAlarmaNuevo;
		EscribirPin(PuertoSalidas, PIN_ALARMA, estadoAlarmaNuevo);
		EscribirPin(PuertoSalidas, PIN_BUZZER, estadoAlarmaNuevo);
	}

	if(estadoCompresorNuevo != estadoCompresor){
		estadoCompresor = estadoCompresorNuevo;
		EscribirPin(PuertoSalidas, PIN_COMPRESOR, estadoCompresorNuevo);
	}

	if(estadoDescargaNuevo != estadoDescarga){
		estadoDescarga = estadoDescargaNuevo;
		EscribirPin(PuertoSalidas, PIN_DESCARGA, estadoDescargaNuevo);
	}

	//Para hacer sonar el buzzer

	if (estadoAlarma == activo){
		CambiarPin(PuertoSalidas, PIN_BUZZER);
	}
}
