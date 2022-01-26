#ifndef __DNA_BSP_H_
#define __DNA_BSP_H_

#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "stdint.h"

#define DMA_STREAM     DMA2_Stream0 // change to other stream
#define DMA_CHANNEL    DMA_Channel_0
#define TEMP_SIZE      20
#define TIMEOUT				 0x00000fff

/*Increase portability of bsp_led*/
#define LED_Red_PIN 		GPIO_Pin_10
#define LED_Green_PIN 	GPIO_Pin_11
#define LED_Blue_PIN 		GPIO_Pin_12
#define LED_PORT 				GPIOH
#define LED_RCC 				RCC_AHB1Periph_GPIOH
#define DELAY_TIME			500
#define DMA_CLEAR_FLAG  DMA_FLAG_TCIF0

extern void GPIO_Initial_API(GPIO_TypeDef* GPIO, uint16_t PIN);
extern void App_Init(void);
extern void App_Thread(void);

#endif /*__DNA_BSP_H_*/
