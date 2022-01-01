#include "bsp_led.h"
#include "bsp_key.h"
#include "stm32f4xx_rcc.h"
/**
 * @brief A simple project that allow User to press the botton and toggle the LEDs
 * 				botton GPIOA PIN0 : Red
 *        botton GPIOC PIN13: Green
 *        bit-banding version
 *
 */
int main(void)
{ 
	LED_Init();
	Key_Init();
  /* Infinite loop */
  while (1)
  {
		LED_button_toggle(LED1, Key_scan(LED_KEY1_PORT, LED_KEY1_PIN));
		LED_button_toggle(LED3, Key_scan(LED_KEY2_PORT, LED_KEY2_PIN));
  }
}
