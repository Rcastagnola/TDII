/*
 * memoria.h
 *
 *  Created on: 31 ago. 2020
 *      Author: hanib
 */

#ifndef SRC_APLICACION_MEMORIA_MEMORIA_H_
#define SRC_APLICACION_MEMORIA_MEMORIA_H_

#include "main.h"
#include "Drivers/i2c/i2c.h"
#include "Aplicacion/config.h"

#define MEM_ADDR 0xAE

#define PAGE_SIZE 32 //in bytes
#define CONFIG_START_ADDRESS 0x00 //Direccion de memoria donde empieza la configuracion

#define BYTES_POR_PASO 2


extern tipo_configuracion configuracion;


//Prototipos
uint8_t LeerByteMemoria(uint16_t address);
void EscribirByteMemoria(uint16_t address, uint8_t dato);
void EscribirWordMemoria(uint16_t address, uint16_t dato);

void VolcadoAMemoria(uint8_t* pData, uint8_t size);
void CargarConfiguracionFromMemoria();
void Actualizar_EEPROM(uint8_t data_offset);

//Este metodo carga la configuracion de fabrica en la memoria
void Inicializar_Configuracion();


#endif /* SRC_APLICACION_MEMORIA_MEMORIA_H_ */
