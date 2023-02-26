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
	GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initial_pars.GPIO_Speed = GPIO_Speed_50MHz;
	
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
	I2C_EEPROM_Init_Structure.I2C_OwnAddress1 = I2C_OAR;
	I2C_EEPROM_Init_Structure.I2C_Ack = I2C_Ack_Enable;
	I2C_EEPROM_Init_Structure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(I2C_EEPROM, &I2C_EEPROM_Init_Structure);
	I2C_Cmd(I2C_EEPROM, ENABLE);
	I2C_AcknowledgeConfig(I2C_EEPROM, ENABLE);
}

/**
 * @brief
 *
 */
void I2C_EEPROM_Config(void){

	/* Enable peripheral clock */
	RCC_AHB1PeriphClockCmd(I2C_EEPROM_SCL_GPIO_CLK|I2C_EEPROM_SDA_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(I2C_EEPROM_CLK, ENABLE);
	
	/* GPIO AF Mode Setting */
	GPIO_PinAFConfig(I2C_EEPROM_SCL_GPIO_PORT, I2C_EEPROM_SCL_SOURCE, I2C_EEPROM_SCL_AF);
	GPIO_PinAFConfig(I2C_EEPROM_SDA_GPIO_PORT, I2C_EEPROM_SDA_SOURCE, I2C_EEPROM_SCL_AF);

	/* Initialize I2C GPIO Pin */
	GPIO_I2C_EEPROM_Init(I2C_EEPROM_SCL_GPIO_PORT, I2C_EEPROM_SCL_GPIO_PIN);
	GPIO_I2C_EEPROM_Init(I2C_EEPROM_SDA_GPIO_PORT, I2C_EEPROM_SDA_GPIO_PIN);	
	
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
	
	/* generate start bit */
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
	
	/* EV5 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
		
	/* send Address and read/write bits */
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
		
	/* EV6 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
		
	/* send EEPROM Internal Address */
	I2C_SendData(I2C_EEPROM, addr);
	
	/* EV8_2 */
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
	
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_BUSY) == SET);
	
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_MSL) == SET);
					
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
		
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
		
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	I2C_Cmd(I2C_EEPROM, ENABLE);

	I2C_SendData(I2C_EEPROM, addr);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
		
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
	
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, READ);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR);
		
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_RXNE) == RESET);

	I2C_AcknowledgeConfig(I2C_EEPROM, DISABLE);

	uint8_t ret = I2C_ReceiveData(I2C_EEPROM);
	
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	I2C_AcknowledgeConfig(I2C_EEPROM, ENABLE);
	
	return ret;
}

ErrorStatus I2C_EEPROM_Page_Write(uint8_t* buf, uint8_t addr, uint8_t numbers){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	if(numbers <= 0 || numbers > PAGE_SIZE){
		
		return ERROR;
	}
	
	/* generate start bit */
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
	
	/* EV5 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
		
	/* send Address and read/write bits */
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
	
	/* EV6 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR);
	
	/* send EEPROM Internal Address */
	I2C_SendData(I2C_EEPROM, addr);
	
	/* EV8_2 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	
	do{
		
		I2C_SendData(I2C_EEPROM, *buf);
		++buf;
		while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);
	}while(--numbers);
	
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	return SUCCESS;
}