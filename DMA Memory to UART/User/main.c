#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "dma_bsp.h"

int main(void)
{  
	USART_CFG();
	GPIO_flash_Init();
	dma_cfg();
	
  /* Infinite loop */
	DEBUG_PRINT("[USART][LED Control]:\n1. led r/g/b 0/1\n2. flash\n3. stop\n4. clear\n");
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
  while (1)
  {
		if(led_control){
			delay_ms(5);
			GPIO_Flash();
		}
  }
}
