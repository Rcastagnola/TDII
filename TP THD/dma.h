/*
 * dma.h
 *
 *  Created on: 24/06/2012
 *      Author: juan
 */

#ifndef DMA_H_
#define DMA_H_

#ifndef NULL
#define NULL	((void*)0)
#endif

#define DMA_E			1
#define DMA_LLIST		1
#define DMA_NOLLIST		0
#define	DMA_INT			(1<<31)
#define DMA_IE			(1<<14)
#define DMA_DBS			12
#define DMA_SBS			15
#define DMA_SW			18
#define DMA_DW			21
#define DMA_SOURCE_INC	(1<<26)
#define DMA_DEST_INC	(1<<27)

#define LPC_AHB_BASE          (0x50000000UL)
//#define LPC_GPDMA_BASE        (LPC_AHB_BASE  + 0x04000)
#define LPC_GPDMACH0_BASE     (LPC_AHB_BASE  + 0x04100)
#define LPC_GPDMACH1_BASE     (LPC_AHB_BASE  + 0x04120)
#define LPC_GPDMACH2_BASE     (LPC_AHB_BASE  + 0x04140)
#define LPC_GPDMACH3_BASE     (LPC_AHB_BASE  + 0x04160)
#define LPC_GPDMACH4_BASE     (LPC_AHB_BASE  + 0x04180)
#define LPC_GPDMACH5_BASE     (LPC_AHB_BASE  + 0x041A0)
#define LPC_GPDMACH6_BASE     (LPC_AHB_BASE  + 0x041C0)
#define LPC_GPDMACH7_BASE     (LPC_AHB_BASE  + 0x041E0)
#define LPC_GPDMACH0          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH0_BASE )
#define LPC_GPDMACH1          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH1_BASE )
#define LPC_GPDMACH2          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH2_BASE )
#define LPC_GPDMACH3          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH3_BASE )
#define LPC_GPDMACH4          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH4_BASE )
#define LPC_GPDMACH5          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH5_BASE )
#define LPC_GPDMACH6          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH6_BASE )
#define LPC_GPDMACH7          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH7_BASE )

typedef enum{
	DMA_BURST_1  = 0,
	DMA_BURST_4,
	DMA_BURST_8,
	DMA_BURST_16,
	DMA_BURST_32,
	DMA_BURST_64,
	DMA_BURST_128,
	DMA_BURST_256
}dma_sbsize;

typedef enum
{
	DMA_BYTE = 0,
	DMA_HALF_WORD,
	DMA_WORD
}dma_word_size;

typedef enum {
	DMA_M2M	= 0x00,
	DMA_M2P,
	DMA_P2M,
	DMA_P2P
} dma_type_transfer;

typedef enum{
	DMA_MEM		= 0x00,
	DMA_SSP0_TX	= 0x00,
	DMA_SSP0_RX,
	DMA_SSP1_TX,
	DMA_SSP1_RX,
	DMA_ADC,
	DMA_I2S0,
	DMA_I2S1,
	DMA_DAC,
	DMA_UART0_TX,
	DMA_UART0_RX,
	DMA_UART1_TX,
	DMA_UART1_RX,
	DMA_UART2_TX,
	DMA_UART2_RX,
	DMA_UART3_TX,
	DMA_UART3_RX
} dma_perif_list;

typedef struct {
	unsigned int source;
	unsigned int destination;
	unsigned int next;
	unsigned int control;
} DMALLIO;


typedef struct
{
  __IO uint32_t DMACCSrcAddr;
  __IO uint32_t DMACCDestAddr;
  __IO uint32_t DMACCLLI;
  __IO uint32_t DMACCControl;
  __IO uint32_t DMACCConfig;
} LPC_GPDMACH_TypeDef;



extern void InitDMA();
extern void SetearCanal(int nrocanal,int tipotransf, int sourceper, int destper, int llist,DMALLIO *config, void (*callback)(void), void (*error)(void));
extern void IniciarTransferenciaDMA(int nrocanal);
extern void TerminarTransferenciaDMA(int nrocanal);
extern int isActivoDMA();
extern int isEnErrorDMA(int canal);
extern int isDMALibre(int canal);
extern void DMA_IRQHandler (void);


#endif /* DMA_H_ */
