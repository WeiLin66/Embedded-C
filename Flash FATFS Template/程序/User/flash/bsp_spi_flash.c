 /**
  ******************************************************************************
  * @file    bsp_spi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   spi flash 底層應用函數bsp 
  ******************************************************************************
  * @attention
  *
  * 實驗平台:野火STM32 F429 開發板
  * 論壇    :http://www.firebbs.cn
  * 淘寶    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./flash/bsp_spi_flash.h"


static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

 /**
  * @brief  SPI_FLASH初始化
  * @param  無
  * @retval 無
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 使能 FLASH_SPI 及GPIO 時鐘 */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO,SPI_FLASH_SPI_SCK_GPIO 時鐘使能 */
  RCC_AHB1PeriphClockCmd (FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK|FLASH_SPI_MOSI_GPIO_CLK|FLASH_CS_GPIO_CLK, ENABLE);

  /*!< SPI_FLASH_SPI 時鐘使能 */
  FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);
 
  //設置引腳覆用
  GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT,FLASH_SPI_SCK_PINSOURCE,FLASH_SPI_SCK_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT,FLASH_SPI_MISO_PINSOURCE,FLASH_SPI_MISO_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT,FLASH_SPI_MOSI_PINSOURCE,FLASH_SPI_MOSI_AF); 
  
  /*!< 配置 SPI_FLASH_SPI 引腳: SCK */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_FLASH_SPI 引腳: MISO */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< 配置 SPI_FLASH_SPI 引腳: MOSI */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/*!< 配置 SPI_FLASH_SPI 引腳: CS */
  GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);

  /* 停止信號 FLASH: CS引腳高電平*/
  SPI_FLASH_CS_HIGH();

  /* FLASH_SPI 模式配置 */
  // FLASH芯片 支持SPI模式0及模式3，據此設置CPOL CPHA
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

	/* 使 SPI_FLASH 進入 4 字節地址模式 */
	SPI_FLASH_Mode_Init();
}

 /**
  * @brief  擦除FLASH扇區
  * @param  SectorAddr：要擦除的扇區地址
  * @retval 無
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* 發送FLASH寫使能命令 */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* 擦除扇區 */
  /* 選擇FLASH: CS低電平 */
  SPI_FLASH_CS_LOW();
  /* 發送扇區擦除指令*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*發送擦除扇區地址的高8位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  /*發送擦除扇區地址的中前8位*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* 發送擦除扇區地址的中後8位 */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* 發送擦除扇區地址的低8位 */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* 停止信號 FLASH: CS 高電平 */
  SPI_FLASH_CS_HIGH();
  /* 等待擦除完畢*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  擦除FLASH扇區，整片擦除
  * @param  無
  * @retval 無
  */
void SPI_FLASH_BulkErase(void)
{
  /* 發送FLASH寫使能命令 */
  SPI_FLASH_WriteEnable();

  /* 整塊 Erase */
  /* 選擇FLASH: CS低電平 */
  SPI_FLASH_CS_LOW();
  /* 發送整塊擦除指令*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* 停止信號 FLASH: CS 高電平 */
  SPI_FLASH_CS_HIGH();

  /* 等待擦除完畢*/
  SPI_FLASH_WaitForWriteEnd();
}



 /**
  * @brief  對FLASH按頁寫入數據，調用本函數寫入數據前需要先擦除扇區
  * @param	pBuffer，要寫入數據的指針
  * @param WriteAddr，寫入地址
  * @param  NumByteToWrite，寫入數據長度，必須小於等於SPI_FLASH_PerWritePageSize
  * @retval 無
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  /* 發送FLASH寫使能命令 */
  SPI_FLASH_WriteEnable();

  /* 選擇FLASH: CS低電平 */
  SPI_FLASH_CS_LOW();
  /* 寫頁寫指令*/
  SPI_FLASH_SendByte(W25X_PageProgram);
	
  /*發送寫地址的高8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
  /*發送寫地址的中前8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*發送寫地址的中後8位*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*發送寫地址的低8位*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* 寫入數據*/
  while (NumByteToWrite--)
  {
    /* 發送當前要寫入的字節數據 */
    SPI_FLASH_SendByte(*pBuffer);
    /* 指向下一字節數據 */
    pBuffer++;
  }

  /* 停止信號 FLASH: CS 高電平 */
  SPI_FLASH_CS_HIGH();

  /* 等待寫入完畢*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  對FLASH寫入數據，調用本函數寫入數據前需要先擦除扇區
  * @param	pBuffer，要寫入數據的指針
  * @param  WriteAddr，寫入地址
  * @param  NumByteToWrite，寫入數據長度
  * @retval 無
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod運算求余，若writeAddr是SPI_FLASH_PageSize整數倍，運算結果Addr值為0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*差count個數據值，剛好可以對齊到頁地址*/
  count = SPI_FLASH_PageSize - Addr;	
	/*計算出要寫多少整數頁*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod運算求余，計算出剩余不滿一頁的字節數*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,則WriteAddr 剛好按頁對齊 aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*先把整數頁都寫了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*若有多余的不滿一頁的數據，把它寫完*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* 若地址與 SPI_FLASH_PageSize 不對齊  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*當前頁剩余的count個位置比NumOfSingle小，寫不完*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*先寫滿當前頁*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*再寫剩余的數據*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*當前頁剩余的count個位置能寫完NumOfSingle個數據*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*地址不對齊多出的count分開處理，不加入這個運算*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*把整數頁都寫了*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*若有多余的不滿一頁的數據，把它寫完*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  讀取FLASH數據
  * @param 	pBuffer，存儲讀出數據的指針
  * @param   ReadAddr，讀取地址
  * @param   NumByteToRead，讀取數據長度
  * @retval 無
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
  /* 選擇FLASH: CS低電平 */
  SPI_FLASH_CS_LOW();

  /* 發送 讀 指令 */
  SPI_FLASH_SendByte(W25X_ReadData);
	
  /* 發送 讀 地址高8位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  /* 發送 讀 地址中前8位 */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* 發送 讀 地址中後8位 */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* 發送 讀 地址低8位 */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  
	/* 讀取數據 */
  while (NumByteToRead--)
  {
    /* 讀取一個字節*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* 指向下一個字節緩沖區 */
    pBuffer++;
  }

  /* 停止信號 FLASH: CS 高電平 */
  SPI_FLASH_CS_HIGH();
}


 /**
  * @brief  讀取FLASH ID
  * @param 	無
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* 開始通訊：CS低電平 */
  SPI_FLASH_CS_LOW();

  /* 發送JEDEC指令，讀取ID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* 讀取一個字節數據 */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 讀取一個字節數據 */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 讀取一個字節數據 */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* 停止通訊：CS高電平 */
  SPI_FLASH_CS_HIGH();

	/*把數據組合起來，作為函數的返回值*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

 /**
  * @brief  讀取FLASH Device ID
  * @param 	無
  * @retval FLASH Device ID
  */
