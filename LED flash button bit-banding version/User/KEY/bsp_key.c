#include "bsp_key.h"

void GPIO_KEY_Init(GPIO_TypeDef* GPIO, uint16_t PIN){
    GPIO_InitTypeDef GPIO_Initial_pars;
    	
    GPIO_Initial_pars.GPIO_Pin = PIN;
    GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_DOWN;
    
    GPIO_Init(GPIO, &GPIO_Initial_pars);
	  GPIO_SetBits(GPIO, PIN);
}

void Key_Init(){
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(LED_KEY1_RCC, ENABLE);
		RCC_AHB1PeriphClockCmd(LED_KEY2_RCC, ENABLE);
	
		/*Initialize pin to set*/
		GPIO_KEY_Init(LED_KEY1_PORT, LED_KEY1_PIN);
		GPIO_KEY_Init(LED_KEY2_PORT, LED_KEY2_PIN);
}

uint8_t Key_scan(GPIO_TypeDef* GPIO, uint16_t PIN){
		if(GPIO_ReadInputDataBit(GPIO, PIN) == LED_ON){
			while(GPIO_ReadInputDataBit(GPIO, PIN) == LED_ON);
			
			return SET;
		}
		else{
			return RESET;
		}
}
