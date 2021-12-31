#ifndef __BSP_LED_H
#define __BSP_LED_H

/*Increase portability of bsp_led*/
#define LED_Red_PIN 	GPIO_Pin_10
#define LED_Red_PORT 	GPIOH
#define LED_Red_RCC 	RCC_AHB1Periph_GPIOH

extern void GPIO_Initial_API(void);

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#endif
