#include "bsp_led.h"

uint32_t static mutipliter;

void assert_failed(uint8_t* file, uint32_t line){
    printf("file: %s\nline: %d\n", file, line);
}

void GPIO_Initial_API(GPIO_TypeDef* GPIO, uint16_t PIN){
		assert_param(GPIO);
		assert_param(PIN);
	
    GPIO_InitTypeDef GPIO_Initial_pars;
    	
    GPIO_Initial_pars.GPIO_Pin = PIN;
    GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Initial_pars.GPIO_OType = GPIO_OType_PP;
    GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Initial_pars.GPIO_Speed = GPIO_Fast_Speed;
    
    GPIO_Init(GPIO, &GPIO_Initial_pars);
	  GPIO_SetBits(GPIO, PIN);
}

void Delay_Init(void){
		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);
		
		mutipliter = RCC_Clocks.HCLK_Frequency / 4000000;
}

void DelayMs(uint32_t miles){
		miles = mutipliter * miles * 1000 -10;
		while(miles--);
}

void App_Init(void){
		/*Initialize clock timer*/
		Delay_Init();
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(LED_RCC, ENABLE);
		
		/*Initialize pin to set*/
		GPIO_Initial_API(LED_PORT, LED_Red_PIN);
		GPIO_Initial_API(LED_PORT, LED_Green_PIN);
		GPIO_Initial_API(LED_PORT, LED_Blue_PIN);
}

void LED_Flash(GPIO_TypeDef* GPIO, uint16_t PIN, uint16_t delay_time){
		assert_param(GPIO);
		assert_param(PIN);
		assert_param(delay_time);
	
		GPIO_ResetBits(GPIO, PIN);
		DelayMs(delay_time);
		GPIO_SetBits(GPIO, PIN);
		DelayMs(delay_time);
}

void App_Thread(void){
		LED_Flash(LED_PORT, LED_Red_PIN, DELAY_TIME);
		LED_Flash(LED_PORT, LED_Green_PIN, DELAY_TIME);
		LED_Flash(LED_PORT, LED_Blue_PIN, DELAY_TIME);	
}

