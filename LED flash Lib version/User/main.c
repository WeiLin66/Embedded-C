#include "bsp_led.h"
#include "stm32f4xx_rcc.h"
/**
 * @brief A simple project that allow LEDs flash respectively: red -> blue -> green
 * 				time delay: 500ms
 *
 */
int main(void)
{  
	/*Initialize GPIOH Pin 10*/
	GPIO_Initial_API();
	
	/*Output operation*/
	GPIO_ResetBits(LED_Red_PORT, LED_Red_RCC);
//	GPIO_SetBits(GPIOH, RCC_AHB1Periph_GPIOH);
	
  /* Infinite loop */
  while (1)
  {
  }
}
