#ifndef _FFT_CALCULATE_H
#define _FFT_CALCULATE_H

#include "stm32f10x.h"
#include "stm32_dsp.h"
#include "bsp_advanced_timer.h"

#define NPT 1024				//采样数据个数
#define Fs	72000000/((ADVANCED_TIM_PSC+1)*(ADVANCED_TIM_ARR+1))	//采样频率
#define	Freq_res	Fs/NPT																					//频率分辨率
extern long InBufArray[NPT];
extern long OutBufArray[NPT/2];
extern long MagBufArray[NPT/2];
//extern float  Vpp;				//实际电压幅值
extern float	Freq;				//实际频率

void FFT_Parameter_Return(uint16_t  *buffer_in);

#endif
