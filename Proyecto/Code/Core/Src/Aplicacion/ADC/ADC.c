 /*
 * ADC.c
 *
 *  Created on: 30 sep. 2020
 *      Author: Luis
 */
#include "ADC.h"

uint8_t buf_adc[LEN_BUF];
uint16_t valor_adc;
uint16_t mediciones_ADC[LEN_PROMEDIO];
uint32_t corrienteMedida=0;
int32_t presionMedida=0;
//uint32_t valor_promediado;


void ADC_init(void)
{
	uint32_t i;
	/*Enciendo el ADC*/
	/*Espero que arranque y lo configuro*/
	for(i=0;i<1500000;i++);
	memset(buf_adc,0,sizeof(uint8_t)*LEN_BUF);
	buf_adc[0]=0x01;  //adress pointer a config register
	buf_adc[1]=0xC2;  // OS=1,MUX=100,PGA=001,MODE=0  Vref=4.096V
	buf_adc[2]=0x83;  //Datarate=128SPS
	EscribirBufferToI2C(DIR_ADC, buf_adc, 3);
	memset(buf_adc,0,sizeof(uint8_t)*LEN_BUF);
	buf_adc[0]=0x00;
	EscribirBufferToI2C(DIR_ADC, buf_adc, 1);
	valor_promediado = 0;
	flag_datos_completos = 0;
}

void tarea_adc(void *p)
{
	valor_adc = LeerWordFromI2C(DIR_ADC);
}

void tarea_promediar(void *p)
{
	static int ix = 0;

	uint32_t acumulador = 0;
	mediciones_ADC[ix++] = valor_adc;
	if (ix == LEN_PROMEDIO)
	{
		flag_datos_completos = 1;
		ix = 0;
	}

	if(flag_datos_completos)
	{
		for(uint8_t i = 0; i< LEN_PROMEDIO; i++)
			acumulador += mediciones_ADC[i];

		valor_promediado = acumulador / LEN_PROMEDIO;
		corrienteMedida=(valor_promediado*4096/32768)*10;
		presionMedida= (corrienteMedida-4000);
		if(presionMedida<0)presionMedida=0;
		presionMedida=presionMedida*625/1000;
	}
}
