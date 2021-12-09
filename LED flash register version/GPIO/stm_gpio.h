#ifndef __STM32FXX__H
#define __STM32FXX__H

#include "stm32f4xx.h"

#endif
#include <stdbool.h>

typedef enum{
	port_A=0,
	port_B,
	port_C,
	port_D,
	port_E,
	port_F,
	port_G,
	port_H
}port;

extern void GPIO_Init(GPIO_Typedef, uint8_t);
extern void GPIO_LED_Control(GPIO_Typedef, uint8_t, bool);
