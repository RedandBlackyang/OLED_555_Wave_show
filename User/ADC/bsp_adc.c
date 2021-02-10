#include "bsp_adc.h"
#include "OLED_I2C.h"
#include "bsp_systick.h"
#include "fft_calculate.h"
#include "bsp_advanced_timer.h"
#include <stdio.h>
#define accur 1/64
uint16_t ADC_Data[NPT];
extern uint8_t y1[128],y2[128];
extern uint8_t key_status;
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*开时钟*/
	RCC_APB2PeriphClockCmd(ADCx_PORT_CLK_1, ENABLE);
	RCC_APB2PeriphClockCmd(ADCx_PORT_CLK_2, ENABLE);
	/*配置参数*/
	GPIO_InitStruct.GPIO_Pin=ADCx_PIN_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;/*模拟输入模式*/
	/*写入寄存器*/
	GPIO_Init(ADCx_PORT_1, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=ADCx_PIN_2;
	GPIO_Init(ADCx_PORT_2, &GPIO_InitStruct);

}

static void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;

	RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);

	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;				//使用外部触发应该关闭连续模式,因为只要有外部触发存在就可以触发新转换
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;			//使用外部触发时打开此功能,显示的个数就受ADC_SampleTime_239Cycles5影响,这很奇怪;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct.ADC_NbrOfChannel=1;
	ADC_InitStruct.ADC_ScanConvMode=DISABLE;

	ADC_Init(ADC_x, &ADC_InitStruct);

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	ADC_RegularChannelConfig(ADC_x, ADCx_CHx_1, 1, ADC_SampleTime_239Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADCx_CHx_2, 2, ADC_SampleTime_239Cycles5);

	ADC_Cmd(ADC_x, ENABLE);

	ADC_StartCalibration(ADC_x);
	while(ADC_GetCalibrationStatus(ADC_x)==RESET);

	ADC_DMACmd(ADC_x, ENABLE);

	ADC_ExternalTrigConvCmd(ADC_x, ENABLE);
}
static void DMA_NVIC_Config(void)
{    
		NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
}
static void ADCx_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)(&(ADC_x->DR));
  DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)(ADC_Data);
  DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;
  DMA_InitStruct.DMA_BufferSize=NPT;
  DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; 
  DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority=DMA_Priority_High;
  DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;

  DMA_Init(ADCx_DMA_CHx, &DMA_InitStruct);

  DMA_Cmd(ADCx_DMA_CHx, ENABLE);
}
void ADCx_Init(void)
{
	
	ADCx_GPIO_Config();
	DMA_NVIC_Config();
	ADCx_DMA_Config();
	ADCx_Config();
	
}
void DMA1_Channel1_IRQHandler(void)
{
	u16 x=0;
	char str_Freq[10]={0};
  DMA_Cmd(DMA1_Channel1, DISABLE);

  DMA_ClearITPendingBit(DMA1_IT_TC1);
	
	FFT_Parameter_Return(ADC_Data);		//FFT计算实际频率
	OLED_CLS();
	
	sprintf(str_Freq,"%.1f Hz",Freq);
	OLED_ShowStr(0,0,(unsigned char *)str_Freq,1);
	
	for(x=0;x<128;x++)								
	{
		y1[x]=ADC_Data[x]*accur;
	}
	for(x=1;x<128;x++)							//画波形
	{
		draw_vertical_line(x,y1[x-1],y1[x]);
	}
	
	Delay_ms(150);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

