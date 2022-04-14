/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   led應用函數接口
  ******************************************************************************
  * @attention
  *
  * 實驗平台:野火  STM32 F429 開發板  
  * 論壇    :http://www.firebbs.cn
  * 淘寶    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"   

 /**
  * @brief  初始化控制LED的IO
  * @param  無
  * @retval 無
  */
void LED_GPIO_Config(void)
{		
		/*定義一個GPIO_InitTypeDef類型的結構體*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*開啟LED相關的GPIO外設時鐘*/
		RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
	                           LED2_GPIO_CLK|
	                           LED3_GPIO_CLK, ENABLE); 

		/*選擇要控制的GPIO引腳*/															   
		GPIO_InitStructure.GPIO_Pin = LED1_PIN;	

		/*設置引腳模式為輸出模式*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*設置引腳的輸出類型為推挽輸出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*設置引腳為上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*設置引腳速率為2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*調用庫函數，使用上面配置的GPIO_InitStructure初始化GPIO*/
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);	
    
    /*選擇要控制的GPIO引腳*/															   
		GPIO_InitStructure.GPIO_Pin = LED2_PIN;	
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);	
    
    /*選擇要控制的GPIO引腳*/															   
		GPIO_InitStructure.GPIO_Pin = LED3_PIN;	
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);	
		
		/*關閉RGB燈*/
		LED_RGBOFF;		
}
/*********************************************END OF FILE**********************/
