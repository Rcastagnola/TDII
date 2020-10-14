#include "chip.h"

void MONITOR_I_Init(void)
{

	Chip_TIMER_Init(LPC_TIMER0);
	Chip_Clock_SetPCLKDiv(SYSCTL_PCLK_TIMER0,SYSCTL_CLKDIV_1);
	Chip_TIMER_PrescaleSet(LPC_TIMER0,(SystemCoreClock/1000000)-1);
}

void MONITOR_I_Start(void)
{
	Chip_TIMER_Reset(LPC_TIMER0);
	Chip_TIMER_Enable(LPC_TIMER0);
}

uint32_t MONITOR_I_Stop(void)
{
	Chip_TIMER_Disable(LPC_TIMER0);
	return Chip_TIMER_ReadCount(LPC_TIMER0);
}

int despacharTarea(void (*Tarea)(void), uint32_t bcet, uint32_t wcet, uint32_t *et)
{
	int ret = 0;
	int valor_us;
	MONITOR_I_Start();
	Tarea();
	valor_us = MONITOR_I_Stop();
	if(et!=NULL) *et=valor_us;
	if(valor_us<bcet || valor_us>wcet) ret--;
	return ret;
}