u32 SPI_FLASH_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  SPI_FLASH_SendByte(W25X_DeviceID);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = SPI_FLASH_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(W25X_ReadData);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


 /**
  * @brief  使用SPI讀取一個字節的數據
  * @param  無
  * @retval 返回接收到的數據
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  使用SPI發送一個字節的數據
  * @param  byte：要發送的數據
  * @retval 返回接收到的數據
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待發送緩沖區為空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 寫入數據寄存器，把要寫入的數據寫入發送緩沖區 */
  SPI_I2S_SendData(FLASH_SPI, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收緩沖區非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 讀取數據寄存器，獲取接收緩沖區數據 */
  return SPI_I2S_ReceiveData(FLASH_SPI);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
  
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
   }

  /* Send Half Word through the FLASH_SPI peripheral */
  SPI_I2S_SendData(FLASH_SPI, HalfWord);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
   }
  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(FLASH_SPI);
}


 /**
  * @brief  向FLASH發送 寫使能 命令
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* 通訊開始：CS低 */
  SPI_FLASH_CS_LOW();

  /* 發送寫使能命令*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*通訊結束：CS高 */
  SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  等待WIP(BUSY)標志被置0，即等待到FLASH內部數據寫入完畢
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* 選擇 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 發送 讀狀態寄存器 命令 */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 若FLASH忙碌，則等待 */
  do
  {
    /* 讀取FLASH芯片的狀態寄存器 */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* 正在寫入標志 */

  /* 停止信號  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}


//進入掉電模式
void SPI_Flash_PowerDown(void)   
{ 
  /* 選擇 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 發送 掉電 命令 */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* 停止信號  FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}   

//喚醒
void SPI_Flash_WAKEUP(void)   
{
  /*選擇 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 發上 上電 命令 */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* 停止信號 FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();                   //等待TRES1
}   

 /**
  * @brief  使 SPI_FLASH 進入 4 字節地址模式
  * @param  none
  * @retval none
  */
void SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*選擇 FLASH: CS 低 */
	SPI_FLASH_CS_LOW();
	
	/* 發送狀態寄存器 3 命令 */
	SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = SPI_FLASH_SendByte(Dummy_Byte);
	
	/* 停止信號 FLASH: CS 高 */
	SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*選擇 FLASH: CS 低 */
		SPI_FLASH_CS_LOW();
		
		/* 進入4字節模式 */
		SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* 停止信號 FLASH: CS 高 */
		SPI_FLASH_CS_HIGH();
	}
}
	
/**
  * @brief  等待超時回調函數
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超時後的處理,輸出錯誤信息 */
  FLASH_ERROR("SPI 等待超時!errorCode = %d",errorCode);
  return 0;
}
   
/*********************************************END OF FILE**********************/
