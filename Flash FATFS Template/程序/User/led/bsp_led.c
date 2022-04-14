/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   led���ú����ӿ�
  ******************************************************************************
  * @attention
  *
  * ���ƽ̨:Ұ��  STM32 F429 �_�l��  
  * Փ��    :http://www.firebbs.cn
  * �Ԍ�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"   

 /**
  * @brief  ��ʼ������LED��IO
  * @param  �o
  * @retval �o
  */
void LED_GPIO_Config(void)
{		
		/*���xһ��GPIO_InitTypeDef��͵ĽY���w*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*�_��LED���P��GPIO���O�r�*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK|
	                           LED3_GPIO_CLK, ENABLE); 

		/*�x��Ҫ���Ƶ�GPIO���_*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*�O�����_ģʽ��ݔ��ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�O�����_��ݔ����͞�����ݔ��*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*�O�����_������ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*�O�����_���ʞ�2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*�{�Î캯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*�x��Ҫ���Ƶ�GPIO���_*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*�x��Ҫ���Ƶ�GPIO���_*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*�P�]RGB��*/
		LED_RGBOFF;		
}
/*********************************************END OF FILE**********************/
