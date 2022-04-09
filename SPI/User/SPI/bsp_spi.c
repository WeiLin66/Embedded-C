#include "bsp_spi.h"

/*
 * GPIO Init function
 */
void Flash_GPIO_Init(){
	GPIO_InitTypeDef gpio_init;

	/* RCC Enable */
	GPIO_RCC_CLK_CMD(GPIO_RCC_CLK_PERIPH, GPIO_RCC_CLK_OP);
	
	/* Config AF Pin */
	GPIO_PinAFConfig(SPI_SCLK_GPIO_PORT, SPI_SCLK_GPIO_PIN_SOURCE, SPI_SCLK_GPIO_AF);
	GPIO_PinAFConfig(SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN_SOURCE, SPI_MSIO_GPIO_AF);
	GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN_SOURCE, SPI_MOSI_GPIO_AF);	
	
	/* configure css pin */
	gpio_init.GPIO_Pin = SPI_CSS_GPIO_PIN;
	gpio_init.GPIO_Mode = SPI_CSS_GPIO_MODE;
	gpio_init.GPIO_Speed = SPI_CSS_GPIO_SPEED;
	gpio_init.GPIO_PuPd = SPI_CSS_GPIO_PUPD;
	gpio_init.GPIO_OType = SPI_CSS_GPIO_OTYPE;
	GPIO_Init(SPI_CSS_GPIO_PORT, &gpio_init);
	
	/* configure sclk pin */
	gpio_init.GPIO_Pin = SPI_SCLK_GPIO_PIN;
	gpio_init.GPIO_Mode = SPI_SCLK_GPIO_MODE;
	GPIO_Init(SPI_SCLK_GPIO_PORT, &gpio_init);

	/* configure miso pin */
	gpio_init.GPIO_Pin = SPI_MISO_GPIO_PIN;
	GPIO_Init(SPI_MISO_GPIO_PORT, &gpio_init);
	
  /* configure mosi pin */
	gpio_init.GPIO_Pin = SPI_MOSI_GPIO_PIN;
	GPIO_Init(SPI_MOSI_GPIO_PORT, &gpio_init);
}


/*
 * SPI Init function
 */
void Flash_SPI_Init(){
	SPI_InitTypeDef spi_init;
	
	/* RCC Enable */
	SPI_RCC_CLK_CMD(SPI_RCC_CLK_PERIPH, SPI_RCC_CLK_OP);
	
	/* Stop Flash Signal */
	FLASH_SPI_CSS_HIGH();

	/* configure SPI */
	spi_init.SPI_BaudRatePrescaler = FLASH_SPI_BRP;
	spi_init.SPI_CPHA = FLASH_SPI_CPHA;
	spi_init.SPI_CPOL = FLASH_SPI_CPOL;
	spi_init.SPI_CRCPolynomial = FLASH_SPI_CRC;
	spi_init.SPI_DataSize = FLASH_SPI_DATA_SIZE;
	spi_init.SPI_Direction = FLASH_SPI_DIRECTION;
	spi_init.SPI_FirstBit = FLASH_SPI_FIRSTBIT;
	spi_init.SPI_Mode = FLASH_SPI_MODE;
	spi_init.SPI_NSS = FLASH_SPI_NSS;
	SPI_Init(FLASH_SPI, &spi_init);
	
	/* Enable SPI Flash */
	SPI_Cmd(FLASH_SPI, FLASH_SPI_OP);
}


/*
 * API interface
 */
void Flash_Init(){
	Flash_GPIO_Init();
	Flash_SPI_Init();
}


/*
 * Flash Send & Read operation
 */
static uint8_t SPI_FLASH_SendByte(uint8_t byte){

  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET){}

  SPI_I2S_SendData(FLASH_SPI, byte);

  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET){}

  return SPI_I2S_ReceiveData(FLASH_SPI);
}


/**
 * Read Flash data
 */
static uint8_t SPI_FLASH_ReadByte(){
	return SPI_FLASH_SendByte(FLASH_DUMMY);
}	


/*
 * FLASH W25Q256JV read/write JEDEC ID command
 */
