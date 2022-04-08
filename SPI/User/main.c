#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_spi.h"

int main(void)
{  
	USART_CFG();	
	Flash_Init();
	uint32_t id = 0;
	DEBUG_PRINT("[SPI][FLASH W25Q256JV read JEDEC ID TEST]\n");
	id = Flash_Read_WRITE_JEDEC_ID();
	DEBUG_PRINT("[SPI][FLASH W25Q256JV JEDEC ID: 0x%X]\n", id);
	/* Infinite loop */
  while (1){

  }
}
