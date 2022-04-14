 /**
  ******************************************************************************
  * @file    bsp_spi_flash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   spi flash �׌ӑ��ú���bsp 
  ******************************************************************************
  * @attention
  *
  * ���ƽ̨:Ұ��STM32 F429 �_�l��
  * Փ��    :http://www.firebbs.cn
  * �Ԍ�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./flash/bsp_spi_flash.h"


static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

 /**
  * @brief  SPI_FLASH��ʼ��
  * @param  �o
  * @retval �o
  */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ�� FLASH_SPI ��GPIO �r� */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO,SPI_FLASH_SPI_SCK_GPIO �r�ʹ�� */
  RCC_AHB1PeriphClockCmd (FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK|FLASH_SPI_MOSI_GPIO_CLK|FLASH_CS_GPIO_CLK, ENABLE);

  /*!< SPI_FLASH_SPI �r�ʹ�� */
  FLASH_SPI_CLK_INIT(FLASH_SPI_CLK, ENABLE);
 
  //�O�����_����
  GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT,FLASH_SPI_SCK_PINSOURCE,FLASH_SPI_SCK_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT,FLASH_SPI_MISO_PINSOURCE,FLASH_SPI_MISO_AF); 
	GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT,FLASH_SPI_MOSI_PINSOURCE,FLASH_SPI_MOSI_AF); 
  
  /*!< ���� SPI_FLASH_SPI ���_: SCK */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ���_: MISO */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  
	/*!< ���� SPI_FLASH_SPI ���_: MOSI */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);  

	/*!< ���� SPI_FLASH_SPI ���_: CS */
  GPIO_InitStructure.GPIO_Pin = FLASH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);

  /* ֹͣ��̖ FLASH: CS���_���ƽ*/
  SPI_FLASH_CS_HIGH();

  /* FLASH_SPI ģʽ���� */
  // FLASHоƬ ֧��SPIģʽ0��ģʽ3�������O��CPOL CPHA
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

  /* ʹ�� FLASH_SPI  */
  SPI_Cmd(FLASH_SPI, ENABLE);

	/* ʹ SPI_FLASH �M�� 4 �ֹ���ַģʽ */
	SPI_FLASH_Mode_Init();
}

 /**
  * @brief  ����FLASH�ȅ^
  * @param  SectorAddr��Ҫ�������ȅ^��ַ
  * @retval �o
  */
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
  /* �l��FLASH��ʹ������ */
  SPI_FLASH_WriteEnable();
  SPI_FLASH_WaitForWriteEnd();
  /* �����ȅ^ */
  /* �x��FLASH: CS���ƽ */
  SPI_FLASH_CS_LOW();
  /* �l���ȅ^����ָ��*/
  SPI_FLASH_SendByte(W25X_SectorErase);
  /*�l�Ͳ����ȅ^��ַ�ĸ�8λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF000000) >> 24);
  /*�l�Ͳ����ȅ^��ַ����ǰ8λ*/
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  /* �l�Ͳ����ȅ^��ַ������8λ */
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  /* �l�Ͳ����ȅ^��ַ�ĵ�8λ */
  SPI_FLASH_SendByte(SectorAddr & 0xFF);
  /* ֹͣ��̖ FLASH: CS ���ƽ */
  SPI_FLASH_CS_HIGH();
  /* �ȴ������ꮅ*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  ����FLASH�ȅ^����Ƭ����
  * @param  �o
  * @retval �o
  */
