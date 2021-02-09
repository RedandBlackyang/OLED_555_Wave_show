#ifndef _FFT_CALCULATE_H
#define _FFT_CALCULATE_H

#include "stm32f10x.h"
#include "stm32_dsp.h"
#include "bsp_advanced_timer.h"

#define NPT 1024				//�������ݸ���
#define Fs	72000000/((ADVANCED_TIM_PSC+1)*(ADVANCED_TIM_ARR+1))	//����Ƶ��
#define	Freq_res	Fs/NPT																					//Ƶ�ʷֱ���
extern long InBufArray[NPT];
extern long OutBufArray[NPT/2];
extern long MagBufArray[NPT/2];
//extern float  Vpp;				//ʵ�ʵ�ѹ��ֵ
extern float	Freq;				//ʵ��Ƶ��

void FFT_Parameter_Return(uint16_t  *buffer_in);

#endif
