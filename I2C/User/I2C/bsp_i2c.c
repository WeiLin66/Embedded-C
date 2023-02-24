#include "bsp_i2c.h"

/**
 * @brief funciton for GPIO Initialization
 *
 */
static void GPIO_I2C_EEPROM_Init(GPIO_TypeDef* GPIO, uint16_t PIN){
	
	GPIO_InitTypeDef GPIO_Initial_pars;
	
	GPIO_Initial_pars.GPIO_Pin = PIN;
	GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Initial_pars.GPIO_OType = GPIO_OType_OD; // high-resistence
	GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Initial_pars.GPIO_Speed = GPIO_Fast_Speed;
	
	GPIO_Init(GPIO, &GPIO_Initial_pars);
}

/**
 * @brief
 *
 */
static void I2C_EEPROM_Init(){
	
	I2C_InitTypeDef I2C_EEPROM_Init_Structure;
	
	I2C_EEPROM_Init_Structure.I2C_ClockSpeed = DEVICE_SPEED;
	I2C_EEPROM_Init_Structure.I2C_Mode = I2C_Mode_I2C;
	I2C_EEPROM_Init_Structure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_EEPROM_Init_Structure.I2C_OwnAddress1 = DEVICE_ADDRESS;
	I2C_EEPROM_Init_Structure.I2C_Ack = I2C_Ack_Enable;
	I2C_EEPROM_Init_Structure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C_EEPROM, &I2C_EEPROM_Init_Structure);
	
	I2C_Cmd(I2C_EEPROM, ENABLE);
}

/**
 * @brief
 *
 */
void I2C_EEPROM_Config(void){

	/* Enable peripheral clock */
	RCC_AHB1PeriphClockCmd(I2C_EEPROM_SCL_GPIO_CLK|I2C_EEPROM_SDA_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(I2C_EEPROM_CLK, ENABLE);

	/* Initialize I2C GPIO Pin */
	GPIO_I2C_EEPROM_Init(I2C_EEPROM_SCL_GPIO_PORT, I2C_EEPROM_SCL_GPIO_PIN);
	GPIO_I2C_EEPROM_Init(I2C_EEPROM_SDA_GPIO_PORT, I2C_EEPROM_SDA_GPIO_PIN);	
	
	/* GPIO AF Mode Setting */
	GPIO_PinAFConfig(I2C_EEPROM_SCL_GPIO_PORT, I2C_EEPROM_SCL_SOURCE, I2C_EEPROM_SCL_AF);
	GPIO_PinAFConfig(I2C_EEPROM_SDA_GPIO_PORT, I2C_EEPROM_SDA_SOURCE, I2C_EEPROM_SCL_AF);
	
	/* Enable I2C */
	I2C_EEPROM_Init();
}

/**
 * @brief
 *
 */
ErrorStatus I2C_EEPROM_Byte_Write(uint8_t* buf, uint8_t addr){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
	
	while(I2C_GetITStatus(I2C_EEPROM, I2C_IT_SB) == RESET);
		
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
		
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	I2C_SendData(I2C_EEPROM, addr);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	
	I2C_SendData(I2C_EEPROM, *buf);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	return SUCCESS;
}

/**
 * @brief
 *
 */
uint8_t I2C_EEPROM_Byte_Read(uint8_t addr){
	
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
	
	while(I2C_GetITStatus(I2C_EEPROM, I2C_IT_SB) == RESET);
	
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	I2C_SendData(I2C_EEPROM, addr);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
	
	while(I2C_GetITStatus(I2C_EEPROM, I2C_IT_SB) == RESET);
	
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, READ);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR);
	
	I2C_AcknowledgeConfig(I2C_EEPROM, DISABLE);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR);
	
	uint8_t ret = I2C_ReceiveData(I2C_EEPROM);
	
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	return ret;
}
