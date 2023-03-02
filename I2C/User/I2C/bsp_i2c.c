#include "bsp_i2c.h"

static uint32_t I2C_Event_Timeout=0;
i2c_error_handler i2c_err = NULL;

__INLINE static uint8_t I2C_Timeout_Trigger(uint8_t*, uint8_t*, uint32_t);

static void GPIO_I2C_EEPROM_Init(GPIO_TypeDef* GPIO, uint16_t PIN){
	
	GPIO_InitTypeDef GPIO_Initial_pars;
	
	GPIO_Initial_pars.GPIO_Pin = PIN;
	GPIO_Initial_pars.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Initial_pars.GPIO_OType = GPIO_OType_OD; // high-resistence and low
	GPIO_Initial_pars.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Initial_pars.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIO, &GPIO_Initial_pars);
}

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

__INLINE static uint8_t I2C_Timeout_Trigger(uint8_t* filename, uint8_t* func, uint32_t line){
	
	if(i2c_err == NULL){
		
		return UNINIT_HANDLER;
	}
	
	if(--I2C_Event_Timeout == 0){
		
		i2c_err(filename, func, line);
		
		I2C_TIMEOUT_RESET(I2C_Event_Timeout);
		
		return TIMEOUT;
	}
	
	return XTIMEOUT;
}

static void I2C_EEPROM_Error_Handler(uint8_t* filename, uint8_t* function, uint32_t line){
	
	if(!filename || !function){
		
		return;
	}
	
	printf("[Error] [Timeout][File: %s][Func: %s][Line: %d]\n",
				filename, function, line);
}

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
	
	/* Reset Timeout Ticks */
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	/* enroll callback function */
	I2C_EEPROM_Error_CB(i2c_err);
}

ErrorStatus I2C_EEPROM_Wait(void){

	do{
		
		I2C_GenerateSTART(I2C_EEPROM, ENABLE);
		I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
	}while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_ADDR) == RESET);
		
	I2C_ClearFlag(I2C_EEPROM, I2C_FLAG_AF);
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_BUSY) == SET){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	return SUCCESS;
}

void I2C_EEPROM_Error_CB(i2c_error_handler cbfunc){
	
	if(cbfunc == NULL){
		
		i2c_err = I2C_EEPROM_Error_Handler;
		return;
	}
	
	i2c_err = cbfunc;
}

