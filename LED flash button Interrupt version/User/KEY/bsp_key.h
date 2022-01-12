#ifndef __BSP_KEY_H_
#define __BSP_KEY_H_
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>

#define LED_KEY1_PORT 	GPIOA
#define LED_KEY2_PORT 	GPIOC
#define LED_KEY1_PIN 		GPIO_Pin_0
#define LED_KEY2_PIN 		GPIO_Pin_13
#define LED_KEY1_RCC 		RCC_AHB1Periph_GPIOA
#define LED_KEY2_RCC 		RCC_AHB1Periph_GPIOC
#define LED_ON					1
#define LED_OFF					0

extern void Key_Init(void);
extern uint8_t Key_scan(GPIO_TypeDef* GPIO, uint16_t PIN);

#endif /*__BSP_KEY_H_*/
