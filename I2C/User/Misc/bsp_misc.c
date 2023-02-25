#include "bsp_misc.h"

void delay_ms(uint32_t delay){
	uint32_t mutipliter;
	
	mutipliter = SystemCoreClock/1000;
	mutipliter *= delay;
	while(mutipliter--);
}
