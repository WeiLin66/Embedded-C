#ifndef __BSP_LED_H
#define __BSP_LED_H

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

/*Increase portability of bsp_led*/
#define LED_Red_PIN 		GPIO_Pin_10
#define LED_Green_PIN 	GPIO_Pin_11
#define LED_Blue_PIN 		GPIO_Pin_12

#define LED_PORT 				GPIOH
#define LED_RCC 				RCC_AHB1Periph_GPIOH

#define DELAY_TIME			100
#define DELAY_OPARATOR  0xfffff

#define TIME						1
#define OPERATION				2
#define FLASH_MECHANISM OPERATION

extern void DelayMs(uint32_t miles);
extern void GPIO_Initial_API(GPIO_TypeDef* GPIO, uint16_t PIN);
extern void App_Init(void);
extern void App_Thread(void);
extern void LED_Flash_Sysclk(GPIO_TypeDef* GPIO, uint16_t PIN);

#endif
