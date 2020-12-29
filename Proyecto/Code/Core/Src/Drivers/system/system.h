/*
 * system.h
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */

#ifndef SRC_DRIVERS_SYSTEM_SYSTEM_H_
#define SRC_DRIVERS_SYSTEM_SYSTEM_H_

#include "main.h"

#define dwt_init() 	{DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; DWT->CYCCNT=0;}
#define dwt_reset() {DWT->CYCCNT=0;}
#define dwt_read() 	(DWT->CYCCNT)

#define CORE_FREQ 72000000
#define CORE_TICKS_POR_US  0.000001 * CORE_FREQ


void demoraUS(uint32_t demora);
void delay_ms (uint32_t ms);

void MONITOR_I_Start(void);
uint32_t MONITOR_I_Stop(void);

#endif /* SRC_DRIVERS_SYSTEM_SYSTEM_H_ */
