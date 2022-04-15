#ifndef __BSP_SPI_H_
#define __BSP_SPI_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stdint.h"

// RCC CLOCK
/*****************************************/
// SPI
#define SPI_RCC_CLK_CMD			     RCC_APB2PeriphClockCmd
#define SPI_RCC_CLK_PERIPH			 RCC_APB2Periph_SPI5
#define SPI_RCC_CLK_OP					 ENABLE 

// GPIO
#define GPIO_RCC_CLK_CMD				 RCC_AHB1PeriphClockCmd
#define GPIO_RCC_CLK_PERIPH			 RCC_AHB1Periph_GPIOF
#define GPIO_RCC_CLK_OP					 ENABLE

/*****************************************/

// PIN Define
/*****************************************/
// CSS
#define SPI_CSS_GPIO_PORT        GPIOF
#define SPI_CSS_GPIO_PIN         GPIO_Pin_6
#define SPI_CSS_GPIO_MODE  			 GPIO_Mode_OUT
#define SPI_CSS_GPIO_SPEED			 GPIO_Fast_Speed
#define SPI_CSS_GPIO_PUPD				 GPIO_PuPd_NOPULL
#define SPI_CSS_GPIO_OTYPE			 GPIO_OType_PP 

// SCLK
#define SPI_SCLK_GPIO_PORT       GPIOF
#define SPI_SCLK_GPIO_PIN        GPIO_Pin_7
#define SPI_SCLK_GPIO_MODE  		 GPIO_Mode_AF
#define SPI_SCLK_GPIO_PIN_SOURCE GPIO_PinSource7
#define SPI_SCLK_GPIO_AF				 GPIO_AF_SPI5


// MISO
#define SPI_MISO_GPIO_PORT       GPIOF
#define SPI_MISO_GPIO_PIN        GPIO_Pin_8
#define SPI_MISO_GPIO_MODE  		 GPIO_Mode_AF
#define SPI_MISO_GPIO_PIN_SOURCE GPIO_PinSource8
#define SPI_MSIO_GPIO_AF				 GPIO_AF_SPI5


// MOSI
#define SPI_MOSI_GPIO_PORT       GPIOF
#define SPI_MOSI_GPIO_PIN        GPIO_Pin_9
#define SPI_MOSI_GPIO_MODE  		 GPIO_Mode_AF
#define SPI_MOSI_GPIO_PIN_SOURCE GPIO_PinSource9
#define SPI_MOSI_GPIO_AF				 GPIO_AF_SPI5

// CS PIN Control
#define FLASH_SPI_CSS_LOW()			 GPIO_ResetBits(SPI_CSS_GPIO_PORT, SPI_CSS_GPIO_PIN)
#define FLASH_SPI_CSS_HIGH()		 GPIO_SetBits(SPI_CSS_GPIO_PORT, SPI_CSS_GPIO_PIN)

/*****************************************/

// SPI
/*****************************************/
#define FLASH_SPI 							 SPI5
#define FLASH_SPI_OP						 ENABLE	
#define FLASH_SPI_BRP						 SPI_BaudRatePrescaler_2
#define FLASH_SPI_CPHA					 SPI_CPHA_2Edge 
#define FLASH_SPI_CPOL					 SPI_CPOL_High 
#define FLASH_SPI_CRC						 7			 
#define FLASH_SPI_DATA_SIZE      SPI_DataSize_8b
#define FLASH_SPI_DIRECTION			 SPI_Direction_2Lines_FullDuplex 
#define FLASH_SPI_FIRSTBIT			 SPI_FirstBit_MSB
#define FLASH_SPI_MODE					 SPI_Mode_Master
#define FLASH_SPI_NSS						 SPI_NSS_Soft

/*****************************************/

// Flash Command
/*****************************************/
#define FLASH_DUMMY					 		 0xFF
#define FLASH_SPI_JEDEC					 0x9F
#define FLASH_WRITE_EN					 0x06
#define FLASH_READ_STATUS_REG		 0x05
#define FLASH_POWER_DOWN   			 0xB9
#define FLASH_RELEASE_POWER_DOWN 0xAB
#define FLASH_ERASE_SECTOR			 0x20
#define FLASH_READ_DATA					 0x03
#define FLASH_PAGE_PROGRAME			 0x02

#define FLASH_ID								 0xEF4019
#define SPI_FLASH_PageSize			 256
/*****************************************/

// Functions
/*****************************************/
void 														 Flash_Init(void);
uint32_t  											 Flash_Read_WRITE_JEDEC_ID(void);
uint32_t  											 SPI_FLASH_ReadID(void);
void  													 Flash_Write_Enable(void);
void 														 Flash_Wait_For_Ready(void);
void 														 Flash_Power_Down(void);
uint32_t  											 Flash_Release_Power_Down(void);
void 														 Flash_Erase_Sector(uint32_t addr);
void 														 Flash_Read_Data(uint32_t addr, uint8_t* buffer, uint32_t count);
void 														 Flash_Write_Data(uint32_t addr, uint8_t* data, int count);
void 														 FLASH_Read_MutiData(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
void 														 FLASH_Write_MutiData(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
/*****************************************/

#endif /* __BSP_SPI_H_ */
