#include "test_code.h"

/**
 * @brief
 *
 */
void I2C_Single_Read_Write(void){
	
	uint16_t test_addr = 0x02;
	uint8_t test_date = 0x1a;
	
	USART_CFG();
	
	Usart_SendString(DEBUG_USART, "This is a I2C single byte read & write example code!\n");
	
	I2C_EEPROM_Config();
	
	Usart_SendString(DEBUG_USART, "I2C Configuration OK!\n");
	
	I2C_EEPROM_Byte_Write(&test_date, test_addr);
	
	Usart_SendString(DEBUG_USART, "I2C Write Data\n");
	
	delay_ms(1000); // 1000ms
	
	printf("I2C Read Data: 0x%X\n", I2C_EEPROM_Byte_Read(test_addr));
	
}
