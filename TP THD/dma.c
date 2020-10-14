#include "chip.h"
#include "dma.h"
#include "lpc_types.h"
#define NRO_CHAN_DMA	8

static int errordma;
static int findma;
static void (*callbacks[NRO_CHAN_DMA])(void);
static void (*errores[NRO_CHAN_DMA])(void);


static LPC_GPDMACH_TypeDef* getDmaChPtr(int canal)
{
	LPC_GPDMACH_TypeDef *ret;
	switch(canal)
	{
		case 0: ret = LPC_GPDMACH0; break;
		case 1: ret = LPC_GPDMACH1; break;
		case 2: ret = LPC_GPDMACH2; break;
		case 3: ret = LPC_GPDMACH3; break;
		case 4: ret = LPC_GPDMACH4; break;
		case 5: ret = LPC_GPDMACH5; break;
		case 6: ret = LPC_GPDMACH6; break;
		case 7: ret = LPC_GPDMACH7; break;
		default: ret = (LPC_GPDMACH_TypeDef*)0;
	}
	return ret;
}



void InitDMA(void)
{
	int i;
	for(i=0; i<NRO_CHAN_DMA;i++)
	{
		callbacks[i] = NULL;
		errores[i] = NULL;
	}

	errordma = 0;
	findma = 0xFF;
	LPC_SYSCON->PCONP |= (1 << 29);
	LPC_GPDMA->CONFIG  = DMA_E;

	while ( !(LPC_GPDMA->CONFIG & DMA_E) );
	//LPC_GPDMA->DMACSync = 0xFFFF;
	LPC_GPDMA->INTTCCLEAR = 0xFF;
	//NVIC_SetPriority(DMA_IRQn,8);
	//NVIC_EnableIRQ(DMA_IRQn);
}

void SetearCanal(int nrocanal,int tipotransf, int sourceper, int destper, int llist,DMALLIO *config, void (*callback)(void), void (*error)(void))
{
	LPC_GPDMACH_TypeDef *canal;

	canal = getDmaChPtr(nrocanal);
	canal->DMACCSrcAddr = 	config->source;
	canal->DMACCDestAddr = 	config->destination;
	canal->DMACCControl = config->control;
	if(llist==DMA_LLIST)
	{
		canal->DMACCLLI = (unsigned int)config;
	}
	else
	{
		canal->DMACCLLI = 0;
	}
	canal->DMACCConfig = 0xC000|((tipotransf&0x7)<<11)|((sourceper&0xF)<<1)|((destper&0x0F)<<6);

	if(callback)
	{
		callbacks[nrocanal] = callback;
	}
	if(error)
	{
		errores[nrocanal] = error;
	}
}

void IniciarTransferenciaDMA(int nrocanal)
{
	LPC_GPDMACH_TypeDef *canal;
	errordma &= ~(1<<nrocanal);
	findma &= ~(1<<nrocanal);
	canal = getDmaChPtr(nrocanal);
	canal->DMACCConfig |= DMA_E;
}

void TerminarTransferenciaDMA(int nrocanal)
{
	findma |= (1<<nrocanal);
	LPC_GPDMACH_TypeDef *canal;
	canal = getDmaChPtr(nrocanal);
	canal->DMACCConfig &= ~(DMA_E);
}

int isActivoDMA()
{
	int ret = 0;
	if(LPC_GPDMA->CONFIG&DMA_E)
	{
		ret = 1;
	}
	return ret;
}

int isEnErrorDMA(int canal)
{
	int ret;
	ret = (errordma&(1<<canal))?1:0;
	return ret;
}

int isDMALibre(int canal)
{
	int ret;
	ret = (findma&(1<<canal))?1:0;
	return ret;
}

void DMA_IRQHandler (void)
{
  int StatVal;
  int ErrVal;
  int i;

  StatVal = LPC_GPDMA->INTTCSTAT;
  ErrVal =  LPC_GPDMA->INTERRSTAT;

  for(i=0;i<8;i++)
  {
	  if(StatVal & (1<<i))
	  {
		findma |= (1<<i);
		if(callbacks[i])
		{
			callbacks[i]();
		}
	  }

	  if(ErrVal & (1<<i))
	  {
		  errordma |= (1<<i);
		  if(errores[i])
		  {
			  errores[i]();
		  }
	  }
  }

  if(StatVal)
  {
	  LPC_GPDMA->INTTCCLEAR |= StatVal;
  }
  if(ErrVal)
  {
	  LPC_GPDMA->INTERRCLR |= ErrVal;
  }
}
