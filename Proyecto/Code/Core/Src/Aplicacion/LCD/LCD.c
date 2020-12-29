/*
 * LCD.c
 *
 *  Created on: 27 ago. 2020
 *      Author: Luis
 */

#include "LCD.h"

uint8_t buf[LEN_BUF];
uint8_t Estado_pantalla=0;
uint8_t lcd_ocupado=0;
uint8_t Dato_LCD=0;
uint8_t LCD_STATUS=0;
uint8_t pos_LCD_VARIABLE=0;
char LCD_VARIABLE[7];

/*****************************************  TAREAS DE INTERFASE**************************************/

void Enviar_I2C(uint8_t dir,uint8_t len)
{
	//Chip_I2C_MasterSend(I2C2,dir,buf,len);
	EscribirBufferToI2C(dir, buf, len);
}

/*void delay_ms (uint32_t ms)
{
 uint32_t delay;
 volatile uint32_t i;
 for (delay = ms; delay >0 ; delay--)
//1ms loop with -Os optimisation
  {
  for (i=10000; i >0;i--){};
  }

}*/
/***************************************** FUNCIONES DE MANEJO DE LCD**************************************/
uint8_t PantallaIndicacion(uint16_t presion_actual)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=MAIN_INDICACION;
	conversion_lcd(presion_actual);
	return WRITING_OK;
}

uint8_t PantallaActualizarIndicacion(uint16_t presion_actual)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=MAIN_INDICACION_2;
	conversion_lcd(presion_actual);
	return WRITING_OK;
}

uint8_t PantallaActualizarTecla_TEST_TECLADO(uint8_t tecla)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_ENTRADAS_TEST_TECLA_2;
	switch(tecla){
		case TECLA_0:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '0';
			break;
		case TECLA_1:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '1';
			break;
		case TECLA_2:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '2';
			break;
		case TECLA_3:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '3';
			break;
		case TECLA_4:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '4';
			break;
		case TECLA_5:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '5';
			break;
		case TECLA_6:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '6';
			break;
		case TECLA_7:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '7';
			break;
		case TECLA_8:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '8';
			break;
		case TECLA_9:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '9';
			break;
		case TECLA_AST:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '*';
			break;
		case TECLA_NUM:
			LCD_VARIABLE[pos_LCD_VARIABLE] = '#';
			break;
		default:
			LCD_VARIABLE[pos_LCD_VARIABLE] = 'e';
			break;
		}

	if(++pos_LCD_VARIABLE > 6) pos_LCD_VARIABLE=0;
	return WRITING_OK;
}

uint8_t PantallaConfig(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=MAIN_CONFIG;
	return WRITING_OK;
}

uint8_t PantallaAlarm(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_ALARMAS;
	return WRITING_OK;
}

uint8_t PantallaBajaPresion(uint16_t alarma_baja_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_ALARMAS_BAJA;
	conversion_lcd(alarma_baja_presion);
	return WRITING_OK;
}

uint8_t PantallaAltaPresion(uint16_t alarma_alta_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_ALARMAS_ALTA;
	conversion_lcd(alarma_alta_presion);
	return WRITING_OK;
}

uint8_t PantallaActualizarBajaPresion(uint16_t alarma_baja_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_ALARMAS_BAJA_2;
	conversion_lcd(alarma_baja_presion);
	return WRITING_OK;
}

uint8_t PantallaActualizarAltaPresion(uint16_t alarma_alta_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_ALARMAS_ALTA_2;
	conversion_lcd(alarma_alta_presion);
	return WRITING_OK;
}

uint8_t PantallaVarOp(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_VARIABLES;
	return WRITING_OK;
}

uint8_t PantallaRangoMin(uint16_t compresor_baja_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_VARIABLES_RANGOMIN;
	conversion_lcd(compresor_baja_presion);
	return WRITING_OK;
}

uint8_t PantallaRangoMax(uint16_t compresor_alta_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_VARIABLES_RANGOMAX;
	conversion_lcd(compresor_alta_presion);
	return WRITING_OK;
}

uint8_t PantallaActualizarRangoMax(uint16_t compresor_alta_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_VARIABLES_RANGOMAX_2;
	conversion_lcd(compresor_alta_presion);
	return WRITING_OK;
}

