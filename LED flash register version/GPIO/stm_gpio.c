#include "stm_gpio.h"


void GPIO_Init(GPIO_Typedef gpio, uint8_t port){
	RCC_AHB1_ENR |= (1<<port);
	
	gpio->MODER = 0x00;
	gpio->OTYPER = 0x00;	
	gpio->OSPEEDER = 0x00;	
	gpio->PUPDR = 0x00;
}

void GPIO_Config(GPIO_Typedef gpio, uint8_t pin){
	gpio->MODER |= (1<<2*pin);
	gpio->OTYPER |= (0<<pin);
	gpio->OSPEEDER |= (2<<2*pin);
	gpio->PUPDR |= (1<<2*pin);	
}

void GPIO_SET(GPIO_Typedef gpio, uint8_t pin){
	gpio->BSRRL &= ~(1<<pin);
	gpio->BSRRL |= (1<<pin);
}

void GPIO_RESET(GPIO_Typedef gpio, uint8_t pin){
	gpio->BSRRH &= ~(1<<pin);
	gpio->BSRRH |= (1<<pin);
}

void GPIO_LED_Control(GPIO_Typedef gpio, uint8_t pin, bool output){
	GPIO_Config(gpio, pin);
	if(output)
		GPIO_SET(gpio, pin);
	else
		GPIO_RESET(gpio, pin);
}


