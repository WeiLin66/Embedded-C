#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_systick.h"

int main(void)
{  
	 GPIO_flash_Init();
	 System_tick_Init(1);
  /* Infinite loop */
  while (1)
  {
//		GPIO_ResetBits(LED_PORT, LED_Red_PIN);
//		SystemTick_ms(100);
//		GPIO_SetBits(LED_PORT, LED_Red_PIN);
//		SystemTick_ms(100);

//		GPIO_ResetBits(LED_PORT, LED_Green_PIN);
//		SystemTick_ms(100);
//		GPIO_SetBits(LED_PORT, LED_Green_PIN);
//		SystemTick_ms(100);		

//		GPIO_ResetBits(LED_PORT, LED_Blue_PIN);
//		SystemTick_ms(100);
//		GPIO_SetBits(LED_PORT, LED_Blue_PIN);
//		SystemTick_ms(100);					
  }
}
