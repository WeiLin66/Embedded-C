#include "stm32f4xx.h"
#include "bsp_usart.h"
int main(void)
{  
	USART_CFG();
  /* Infinite loop */
  while (1)
  {
		USART_SendData(USART1, 'A');
		delay_ms(1000);
  }
}
