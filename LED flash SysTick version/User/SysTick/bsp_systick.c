#include "bsp_systick.h"

/**
 * @brief System Ticks time interval comfiguration
 * @param ticks Interrupt time interval(ms)
 * @reval None
 */
void System_tick_Init(uint32_t ticks){
	ticks = ticks*(SystemCoreClock/1000);
	SysTick_Config(ticks);
}

/**
 * @brief time delay funtion, unit: ms
 * @param delay delay time interval
 * @reval None
 */
void delay_ms(uint32_t delay){
	uint32_t mutipliter;
	
	RCC_ClocksTypeDef RCC_Get_Frequency;
	RCC_GetClocksFreq(&RCC_Get_Frequency);
	
//	mutipliter = RCC_Get_Frequency.SYSCLK_Frequency;
	mutipliter = SystemCoreClock/1000;
	mutipliter *= delay;
	while(mutipliter--);
}