uint8_t PantallaActualizarRangoMin(uint16_t compresor_baja_presion)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=CONFIG_VARIABLES_RANGOMIN_2;
	conversion_lcd(compresor_baja_presion);
	return WRITING_OK;
}

uint8_t PantallaDebug(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=MAIN_DEBUG;
	return WRITING_OK;
}

uint8_t PantallaSalidas(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_SALIDAS;
	return WRITING_OK;
}

uint8_t PantallaTestAlarm(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_SALIDAS_TEST_ALARMAS;
	return WRITING_OK;
}

uint8_t PantallaTestCompr(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_SALIDAS_TEST_COMPR;
	return WRITING_OK;
}

uint8_t PantallaTestDesc(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_SALIDAS_TEST_DESC;
	return WRITING_OK;
}

uint8_t PantallaEntradas(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_ENTRADAS;
	return WRITING_OK;
}

uint8_t PantallaTestTeclado(uint16_t tecla_presionada)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_ENTRADAS_TEST_TECLA;
	//conversion_lcd_tecla(tecla_presionada);//Viene del teclado
	memset(LCD_VARIABLE,' ',7);
	pos_LCD_VARIABLE=0;
	return WRITING_OK;
}

uint8_t PantallaTestLazo(uint16_t lazo_i)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_ENTRADAS_TEST_ADC;
	conversion_lcd(lazo_i); //Viene del adc pero hay que convertir
	return WRITING_OK;
}

uint8_t PantallaActualizarTestLazo(uint16_t lazo_i)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=DEBUG_ENTRADAS_TEST_ADC_2;
	conversion_lcd(lazo_i); //Viene del adc pero hay que convertir
	return WRITING_OK;
}

uint8_t BorrarPantalla(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=LCD_CLEAR_SCREEN;
	return WRITING_OK;
}
/******************************* PANTALLAS QUE MUESTRAN ERROR ********************************************/

uint8_t PantallaAlertaBajaPresion(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=ALERTA_BAJA_PRESION;
	return WRITING_OK;
}

uint8_t PantallaAlertaAltaPresion(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=ALERTA_ALTA_PRESION;
	return WRITING_OK;
}

uint8_t PantallaAlertaLazo(void)
{
	if(Estado_pantalla!=LCD_IDLE)
	{return WRITING_ERROR;}
	Estado_pantalla=ALERTA_FALLA_LAZO;
	return WRITING_OK;
}


/******************************* TAREA QUE UTILIZA EL SISTEMA TDS *****************************************/

