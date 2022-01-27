#include "dma_bsp.h"

/*flash*/
const uint8_t source[] = "A type of reptile that became extinct about 65,000,000 years ago. There were many different types of dinosaur, some of which were extremely large.\n";

																 
void dma_cfg(){
	DMA_InitTypeDef DMA_Init_Structure;
	__IO uint32_t timeout = TIMEOUT;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA_STREAM);
	
	while(DMA_GetCmdStatus(DMA_STREAM) == ENABLE){}
		
	/*DMA Channel*/
	DMA_Init_Structure.DMA_Channel = DMA_CHANNEL; //support other channels
	/*source address*/
	DMA_Init_Structure.DMA_PeripheralBaseAddr = (uint32_t)(USART1_BASE + 0x0004);
	/*destination address*/
	DMA_Init_Structure.DMA_Memory0BaseAddr = (uint32_t)source;
	/*transfer modes*/
	DMA_Init_Structure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	/*data total number*/
	DMA_Init_Structure.DMA_BufferSize =  strlen((char*)source);
	/*PeripheralInc increase automatically*/
	DMA_Init_Structure.DMA_PeripheralInc = DMA_DIR_MemoryToPeripheral;
	/*PeripheralInc increase automatically*/	
	DMA_Init_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/*source data size*/
	DMA_Init_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // DMA_PeripheralDataSize_HalfWord, DMA_PeripheralDataSize_Byte, DMA_PeripheralDataSize_Word
	/*destination data size*/
	DMA_Init_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // DMA_MemoryDataSize_Word, DMA_MemoryDataSize_Byte
	/*circular mode or not*/
	DMA_Init_Structure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal, DMA_Mode_Circular
	/*priority*/
	DMA_Init_Structure.DMA_Priority = DMA_Priority_Medium;
	/*FIFO setting*/
	DMA_Init_Structure.DMA_FIFOMode = DMA_FIFOMode_Enable; // DMA_FIFOMode_Disable, DMA_FIFOMode_Enable
	DMA_Init_Structure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull; // DMA_FIFOThreshold_Full, DMA_FIFOThreshold_1QuarterFull
	DMA_Init_Structure.DMA_MemoryBurst = DMA_MemoryBurst_INC4; // DMA_MemoryBurst_Single, DMA_MemoryBurst_INC4
	DMA_Init_Structure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // DMA_PeripheralBurst_INC4, DMA_PeripheralBurst_Single
	
	DMA_Init(DMA_STREAM, &DMA_Init_Structure);	
	DMA_ClearFlag(DMA_STREAM, DMA_CLEAR_FLAG);
	DMA_Cmd(DMA_STREAM, ENABLE);
	
	while(DMA_GetCmdStatus(DMA_STREAM) == DISABLE){
		timeout--;
	}
		
	if(timeout == 0){
		while(1){
			GPIO_Flash();
		}
	}
}

/* comapre two data*/
uint8_t comapreData(const uint8_t* src, uint8_t* dest, uint32_t len){
	if(src == NULL || dest == NULL)
		return 2;
	

	for(int i=0; i<len; i++){
		if(src[i] != dest[i])
			return 0;
	}
	return 1;

}

void Status_LED(uint8_t flag){
	if(flag == 1){
		/* green light */
		GPIO_ResetBits(LED_PORT,LED_Green_PIN);
	}
	else if(flag == 0){
		/* red light */
		GPIO_ResetBits(LED_PORT,LED_Red_PIN);
	}
	else{
		/* blue light */
		GPIO_ResetBits(LED_PORT,LED_Blue_PIN);			
	}	
}
