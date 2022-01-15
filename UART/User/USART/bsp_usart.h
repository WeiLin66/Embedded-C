#ifndef __BSP_USART_H_
#define __BSP_USART_H_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdint.h>
#include "stm32f4xx_rcc.h"

#define USART_TX_PIN 			GPIO_Pin_9
#define USART_RX_PIN 			GPIO_Pin_10
#define USART_PORT 				GPIOA
#define USART_RCC 				RCC_AHB1Periph_GPIOH
#define USART_BAUDRATE		115200
#define delay_ms(x)				delay_ms(x)

extern void USART_CFG(void);
extern void delay_ms(uint32_t delay);

#endif /*__BSP_USART_H_*/