void SPI_FLASH_BulkErase(void)
{
  /* �l��FLASH��ʹ������ */
  SPI_FLASH_WriteEnable();

  /* ���K Erase */
  /* �x��FLASH: CS���ƽ */
  SPI_FLASH_CS_LOW();
  /* �l�����K����ָ��*/
  SPI_FLASH_SendByte(W25X_ChipErase);
  /* ֹͣ��̖ FLASH: CS ���ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ������ꮅ*/
  SPI_FLASH_WaitForWriteEnd();
}



 /**
  * @brief  ��FLASH��퓌��딵�����{�ñ��������딵��ǰ��Ҫ�Ȳ����ȅ^
  * @param	pBuffer��Ҫ���딵����ָ�
  * @param WriteAddr�������ַ
  * @param  NumByteToWrite�����딵���L�ȣ����С춵��SPI_FLASH_PerWritePageSize
  * @retval �o
  */
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  /* �l��FLASH��ʹ������ */
  SPI_FLASH_WriteEnable();

  /* �x��FLASH: CS���ƽ */
  SPI_FLASH_CS_LOW();
  /* ��퓌�ָ��*/
  SPI_FLASH_SendByte(W25X_PageProgram);
	
  /*�l�͌���ַ�ĸ�8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF000000) >> 24);
  /*�l�͌���ַ����ǰ8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /*�l�͌���ַ������8λ*/
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /*�l�͌���ַ�ĵ�8λ*/
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* ���딵��*/
  while (NumByteToWrite--)
  {
    /* �l�ͮ�ǰҪ������ֹ����� */
    SPI_FLASH_SendByte(*pBuffer);
    /* ָ����һ�ֹ����� */
    pBuffer++;
  }

  /* ֹͣ��̖ FLASH: CS ���ƽ */
  SPI_FLASH_CS_HIGH();

  /* �ȴ������ꮅ*/
  SPI_FLASH_WaitForWriteEnd();
}


 /**
  * @brief  ��FLASH���딵�����{�ñ��������딵��ǰ��Ҫ�Ȳ����ȅ^
  * @param	pBuffer��Ҫ���딵����ָ�
  * @param  WriteAddr�������ַ
  * @param  NumByteToWrite�����딵���L��
  * @retval �o
  */
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�\�����࣬��writeAddr��SPI_FLASH_PageSize���������\��Y��Addrֵ��0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ�����ÿ��Ԍ��R��퓵�ַ*/
  count = SPI_FLASH_PageSize - Addr;	
	/*Ӌ���Ҫ�����������*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�\�����࣬Ӌ���ʣ�಻�Mһ퓵��ֹ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

	 /* Addr=0,�tWriteAddr ���ð�퓌��R aligned  */
  if (Addr == 0) 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*�Ȱ�����퓶�����*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			
			/*���ж���Ĳ��Mһ퓵Ĕ�������������*/
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
	/* ����ַ�c SPI_FLASH_PageSize �����R  */
  else 
  {
		/* NumByteToWrite < SPI_FLASH_PageSize */
    if (NumOfPage == 0) 
    {
			/*��ǰ�ʣ���count��λ�ñ�NumOfSingleС��������*/
      if (NumOfSingle > count) 
      {
        temp = NumOfSingle - count;
				
				/*�Ȍ��M��ǰ�*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;
				
				/*�ٌ�ʣ��Ĕ���*/
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else /*��ǰ�ʣ���count��λ���܌���NumOfSingle������*/
      {				
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */
    {
			/*��ַ�����R�����count���_̎���������@���\��*/
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;
			
			/*������퓶�����*/
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }
			/*���ж���Ĳ��Mһ퓵Ĕ�������������*/
      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

 /**
  * @brief  �xȡFLASH����
  * @param 	pBuffer���惦�x��������ָ�
  * @param   ReadAddr���xȡ��ַ
  * @param   NumByteToRead���xȡ�����L��
  * @retval �o
  */
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
  /* �x��FLASH: CS���ƽ */
  SPI_FLASH_CS_LOW();

  /* �l�� �x ָ�� */
  SPI_FLASH_SendByte(W25X_ReadData);
	
  /* �l�� �x ��ַ��8λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF000000) >> 24);
  /* �l�� �x ��ַ��ǰ8λ */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* �l�� �x ��ַ����8λ */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* �l�� �x ��ַ��8λ */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
  
	/* �xȡ���� */
  while (NumByteToRead--)
  {
    /* �xȡһ���ֹ�*/
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* ָ����һ���ֹ����_�^ */
    pBuffer++;
  }

  /* ֹͣ��̖ FLASH: CS ���ƽ */
  SPI_FLASH_CS_HIGH();
}


 /**
  * @brief  �xȡFLASH ID
  * @param 	�o
  * @retval FLASH ID
  */
u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* �_ʼͨӍ��CS���ƽ */
  SPI_FLASH_CS_LOW();

  /* �l��JEDECָ��xȡID */
  SPI_FLASH_SendByte(W25X_JedecDeviceID);

  /* �xȡһ���ֹ����� */
  Temp0 = SPI_FLASH_SendByte(Dummy_Byte);

  /* �xȡһ���ֹ����� */
  Temp1 = SPI_FLASH_SendByte(Dummy_Byte);

  /* �xȡһ���ֹ����� */
  Temp2 = SPI_FLASH_SendByte(Dummy_Byte);

  /* ֹͣͨӍ��CS���ƽ */
  SPI_FLASH_CS_HIGH();

	/*�є����M���������麯���ķ���ֵ*/
  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

 /**
  * @brief  �xȡFLASH Device ID
  * @param 	�o
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
  * @brief  ʹ��SPI�xȡһ���ֹ��Ĕ���
  * @param  �o
  * @retval ���ؽ��յ��Ĕ���
  */
u8 SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(Dummy_Byte));
}


 /**
  * @brief  ʹ��SPI�l��һ���ֹ��Ĕ���
  * @param  byte��Ҫ�l�͵Ĕ���
  * @retval ���ؽ��յ��Ĕ���
  */
u8 SPI_FLASH_SendByte(u8 byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ��l�;��_�^��գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* ���딵���Ĵ�������Ҫ����Ĕ�������l�;��_�^ */
  SPI_I2S_SendData(FLASH_SPI, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����վ��_�^�ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* �xȡ�����Ĵ������@ȡ���վ��_�^���� */
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
  * @brief  ��FLASH�l�� ��ʹ�� ����
  * @param  none
  * @retval none
  */
void SPI_FLASH_WriteEnable(void)
{
  /* ͨӍ�_ʼ��CS�� */
  SPI_FLASH_CS_LOW();

  /* �l�͌�ʹ������*/
  SPI_FLASH_SendByte(W25X_WriteEnable);

  /*ͨӍ�Y����CS�� */
  SPI_FLASH_CS_HIGH();
}

 /**
  * @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�Ȳ����������ꮅ
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* �x�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* �l�� �x��B�Ĵ��� ���� */
  SPI_FLASH_SendByte(W25X_ReadStatusReg);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* ��FLASHæµ���t�ȴ� */
  do
  {
    /* �xȡFLASHоƬ�Ġ�B�Ĵ��� */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 

    {
      if((SPITimeout--) == 0) 
      {
        SPI_TIMEOUT_UserCallback(4);
        return;
      }
    } 
  }
  while ((FLASH_Status & WIP_Flag) == SET); /* ���ڌ����־ */

  /* ֹͣ��̖  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}


//�M����ģʽ
void SPI_Flash_PowerDown(void)   
{ 
  /* �x�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* �l�� ��� ���� */
  SPI_FLASH_SendByte(W25X_PowerDown);

  /* ֹͣ��̖  FLASH: CS �� */
  SPI_FLASH_CS_HIGH();
}   

//����
void SPI_Flash_WAKEUP(void)   
{
  /*�x�� FLASH: CS �� */
  SPI_FLASH_CS_LOW();

  /* �l�� ��� ���� */
  SPI_FLASH_SendByte(W25X_ReleasePowerDown);

  /* ֹͣ��̖ FLASH: CS �� */
  SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   

 /**
  * @brief  ʹ SPI_FLASH �M�� 4 �ֹ���ַģʽ
  * @param  none
  * @retval none
  */
void SPI_FLASH_Mode_Init(void)
{
	uint8_t Temp;
	
	/*�x�� FLASH: CS �� */
	SPI_FLASH_CS_LOW();
	
	/* �l�͠�B�Ĵ��� 3 ���� */
	SPI_FLASH_SendByte(W25X_ReadStatusRegister3); 
	
	Temp = SPI_FLASH_SendByte(Dummy_Byte);
	
	/* ֹͣ��̖ FLASH: CS �� */
	SPI_FLASH_CS_HIGH();
	
	if((Temp&0x01) == 0)
	{
		/*�x�� FLASH: CS �� */
		SPI_FLASH_CS_LOW();
		
		/* �M��4�ֹ�ģʽ */
		SPI_FLASH_SendByte(W25X_Enter4ByteMode);
		
		/* ֹͣ��̖ FLASH: CS �� */
		SPI_FLASH_CS_HIGH();
	}
}
	
/**
  * @brief  �ȴ����r���{����
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ����r���̎��,ݔ���e�`��Ϣ */
  FLASH_ERROR("SPI �ȴ����r!errorCode = %d",errorCode);
  return 0;
}
   
/*********************************************END OF FILE**********************/
