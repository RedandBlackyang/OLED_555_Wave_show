#include "bsp_adc.h"
#include "OLED_I2C.h"
#include "bsp_systick.h"
#include "fft_calculate.h"
#include "bsp_advanced_timer.h"
#include <stdio.h>
#include <math.h>
#define accur 1/64
uint16_t ADC_Data[NPT]={0};
float  wave_value_V[128]={0};		//��ѹֵ
float  wave_cycle;					//����
float  wave_t1;						//������
float  wave_t2;						//������
float  wave_tw;						//������
float  wave_q;						//ռ�ձ�
float  wave_max_V;					//���ֵ
float  wave_min_V;					//��Сֵ
float  wave_Vpp;					//���ֵ
float  wave_average=0;				//ƽ��ֵ
float  wave_Vrms;					//��Чֵ
extern uint8_t y1[128],y2[128];
extern uint8_t key_status;

static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*��ʱ��*/
	RCC_APB2PeriphClockCmd(ADCx_PORT_CLK_1, ENABLE);
	RCC_APB2PeriphClockCmd(ADCx_PORT_CLK_2, ENABLE);
	/*���ò���*/
	GPIO_InitStruct.GPIO_Pin=ADCx_PIN_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;/*ģ������ģʽ*/
	/*д��Ĵ���*/
	GPIO_Init(ADCx_PORT_1, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=ADCx_PIN_2;
	GPIO_Init(ADCx_PORT_2, &GPIO_InitStruct);

}

static void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;

	RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);

	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;				//ʹ���ⲿ����Ӧ�ùر�����ģʽ,��ΪֻҪ���ⲿ�������ھͿ��Դ�����ת��
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;			//ʹ���ⲿ����ʱ�򿪴˹���,��ʾ�ĸ�������ADC_SampleTime_239Cycles5Ӱ��,������;
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

//stm32ΪС�˸�ʽ,���ݸ��ֽڴ����ڴ��ַ,���ֽڴ�����С��ַ
void TX_ADCdata(uint16_t adc_data)
{
	uint8_t i,str[2];
	str[0]=adc_data>>8;		//���ʹ��
	str[1]=adc_data;
	for(i=0;i<2;i++)
	{			   
		while((USART1->SR&0X40)==0);   /* ѭ������,ֱ��������� */
		USART1->DR = str[i];  
	}		

}


void DMA1_Channel1_IRQHandler(void)
{

	DMA_Cmd(DMA1_Channel1, DISABLE);

	DMA_ClearITPendingBit(DMA1_IT_TC1);
	
	Oled_Message_Show();
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}


void Oled_Message_Show(void)
{
	uint16_t  x=0,x_min=0;
	uint8_t   flag1=0,flag2=0;			//flag1��֤֮ǰ��ƽΪ�ߣ�flag2��֤�͵�ƽ���ߵ�ƽ
	char str_show[20]={0};
	
	OLED_CLS();
	
	if(key_status==0)
	{
		
		for(x=0;x<128;x++)								
		{
			y1[x]=ADC_Data[x]*accur;
			TX_ADCdata(ADC_Data[x]);				//�򴮿�ʾ������������
		}
		
		for(x=1;x<128;x++)							//������
		{	
			draw_vertical_line(x,y1[x-1],y1[x]);
		}
	
	}
	else
	{
		wave_average=0;
		wave_Vrms=0;
		wave_max_V=wave_value_V[0];
		wave_min_V=wave_value_V[0];
		for(x=0;x<128;x++)								
		{
			#if 		VREF3_3						//���ADC�ο�ֵΪ3.3V
			wave_value_V[x]=ADC_Data[x]*3.3/4096;
			
			#else									//���ADC�ο�ֵΪ5V
			wave_value_V[x]=ADC_Data[x]*5/4096;
			#endif
			
			wave_average+=wave_value_V[x]/128;		
			
			wave_Vrms+=wave_value_V[x]*wave_value_V[x]/128;
			
			if(wave_value_V[x]>wave_max_V)
			{
				wave_max_V=wave_value_V[x];
			}
			
			if(wave_value_V[x]<wave_min_V)
			{
				wave_min_V=wave_value_V[x];
			}
			
			
		}

			
		for(x=0;x<128;x++)
		{
			if(fabs(wave_value_V[x]-wave_min_V)<0.01)
				flag1=1;			//��֤����������֮ǰΪ�͵�ƽ
			
			if(((wave_value_V[x]-(wave_min_V+wave_max_V)/2)>0)&&(flag1==1))
			{
				x_min++;
				flag2=1;
			}
			
			if((fabs(wave_value_V[x]-wave_min_V)<0.01)&&(flag1==1)&&(flag2==1))
				break;
		}
		
		wave_Vpp=wave_max_V-wave_min_V;				//������ֵ
		
		FFT_Parameter_Return(ADC_Data);				//FFT����ʵ��Ƶ��		
		
		wave_cycle=1/Freq*1000000;					//���� ��λus
		
		wave_tw=x_min*1000000/Fs;					//�������� ��λus
		
		wave_q=wave_tw/wave_cycle;					//ռ�ձ�
		
		wave_Vrms=sqrt(wave_Vrms);					//������ֵ������Чֵ
		
		wave_t1=wave_tw;							//������

		wave_t2=wave_cycle-wave_tw;					//������
		
		if(key_status==1){
		
			sprintf(str_show,"F=%.1f Hz",Freq);					//Ƶ��
			OLED_ShowStr(0,0,(unsigned char *)str_show,1);

			sprintf(str_show,"T=%.1f us",wave_cycle);			//����
			OLED_ShowStr(0,1,(unsigned char *)str_show,1);
												
			sprintf(str_show,"Vrms=%.3f V",wave_Vrms);			//��Чֵ
			OLED_ShowStr(0,2,(unsigned char *)str_show,1);
				
			sprintf(str_show,"V_average=%.3f V",wave_average);	//ƽ��ֵ
			OLED_ShowStr(0,3,(unsigned char *)str_show,1);
			
			sprintf(str_show,"V_Max=%.3f V",wave_max_V);		//���ֵ
			OLED_ShowStr(0,4,(unsigned char *)str_show,1);
			
			sprintf(str_show,"V_Min=%.3f V",wave_min_V);		//��Сֵ
			OLED_ShowStr(0,5,(unsigned char *)str_show,1);
			
			sprintf(str_show,"Vpp=%.3f",wave_Vpp);				//���ֵ
			OLED_ShowStr(0,6,(unsigned char *)str_show,1);	
		
		}
		else
		{
			sprintf(str_show,"tw=%.3f us",wave_tw);				//������
			OLED_ShowStr(0,0,(unsigned char *)str_show,1);
			
			sprintf(str_show,"q=%.3f",wave_q);					//ռ�ձ�
			OLED_ShowStr(0,1,(unsigned char *)str_show,1);
			
			sprintf(str_show,"t1=%.3f",wave_t1);				//������
			OLED_ShowStr(0,2,(unsigned char *)str_show,1);

			sprintf(str_show,"t2=%.3f",wave_t2);				//������
			OLED_ShowStr(0,3,(unsigned char *)str_show,1);	

			
		}
	}

	
	Delay_ms(150);
	
}
