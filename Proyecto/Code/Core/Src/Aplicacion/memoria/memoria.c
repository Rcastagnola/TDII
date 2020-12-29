/*
 * memoria.c
 *
 *  Created on: 31 ago. 2020
 *      Author: hanib
 */
#include "Aplicacion/memoria/memoria.h"


uint8_t LeerByteMemoria(uint16_t address){

	//Borro los bits mas significativos ya que la memoria no los usa.
	address &= 0x0FFF;


	uint8_t lectura = LeerByteFromI2C_MEM(MEM_ADDR, address);

	return lectura;
}
void EscribirByteMemoria(uint16_t address, uint8_t dato){
	//Borro los bits mas significativos ya que la memoria no los usa.
	address &= 0x0FFF;

	EscribirByteToI2C_MEM(MEM_ADDR, address, dato);

}

void EscribirWordMemoria(uint16_t address, uint16_t dato){
	//Borro los bits mas significativos ya que la memoria no los usa.
	address &= 0x0FFF;

	EscribirBufferToI2C_MEM(MEM_ADDR, address, (uint8_t*)&dato, sizeof(uint16_t));
}


void VolcadoAMemoria(uint8_t* pData, uint8_t size){

	//Vuelco la configuracion a memoria
	// limito la escritura a PAGE_SIZE ya que la memoria no puede escribir mas que eso es una sola trama

	if(size > PAGE_SIZE) size = PAGE_SIZE;

	EscribirBufferToI2C_MEM(MEM_ADDR, CONFIG_START_ADDRESS, pData, size);
}


//et = 425 us
void CargarConfiguracionFromMemoria(){
	// Este metodo lee la configuracion desde la eeprom y la carga en la estructura de configuraciones.

	memset(&configuracion, 0, sizeof(configuracion));

	LeerBufferFromI2C_MEM(MEM_ADDR, CONFIG_START_ADDRESS, (uint8_t*)&configuracion, sizeof(configuracion));
	return;
}


void Inicializar_Configuracion(){
	// Este metodo inicializa la memoria con la configuracion por defecto,
	// Es necesario ejecutar este metodo al menos una vez para inicializar la memoria.



	//Los 3 digitos menos significativo son decimales
	//Rango maximo de operacion del sensor 0 a 10 bar
	configuracion.alarma_alta_presion = 9000;
	configuracion.alarma_baja_presion = 5000;
	configuracion.compresor_alta_presion = 8000;
	configuracion.compresor_baja_presion = 6000;

	//Valores del ADC maximo y minimo donde el sensor esta trabajando correctamente
	//Rango del sensor 4 a 20 mA
	configuracion.sensor_maximo_adc = 21000;
	configuracion.sensor_minimo_adc = 3900;


	//Seteo los flags en cero
	configuracion.flags = 0;
	//Y luego activo la alarma y el lazo del sensor
	configuracion.flags |= estado_alarma;
	configuracion.flags |= estado_lazo_sensor;

	VolcadoAMemoria((uint8_t*)&configuracion, sizeof(configuracion));
	delay_ms(50);
}

//et = 123 us
void Actualizar_EEPROM(uint8_t data_offset){
	uint8_t* pData = (uint8_t*)&configuracion;

	EscribirBufferToI2C_MEM(MEM_ADDR, CONFIG_START_ADDRESS + data_offset, &pData[data_offset], sizeof(uint16_t));
}
