#include "bsp_advanced_timer.h"
uint16_t  	ADVANCED_TIM_PSC 	=		24-1;	
uint16_t 	ADVANCED_TIM_ARR 	=		1000-1;
uint16_t 	ADVANCED_TIM_CCR	=		400;


static void AdvancedTim_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*开时钟*/
	TIMx_CH1_GPIO_CLK_CMD(TIMx_CH1_GPIO_CLK, ENABLE);
	/*设置参数1*/
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=TIMx_CH1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	/*写参数*/
	GPIO_Init(TIMx_CH1_GPIO_PORT, &GPIO_InitStruct);	
}

static void AdvancedTim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_APB2PeriphClockCmd(ADVANCED_TIM_CLK, ENABLE);

	TIM_TimeBaseInitStruct.TIM_Prescaler=ADVANCED_TIM_PSC;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=ADVANCED_TIM_ARR;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;

	TIM_TimeBaseInit(ADVANCED_TIM, &TIM_TimeBaseInitStruct);
	

	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=ADVANCED_TIM_CCR;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;

	/*写参数*/
	TIM_OC1Init(ADVANCED_TIM, &TIM_OCInitStruct);		//通过此控制输出通道
	TIM_CtrlPWMOutputs(ADVANCED_TIM, ENABLE);
	TIM_Cmd(ADVANCED_TIM, DISABLE);	
}

void AdvancedTim_Init(void)
{
		AdvancedTim_GPIO_Config();
		AdvancedTim_Config();
}
