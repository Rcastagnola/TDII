/*
 * i2c.h
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */

#ifndef SRC_DRIVERS_I2C_I2C_H_
#define SRC_DRIVERS_I2C_I2C_H_

#include "main.h"

extern I2C_HandleTypeDef hi2c1;

//I2C Timeout en milisegundos
#define I2C_TIMEOUT 2

//Prototipos
uint8_t LeerByteFromI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address);
void EscribirByteToI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address, uint8_t dato);
void EscribirBufferToI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address, uint8_t* buffer, uint8_t buff_len);
void LeerBufferFromI2C_MEM(uint8_t I2C_Address, uint16_t MEM_Address, uint8_t* buffer, uint8_t buff_len);

uint8_t LeerByteFromI2C(uint8_t I2C_Address);
uint16_t LeerWordFromI2C(uint8_t I2C_Address);


void EscribirByteToI2c(uint8_t I2C_Address, uint8_t dato);
void EscribirBufferToI2C(uint8_t I2C_Address, uint8_t* buff, uint8_t buff_len);



#endif /* SRC_DRIVERS_I2C_I2C_H_ */
