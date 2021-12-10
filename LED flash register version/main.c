#include "stm32f4xx.h"
#include "stm_gpio.h"

/**
  *   main
  */
int main(void)
{		
	GPIO_Typedef GPIO = (GPIO_Typedef)GPIO_H_BASE;

	#ifdef GPIO_register	
	RCC_AHB1_ENR |= (1<<7);
	
		/*MODER*/
	GPIO->MODER &= ~(3<<2*10);
	GPIO->MODER &= ~(3<<2*11); 
	GPIO->MODER &= ~(3<<2*12);
	
	GPIO->MODER |= (1<<2*10);
	GPIO->MODER |= (1<<2*11); 
	GPIO->MODER |= (1<<2*12); 	
	
	/*OTYPER*/
	GPIO->OTYPER &= ~(1<<10);
	GPIO->OTYPER &= ~(1<<11);
	GPIO->OTYPER &= ~(1<<12);
	
	GPIO->OTYPER |= (0<<10);
	GPIO->OTYPER |= (0<<11);
	GPIO->OTYPER |= (0<<12);
	
	/*OSPEEDER*/
	GPIO->OSPEEDER &= ~(3<<2*10);
	GPIO->OSPEEDER &= ~(3<<2*11);
	GPIO->OSPEEDER &= ~(3<<2*12);
	
	GPIO->OSPEEDER |= (2<<2*10);
	GPIO->OSPEEDER |= (2<<2*11);
	GPIO->OSPEEDER |= (2<<2*12);
	
	/*PUPDR*/
	GPIO->PUPDR &= ~(3<<2*10);
	GPIO->PUPDR &= ~(3<<2*11);
	GPIO->PUPDR &= ~(3<<2*12);
	
	GPIO->PUPDR |= (1<<2*10);
	GPIO->PUPDR |= (1<<2*11);
	GPIO->PUPDR |= (1<<2*12);	

	/*BSRRL*/
	GPIO->BSRRL &= ~(1<<10);
	GPIO->BSRRL &= ~(1<<11);
	GPIO->BSRRL &= ~(1<<12);
	
//	GPIO_H->BSRRL |= (1<<10);
	GPIO->BSRRL |= (1<<11);
	GPIO->BSRRL |= (1<<12);
	
	/*BSRRH*/
	GPIO->BSRRH &= ~(1<<10);
	GPIO->BSRRH &= ~(1<<11);
	GPIO->BSRRH &= ~(1<<12);
	
	GPIO->BSRRH |= (1<<10);
//	GPIO->BSRRH |= (1<<11);
//	GPIO->BSRRH |= (1<<12);
#else
	/* GPIO H Initial */
	GPIO_Init(GPIO, port_H);
	
	/* pin 10 config*/
	GPIO_LED_Control(GPIO, 10, 0);
	
	/* pin 11 config*/
	GPIO_LED_Control(GPIO, 11, 1);
	
	/* pin 12 config*/
	GPIO_LED_Control(GPIO, 12, 0);
	
#endif

	while(1);
}
// void SystemInit(void)
// {	
// }

/*********************************************END OF FILE**********************/

