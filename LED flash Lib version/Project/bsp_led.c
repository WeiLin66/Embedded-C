#include "bsp_led.h"


void GPIO_Init(){
	/*still not done yet...*/
	GPIO_InitTypeDef GPIO_Initial_pars;
	
	GPIO_Initial_pars.GPIO_Pin = 0;
	GPIO_Initial_pars.GPIO_Mode = 0;
	GPIO_Initial_pars.GPIO_OType = 0;
	GPIO_Initial_pars.GPIO_PuPd = 0;
	GPIO_Initial_pars.GPIO_Speed = 0;
	
	GPIO_Init(GPIOH, &GPIO_Initial_pars);
}
	