#include "bsp_led.h"
void GPIO_Initial_API(uint16_t LED){	
    BIT_GPIOH(BIT_MODER, (LED*2)) = SET;
		BIT_GPIOH(BIT_MODER, ((LED*2)+1)) = RESET;
	
		BIT_GPIOH(BIT_TYPER, LED) = GPIO_OType_PP;
	
		BIT_GPIOH(BIT_SPEEDER, (LED*2)) = SET;
		BIT_GPIOH(BIT_SPEEDER, ((LED*2)+1)) = SET;

		BIT_GPIOH(BIT_PuPr, (LED*2)) = SET;
		BIT_GPIOH(BIT_PuPr, ((LED*2)+1)) = RESET;
	
		BIT_GPIOH(BIT_ODR, LED) = SET;
}

void LED_Init(void){
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
		
		/*Initialize pin to set*/
		GPIO_Initial_API(LED1);
		GPIO_Initial_API(LED2);
		GPIO_Initial_API(LED3);
}

void LED_button_toggle(uint16_t LED, uint8_t key){
	if(key){
		BIT_GPIOH(BIT_ODR, LED) = BIT_GPIOH(BIT_IDR, LED) ^ 0x01;
	}
}

