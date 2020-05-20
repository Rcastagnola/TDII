#include "../Inc/main.h"
#include "../Inc/driverled.h"
#include "../Inc/tasks.h"

		.syntax		unified
	    .arch       armv7-m
		.cpu		cortex-m0
		.thumb
		.equ		tasksTablePartSize, 0x04	//Todos los punteros miden 4 bytes en Cortex-M
		.extern		task1Init, task1Update
		.extern		task2Init, task2Update

		.section	.bss
		.align 	4
		.comm	mainLoopCnt,4
		.comm	taskCnt,4
		.comm   Estado,2
		.section	.text
		.align		2
 		.global		main
		.type		main, %function



main:
		PUSH {LR}
		MOVS R0, #LEDoFF
		LDR R1, =Estado
		STR R0, [R1]

		BL setup

mainLoop:
		LDR R1, =Estado
		LDR R0, [R1]
		CMP R0, #LEDoFF
		BEQ LEDON


LEDOFF:
		MOVS R0, #LEDoFF
		LDR R1, =Estado
		STR R0, [R1]
		LDR R1, =LEDoFFdELAY

		B NEXT

LEDON:
		MOVS R0, #LEDoN
		LDR R1, =Estado
		STR R0, [R1]
		LDR R1, =LEDoNdELAY



NEXT:
		BLX task1Update
		B mainLoop



mainLoopEnd:
		POP {PC}

		/************************************/
		/*	Función de Inicialización de las*/
		/*	tareas.							*/
		/************************************/
		.global	setup
		.type	setup, %function
setup:
		PUSH	{LR}
		BL		task1Init
		BL		task2Init
		POP		{PC}


		/************************************/
		/*	Tabla	que tiene los punteros a*/
		/*	todas las funciones update de	*/
		/*	las tareas						*/
		/************************************/
		.global	tasksTable
		.type	tasksTable, %object
tasksTable:
		.word	task1Update				// task1Update();
		.word	task2Update				// task1Update();














