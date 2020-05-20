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

task1InitEnd:
		POP		{PC}

		.size	task1Init, . - task1Init

		.global	task1Update
		.type	task1Update, %function

task1Update:
		PUSH	{LR}
/*
        LDR		R0, =estructura
		STR		R1, [R0] // R1 ES LA DIRECCION DEL VECTOR
		STR		R2, [R0, #0x04] // R2 ES EL TIPO DE VAR QUE TIENE EL VECTOR
		STR		R3, [R0, #0x08] //R3 ES LA CANTIDAD DE DATOS
        STR	    R4, [R0, #0x0C] // R4 ES EL VALOR QUE BUSCO
*/
        MOVS  R5, #0
		CMP   R2, #8
		BEQ   esdeocho
		CMP   R2, #16
		BEQ   esdeDseis

esdeTdos:
        LDR  R6, [R1]
        ADDS  R1,#4
		CMP  R6, R4
		BEQ  TENGOUNO


        SUBS R3,1
        CMP  R3,0
        BEQ  FIN
        B    esdeTdos

esdeocho:

        LDRB  R6, [R1]
        ADDS  R1,#1
		CMP  R6, R4
		BEQ  TENGOUNO


        SUBS R3,1
        CMP  R3,0
        BEQ  FIN
        B    esdeocho

esdeDseis:

 	    LDRH  R6, [R1]
 	    ADDS  R1,#2
		CMP  R6, R4
		BEQ  TENGOUNO


        SUBS R3,1
        CMP  R3,0
        BEQ  FIN
        B    esdeDseis


TENGOUNO:

		ADDS   R5,#1

		SUBS R3,1   // PUEDE SER EL ULTIMO DATO POR ESO
        CMP  R3,0
        BEQ  FIN

        CMP   R2, #8
		BEQ   esdeocho
		CMP   R2, #16
	 	BEQ   esdeDseis
	    CMP   R2, #32
		BEQ   esdeTdos
		B     task1UpdateEnd // no deberia llegar aca

FIN:

      STR  R5,[R0, #0x10]

task1UpdateEnd:

		POP		{PC}					// Return

		.size	task1Update, . - task1Update

