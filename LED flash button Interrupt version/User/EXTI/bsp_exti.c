#include "bsp_exti.h"

void NVIC_Init_CFG(){
	NVIC_InitTypeDef NVIC_Init_struct;
	
	/*Initial NVIC*/
	NVIC_PriorityGroupConfig(PRIORITY);
	NVIC_Init_struct.NVIC_IRQChannel = EXTI_IRQ1;
	NVIC_Init_struct.NVIC_IRQChannelPreemptionPriority = PREPRIORITY;
	NVIC_Init_struct.NVIC_IRQChannelSubPriority =SUBPRIORITY;
	NVIC_Init_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Init_struct);
	
	NVIC_Init_struct.NVIC_IRQChannel = EXTI_IRQ2;
	NVIC_Init(&NVIC_Init_struct);	
}

void Interrupt_Init_CFG(){
	EXTI_InitTypeDef EXTI_Init_struct;
	
	/*Initial Interrupt*/	
	EXTI_Init_struct.EXTI_Line = LINE1;
	EXTI_Init_struct.EXTI_Mode = INTERRUPT;
	EXTI_Init_struct.EXTI_Trigger = TRIGGER_RISING;
	EXTI_Init_struct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_Init_struct);
	
	EXTI_Init_struct.EXTI_Line = LINE2;
	EXTI_Init(&EXTI_Init_struct);	
}

void Interrupt_KEY_Init(){
	LED_Init();
	Key_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	NVIC_Init_CFG();

	SYSCFG_EXTILineConfig(SYSCFG_SOURCE_PORT1, SYSCFG_SOURCE_PIN1);
	SYSCFG_EXTILineConfig(SYSCFG_SOURCE_PORT2, SYSCFG_SOURCE_PIN2);	
	
	Interrupt_Init_CFG();
}

void BUTTON1_HANDLER(){
	if(EXTI_GetITStatus(LINE1 == SET)){
		LED_button_toggle(LED_PORT, LED_PIN1);
		EXTI_ClearITPendingBit(LINE1);
	}
}

void BUTTON2_HANDLER(){
	if(EXTI_GetITStatus(LINE2 == SET)){
		LED_button_toggle(LED_PORT, LED_PIN2);
		EXTI_ClearITPendingBit(LINE2);
	}	
}
