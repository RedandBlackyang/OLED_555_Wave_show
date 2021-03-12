#include "fft_calculate.h"
#include "math.h"

long InBufArray[NPT]={0};
long OutBufArray[NPT/2];
long MagBufArray[NPT/2];
float  Vpp=0;				//实际电压幅值
float	Freq=0;				//实际频率
/******************************************************************
函数名称:GetPowerMag()
函数功能:计
算各次谐波  幅值

备　　注:	高16位存储实部，低16位存储虚部。
					先将lBufOutArray分解成实部(X)和虚部(Y)，然后计算幅值sqrt(X*X+Y*Y)
*******************************************************************/
static void GetPowerMag()
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        //取高16位,实部
        lX  = (OutBufArray[i] >> 16);
				//取低16位,虚部
        lY  = (OutBufArray[i] << 16) >> 16;	
        //除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;			
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            MagBufArray[i] = (unsigned long)(Mag * 32768);		//0特殊,为了幅值
        else
            MagBufArray[i] = (unsigned long)(Mag * 65536);
    }
}
/******************************************************************
函数名称:FFT_Parameter_Return(uint16_t  buffer_in[NPT])
函数功能:通过全局变量返回原信号实际频率和电压幅值

备　　注:输入16的数组,内容为adc采样的数据
*******************************************************************/
void FFT_Parameter_Return(uint16_t  *buffer_in)
{

	uint16_t i=0,i_max=0;
	long v_max=0;
	//将adc采样数据转换为复数,因为cr4_fft_256_stm32输入为32位数据且高16位存储实部，低16位存储虚部。
	for(i=0;i<NPT;i++)
		InBufArray[i] = ((signed short)(buffer_in[i])) << 16;	
	cr4_fft_1024_stm32(OutBufArray,InBufArray,NPT);
	GetPowerMag();	
	
	for(i=1;i<NPT/2;i++)
	{
		if(MagBufArray[i]>v_max)
		{
			v_max=MagBufArray[i];
			i_max=i;
		}
	}
	printf("%d",i_max);
	//FFT之后最大幅值对应的横坐标*频率分辨率=实际频率
	Freq=Freq_res*i_max;	
}
