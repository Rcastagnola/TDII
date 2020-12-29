/*
 * system.c
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */


#include "Drivers/system/system.h"
/*
void demoraUS(uint32_t demora){
	uint32_t core_ticks = dwt_read();
	core_ticks += demora * CORE_TICKS_POR_US;

	//Si produje un overflow cuando hice la suma espero que se produzca un overflow en el contador
	if(dwt_read() > core_ticks)
		while(dwt_read() > core_ticks);


	while(dwt_read() < core_ticks);

	return;
}
*/
void demoraUS(uint32_t microseconds)
{
 uint32_t clk_cycle_start = dwt_read();
 /* Go to number of cycles for system */
 microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
 /* Delay till end */
 while ((dwt_read() - clk_cycle_start) < microseconds);
}


//Demora en milisegundos
void delay_ms (uint32_t ms){
	HAL_Delay(ms);
}

void MONITOR_I_Start(void)
{
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
}

uint32_t MONITOR_I_Stop(void)
{
	uint32_t ret = TIM2->CNT;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	return ret;
}
