/*
 * gpio.h
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */

#ifndef SRC_DRIVERS_GPIO_GPIO_H_
#define SRC_DRIVERS_GPIO_GPIO_H_

#include "main.h"


#define PuertoA GPIOA
#define PuertoB GPIOB
#define PuertoC GPIOC

#define Pin0 GPIO_PIN_0
#define Pin1 GPIO_PIN_1
#define Pin2 GPIO_PIN_2
#define Pin3 GPIO_PIN_3
#define Pin4 GPIO_PIN_4
#define Pin5 GPIO_PIN_5
#define Pin6 GPIO_PIN_6
#define Pin7 GPIO_PIN_7
#define Pin8 GPIO_PIN_8
#define Pin9 GPIO_PIN_9
#define Pin10 GPIO_PIN_10
#define Pin11 GPIO_PIN_11
#define Pin12 GPIO_PIN_12
#define Pin13 GPIO_PIN_13
#define Pin14 GPIO_PIN_14
#define Pin15 GPIO_PIN_15


#define PIN_CLEAR GPIO_PIN_RESET
#define PIN_SET GPIO_PIN_SET

//Prototipos de funciones
uint8_t LeerPin(GPIO_TypeDef* Puerto, uint16_t Pin);
void EscribirPin(GPIO_TypeDef* Puerto, uint16_t Pin, GPIO_PinState Estado);
void CambiarPin(GPIO_TypeDef* Puerto, uint16_t Pin);



#endif /* SRC_DRIVERS_GPIO_GPIO_H_ */
