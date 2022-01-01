#include "bsp_led.h"
#include "stm32f4xx_rcc.h"
/**
 * @brief A simple project that allow LEDs flash respectively: red -> blue -> green
 * 				time delay: 500ms
 *
 */
int main(void)
{  
	/*Initialize LEDs*/
	App_Init();
	
  /* Infinite loop */
  while (1)
  {
			App_Thread();
  }
}
