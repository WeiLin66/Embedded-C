#ifndef __BSP_LED_H_
#define __BSP_LED_H_
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>
#include <stdint.h>
#include "time.h"

/*Increase portability of bsp_led*/
#define LED_Red_PIN 		GPIO_Pin_10
#define LED_Green_PIN 	GPIO_Pin_11
#define LED_Blue_PIN 		GPIO_Pin_12
#define LED_PORT 				GPIOH
#define LED_RCC 				RCC_AHB1Periph_GPIOH

#define LED1 10
#define LED2 11
#define LED3 12

#define SET 	1
#define RESET 0

#define BIT_MODER 				(GPIOH_BASE)
#define BIT_TYPER 				(GPIOH_BASE + 0x04)
#define BIT_SPEEDER 			(GPIOH_BASE + 0x08)
#define BIT_PuPr 					(GPIOH_BASE + 0x0c)
#define BIT_IDR 					(GPIOH_BASE + 0x10)
#define BIT_ODR 					(GPIOH_BASE + 0x14)


#define BITBAND(addr, bitnum)  			((addr&0xf0000000) + 0x2000000 + ((addr&0x000fffff)<<5) + (bitnum<<2))
#define MEM_ADDR(addr) 							*(volatile uint32_t*)(addr)
#define BIT_GPIOH(addr, n) 					MEM_ADDR(BITBAND(addr, n))

extern void LED_Init(void);
extern void LED_button_toggle(uint16_t PIN, uint8_t key);

#endif /*__BSP_LED_H_*/
