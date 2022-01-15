#include "bsp_usart.h"

void delay_ms(uint32_t delay){
	uint32_t mutipliter;
	
	mutipliter = SystemCoreClock/1000;
	mutipliter *= delay;
	while(mutipliter--);
}

static void GPIO_USART_Init(GPIO_TypeDef* GPIO, uint16_t PIN){
    GPIO_InitTypeDef GPIO_Initial_pars;
    	
    GPIO_Initial_pars.GPIO_Pin = PIN;
    GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Initial_pars.GPIO_OType = GPIO_OType_PP;
    GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Initial_pars.GPIO_Speed = GPIO_Fast_Speed;
    
    GPIO_Init(GPIO, &GPIO_Initial_pars);
	  GPIO_SetBits(GPIO, PIN);
}

static void USART_Init_Config(){
	USART_InitTypeDef USART_Init_Structure;
	USART_Init_Structure.USART_BaudRate = USART_BAUDRATE;
	USART_Init_Structure.USART_WordLength = USART_WordLength_8b;
	USART_Init_Structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init_Structure.USART_Parity = USART_Parity_No;
	USART_Init_Structure.USART_StopBits = USART_StopBits_1;
	USART_Init_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &USART_Init_Structure);
	RCC_AHB1PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

void USART_CFG(void){
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(USART_RCC, ENABLE);
		
		/*Initialize PA9, PA10*/
		GPIO_USART_Init(USART_PORT, USART_TX_PIN);
		GPIO_USART_Init(USART_PORT, USART_RX_PIN);
	
		/* GPIO AF Mode Setting*/
		GPIO_PinAFConfig(USART_PORT, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(USART_PORT, GPIO_PinSource10, GPIO_AF_USART1);
	
		/*USART Configuration*/
		USART_Init_Config();
		USART_Cmd(USART1, ENABLE);
}
