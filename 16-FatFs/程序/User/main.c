/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SPI FLASH�ļ�ϵͳ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  *                              ͷ�ļ�
  ******************************************************************************
  */
#include "stm32f4xx.h"
#include "led/bsp_led.h" 
#include "usart/bsp_debug_usart.h"
#include "ff.h"
/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"��ӭʹ��Ұ��STM32 F429������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  

/**
  ******************************************************************************
  *                                ������
  ******************************************************************************
  */
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	/* ��ʼ��LED */
	LED_GPIO_Config();	
	LED_BLUE;
	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	Debug_USART_Config();	
  printf("****** SPI FLASH TEST ******\r\n");
  
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- ��ʽ������ ---------------------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH NO FILE SYSTEM...\r\n");
    /* ��ʽ�� */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("��FORMAT SUCCESS\r\n");
      /* ��ʽ������ȡ������ */
			res_flash = f_mount(NULL,"1:",1);			
      /* ���¹���	*/			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED_RED;
			printf("����FORMAT FAIL����\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("����MOUNT FAIL(%d)\r\n",res_flash);
    printf("����SPI FLASH INIT FAIL\r\n");
		while(1);
  }
  else
  {
    printf("��MOUNT SUCCESS\r\n");
  }
  
///*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
//	/* ���ļ�������ļ��������򴴽��� */
//	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
//	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_flash == FR_OK )
//	{
//		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
//    /* ��ָ���洢������д�뵽�ļ��� */
//		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//    if(res_flash==FR_OK)
//    {
//      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
//      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
//    }
//    else
//    {
//      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
//    }    
//		/* ���ٶ�д���ر��ļ� */
//    f_close(&fnew);
//	}
//	else
//	{	
//		LED_RED;
//		printf("������/�����ļ�ʧ�ܡ�\r\n");
//	}
//	
///*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
//	printf("****** ���������ļ���ȡ����... ******\r\n");
//	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ); 	 
//	if(res_flash == FR_OK)
//	{
//		LED_GREEN;
//		printf("�����ļ��ɹ���\r\n");
//		res_flash = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
//    if(res_flash==FR_OK)
//    {
//      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
//      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
//    }
//    else
//    {
//      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
//    }		
//	}
//	else
//	{
//		LED_RED;
//		printf("�������ļ�ʧ�ܡ�\r\n");
//	}
//	/* ���ٶ�д���ر��ļ� */
//	f_close(&fnew);	
//  
//	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
//	f_mount(NULL,"1:",1);
//  
//  /* ������ɣ�ͣ�� */
	while(1)
	{
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
