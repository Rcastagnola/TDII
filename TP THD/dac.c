#include "chip.h"
#include "dac.h"

static unsigned int estadodac;
static unsigned int dmanro;

void InitDAC(void)
{
	//PIN_Init((int)AOUT__0_26);
	Chip_IOCON_PinMuxSet(LPC_IOCON,0,26,IOCON_FUNC2);
}

void SetValueDAC(unsigned int dato)
{
	//No llamar más de una vez por us.
	LPC_DAC->CR = (dato&0x3FF)<<6;

}

void EnviarBloqueDAC(int *datos, int len, int freq, int canaldma, DMALLIO *dmaconfig, void(*finbloque)(void))
{
	if(!isActivoDMA())
	{
		InitDMA();
	}
	LPC_DAC->CNTVAL = ((SystemCoreClock>>2)/freq)&0xFFFF;
	LPC_DAC->CTRL = (DAC_CNT_ENA|DAC_DBLBUF_ENA|DAC_DMA_ENA);

	dmaconfig->source = (int)datos;
	dmaconfig->destination = (int)&(LPC_DAC->CR);
	dmaconfig->next = 0;
	dmaconfig->control = (len&0xFFF) |(2<<DMA_SW)|(2<<DMA_DW)|DMA_SOURCE_INC|DMA_INT;
	SetearCanal(dmanro,(int)DMA_M2P,(int)DMA_MEM,(int)DMA_DAC,DMA_LLIST,dmaconfig,finbloque,NULL);
	IniciarTransferenciaDMA(dmanro);
}

void SetTableDAC(int *datos, int len, int freq, int canaldma, DMALLIO *dmaconfig)
{
	//freq tiene que ir entre 390Hz y 1MHz.
	//Considerando que PCLK_DAC = CCLK/4.
	estadodac = 0;
	dmanro = canaldma;

	if(!isActivoDMA())
	{
		InitDMA();
	}
	LPC_DAC->CNTVAL = ((SystemCoreClock>>2)/freq)&0xFFFF;
	LPC_DAC->CTRL = (M_DAC_CNT_ENA|M_DAC_DBLBUF_ENA|M_DAC_DMA_ENA);

	dmaconfig->source = (int)datos;
	dmaconfig->destination = (int)&(LPC_DAC->CR);
	dmaconfig->next = (int)dmaconfig;
	dmaconfig->control = 	(len&0xFFF) |(2<<DMA_SW)|(2<<DMA_DW)|DMA_SOURCE_INC|DMA_INT;

	SetearCanal(dmanro,(int)DMA_M2P,(int)DMA_MEM,(int)DMA_DAC,DMA_LLIST,dmaconfig,NULL,NULL);
	IniciarTransferenciaDMA(dmanro);
	estadodac = 1;
}

int isDACActivo()
{
	return estadodac;
}


void StopTableDAC(void)
{
	estadodac = 0;
	TerminarTransferenciaDMA(dmanro);
}
