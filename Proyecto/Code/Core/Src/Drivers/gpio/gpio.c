/*
 * gpio.c
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */


#include "Drivers/gpio/gpio.h"

uint8_t LeerPin(GPIO_TypeDef* Puerto, uint16_t Pin){
	return HAL_GPIO_ReadPin(Puerto, Pin);
}

void EscribirPin(GPIO_TypeDef* Puerto, uint16_t Pin, GPIO_PinState Estado){
	HAL_GPIO_WritePin(Puerto, Pin, Estado);
}

void CambiarPin(GPIO_TypeDef* Puerto, uint16_t Pin){
	HAL_GPIO_TogglePin(Puerto, Pin);
}
