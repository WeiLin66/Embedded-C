#ifndef __STM32F4XX_H
#define __STM32F4XX_H

#include <stdio.h>
#include <stdint.h>

//#define GPIO_register 1

/*Memory mapping*/
#define PERIPH_BASE           		((unsigned int)0x40000000)                          
#define AHB1_PERIPH_BASE       		(PERIPH_BASE + 0x00020000)	
#define GPIO_H_BASE            		(AHB1_PERIPH_BASE + 0x1C00)
	
/*RCC*/
#define RCC_BASE              		(AHB1_PERIPH_BASE + 0x3800)
#define RCC_AHB1_ENR				*(unsigned int*)(RCC_BASE+0x30)

/*GPIO*/

typedef struct{
	uint32_t 	MODER;
	uint32_t 	OTYPER;
	uint32_t 	OSPEEDER;
	uint32_t 	PUPDR;
	uint32_t 	IDR;
	uint32_t	ODR;
	uint16_t	BSRRL;
	uint16_t  BSRRH;
	uint32_t	LCKR;
	uint32_t	AFRL;
	uint32_t	AFGH;
}GPIO_TYPEDEF;

typedef GPIO_TYPEDEF* GPIO_Typedef; //GPIO pointer
#endif
