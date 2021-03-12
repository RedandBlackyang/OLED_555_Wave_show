#include "fft_calculate.h"
#include "math.h"

long InBufArray[NPT]={0};
long OutBufArray[NPT/2];
long MagBufArray[NPT/2];
float  Vpp=0;				//ʵ�ʵ�ѹ��ֵ
float	Freq=0;				//ʵ��Ƶ��
/******************************************************************
��������:GetPowerMag()
��������:��
�����г��  ��ֵ

������ע:	��16λ�洢ʵ������16λ�洢�鲿��
					�Ƚ�lBufOutArray�ֽ��ʵ��(X)���鲿(Y)��Ȼ������ֵsqrt(X*X+Y*Y)
*******************************************************************/
static void GetPowerMag()
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        //ȡ��16λ,ʵ��
        lX  = (OutBufArray[i] >> 16);
				//ȡ��16λ,�鲿
        lY  = (OutBufArray[i] << 16) >> 16;	
        //����32768�ٳ�65536��Ϊ�˷��ϸ������������
        X = NPT * ((float)lX) / 32768;			
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            MagBufArray[i] = (unsigned long)(Mag * 32768);		//0����,Ϊ�˷�ֵ
        else
            MagBufArray[i] = (unsigned long)(Mag * 65536);
    }
}
/******************************************************************
��������:FFT_Parameter_Return(uint16_t  buffer_in[NPT])
��������:ͨ��ȫ�ֱ�������ԭ�ź�ʵ��Ƶ�ʺ͵�ѹ��ֵ

������ע:����16������,����Ϊadc����������
*******************************************************************/
void FFT_Parameter_Return(uint16_t  *buffer_in)
{

	uint16_t i=0,i_max=0;
	long v_max=0;
	//��adc��������ת��Ϊ����,��Ϊcr4_fft_256_stm32����Ϊ32λ�����Ҹ�16λ�洢ʵ������16λ�洢�鲿��
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
	//FFT֮������ֵ��Ӧ�ĺ�����*Ƶ�ʷֱ���=ʵ��Ƶ��
	Freq=Freq_res*i_max;	
}
