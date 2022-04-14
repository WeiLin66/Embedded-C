#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_spi.h"

uint32_t id = 0;
uint8_t buf[4096] = {0};
uint8_t data[4096] = {0};
	
int main(void){
  /* Init() */  
	USART_CFG();	
	Flash_Init();
	
	for(int i=0; i<256; i++){
		data[i] = i;
	}
	
	DEBUG_PRINT("[SPI][FLASH W25Q256JV READ JEDEC ID TEST]\n");

	/* Confirm Flash ID */
	id = Flash_Read_WRITE_JEDEC_ID();
	DEBUG_PRINT("[SPI][FLASH W25Q256JV JEDEC ID: 0x%X]\n", id);
	
	Flash_Erase_Sector(0x00);
	Flash_Write_Data(0x00, data, 256);
	Flash_Read_Data(0x00, buf, 4096);
	
	DEBUG_PRINT("[SPI][FLASH W25Q256JV READ TEST][FINISH]\n\n");
	for(int i=0; i<16; i++){
		for(int j=0; j<256; j++){
			DEBUG_PRINT("0x%X ", buf[256*i+j]);
		}
		DEBUG_PRINT("\r\n");
	}
	/* Infinite loop */
  while (1){

  }
}
