#include "../Inc/driverled.h"

		.syntax		unified
		.cpu		cortex-m0
		.thumb
		.section	.text
		.align		2


		/************************************/
		/*	Función de Inicializa el LED de */
		/*	la bluepill.					*/
		/************************************/
		.global	ledInit
		.type	ledInit, %function
ledInit:
		// Set P0.22 as a GPIO pin => "LED2" on LPC1769 board
		// P0.22 is controlled by bits 13:12 of PINSEL1
		// xxxx xxxx xxxx xxxx xx11 xxxx xxxx xxxx
		//    0    0    0    0    3    0    0    0
		PUSH	{R0, R1, R2, LR}
		LDR		R2, =PINSEL1
        LDR		R0, [R2]
		LDR		R1, =0x00003000
		BICS	R0, R0, R1
        STR		R0, [R2]
        LDR		R2, =FIO0DIR
        LDR		R0, =LED_MASK
        STR		R0, [R2]
		POP		{R0, R1, R2, PC}


		/************************************/
		/*	Función de escribe el LED de la */
		/*	Placa.							*/
		/*	Recibe por R0 el estado.		*/
		/*	(1	enciende)					*/
		/************************************/
		.global	ledWrite
		.type	ledWrite, %function
ledWrite:
		PUSH	{R1, R3, LR}
		LDR		R3, =FIO0SET
		CMP		R0, #LEDoN
		BEQ		ledEnd
        LDR		R3, =FIO0CLR
ledEnd:
        LDR		R1, =LED_MASK
		STR		R1, [R3]
		POP		{R1, R3, PC}
