#include "bsp_led.h"

void GPIO_Initial_API(GPIO_TypeDef* GPIO, uint16_t PIN){
    GPIO_InitTypeDef GPIO_Initial_pars;
    	
    GPIO_Initial_pars.GPIO_Pin = PIN;
    GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Initial_pars.GPIO_OType = GPIO_OType_PP;
    GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Initial_pars.GPIO_Speed = GPIO_Fast_Speed;
    
    GPIO_Init(GPIO, &GPIO_Initial_pars);
	  GPIO_SetBits(GPIO, PIN);
}

void GPIO_flash_Init(void){
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
		
		/*Initialize pin to set*/
		GPIO_Initial_API(LED_PORT, LED_Red_PIN);
		GPIO_Initial_API(LED_PORT, LED_Green_PIN);
		GPIO_Initial_API(LED_PORT, LED_Blue_PIN);
}

void GPIO_Flash(){
	static int32_t count=0, cur_flash=0, blink;
	
	if(count >= 1000){
		if(blink%2 == 0){
			cur_flash %= 3;
			switch(cur_flash){
				case 0:
					GPIO_ResetBits(LED_PORT, LED_Red_PIN);
					GPIO_SetBits(LED_PORT, LED_Blue_PIN);
				break;
					
				case 1:
					GPIO_ResetBits(LED_PORT, LED_Green_PIN);
					GPIO_SetBits(LED_PORT, LED_Red_PIN);
				break;
				
				case 2:
					GPIO_ResetBits(LED_PORT, LED_Blue_PIN);
					GPIO_SetBits(LED_PORT, LED_Green_PIN);
				break;
			}
			cur_flash++;
		}
		else{
				GPIO_SetBits(LED_PORT, LED_Red_PIN);
				GPIO_SetBits(LED_PORT, LED_Green_PIN);
				GPIO_SetBits(LED_PORT, LED_Blue_PIN);
		}
			
		count = 0;
		blink++;
	}
	count++;
}
