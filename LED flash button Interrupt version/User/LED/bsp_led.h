#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "time.h"

/*Increase portability of bsp_led*/
#define LED_PORT				GPIOH
#define LED_PIN1  		  GPIO_Pin_10
#define LED_PIN2  		  GPIO_Pin_11
#define LED_RCC  				RCC_AHB1Periph_GPIOH

extern void LED_Init(void);
extern void LED_button_toggle(GPIO_TypeDef* GPIO, uint16_t PIN);

#endif /*__BSP_LED_H_*/
