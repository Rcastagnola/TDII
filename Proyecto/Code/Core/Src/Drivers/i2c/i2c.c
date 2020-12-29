/*
 * i2c.c
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */

#include "Drivers/i2c/i2c.h"

uint8_t LeerByteFromI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address){
	//Lee un byte desde el i2c.
	//I2C_Address tiene la direccion del dispositivo i2c
	//MEM_Address tiene la direccion de memoria que tiene que leer del dispositivo
	//Devuelve un byte con el dato leido

	uint8_t pData=0;
	HAL_I2C_Mem_Read(&hi2c1, I2C_Address | 0x1, MEM_Address, sizeof(MEM_Address), &pData, sizeof(uint8_t), I2C_TIMEOUT);
	return pData;
}

void EscribirByteToI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address, uint8_t dato){
	//Escribe un byte en la memoria I2C
	// I2C_Address contiene la direccion i2c de la memoria
	// MEM_Address contiene la direccion de memoria donde voy a escribir
	// dato contiene el dato que voy a escribir

	HAL_I2C_Mem_Write(&hi2c1, I2C_Address, MEM_Address, sizeof(MEM_Address), &dato, sizeof(uint8_t), I2C_TIMEOUT);
	return;
}

void EscribirBufferToI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address, uint8_t* buffer, uint8_t buff_len){
	// Escribe un buffer a una direccion de memoria especifica
	// I2C_Address contiene la direccion i2c de la memoria
	// MEM_Address contiene la direccion de memoria donde voy a empezar a escribir
	// buffer contiene un puntero al buufer a escribir
	// buff_Len especifica el largo del buffer en bytes
	HAL_I2C_Mem_Write(&hi2c1, I2C_Address, MEM_Address, sizeof(MEM_Address), buffer, buff_len, I2C_TIMEOUT);

	return;
}

void LeerBufferFromI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address, uint8_t* buffer, uint8_t buff_len){
	// Lee un buffer de una direccion de memoria especifica
	// I2C_Address contiene la direccion i2c de la memoria
	// MEM_Address contiene la direccion de memoria donde voy a empezar a leer
	// buffer contiene un puntero al buffer para leer, se debe preveer tener la suficiente memoria alocada
	// buff_Len especifica la cantidad de bytes a leer
	HAL_I2C_Mem_Read(&hi2c1, I2C_Address | 0x1, MEM_Address, sizeof(MEM_Address), buffer, buff_len, I2C_TIMEOUT);

	return;
}

uint8_t LeerByteFromI2C(uint8_t I2C_Address){
	//Lee un byte desde el i2c.
	//I2C_Address tiene la direccion del dispositivo i2c
	//Devuelve un byte con el dato leido

	uint8_t pData=0;
	HAL_I2C_Master_Receive(&hi2c1, I2C_Address | 0x1, &pData, sizeof(uint8_t), I2C_TIMEOUT);
	return pData;
}



uint16_t LeerWordFromI2C(uint8_t I2C_Address){
	//Lee un word desde el i2c.
	//I2C_Address tiene la direccion del dispositivo i2c
	//Devuelve un byte con el dato leido

	uint16_t pData=0;
	uint8_t buff[2];
	buff[0] = 0;
	buff[1] = 0;

	HAL_I2C_Master_Receive(&hi2c1, I2C_Address, buff, sizeof(uint16_t), I2C_TIMEOUT);

	pData = (buff[0] << 8) | buff[1];
	return pData;
}

void EscribirByteToI2c(uint8_t I2C_Address, uint8_t dato){
	//Escribe un byte en el bus I2C
	//I2C_Address tiene la direccion del dispositivo i2c

	HAL_I2C_Master_Transmit(&hi2c1, I2C_Address | 0x1, &dato, sizeof(uint8_t), I2C_TIMEOUT);
	return;

}

void EscribirBufferToI2C(uint8_t I2C_Address, uint8_t* buff, uint8_t buff_len){
	//Escribo en el bus I2C el contenido del buffer. Se debe especificar la cantidad de bytes del buffer en el parametro buff_len
	//I2C_Address representa la direccion del dispositivo a escribir, No es necesario setear el flag de escritura

	HAL_I2C_Master_Transmit(&hi2c1, I2C_Address | 0x1, buff, buff_len, I2C_TIMEOUT);
	return;
}