void Tarea_actualizar_lcd(void* p)
{
	switch(Estado_pantalla)
	{
		case LCD_IDLE:
			break;
		case LCD_CLEAR_SCREEN:
			Dato_LCD=0x01;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=LCD_IDLE;}
			break;
		//INDICACION DE PRESION
		case MAIN_INDICACION:
			LCD_Escribir_String("PRESION ACTUAL: ");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=MAIN_INDICACION_2;}
			break;
		case MAIN_INDICACION_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=MAIN_INDICACION_3;}
			break;
		case MAIN_INDICACION_3:
			LCD_Escribir_String(LCD_VARIABLE);
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=MAIN_INDICACION_4;}
			break;
		case MAIN_INDICACION_4:
			LCD_Escribir_String("BAR");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		//CONFIGURACION
		case MAIN_CONFIG:
			LCD_Escribir_String("CONFIGURACION");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		//ALARMAS
		case CONFIG_ALARMAS:
			LCD_Escribir_String("ALARMAS");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		//ALARMA BAJA PRESION
		case CONFIG_ALARMAS_BAJA:
			LCD_Escribir_String("BAJA PRESION");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_ALARMAS_BAJA_2;}
			break;
		case CONFIG_ALARMAS_BAJA_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=CONFIG_ALARMAS_BAJA_3;}
			break;
		case CONFIG_ALARMAS_BAJA_3:
			LCD_Escribir_String(LCD_VARIABLE);
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_ALARMAS_BAJA_4;}
			break;
		case CONFIG_ALARMAS_BAJA_4:
			LCD_Escribir_String("BAR");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		//ALARMA ALTA PRESION
		case CONFIG_ALARMAS_ALTA:
			LCD_Escribir_String("ALTA PRESION");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_ALARMAS_ALTA_2;}
			break;
		case CONFIG_ALARMAS_ALTA_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=CONFIG_ALARMAS_ALTA_3;}
			break;
		case CONFIG_ALARMAS_ALTA_3:
			LCD_Escribir_String(LCD_VARIABLE);
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_ALARMAS_ALTA_4;}
			break;
		case CONFIG_ALARMAS_ALTA_4:
			LCD_Escribir_String("BAR");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		//VARIABLES DE OPERACION
		case CONFIG_VARIABLES:
			LCD_Escribir_String("VARIABLES DE ");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_VARIABLES_2;}
			break;
		case CONFIG_VARIABLES_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=CONFIG_VARIABLES_3;}
			break;
		case CONFIG_VARIABLES_3:
			LCD_Escribir_String("OPERACION");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// COMPRESOR RANGO MINIMO
		case CONFIG_VARIABLES_RANGOMIN:
			LCD_Escribir_String("RANGO MINIMO");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_VARIABLES_RANGOMIN_2;}
			break;
		case CONFIG_VARIABLES_RANGOMIN_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=CONFIG_VARIABLES_RANGOMIN_3;}
			break;
		case CONFIG_VARIABLES_RANGOMIN_3:
			LCD_Escribir_String(LCD_VARIABLE);
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_VARIABLES_RANGOMIN_4;}
			break;
		case CONFIG_VARIABLES_RANGOMIN_4:
			LCD_Escribir_String("BAR");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// COMPRESOR RANGO MAXIMO
		case CONFIG_VARIABLES_RANGOMAX:
			LCD_Escribir_String("RANGO MAXIMO");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_VARIABLES_RANGOMAX_2;}
			break;
		case CONFIG_VARIABLES_RANGOMAX_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=CONFIG_VARIABLES_RANGOMAX_3;}
			break;
		case CONFIG_VARIABLES_RANGOMAX_3:
			LCD_Escribir_String(LCD_VARIABLE);
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=CONFIG_VARIABLES_RANGOMAX_4;}
			break;
		case CONFIG_VARIABLES_RANGOMAX_4:
			LCD_Escribir_String("BAR");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// DEBUG
		case MAIN_DEBUG:
			LCD_Escribir_String("DEBUG");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// SALIDAS
		case DEBUG_SALIDAS:
			LCD_Escribir_String("SALIDAS");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// TEST ALARMAS
		case DEBUG_SALIDAS_TEST_ALARMAS:
			LCD_Escribir_String("PROBAR ALARMA");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_SALIDAS_TEST_ALARMAS_2;}
			break;
		case DEBUG_SALIDAS_TEST_ALARMAS_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=DEBUG_SALIDAS_TEST_ALARMAS_3;}
			break;
		case DEBUG_SALIDAS_TEST_ALARMAS_3:
			LCD_Escribir_String("1:ENC  2:APAG");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// TEST COMPRESOR
		case DEBUG_SALIDAS_TEST_COMPR:
			LCD_Escribir_String("PROBAR COMPRESOR");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_SALIDAS_TEST_COMPR_2;}
			break;
		case DEBUG_SALIDAS_TEST_COMPR_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=DEBUG_SALIDAS_TEST_COMPR_3;}
			break;
		case DEBUG_SALIDAS_TEST_COMPR_3:
			LCD_Escribir_String("1:ENC  2:APAG");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// VALVULA DE DESCARGA
		case DEBUG_SALIDAS_TEST_DESC:
			LCD_Escribir_String("PROBAR VALV DESC");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_SALIDAS_TEST_DESC_2;}
			break;
		case DEBUG_SALIDAS_TEST_DESC_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=DEBUG_SALIDAS_TEST_DESC_3;}
			break;
		case DEBUG_SALIDAS_TEST_DESC_3:
			LCD_Escribir_String("1:ENC  2:APAG");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;


		// ENTRADAS
		case DEBUG_ENTRADAS:
			LCD_Escribir_String("ENTRADAS");

			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// TEST TECLADO
		case DEBUG_ENTRADAS_TEST_TECLA:
			LCD_Escribir_String("PROBAR TECLADO");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_ENTRADAS_TEST_TECLA_2;}
			break;
		case DEBUG_ENTRADAS_TEST_TECLA_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=DEBUG_ENTRADAS_TEST_TECLA_3;}
			break;
		case DEBUG_ENTRADAS_TEST_TECLA_3:
			LCD_Escribir_String("TECLA: ");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_ENTRADAS_TEST_TECLA_4;}
			break;
		case DEBUG_ENTRADAS_TEST_TECLA_4:
			LCD_Escribir_String(LCD_VARIABLE);
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		// TEST LAZO I
		case DEBUG_ENTRADAS_TEST_ADC:
			LCD_Escribir_String("PROBAR LAZO 20mA");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_ENTRADAS_TEST_ADC_2;}
			break;
		case DEBUG_ENTRADAS_TEST_ADC_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=DEBUG_ENTRADAS_TEST_ADC_3;}
			break;
		case DEBUG_ENTRADAS_TEST_ADC_3:
			LCD_Escribir_String("LAZO: ");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_ENTRADAS_TEST_ADC_4;}
			break;
		case DEBUG_ENTRADAS_TEST_ADC_4:
			LCD_Escribir_String(LCD_VARIABLE);
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=DEBUG_ENTRADAS_TEST_ADC_5;}
			break;
		case DEBUG_ENTRADAS_TEST_ADC_5:
			LCD_Escribir_String("mA");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		case ALERTA_ALTA_PRESION:
			LCD_Escribir_String("ALERTA:");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=ALERTA_ALTA_PRESION_2;}
			break;
		case ALERTA_ALTA_PRESION_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=ALERTA_ALTA_PRESION_3;}
			break;
		case ALERTA_ALTA_PRESION_3:
			LCD_Escribir_String("ALTA PRESION!!!");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		case ALERTA_BAJA_PRESION:
			LCD_Escribir_String("ALERTA:");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=ALERTA_BAJA_PRESION_2;}
			break;
		case ALERTA_BAJA_PRESION_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=ALERTA_BAJA_PRESION_3;}
			break;
		case ALERTA_BAJA_PRESION_3:
			LCD_Escribir_String("BAJA PRESION!!!");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
		case ALERTA_FALLA_LAZO:
			LCD_Escribir_String("ALERTA:");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=ALERTA_FALLA_LAZO_2;}
			break;
		case ALERTA_FALLA_LAZO_2:
			Dato_LCD=0xC0;
			LCD_Escribir_Comando();
			if(lcd_ocupado == 0)
			{Estado_pantalla=ALERTA_FALLA_LAZO_3;}
			break;
		case ALERTA_FALLA_LAZO_3:
			LCD_Escribir_String("FALLA DE LAZO!!!");
			if(LCD_STATUS == LCD_DONE)
			{Estado_pantalla=LCD_IDLE;}
			break;
	}

}

