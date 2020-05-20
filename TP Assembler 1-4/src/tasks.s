#include "../Inc/main.h"
#include "../Inc/tasks.h"
#include "../Inc/driverled.h"

		.syntax		unified
		.cpu		cortex-m0
		.thumb
		.extern 	ledInit, ledWrite
		.section	.text
		.align		2


		/************************************/
		/*	Defino las funciones init y 	*/
		/*	update de la tarea 1.			*/
		/************************************/
		.global	task1Init
		.type	task1Init, %function
task1Init:
		PUSH	{LR}
		BL		ledInit
		MOVS	R0, #LEDoN
		BL		ledWrite
		POP		{PC}

		.global	task1Update
		.type	task1Update, %function
task1Update:
		PUSH	{LR}
		MOVS	R0, #LEDoN
		BL		ledWrite
		LDR		R0, =LEDoNdELAY
		BL		delay
		POP		{PC}


		/************************************/
		/*	Defino las funciones init y 	*/
		/*	update de la tarea 2.			*/
		/************************************/
		.global	task2Init
		.type	task2Init, %function
task2Init:
		PUSH	{LR}
		BL		ledInit
		MOVS	R0, #LEDoFF
		BL		ledWrite
		POP		{PC}

		.global	task2Update
		.type	task2Update, %function
task2Update:
		PUSH	{LR}
		MOVS	R0, #LEDoFF
		BL		ledWrite
        LDR		R0, =LEDoFFdELAY
		BL		delay
		POP		{PC}

		/************************************/
		/*	Funcion que hace una demora 	*/
		/*	en funcion del valor recibido	*/
		/*	en R0 (en ciclos)				*/
		/************************************/
		.type	delay, %function
delay:
        SUBS	R0, 1			// R0 <- R0 - 1
        BNE		delay			// Jump if (R0 != 0)
		BX		LR				// Return
		.end
