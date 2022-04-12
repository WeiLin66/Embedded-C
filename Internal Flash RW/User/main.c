#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"   
#include "./internalFlash/bsp_internalFlash.h"   


int main(void){
  Debug_USART_Config();
	LED_GPIO_Config();
 
	LED_BLUE;
	
	printf("Internal Flash Erase Begin~~~\r\n");
	FLASH_Unlock();
	
	if(InternalFlash_Test() == RESET){
		printf("Flash Operation Success!");
		LED_GREEN;
	}else{
		printf("Flash Operation Fail!");
		LED_RED;
	}
	
//	FLASH_EraseSector(FLASH_Sector_1, VoltageRange_3);
//	
//	printf("Internal Flash Write: %u\r\n", data);

//	FLASH_ProgramWord(ADDR_FLASH_SECTOR_1, data);
//	
//	printf("Read data from 0x08004000: %u\r\n", FLASH_M ADDR_FLASH_SECTOR_1);
	
	FLASH_Lock();
	
  while(1){} 

}