/*************************************************    LCD    ****************************************/

void LCD_Escribir_String(char *string)
{
	static uint8_t c=0;

	if(string[c] != '\0')
	{
		LCD_STATUS=1;
		Dato_LCD=(uint8_t)string[c];
		LCD_Escribir_Dato();
		if(lcd_ocupado==0)
		{
			c++;
		}
	}
	else
	{
		c=0;
		LCD_STATUS=0;
	}
}


void LCD_Escribir_Dato(void)
{
	static uint8_t estado=0;
	static uint8_t nibble=0;
	static uint8_t dato_escritura=0;

	switch(estado)
	{
		case 0:
			lcd_ocupado=1;
			dato_escritura=Dato_LCD;
			memset(buf,0,sizeof(uint8_t)*LEN_BUF);
			nibble=dato_escritura & 0xF0;
			buf[0]=nibble|BK_ON|ENABLE_ON|RS_ON; //00001101
			Enviar_I2C(DIR_LCD,1);
			estado=1;
			break;
		case 1:
		    buf[0]=buf[0]&ENABLE_OFF; //00001001
		    Enviar_I2C(DIR_LCD,1);
			estado=2;
			break;
		case 2:
		    nibble=(dato_escritura << 0x04) & 0xF0;
		    buf[0]=nibble|BK_ON|ENABLE_ON|RS_ON; //00001101
		    Enviar_I2C(DIR_LCD,1);
			estado=3;
			break;
		case 3:
		    buf[0]=buf[0]&ENABLE_OFF; //00001001
		    Enviar_I2C(DIR_LCD,1);
			estado=0;
			lcd_ocupado=0;
			break;
	}
}

