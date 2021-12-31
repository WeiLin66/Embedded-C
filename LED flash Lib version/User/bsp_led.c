#include "bsp_led.h"


void GPIO_Initial_API(void){
    GPIO_InitTypeDef GPIO_Initial_pars;
    
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);
	
    GPIO_Initial_pars.GPIO_Pin = LED_Red_PIN;
    GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Initial_pars.GPIO_OType = GPIO_OType_PP;
    GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Initial_pars.GPIO_Speed = GPIO_Fast_Speed;
    
    GPIO_Init(GPIOH, &GPIO_Initial_pars);
}
