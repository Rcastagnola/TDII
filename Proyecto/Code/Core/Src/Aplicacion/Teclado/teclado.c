/*
 * teclado.c
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */


#include "teclado.h"

//bcet = 1.9 uS
//wcet = 195 uS
void tarea_leer_teclado(){

	switch(estado){

		//bcet = 192 uS
		//wcer = 192 uS
		case escribir_fila:
			EscribirByteToI2c(TECLADO_ADDR, array_filas[row]);
			estado = leer_columnas;
			break;

		//bcet = 192 uS
		//wcer = 192 uS
		case leer_columnas:
			LecturaTeclado = LeerByteFromI2C(TECLADO_ADDR);
			estado = procesar_columnas;
			break;
		//bcet = 1.3 uS
		//wcet = 2.3 uS
		case procesar_columnas:
			switch(row){
				case 0:
					if(!(LecturaTeclado & COL1)){
						DatosTeclas |= 0x01;
					}else if(!(LecturaTeclado & COL2)){
						DatosTeclas |= 0x02;
					}else if(!(LecturaTeclado & COL3)){
						DatosTeclas |= 0x04;
					}else if(!(LecturaTeclado & COL4)){
						DatosTeclas |= 0x08;
					}
					break;
				case 1:
					if(!(LecturaTeclado & COL1)){
						DatosTeclas |= 0x10;
					}else if(!(LecturaTeclado & COL2)){
						DatosTeclas |= 0x20;
					}else if(!(LecturaTeclado & COL3)){
						DatosTeclas |= 0x40;
					}else if(!(LecturaTeclado & COL4)){
						DatosTeclas |= 0x80;
					}
					break;
				case 2:
					if(!(LecturaTeclado & COL1)){
						DatosTeclas |= 0x100;
					}else if(!(LecturaTeclado & COL2)){
						DatosTeclas |= 0x200;
					}else if(!(LecturaTeclado & COL3)){
						DatosTeclas |= 0x400;
					}else if(!(LecturaTeclado & COL4)){
						DatosTeclas |= 0x800;
					}
					break;
				case 3:
					if(!(LecturaTeclado & COL1)){
						DatosTeclas |= 0x1000;
					}else if(!(LecturaTeclado & COL2)){
						DatosTeclas |= 0x2000;
					}else if(!(LecturaTeclado & COL3)){
						DatosTeclas |= 0x4000;
					}else if(!(LecturaTeclado & COL4)){
						DatosTeclas |= 0x8000;
					}
					break;
			}
			if(row == ROWS-1){
				estado = procesar_teclas;
			}else{
				row++;
				estado = escribir_fila;
			}
			break;
		//bcet = 26.2 uS
		//wcet = 30.4 uS
		case procesar_teclas:
			ProcesarTeclas();
			estado = escribir_fila;
			DatosTeclas=0;
			row=0;
			break;
	}
}


void inicializar_teclado(){
	EscribirByteToI2c(TECLADO_ADDR, MASK_IO);

	DatosTeclas=0;
	for(uint8_t i=0; i < CANTIDAD_TECLAS; i++){
		ticks_antirebore[i]=0;
		estado_teclas[i]=0;
		tiempo_tecla_presionada[i]=0;
	}

	array_filas[0] = (uint8_t)(~ROW1);
	array_filas[1] = (uint8_t)(~ROW2);
	array_filas[2] = (uint8_t)(~ROW3);
	array_filas[3] = (uint8_t)(~ROW4);

	LecturaTeclado = 0;
	estado = escribir_fila;
	row = 0;
	teclasApretadas=0;
	return;
}

void ProcesarTeclas(){
	for(uint8_t tecla=0;tecla<CANTIDAD_TECLAS; tecla++){

		if(DatosTeclas & (1<<tecla)){
			if(ticks_antirebore[tecla] < MAX_TICKS_ANTIREBOTE)
				ticks_antirebore[tecla]++;
		}else{
			if(ticks_antirebore[tecla])
				ticks_antirebore[tecla]--;
		}
	}

	for(uint8_t tecla=0;tecla<CANTIDAD_TECLAS; tecla++){
		if(ticks_antirebore[tecla] == 0){
			if(estado_teclas[tecla] == 1){
				estado_teclas[tecla] = 0;
				ProcesarFlancoDeBajada(tecla);
			}
		}
		if(estado_teclas[tecla] == 1){
			tiempo_tecla_presionada[tecla]++;
		}

		if(ticks_antirebore[tecla] == MAX_TICKS_ANTIREBOTE){
			if(estado_teclas[tecla] == 0){
				estado_teclas[tecla] = 1;
				tiempo_tecla_presionada[tecla] = 0;
				ProcesarFlancoDeSubida(tecla);
			}
		}
	}
}


void ProcesarFlancoDeSubida(uint8_t tecla){
	switch(tecla){
		case TECLA_1:
		break;
		case TECLA_2:
		break;
		case TECLA_3:
		break;
		case TECLA_4:
		break;
		case TECLA_5:
		break;
		case TECLA_6:
		break;
		case TECLA_7:
		break;
		case TECLA_8:
		break;
		case TECLA_9:
		break;
		case TECLA_0:
		break;
		case TECLA_AST:
		break;
		case TECLA_NUM:
		break;
		case TECLA_A:
		break;
		case TECLA_B:
		break;
		case TECLA_C:
		break;
		case TECLA_D:
		break;
		default:
		break;
	}
	return;
}


void ProcesarFlancoDeBajada(uint8_t tecla){
	teclasApretadas |= (1<<tecla);

 	switch(tecla){
		case TECLA_1:
		break;
		case TECLA_2:
		break;
		case TECLA_3:
		break;
		case TECLA_4:
		break;
		case TECLA_5:
		break;
		case TECLA_6:
		break;
		case TECLA_7:
		break;
		case TECLA_8:
		break;
		case TECLA_9:
		break;
		case TECLA_0:
		break;
		case TECLA_AST:
		break;
		case TECLA_NUM:
		break;
		case TECLA_A:
		break;
		case TECLA_B:
		break;
		case TECLA_C:
		break;
		case TECLA_D:
		break;
		default:
		break;
	}
	return;
}


