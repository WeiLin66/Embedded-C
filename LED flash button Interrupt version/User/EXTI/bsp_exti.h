#ifndef __BSP_EXTI_H_
#define __BSP_EXTI_H_

#include "stm32f4xx_exti.h"
#include "stdint.h" 
#include "stm32f4xx.h"
#include "misc.h"
#include "bsp_led.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "bsp_key.h"

#define LINE1 						 		EXTI_Line0
#define LINE2 						 		EXTI_Line13
#define EXTI_IRQ1					 		EXTI0_IRQn
#define EXTI_IRQ2   			 		EXTI15_10_IRQn
#define INTERRUPT 				 		EXTI_Mode_Interrupt
#define EVENT							 		EXTI_Mode_Event
#define TRIGGER_RISING     		EXTI_Trigger_Rising
#define TRIGGER_FALLING     	EXTI_Trigger_Falling
#define SYSCFG_SOURCE_PORT1 	EXTI_PortSourceGPIOA
#define SYSCFG_SOURCE_PIN1	  EXTI_PinSource0
#define SYSCFG_SOURCE_PORT2 	EXTI_PortSourceGPIOC
#define SYSCFG_SOURCE_PIN2	  EXTI_PinSource13
#define BUTTON1_HANDLER				EXTI0_IRQHandler
#define BUTTON2_HANDLER				EXTI15_10_IRQHandler
#define PRIORITY							NVIC_PriorityGroup_1
#define PREPRIORITY 					1
#define SUBPRIORITY 					1

void BUTTON1_HANDLER(void);
void BUTTON2_HANDLER(void);

extern void Interrupt_KEY_Init(void);

#endif /*__BSP_EXTI_H_*/
