#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "OLED_I2C.h"
#include "bsp_adc.h"
#include "bsp_advanced_timer.h"
#include "bsp_key.h"
#include <math.h>
#include "bsp_advanced_timer.h"
#include "table_fft.h"
#include	"stm32_dsp.h"
#include "fft_calculate.h"
#define pi 3.1415926535
#define accur 1/64	
uint8_t key_status=1 ;
uint8_t y1[128],y2[128];
extern uint16_t ADC_Data[NPT];//ADC采样数据
extern unsigned char BMP1[];
int main()
{
	I2C_Configuration();
	LED_Init();
	OLED_Init();					 /* OLED初始化 */
	EXTI_Key_Config();
	USARTx_Init();
	AdvancedTim_Init();
	ADCx_Init();
	OLED_CLS();
	TIM_Cmd(ADVANCED_TIM, ENABLE);
	while(1)
	{

	}

}
