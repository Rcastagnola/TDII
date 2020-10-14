/*
 * dac.h
 *
 *  Created on: 24/06/2012
 *      Author: juan
 */

#ifndef DAC_H_
#define DAC_H_
#include "dma.h"

#define	DAC_INT_DMA_REQ	1
#define M_DAC_DBLBUF_ENA	2
#define M_DAC_CNT_ENA		4
#define M_DAC_DMA_ENA		8
#define	DAC_BIAS		(1<<16)

extern void InitDAC(void);
extern void SetValueDAC(unsigned int dato);
extern void SetTableDAC(int *datos, int len, int freq, int canaldma, DMALLIO *dmaconfig);
void EnviarBloqueDAC(int *datos, int len, int freq, int canaldma, DMALLIO *dmaconfig, void(*finbloque)(void));
extern void StopTableDAC(void);
extern int EstadoDAC(void);

#endif /* DAC_H_ */
