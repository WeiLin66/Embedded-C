#include "bsp_usart.h"

uint8_t cmd[256]={0};
static uint8_t cmd_count=0;
uint8_t led_control=0;
uint8_t action=0;
char* table[] ={
	"flash",
	"led %s %d",
	"stop",
	"clear"
};

void console_cmd(){
	int arg;
	char led[10];
	if(strcmp((char*)cmd, table[0]) == 0){
		led_control = 1;
	}
	else if(sscanf((char*)cmd, table[1], led, &arg) == 2){
		if(strcmp(led, "r") == 0){
			if(arg == 0)
				GPIO_ResetBits(LED_PORT, LED_Red_PIN);
			else
				GPIO_SetBits(LED_PORT, LED_Red_PIN);
		}
		else if(strcmp(led, "g") == 0){
			if(arg == 0)
				GPIO_ResetBits(LED_PORT, LED_Green_PIN);
			else
				GPIO_SetBits(LED_PORT, LED_Green_PIN);
		}
		else if(strcmp(led, "b") == 0){
			if(arg == 0)
				GPIO_ResetBits(LED_PORT, LED_Blue_PIN);
			else
				GPIO_SetBits(LED_PORT, LED_Blue_PIN);
		}
//		led_control = 1;
	}
	else if(strcmp((char*)cmd, "stop") == 0){
		led_control = 0;
	}
	else if(strcmp((char*)cmd, "clear") == 0){
		GPIO_SetBits(LED_PORT, LED_Red_PIN);
		GPIO_SetBits(LED_PORT, LED_Green_PIN);
		GPIO_SetBits(LED_PORT, LED_Blue_PIN);
		led_control = 0;
	}
	else{
		printf("\nNo such command, please check again!\n");
	}
	
	memset(cmd, '0', 256);
} 

void cmd_receive(uint8_t tmp){
	if(tmp == 0x0D){
		cmd[cmd_count] = '\0';
		console_cmd();
		memset(cmd, '0', 256);
		cmd_count = 0;
	}
	else{
		cmd[cmd_count++] = tmp;
	}
}

void delay_ms(uint32_t delay){
	uint32_t mutipliter;
	
	mutipliter = SystemCoreClock/1000;
	mutipliter *= delay;
	while(mutipliter--);
}

void NVIC_Init_Config(){
	NVIC_InitTypeDef NVIC_Init_Structure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_Init_Structure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init_Structure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init_Structure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init_Structure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_Init_Structure);
	NVIC_EnableIRQ(USART1_IRQn);
}

static void GPIO_USART_Init(GPIO_TypeDef* GPIO, uint16_t PIN){
	GPIO_InitTypeDef GPIO_Initial_pars;
		
	GPIO_Initial_pars.GPIO_Pin = PIN;
	GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Initial_pars.GPIO_OType = GPIO_OType_PP;
	GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Initial_pars.GPIO_Speed = GPIO_Fast_Speed;
	
	GPIO_Init(GPIO, &GPIO_Initial_pars);
}

static void USART_Init_Config(){
	USART_InitTypeDef USART_Init_Structure;
	USART_Init_Structure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_Init_Structure.USART_WordLength = USART_WordLength_8b;
	USART_Init_Structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init_Structure.USART_Parity = USART_Parity_No;
	USART_Init_Structure.USART_StopBits = USART_StopBits_1;
	USART_Init_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &USART_Init_Structure);
}

void USART_CFG(void){
		/*Enable peripheral cloack*/
		RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);
		RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
		
		/*Initialize PA9, PA10*/
		GPIO_USART_Init(DEBUG_USART_TX_GPIO_PORT, DEBUG_USART_TX_PIN);
		GPIO_USART_Init(DEBUG_USART_RX_GPIO_PORT, DEBUG_USART_RX_PIN);
	
		/* GPIO AF Mode Setting*/
		GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT, GPIO_PinSource10, GPIO_AF_USART1);
	
		USART_Init_Config();
		NVIC_Init_Config();
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_Cmd(USART1, ENABLE);
}


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
  USART_SendData(pUSARTx,ch);
    
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);  
}

void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	int len = strlen(str);
	for(int i=0; i<len; i++){
		Usart_SendByte(pUSARTx, str[i]);
	}
	
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}


void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
  uint8_t temp_h, temp_l;
  
  temp_h = (ch&0XFF00)>>8;
  temp_l = ch&0XFF;
  
  USART_SendData(pUSARTx,temp_h); 
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
  
  USART_SendData(pUSARTx,temp_l); 
  while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);  
}

int fputc(int ch, FILE *f)
{
    USART_SendData(DEBUG_USART, (uint8_t) ch);
    
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);    
  
    return (ch);
}

int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(DEBUG_USART);
}

