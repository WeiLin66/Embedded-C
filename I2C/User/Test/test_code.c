#include "test_code.h"

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

static void I2C_EEPROM_Wait(void){
	
#ifdef I2C_SLEEP
	delay_ms(1);
#else
	do{
		
		I2C_GenerateSTART(I2C_EEPROM, ENABLE);
		I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
	}while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_ADDR) == RESET);
	
	I2C_ClearFlag(I2C_EEPROM, I2C_FLAG_AF);
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_BUSY) == SET);
#endif
}


/**
 * @brief
 *
 */
void I2C_Read_Write_Test(void){
	
	uint16_t test_addr = 0x50;
	
	USART_CFG();
	
	Usart_SendString(DEBUG_USART, "This is a I2C single byte read & write example code!\n");
	
	I2C_EEPROM_Config();
	
	Usart_SendString(DEBUG_USART, "I2C Configuration OK!\n");
#ifdef I2C_SINGLE_READ_WRITE
	uint8_t test_date = 0x50;
	
	I2C_EEPROM_Byte_Write(&test_date, test_addr);
	
	Usart_SendString(DEBUG_USART, "I2C Write Data\n");
	
	I2C_EEPROM_Wait();
	
	uint8_t ret = I2C_EEPROM_Byte_Read(test_addr);
	
	printf("I2C Read Data: 0x%X\n", ret);
#elif defined(I2C_PAGE_READ_WRITE)
	uint8_t test_data[8] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
	
	I2C_EEPROM_Page_Write(test_data, test_addr, PAGE_SIZE);
	
	Usart_SendString(DEBUG_USART, "I2C Write Data\n");
	
	I2C_EEPROM_Wait();
	
#else


#endif

}