ErrorStatus I2C_EEPROM_Byte_Write(uint8_t* buf, uint8_t addr){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	/* generate start bit */
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
	
	/* EV5 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
		
	/* send Address and read/write bits */
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
		
	/* EV6 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
		
	/* send EEPROM Internal Address */
	I2C_SendData(I2C_EEPROM, addr);
	
	/* EV8_2 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_SendData(I2C_EEPROM, *buf);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	return SUCCESS;
}

uint8_t I2C_EEPROM_Byte_Read(uint8_t addr){
	
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_BUSY) == SET){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_MSL) == SET){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
					
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
		
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
		
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_Cmd(I2C_EEPROM, ENABLE);

	I2C_SendData(I2C_EEPROM, addr);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
		
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, READ);
	
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	while(I2C_GetFlagStatus(I2C_EEPROM, I2C_FLAG_RXNE) == RESET){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}	
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_AcknowledgeConfig(I2C_EEPROM, DISABLE);

	uint8_t ret = (uint8_t)I2C_EEPROM->DR;
	
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
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
		
	/* send Address and read/write bits */
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);
	
	/* EV6 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	/* send EEPROM Internal Address */
	I2C_SendData(I2C_EEPROM, addr);
	
	/* EV8_2 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	do{
		
		I2C_SendData(I2C_EEPROM, *buf);
		++buf;
		while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR){
		
			if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
		}
		
		I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	}while(--numbers);
	
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	return SUCCESS;
}

ErrorStatus I2C_EEPROM_Sequential_Read(uint8_t* buf, uint8_t addr, uint16_t numbers){
	
	if(numbers > ROM_SIZE){
		
		return ERROR;
	}
	
	if(numbers == 0){
		
		return ERROR;
	}

	/* generate start bit */
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);

	/* EV5 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);

	/* send Address and read/write bits */
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, WRITE);

	/* EV6 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	I2C_Cmd(I2C_EEPROM, ENABLE);

	/* send EEPROM Internal Address */
	I2C_SendData(I2C_EEPROM, addr);
	
	/* EV8_2 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	/* generate start bit */
	I2C_GenerateSTART(I2C_EEPROM, ENABLE);
	
	/* EV5 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_MODE_SELECT) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);

	/* send Address and read/write bits */
	I2C_Send7bitAddress(I2C_EEPROM, DEVICE_ADDRESS, READ);

	/* EV6 */
	while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == ERROR){
		
		if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
	}
	
	I2C_TIMEOUT_RESET(I2C_Event_Timeout);
	
	/* read numbers of datas */
	for(; numbers > 0; --numbers){
		
		if(numbers == 1){
			
			/* generate a NACK if the next data is the last one */
			I2C_AcknowledgeConfig(I2C_EEPROM, DISABLE);
		}
		
		/* EV7 */
		while(I2C_CheckEvent(I2C_EEPROM, I2C_EVENT_MASTER_BYTE_RECEIVED) == ERROR){
			
			if(TIMEOUT_TRIGGER() == TIMEOUT) return ERROR;
		}
		
		I2C_TIMEOUT_RESET(I2C_Event_Timeout);
		
		/* get data from DR register */
		*buf = I2C_ReceiveData(I2C_EEPROM);
		buf++;
	}
		
	/* generate STOP Bit */
	I2C_GenerateSTOP(I2C_EEPROM, ENABLE);
	
	/* Enable ACK */
	I2C_AcknowledgeConfig(I2C_EEPROM, ENABLE);	
	
	return SUCCESS;
}

ErrorStatus I2C_EEPROM_Sequential_Write(uint8_t* buf, uint8_t addr, uint16_t numbers){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	if(numbers == 0){
		
		return ERROR;
	}
	
 /**
	* 254(addr) with 4(numbers) --> 254, 255, 256(illegal), 257(illegal)
	*/
	if((uint16_t)addr + numbers >= ROM_SIZE){
		
			numbers = ROM_SIZE - addr + 1;
	}
		
	uint8_t page_num = numbers / PAGE_SIZE;
	uint8_t page_remain = numbers % PAGE_SIZE;
	
	for(int i=0; i<page_num; i++){
				
		if(I2C_EEPROM_Page_Write(buf, addr, PAGE_SIZE) == ERROR) return ERROR;
		
		if(I2C_EEPROM_Wait() == ERROR) return ERROR;
				
		addr += PAGE_SIZE;
		buf += PAGE_SIZE;
	}
	
	for(int i=0; i<page_remain; i++){
			
		if(I2C_EEPROM_Byte_Write(buf, addr) == ERROR) return ERROR;
		addr++;
		buf++;
	}
	
	return SUCCESS;
}

ErrorStatus I2C_EEPROM_4Byte_Write(void* buf, uint8_t addr){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	return I2C_EEPROM_Page_Write(buf, addr, DWORD);
}

ErrorStatus I2C_EEPROM_4Byte_Read(void* buf, uint8_t addr){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	return I2C_EEPROM_Sequential_Read(buf, addr, DWORD);
}

ErrorStatus I2C_EEPROM_4Byte_Sequential_Write(void* buf, uint8_t addr, uint16_t numbers){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	numbers *= DWORD;
	
	if(numbers > ROM_SIZE){
		
			return ERROR;
	}
	
	return I2C_EEPROM_Sequential_Write(buf, addr, numbers);
}

ErrorStatus I2C_EEPROM_4Byte_Sequential_Read(void* buf, uint8_t addr, uint16_t numbers){
	
	if(buf == NULL){
		
		return ERROR;
	}
	
	numbers *= DWORD;
	
	if(numbers > ROM_SIZE){
		
			return ERROR;
	}
	
	return I2C_EEPROM_Sequential_Write(buf, addr, numbers);
}
