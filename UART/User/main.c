#include "stm32f4xx.h"
#include "bsp_usart.h"
#include "bsp_led.h"

int main(void)
{  
	USART_CFG();
	GPIO_flash_Init();
	
  /* Infinite loop */
	DEBUG_PRINT("[USART][LED Control]\n");
  while (1)
  {
		if(led_control){
			delay_ms(5);
			GPIO_Flash();
		}
  }
}