uint32_t Flash_Read_WRITE_JEDEC_ID(){
	uint32_t ManufacturerID, MemoryID, CapacityID;
	
	/* CS LOW */
	FLASH_SPI_CSS_LOW();
				
	/* send command to flash */	
	SPI_FLASH_SendByte(FLASH_SPI_JEDEC);
	
	/* read data */
	ManufacturerID = SPI_FLASH_ReadByte();
	MemoryID = SPI_FLASH_ReadByte();
	CapacityID = SPI_FLASH_ReadByte();
	
	/* CS HIGH */	
	FLASH_SPI_CSS_HIGH();

	return (ManufacturerID << 16) | (MemoryID << 8) | CapacityID;
}


/*
 * Flash Write Enable
 */
void Flash_Write_Enable(){
	FLASH_SPI_CSS_LOW();
	SPI_FLASH_SendByte(FLASH_WRITE_EN);
	FLASH_SPI_CSS_HIGH();
}


/*
 * Wait until BUSY & WEL become 0
 */
void Flash_Wait_For_Ready(){
	uint8_t status=0;
	FLASH_SPI_CSS_LOW();
	SPI_FLASH_SendByte(FLASH_READ_STATUS_REG);
	
	do{
		status = SPI_FLASH_SendByte(FLASH_DUMMY);
	}while((status & 0x01) == SET);
	
	FLASH_SPI_CSS_HIGH();
}


/*
 * Flash Power Down
 */
void Flash_Power_Down(){
	FLASH_SPI_CSS_LOW();
	SPI_FLASH_SendByte(FLASH_POWER_DOWN);
	FLASH_SPI_CSS_HIGH();
}


/*
 * Flash Release Power Down
 */
uint32_t Flash_Release_Power_Down(){
	uint32_t rev;
	FLASH_SPI_CSS_LOW();
	SPI_FLASH_SendByte(FLASH_RELEASE_POWER_DOWN);
	
	SPI_FLASH_SendByte(FLASH_DUMMY);
	SPI_FLASH_SendByte(FLASH_DUMMY);
	SPI_FLASH_SendByte(FLASH_DUMMY);	
	rev = SPI_FLASH_SendByte(FLASH_DUMMY);
	FLASH_SPI_CSS_HIGH();
	
	return rev;
}


/*
 * Flash Erase Sector
 */
void Flash_Erase_Sector(uint32_t addr){
	Flash_Write_Enable();
	
	FLASH_SPI_CSS_LOW();
	SPI_FLASH_SendByte(FLASH_ERASE_SECTOR);
	SPI_FLASH_SendByte((addr >> 24) & 0xff);
	SPI_FLASH_SendByte((addr >> 16) & 0xff);
	SPI_FLASH_SendByte((addr >> 8) & 0xff);
	SPI_FLASH_SendByte(addr & 0xff);
	FLASH_SPI_CSS_HIGH();
	
	Flash_Wait_For_Ready();
}


/*
 * Flash Read Data
 */
void Flash_Read_Data(uint32_t addr, uint8_t* buffer, uint32_t count){
	Flash_Wait_For_Ready();
	FLASH_SPI_CSS_LOW();
	SPI_FLASH_SendByte(FLASH_READ_DATA);
	SPI_FLASH_SendByte((addr >> 24) & 0xff);
	SPI_FLASH_SendByte((addr >> 16) & 0xff);
	SPI_FLASH_SendByte((addr >> 8) & 0xff);
	SPI_FLASH_SendByte(addr & 0xff);
	
  while (count--){
    *(buffer++) = SPI_FLASH_SendByte(FLASH_DUMMY);
  }
	
	FLASH_SPI_CSS_HIGH();
}


/*
 * Flash Page Write
 */
void Flash_Write_Data(uint32_t addr, uint8_t* data, int count){	
	Flash_Write_Enable();	
	FLASH_SPI_CSS_LOW();
	SPI_FLASH_SendByte(FLASH_PAGE_PROGRAME);
	SPI_FLASH_SendByte((addr >> 24) & 0xff);
	SPI_FLASH_SendByte((addr >> 16) & 0xff);
	SPI_FLASH_SendByte((addr >> 8) & 0xff);
	SPI_FLASH_SendByte(addr & 0xff);
	
	while(count--){
		SPI_FLASH_SendByte(*data);
		data++;
	}
		
	FLASH_SPI_CSS_HIGH();
	Flash_Wait_For_Ready();
}
	
