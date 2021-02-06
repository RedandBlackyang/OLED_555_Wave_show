#ifndef _BSP_ADC_H_
#define _BSP_ADC_H_

#include "stm32f10x.h"
/*GPIO*/
#define ADCx_PORT_CLK_1  		RCC_APB2Periph_GPIOC
#define ADCx_PIN_1  			GPIO_Pin_1
#define ADCx_PORT_1					GPIOC

#define ADCx_PORT_CLK_2  		RCC_APB2Periph_GPIOC
#define ADCx_PIN_2  			GPIO_Pin_2
#define ADCx_PORT_2					GPIOC
/*ADC*/
#define ADC_x							ADC1
#define ADCx_CLK  		    RCC_APB2Periph_ADC1
#define ADCx_CHx_1 		    	ADC_Channel_11
#define ADCx_CHx_2 		    	ADC_Channel_12

/*DMA*/
#define ADCx_DMA_CLK  		RCC_AHBPeriph_DMA1
#define ADCx_DMA_CHx 		  DMA1_Channel1


void ADCx_Init(void);

#endif //_BSP_ADC_H_
