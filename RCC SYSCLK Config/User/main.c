#include "bsp_rccclkconfig.h"
#include "bsp_led.h"

int main(void)
{  
	HSE_SetSysCLK(SYSCLK_M,SYSCLK_N,SYSCLK_P,SYSCLK_Q);
  App_Init();  
  /* Infinite loop */
  while (1)
  {
		App_Thread();
  }
}
