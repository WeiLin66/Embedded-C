#include "bsp_led.h"
void GPIO_Initial_API(GPIO_TypeDef* GPIO, uint16_t PIN){	
    GPIO_InitTypeDef GPIO_Initial_pars;
    	
    GPIO_Initial_pars.GPIO_Pin = PIN;
    GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Initial_pars.GPIO_OType = GPIO_OType_PP;
    GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Initial_pars.GPIO_Speed = GPIO_Fast_Speed;
    
    GPIO_Init(GPIO, &GPIO_Initial_pars);
		GPIO_SetBits(GPIO, PIN);
}

void LED_Init(void){
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
	
		/*Initialize pin to set*/
		GPIO_Initial_API(LED_PORT, LED_PIN1);
		GPIO_Initial_API(LED_PORT, LED_PIN2);	
}

void LED_button_toggle(GPIO_TypeDef* GPIO, uint16_t PIN){
		GPIO_ToggleBits(GPIO, PIN);
}

