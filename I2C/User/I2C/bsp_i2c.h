#ifndef __BSP_I2C_H_
#define __BSP_I2C_H_

#include "stm32f4xx_i2c.h"
#include <stdio.h>

// device
/*******************************************************/
#define DEVICE_ADDRESS						0xA0
#define I2C_OAR										0x0A
#define DEVICE_SPEED							400000
//#define I2C_SLEEP

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

void I2C_EEPROM_Config(void);
ErrorStatus I2C_EEPROM_Byte_Write(uint8_t* buf, uint8_t addr);
uint8_t I2C_EEPROM_Byte_Read(uint8_t addr);

#endif /* __BSP_I2C_H_ */
