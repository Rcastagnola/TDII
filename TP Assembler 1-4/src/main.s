#include "../Inc/main.h"

		.syntax		unified
	    .arch       armv7-m
		.cpu		cortex-m0
		.thumb
		.equ		tasksTablePartSize, 0x04	//Todos los punteros miden 4 bytes en Cortex-M
		.extern		task1Init, task1Update
		.extern		task2Init, task2Update

		/************************************/
		/*	La sección .bss está definida en*/
		/*	ram, por lo que se están 		*/
		/*	generando variables				*/
		/************************************/
		.section	.bss
		.align 	4
		.comm	mainLoopCnt,4
		.comm	taskCnt,4


		/************************************/
		/*	La sección .text está definida 	*/
		/*	en flash. Acá comienza el código*/
		/*	de programa						*/
		/************************************/
		.section	.text
		.align		2
 		.global		main
		.type		main, %function

		/************************************/
		/*	La sección .text está definida 	*/
		/*	en flash. Acá comienza el código*/
		/*	de programa.					*/
		/*									*/
		/*	El programa es un sistema 		*/
		/*	cooperativo tipo RTC (run to 	*/
		/*	completion) que está separado	*/
		/*	en dos rutinas: setup y loop	*/
		/*	setup corre al inicio y nunca 	*/
		/*	más (inicializa) y loop corre	*/
		/*	luego de setup cíclicamente 	*/
		/*	hasta que el sistema se apague	*/
		/*	las tareas tienen que tener dos	*/
		/*	partes, init (corre en setup) y */
		/*	update (corre en loop)			*/
		/************************************/
main:
		PUSH	{LR}
    	LDR		R2, =taskCnt			//
		LDR		R1, =mainLoopCnt		//
		MOVS	R0, #0					//
		STR		R0, [R1]      			//Inicializa la tarea actual y la
		STR		R0, [R2]      			//cantidad de tareas en cero
		BL		setup					//Corre la inicialización de las tareas


mainLoop:
		LDR		R1, =taskCnt			//Compara que la tarea actual no
  		LDR		R0, [R1]				//sea mayor a la cantidad de tareas
		CMP		R0, #tasksTableParts	//en la tabla. Sino se va a error
		BHI		taskCntError			//R0>tasksTablePArts

		MOVS	R2, #tasksTablePartSize	// Toma el número de la tarea actual
		MULS	R0, R0, R2				// (taskCnt) y lo multiplica por el
										// tamaño del puntero, va a R0

		LDR		R1, =tasksTable			// Lee el inicio de la tabla de updates
		LDR		R2, [R1, R0]			// Pone en R2 la tarea a ejecutar
		BLX		R2						// Salta al contenido de R2 actualizando LR

		LDR		R1, =taskCnt			//
  		LDR		R0, [R1]				//
		ADDS	R0, R0, #1				//
		STR		R0, [R1]    			//
		CMP		R0, #tasksTableParts	//
		BLT		mainLoopContinue		//
		MOVS	R0, #0					//
		STR		R0, [R1]      			// Actualiza la tarea actual (taskCnt)

mainLoopContinue:
		LDR		R1, =mainLoopCnt		//Incrementa mainLoopCnt,
  		LDR		R0, [R1]				//mainLoopCnt sólo cuenta la cantidad
		ADDS	R0, R0, #1				//de veces que pasó por el lazo mainLoop
		STR		R0, [R1]      			//
		B		mainLoop				// Continue forever

mainLoopEnd:							//
		POP		{PC}					// Nunca tendría que pasar por acá
taskCntError:							// si llegase a suceder arranca de nuevo (ver
		B		taskCntError			// línea 50)


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
tasksTableEnd:
		.equ	tasksTableSize, 	(tasksTableEnd - tasksTable)
		.equ	tasksTableParts, 	(tasksTableSize/tasksTablePartSize)
		.end
