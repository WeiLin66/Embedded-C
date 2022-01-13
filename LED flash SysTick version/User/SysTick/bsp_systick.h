#ifndef __BSP_SYSTICK_H_
#define __BSP_SYSTICK_H_

#include <stdint.h>
#include "stm32f4xx_rcc.h"

#define SystemTick_ms(x)    delay_ms(x)
#define SystemTick_s(x)     delay_ms((x)*1000)

extern void System_tick_Init(uint32_t ticks);
extern void delay_ms(uint32_t delay);

#endif
