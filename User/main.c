#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "OLED_I2C.h"
#include "bsp_i2c_gpio.h"
#include "bsp_adc.h"
#include "bsp_advanced_timer.h"
#include "bsp_key.h"


#define pi 3.1415926535
#define accur 1/64	
uint8_t key_status=1 ;
extern uint16_t ConvData[256];//ADC采样数据
extern unsigned char BMP1[];
int main()
{
	uint16_t x;
	uint16_t y1[128],y2[128];
	LED_Init();
	EXTI_Key_Config();
	USARTx_Init();
	OLED_Init();					 /* OLED初始化 */
	ADCx_Init();
	OLED_CLS();
	Delay_ms(100);

	while(1)
	{
//		OLED_CLS();
//		Delay_ms(100);
		for(x=0;x<256;x=x+2)
		{
			y1[x/2]=ConvData[x]*accur;
		}
		for(x=1;x<256;x=x+2)
		{
				y2[x/2]=ConvData[x]*accur;
		}
		if(key_status )
		{
			for(x=1;x<128;x++)
			{
				draw_line(x,y1[x-1],y1[x]);
			}
		}
		else
		{
			for(x=1;x<128;x++)
			{
				draw_line(x,y2[x-1],y2[x]);
			}
		}
	}

}
