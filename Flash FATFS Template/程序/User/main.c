/**
  ******************************************************************************
  *                              �^�ļ�
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "led/bsp_led.h" 
#include "usart/bsp_debug_usart.h"
#include "ff.h"
/**
  ******************************************************************************
  *                              ���x׃��
  ******************************************************************************
  */
FATFS fs;													/* FatFs�ļ�ϵ�y���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ������Y�� */
UINT fnum;            					  /* �ļ��ɹ��x������ */
BYTE ReadBuffer[1024]={0};        /* �x���_�^ */
BYTE WriteBuffer[] =              /* �����_�^*/
"Welcome to the STM32 F429 development board. Today is a good day, new file system test file\r\n";  

/**
  ******************************************************************************
  *                                �΄պ���
  ******************************************************************************
  */
/**
  * @brief  ������
  * @param  �o
  * @retval �o
  */
int main(void)
{
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	LED_BLUE;
	
	/* ��ʼ���{ԇ���ڣ�һ��鴮��1 */
	Debug_USART_Config();	
  printf("****** This is an SPI FLASH file system experiment ******\r\n");
  
	//���ⲿSPI Flash���d�ļ�ϵ�y���ļ�ϵ�y���d�r����SPI�O���ʼ��
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- ��ʽ���yԇ ---------------------------*/  
	/* ����]���ļ�ϵ�y�͸�ʽ�����������ļ�ϵ�y */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH does not have a file system yet and will be formatted soon...\r\n");
    /* ��ʽ�� */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("��FLASH has successfully formatted the file system.\r\n");
      /* ��ʽ���ᣬ��ȡ�����d */
			res_flash = f_mount(NULL,"1:",1);			
      /* �����d	*/			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED_RED;
			printf("����Formatting Failure����\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("����External Flash mount file system failure.(%d)\r\n",res_flash);
    printf("����Possible cause: SPI Flash initialization unsuccessful.\r\n");
		while(1);
  }
  else
  {
    printf("��The file system is successfully mounted and ready for read/write test.\r\n");
  }
  
/*----------------------- �ļ�ϵ�y�yԇ�����yԇ -----------------------------*/
	/* ���_�ļ�������ļ������ڄt������ */
	printf("\r\n****** File writing test will be conducted soon... ******\r\n");	
	res_flash = f_open(&fnew, "1:FatFs read and write test files.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_flash == FR_OK )
	{
		printf("��Open/create FatFs read/write test file.txt file successfully, writing data to a file.\r\n");
    /* ��ָ���惦�^���݌��뵽�ļ��� */
		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_flash==FR_OK)
    {
      printf("��File write success, write byte data: %d\n",fnum);
      printf("��The data written to the file is: \r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("����File write failure: (%d)\n",res_flash);
    }    
		/* �����x�����P�]�ļ� */
    f_close(&fnew);
	}
	else
	{	
		LED_RED;
		printf("����Failed to open/create file.\r\n");
	}
	
/*------------------- �ļ�ϵ�y�yԇ���x�yԇ ------------------------------------*/
	printf("****** The file read test will be conducted soon... ******\r\n");
	res_flash = f_open(&fnew, "1:FatFs read and write test files.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_flash == FR_OK)
	{
		LED_GREEN;
		printf("��Open file successfully.\r\n");
		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_flash==FR_OK)
    {
      printf("��File read successfully, read byte dat: %d\r\n",fnum);
      printf("��The file data read is: \r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("����File read failure: (%d)\n",res_flash);
    }		
	}
	else
	{
		LED_RED;
		printf("����Failed to open the file.\r\n");
	}
	/* �����x�����P�]�ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵ�y��ȡ�����d�ļ�ϵ�y */
	f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�C */
	while(1)
	{
	}
}

