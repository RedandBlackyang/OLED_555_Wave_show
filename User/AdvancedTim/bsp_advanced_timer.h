#ifndef _BSP_ADVANCED_TIMER_H_
#define _BSP_ADVANCED_TIMER_H_

#include "stm32f10x.h"
/*GPIO*/
#define TIMx_CH1_GPIO_PORT 				GPIOA
#define TIMx_CH1_GPIO_PIN 				GPIO_Pin_8
#define TIMx_CH1_GPIO_CLK     		RCC_APB2Periph_GPIOA
#define TIMx_CH1_GPIO_CLK_CMD 		RCC_APB2PeriphClockCmd

#define ADVANCED_TIM 							TIM1
#define ADVANCED_TIM_CLK					RCC_APB2Periph_TIM1
	
#define ADVANCED_TIM_PSC 					3-1
	
#define ADVANCED_TIM_ARR 					1000-1
#define ADVANCED_TIM_CCR					100
void AdvancedTim_Init(void);

#endif//_BSP_ADVANCED_TIMER_H_
