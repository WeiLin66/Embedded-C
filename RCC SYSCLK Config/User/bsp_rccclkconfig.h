#ifndef __RCCCLKCONFIG_H_
#define __RCCCLKCONFIG_H_

#include <stdint.h>
#include "stm32f4xx_rcc.h"
#include "misc.h"

#define RCC_TEST 	 			0
#define SET_ON		 			1

#define HSE				 			1
#define AHB_CFG 				1
#define APB2_CFG				4
#define APB1_CFG				5
#define PLLCLK					2

#define PLLM      			25
#define PLLQ      			9
#define PLLN     			  432
#define PLLP      			0

#define SYSCLK_M   			PLLM
#define SYSCLK_Q				PLLQ
#define SYSCLK_N  			PLLN
#define SYSCLK_P				PLLP

extern void HSE_SetSysCLK(uint32_t m, uint32_t n, uint32_t p, uint32_t q);

#endif /*__RCCCLKCONFIG_H_*/
