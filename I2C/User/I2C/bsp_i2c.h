#ifndef __BSP_I2C_H_
#define __BSP_I2C_H_

#include "stm32f4xx_i2c.h"
#include <stdio.h>

// device
/*******************************************************/
#define DEVICE_ADDRESS						0xA0
#define I2C_OAR										0x0A
#define DEVICE_SPEED							400000
#define PAGE_SIZE									0x8
#define ROM_SIZE									0x100
#define DEFAULT_I2C_TIMEOUT_LIMIT	0x2000
#define I2C_TIMEOUT_RESET(x)			(x) = DEFAULT_I2C_TIMEOUT_LIMIT
#define TIMEOUT_TRIGGER()					I2C_Timeout_Trigger((uint8_t*)__FILE__, (uint8_t*)__func__, __LINE__)
//#define I2C_SLEEP
#define DWORD											sizeof(uint32_t)

// Pin define
/*******************************************************/
#define I2C_EEPROM								I2C1
#define I2C_EEPROM_CLK						RCC_APB1Periph_I2C1

#define I2C_EEPROM_SCL_GPIO_PORT	GPIOB
#define I2C_EEPROM_SCL_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define I2C_EEPROM_SCL_GPIO_PIN		GPIO_Pin_6
#define	I2C_EEPROM_SCL_AF					GPIO_AF_I2C1
#define	I2C_EEPROM_SCL_SOURCE			GPIO_PinSource6

#define I2C_EEPROM_SDA_GPIO_PORT	GPIOB
#define I2C_EEPROM_SDA_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define I2C_EEPROM_SDA_GPIO_PIN		GPIO_Pin_7
#define	I2C_EEPROM_SDA_AF					GPIO_AF_I2C1
#define	I2C_EEPROM_SDA_SOURCE			GPIO_PinSource7

typedef enum {WRITE = I2C_Direction_Transmitter, READ = I2C_Direction_Receiver} I2C_READ_WRITE;
typedef enum {UNINIT_HANDLER=0, XTIMEOUT, TIMEOUT}TIMEOUT_STATUS;
typedef void (*i2c_error_handler)(uint8_t*, uint8_t*, uint32_t);

void I2C_EEPROM_Config(void);
ErrorStatus I2C_EEPROM_Wait(void);
void I2C_EEPROM_Error_CB(i2c_error_handler cbfunc);
ErrorStatus I2C_EEPROM_Byte_Write(uint8_t* buf, uint8_t addr);
uint8_t I2C_EEPROM_Byte_Read(uint8_t addr);
ErrorStatus I2C_EEPROM_Page_Write(uint8_t* buf, uint8_t addr, uint8_t numbers);
ErrorStatus I2C_EEPROM_Sequential_Read(uint8_t* buf, uint8_t addr, uint16_t numbers);
ErrorStatus I2C_EEPROM_Sequential_Write(uint8_t* buf, uint8_t addr, uint16_t numbers);
ErrorStatus I2C_EEPROM_4Byte_Write(void* buf, uint8_t addr);
ErrorStatus I2C_EEPROM_4Byte_Read(void* buf, uint8_t addr);
ErrorStatus I2C_EEPROM_4Byte_Sequential_Write(void* buf, uint8_t addr, uint16_t numbers);
ErrorStatus I2C_EEPROM_4Byte_Sequential_Read(void* buf, uint8_t addr, uint16_t numbers);

#endif /* __BSP_I2C_H_ */
