/*
 * teclado.h
 *
 *  Created on: 15 ago. 2020
 *      Author: hanib
 */

#ifndef SRC_APLICACION_TECLADO_TECLADO_H_
#define SRC_APLICACION_TECLADO_TECLADO_H_

#include "main.h"
#include "string.h"
#include "stdio.h"

extern UART_HandleTypeDef huart1;

uint16_t DatosTeclas;

uint16_t teclasApretadas;

#define CANTIDAD_TECLAS 16

enum{
	TECLA_1=0,
	TECLA_2,
	TECLA_3,
	TECLA_A,
	TECLA_4,
	TECLA_5,
	TECLA_6,
	TECLA_B,
	TECLA_7,
	TECLA_8,
	TECLA_9,
	TECLA_C,
	TECLA_AST,
	TECLA_0,
	TECLA_NUM,
	TECLA_D
};

enum{
	escribir_fila=0,
	leer_columnas,
	procesar_columnas,
	procesar_teclas





};
//Variable para contar los tick para hacer el antirebote
uint8_t ticks_antirebore[CANTIDAD_TECLAS];

#define MAX_TICKS_ANTIREBOTE 5

//Variable para almacenar el estado de las teclas
uint8_t estado_teclas[CANTIDAD_TECLAS];

//Variable para almacenar el tiempo que estuvo presionada la tecla
uint32_t tiempo_tecla_presionada[CANTIDAD_TECLAS];

//Prototipos
void ProcesarFlancoDeSubida(uint8_t tecla);
void ProcesarFlancoDeBajada(uint8_t tecla);
void tarea_leer_teclado();
void inicializar_teclado();
void ProcesarTeclas();



/* MATRIZ 4X4
 *
 */

#define ROWS 4
#define COLS 4



//Definicion de los bits de las filas y las columnas
/*#define ROW1 0x01
#define ROW2 0x02
#define ROW3 0x04
#define ROW4 0x08
#define COL1 0x10
#define COL2 0x40
#define COL3 0x20
#define COL4 0x80*/

#define COL1 0x08
#define COL2 0x04
#define COL3 0x02
#define COL4 0x01
#define ROW1 0x80
#define ROW2 0x40
#define ROW3 0x20
#define ROW4 0x10

//Definicion de la direcion configurada en el PCF8574T que controla el teclado
#define TECLADO_ADDR 0x4C //CON PUENTE EN A0

//Configuracion del chip PCF8574
// 1 = Entrada
// 0 = Salida
//Se manda este byte en la inicializacion
#define MASK_IO 0xF0 //Configuro las filas como salida

uint8_t array_filas[ROWS];

uint8_t LecturaTeclado;
uint8_t estado;
uint8_t row;



#endif /* SRC_APLICACION_TECLADO_TECLADO_H_ */
