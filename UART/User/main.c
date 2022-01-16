#include "stm32f4xx.h"
#include "bsp_usart.h"
int main(void)
{  
//	USART_CFG();
	USART_CFG();
  /* Infinite loop */
	DEBUG_PRINT("[USART][Port][%d]\n", 1);
  while (1)
  {
		Usart_SendString(USART1, "[USART] [TX]\n");
		delay_ms(1000);
  }
}