void LCD_Escribir_Comando(void)
{
	static uint8_t estado=0;
	static uint8_t nibble=0;
	static uint8_t dato_escritura=0;

	switch(estado)
	{
		case 0:
			lcd_ocupado=1;
			dato_escritura=Dato_LCD;
			memset(buf,0,sizeof(uint8_t)*LEN_BUF);
			nibble=dato_escritura & 0xF0;
			buf[0]=nibble|BK_ON|ENABLE_ON; //00001100
			Enviar_I2C(DIR_LCD,1);
			estado=1;
			break;
		case 1:
		    buf[0]=buf[0]&ENABLE_OFF; //00001000
		    Enviar_I2C(DIR_LCD,1);
			estado=2;
			break;
		case 2:
		    nibble=(dato_escritura << 0x04) & 0xF0;
		    buf[0]=nibble|BK_ON|ENABLE_ON; //00001100
		    Enviar_I2C(DIR_LCD,1);
			estado=3;
			break;
		case 3:
		    buf[0]=buf[0]&ENABLE_OFF; //00001000
		    Enviar_I2C(DIR_LCD,1);
			estado=0;
			lcd_ocupado=0;
			break;
	}
}

void conversion_lcd(uint16_t dato)
{
	uint32_t temp=dato;
	LCD_VARIABLE[0]=temp/10000+48;
	temp%=10000;
	LCD_VARIABLE[1]=temp/1000+48;
	LCD_VARIABLE[2]=46;
	temp%=1000;
	LCD_VARIABLE[3]=temp/100+48;
	temp%=100;
	LCD_VARIABLE[4]=temp/10+48;
	LCD_VARIABLE[5]=temp%10+48;
	LCD_VARIABLE[6]='\0';
}

void conversion_lcd_tecla(uint16_t dato)
{
	uint32_t temp=dato;

	temp%=10000;
	temp%=1000;
	temp%=100;
	LCD_VARIABLE[0]=temp/10+48;
	LCD_VARIABLE[1]=temp%10+48;
	LCD_VARIABLE[2]=32;
	LCD_VARIABLE[3]=32;
	LCD_VARIABLE[4]=32;
	LCD_VARIABLE[5]=32;
	LCD_VARIABLE[6]='\0';

}
/********************************************   INICIALIZACION    ****************************************************/

void LCD_init(void)
{
	uint32_t i;

	delay_ms(30);

	for(i=0; i<3; i++)
	{
		memset(buf,0,sizeof(uint8_t)*LEN_BUF);
		buf[0]=0x3c; //00111100
		Enviar_I2C(DIR_LCD,1);
		delay_ms(5);
		buf[0]=0x38; //00111000
		Enviar_I2C(DIR_LCD,1);
	}

// Configuracion en 4 bits
	buf[0]=0x2c; //00101100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x28; //00101000
	Enviar_I2C(DIR_LCD,1);

//porque es de 4 bits
	buf[0]=0x2c; //00101100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x28; //00101000
	Enviar_I2C(DIR_LCD,1);

	buf[0]=0xcc; //11001100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0xc8; //11001000
	Enviar_I2C(DIR_LCD,1);

//display switch
	buf[0]=0x0c; //00001100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x08; //00001000
	Enviar_I2C(DIR_LCD,1);

	buf[0]=0xfc; //11111100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0xf8; //11111000
	Enviar_I2C(DIR_LCD,1);

	buf[0]=0x0c; //00001100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x08; //00001000
	Enviar_I2C(DIR_LCD,1);

	buf[0]=0x6c; //01101100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x68; //01101000
	Enviar_I2C(DIR_LCD,1);

//indice = home = posicion 0,0
	buf[0]=0x0c; //00001100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x08; //00001000
	Enviar_I2C(DIR_LCD,1);

	buf[0]=0x1c; //00011100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x18; //00011000
	Enviar_I2C(DIR_LCD,1);

	buf[0]=0x8c; //00001100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x88; //00001000
	Enviar_I2C(DIR_LCD,1);

	buf[0]=0x0c; //00001100
	Enviar_I2C(DIR_LCD,1);
	delay_ms(3);
	buf[0]=0x08; //00001000
	Enviar_I2C(DIR_LCD,1);

}
