#include "stm32f4xx.h"
#include "dma_bsp.h"

const uint8_t source[TEMP_SIZE] = {'a', 'p', 'p', 'l', 'e',
																   'a', 'p', 'p', 'l', 'e',
																   'a', 'p', 'p', 'l', 'e',
																   'a', 'p', 'p', 'l', 'e'};

uint8_t destinattion[TEMP_SIZE];
																 
void dma_cfg(){
	DMA_InitTypeDef DMA_Init_Structure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA_STREAM);
	
	/*DMA Channel*/
	DMA_Init_Structure.DMA_Channel = DMA_CHANNEL;
	/*source address*/
	DMA_Init_Structure.DMA_PeripheralBaseAddr = (uint32_t)source;
	/*destination address*/
	DMA_Init_Structure.DMA_Memory0BaseAddr = (uint32_t)destinattion;
	/*transfer modes*/
	DMA_Init_Structure.DMA_DIR = DMA_DIR_MemoryToMemory;
	/*data total number*/
	DMA_Init_Structure.DMA_BufferSize = TEMP_SIZE;
	/*PeripheralInc increase automatically*/
	DMA_Init_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	/*PeripheralInc increase automatically*/	
	DMA_Init_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/*source data size*/
	DMA_Init_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*destination data size*/
	DMA_Init_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/*circular mode or not*/
	DMA_Init_Structure.DMA_Mode = DMA_Mode_Normal;
	/*priority*/
	DMA_Init_Structure.DMA_Priority = DMA_Priority_Medium;
	/*FIFO setting*/
	DMA_Init_Structure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_Init_Structure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_Init_Structure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Init_Structure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA_STREAM, &DMA_Init_Structure);
	DMA_Cmd(DMA_STREAM, ENABLE);
	
}
int main(void)
{  
	dma_cfg();
     
  /* Infinite loop */
  while (1)
  {
  }
}
