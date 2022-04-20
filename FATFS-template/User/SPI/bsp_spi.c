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
void Flash_Release_Power_Down(){
	FLASH_SPI_CSS_LOW();
	
	SPI_FLASH_SendByte(FLASH_RELEASE_POWER_DOWN);

	FLASH_SPI_CSS_HIGH();
	
}


/*
 * Flash Erase Sector
 */
void Flash_Erase_Sector(uint32_t addr){
	Flash_Write_Enable();
	Flash_Wait_For_Ready();

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
  * Read continuous sector
  */
void FLASH_Read_MutiData(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead){
  FLASH_SPI_CSS_LOW();
  SPI_FLASH_SendByte(FLASH_READ_DATA);
  SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  
  while (NumByteToRead--){
    *pBuffer = SPI_FLASH_SendByte(FLASH_DUMMY);
		pBuffer++;
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
	
	if(count > SPI_FLASH_PageSize){
		count = SPI_FLASH_PageSize;
	}
	
	while(count--){
		SPI_FLASH_SendByte(*data);
		data++;
	}
		
	FLASH_SPI_CSS_HIGH();
	Flash_Wait_For_Ready();
}


/*
 * Flash Page Write Muti-Datas
 */
void FLASH_Write_MutiData(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite){
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/* 差count個數據值，剛好可以對齊到頁地址 */
  count = SPI_FLASH_PageSize - Addr;	
	/* 計算出要寫多少整數頁 */
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/* mod運算求余，計算出剩余不滿一頁的字節數 */
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,則WriteAddr 剛好按頁對齊 aligned  */
  if (Addr == 0) {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) {
      Flash_Write_Data(WriteAddr, pBuffer, NumByteToWrite);
    }else{ /* NumByteToWrite > SPI_FLASH_PageSize */    
			/* 先把整數頁都寫了 */
      while (NumOfPage--){
        Flash_Write_Data(WriteAddr, pBuffer, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/* 若有多余的不滿一頁的數據，把它寫完 */
      Flash_Write_Data(WriteAddr, pBuffer, NumOfSingle);
    }
  }else{	/* 若地址與 SPI_FLASH_PageSize 不對齊 */
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0){
			/* 當前頁剩余的count個位置比NumOfSingle小，寫不完 */
      if (NumOfSingle > count){
        temp = NumOfSingle - count;
				
				/* 先寫滿當前頁 */
        Flash_Write_Data(WriteAddr, pBuffer, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/* 再寫剩余的數據 */
        Flash_Write_Data(WriteAddr, pBuffer, temp);
      }else{ /* 當前頁剩余的count個位置能寫完NumOfSingle個數據 */
        Flash_Write_Data(WriteAddr, pBuffer, NumByteToWrite);
      }
    }else{ /* NumByteToWrite > SPI_FLASH_PageSize */
			/* 地址不對齊多出的count分開處理，不加入這個運算 */
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      Flash_Write_Data(WriteAddr, pBuffer, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/* 把整數頁都寫了 */
      while (NumOfPage--){
        Flash_Write_Data(WriteAddr, pBuffer, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }if (NumOfSingle != 0) /* 若有多余的不滿一頁的數據，把它寫完 */
      {
        Flash_Write_Data(WriteAddr, pBuffer, NumOfSingle);
      }
    }
  }
}
	
/***********************************************************************************/

void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 使能 FLASH_SPI 及GPIO 时钟 */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO,SPI_FLASH_SPI_SCK_GPIO 时钟使能 */
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOF, ENABLE);

  /*!< SPI_FLASH_SPI 时钟使能 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
 
  //设置引脚复用
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5); 
  
  /*!< 配置 SPI_FLASH_SPI 引脚: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
	/*!< 配置 SPI_FLASH_SPI 引脚: MISO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  
	/*!< 配置 SPI_FLASH_SPI 引脚: MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOF, &GPIO_InitStructure);  

	/*!< 配置 SPI_FLASH_SPI 引脚: CS */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* 停止信号 FLASH: CS引脚高电平*/
  FLASH_SPI_CSS_HIGH();

  /* FLASH_SPI 模式配置 */
  // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(FLASH_SPI, &SPI_InitStructure);

  /* 使能 FLASH_SPI  */
  SPI_Cmd(FLASH_SPI, ENABLE);

	/* 使 SPI_FLASH 进入 4 字节地址模式 */
	uint8_t Temp;
	
	/*选择 FLASH: CS 低 */
	FLASH_SPI_CSS_LOW();
	
	/* 发送状态寄存器 3 命令 */
	SPI_FLASH_SendByte(0x15); 
	
	Temp = SPI_FLASH_SendByte(FLASH_DUMMY);
	
	/* 停止信号 FLASH: CS 高 */
	FLASH_SPI_CSS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*选择 FLASH: CS 低 */
		FLASH_SPI_CSS_LOW();
		
		/* 进入4字节模式 */
		SPI_FLASH_SendByte(0xB7);
		
		/* 停止信号 FLASH: CS 高 */
		FLASH_SPI_CSS_HIGH();
	}
}

