/*
 * LCD.h
 *
 *  Created on: 27 ago. 2020
 *      Author: Luis
 */

#ifndef SRC_APLICACION_LCD_LCD_H_
#define SRC_APLICACION_LCD_LCD_H_

#include "main.h"
//#include "chip.h"
#include "string.h"
#include "stdio.h"

#include "Drivers/i2c/i2c.h"

/*
 * Prototipos de las funciones que hacen al LCD.
 */
void LCD_init(void);
void Tarea_actualizar_lcd(void* p);
void Enviar_I2C(uint8_t ,uint8_t);
void delay_ms (uint32_t);

void LCD_Escribir_String(char *);
void LCD_Escribir_Dato(void);
void LCD_Escribir_Comando(void);
void conversion_lcd(uint16_t);
void conversion_lcd_tecla(uint16_t);

uint8_t PantallaIndicacion(uint16_t);
uint8_t PantallaActualizarIndicacion(uint16_t);
uint8_t PantallaActualizarTecla_TEST_TECLADO(uint8_t tecla);
uint8_t PantallaConfig(void);
uint8_t PantallaAlarm(void);
uint8_t PantallaBajaPresion(uint16_t);
uint8_t PantallaActualizarBajaPresion(uint16_t);
uint8_t PantallaAltaPresion(uint16_t);
uint8_t PantallaActualizarAltaPresion(uint16_t);
uint8_t PantallaVarOp(void);
uint8_t PantallaRangoMin(uint16_t);
uint8_t PantallaActualizarRangoMin(uint16_t);
uint8_t PantallaRangoMax(uint16_t);
uint8_t PantallaActualizarRangoMax(uint16_t);
uint8_t PantallaDebug(void);
uint8_t PantallaSalidas(void);
uint8_t PantallaTestAlarm(void);
uint8_t PantallaTestCompr(void);
uint8_t PantallaTestDesc(void);
uint8_t PantallaEntradas(void);
uint8_t PantallaTestTeclado(uint16_t);
uint8_t PantallaActualizarTestLazo(uint16_t);
uint8_t PantallaTestLazo(uint16_t);
uint8_t BorrarPantalla(void);
uint8_t PantallaAlertaBajaPresion(void);
uint8_t PantallaAlertaAltaPresion(void);
uint8_t PantallaAlertaLazo(void);

/* DEFINES DEL LCD*/

#define DIR_LCD			0x4e
#define LEN_BUF			(32)
#define LCD_DONE		0
#define LCD_BUSY		1

#define RS_ON			0x01
#define RS_OFF			0xFE
#define RW_ON			0x02
#define RW_OFF			0xFD
#define ENABLE_ON		0x04
#define ENABLE_OFF		0xFB
#define BK_ON			0x08
#define BK_OFF			0xF7

#define WRITING_ERROR	0
#define WRITING_OK		1

enum{
	LCD_IDLE=0,
	LCD_CLEAR_SCREEN,
	MAIN_INDICACION,
	MAIN_INDICACION_2,
	MAIN_INDICACION_3,
	MAIN_INDICACION_4,
	MAIN_CONFIG,
	CONFIG_ALARMAS,
	CONFIG_ALARMAS_BAJA,
	CONFIG_ALARMAS_BAJA_2,
	CONFIG_ALARMAS_BAJA_3,
	CONFIG_ALARMAS_BAJA_4,
	CONFIG_ALARMAS_ALTA,
	CONFIG_ALARMAS_ALTA_2,
	CONFIG_ALARMAS_ALTA_3,
	CONFIG_ALARMAS_ALTA_4,
	CONFIG_VARIABLES,
	CONFIG_VARIABLES_2,
	CONFIG_VARIABLES_3,
	CONFIG_VARIABLES_RANGOMIN,
	CONFIG_VARIABLES_RANGOMIN_2,
	CONFIG_VARIABLES_RANGOMIN_3,
	CONFIG_VARIABLES_RANGOMIN_4,
	CONFIG_VARIABLES_RANGOMAX,
	CONFIG_VARIABLES_RANGOMAX_2,
	CONFIG_VARIABLES_RANGOMAX_3,
	CONFIG_VARIABLES_RANGOMAX_4,
	MAIN_DEBUG,
	DEBUG_SALIDAS,
	DEBUG_SALIDAS_TEST_ALARMAS,
	DEBUG_SALIDAS_TEST_ALARMAS_2,
	DEBUG_SALIDAS_TEST_ALARMAS_3,
	DEBUG_SALIDAS_TEST_COMPR,
	DEBUG_SALIDAS_TEST_COMPR_2,
	DEBUG_SALIDAS_TEST_COMPR_3,
	DEBUG_SALIDAS_TEST_DESC,
	DEBUG_SALIDAS_TEST_DESC_2,
	DEBUG_SALIDAS_TEST_DESC_3,
	DEBUG_ENTRADAS,
	DEBUG_ENTRADAS_TEST_TECLA,
	DEBUG_ENTRADAS_TEST_TECLA_2,
	DEBUG_ENTRADAS_TEST_TECLA_3,
	DEBUG_ENTRADAS_TEST_TECLA_4,
	DEBUG_ENTRADAS_TEST_ADC,
	DEBUG_ENTRADAS_TEST_ADC_2,
	DEBUG_ENTRADAS_TEST_ADC_3,
	DEBUG_ENTRADAS_TEST_ADC_4,
	DEBUG_ENTRADAS_TEST_ADC_5,
	ALERTA_ALTA_PRESION,
	ALERTA_ALTA_PRESION_2,
	ALERTA_ALTA_PRESION_3,
	ALERTA_BAJA_PRESION,
	ALERTA_BAJA_PRESION_2,
	ALERTA_BAJA_PRESION_3,
	ALERTA_FALLA_LAZO,
	ALERTA_FALLA_LAZO_2,
	ALERTA_FALLA_LAZO_3,
};





#endif /* SRC_APLICACION_LCD_LCD_H_ */
