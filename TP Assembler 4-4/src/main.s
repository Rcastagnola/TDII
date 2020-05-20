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
		.comm 	estructura,20
		.comm   vector, 12
		.comm   vector8,6

		/************************************/
		/*	La sección .text está definida 	*/
		/*	en flash. Acá comienza el código*/
		/*	de programa						*/
		/************************************/
		.section	.text
		.align		2
 		.global		main
		.type		main, %function


main:
		PUSH	{LR}
   		BL		setup

CargoVector8:

        MOVS		R1, #12
        MOVS		R2, #6
        MOVS    	R3, #7
        MOVS		R4, #12
        MOVS		R5, #6
        MOVS    	R6, #7

        LDR		    R0, =vector8
        STRB	    R1, [R0]
        STRB		R2, [R0, #0x01]
		STRB		R3, [R0, #0x02]
		STRB		R4, [R0, #0x03]
        STRB		R5, [R0, #0x04]
		STRB		R6, [R0, #0x05]

/*
CargoVector16:

        MOVS		R1, #12
        MOVS		R2, #6
        MOVS    	R3, #7
        MOVS		R4, #12
        MOVS		R5, #6
        MOVS    	R6, #7

        LDR		    R0, =vector
        STRH	    R1, [R0]
        STRH		R2, [R0, #0x02]
		STRH		R3, [R0, #0x04]
		STRH		R4, [R0, #0x06]
        STRH		R5, [R0, #0x08]
		STRH		R6, [R0, #0x0C]

*/
/*
CargoVector32:

        MOVS		R1, #12
        MOVS		R2, #6
        MOVS    	R3, #7

        LDR		R0, =vector
        STR		R1, [R0]
        STR		R2, [R0, #0x04]
		STR		R3, [R0, #0x08]

*/

CargoEstructura:

        LDR		R0, =estructura
        LDR		R1, =vector8 // ELEGIR EL VECTOR QUE QUIERO VECTOR PARA 16 32 O VECTOR 8
        MOVS	R2, #8 // mi tipo de DATO
        MOVS   	R3, #6 // LA CANTIDAD DE DATOS
        MOVS	R4, #6 // EL DATO QUE BUSCO
        STR		R1, [R0]
	    STR		R2, [R0, #0x04]
     	STR		R3, [R0, #0x08]
        STR	    R4, [R0, #0x0C]

mainLoop: //este seria mi programa

        LDR		R0, =estructura
        LDR		R1, [R0] // R1 ES LA DIRECCION DEL VECTOR
		LDR		R7, [R0] // para probar luego
		LDR		R2, [R0, #0x04] // R2 ES EL TIPO DE VAR QUE TIENE EL VECTOR
		LDR		R3, [R0, #0x08] //R3 ES LA CANTIDAD DE DATOS
        LDR	    R4, [R0, #0x0C] // R4 ES EL VALOR QUE BUSCO

	    BLX		task1Update

//pruebo que me llego bien el dato a la estructura


         LDR  R7, [R0, #0x10]



mainLoopEnd:							//
		POP		{PC}					// Nunca tendría que pasar por acá


		/************************************/
		/*	Función de Inicialización de las*/
		/*	tareas.							*/
		/************************************/
		.global	setup
		.type	setup, %function
setup:
		PUSH	{LR}
		BL		task1Init
		POP		{PC}


