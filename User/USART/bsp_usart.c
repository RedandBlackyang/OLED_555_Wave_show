#include "bsp_usart.h"

void USARTx_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_Rx_GPIO_CLK_CMD(USART_Rx_GPIO_CLK,ENABLE);
    USART_Tx_GPIO_CLK_CMD(USART_Tx_GPIO_CLK,ENABLE);
    
    GPIO_InitStruct.GPIO_Pin=USART_Rx_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART_Rx_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin=USART_Tx_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(USART_Tx_GPIO_PORT, &GPIO_InitStruct);
    
}
void USARTx_Config(void)
{
    USART_InitTypeDef USART_InitStruct;
    USARTx_CLK_CMD(USARTx_CLK,ENABLE);
    //初始化串口相关数据
    USART_InitStruct.USART_BaudRate=USARTx_BAUD_RATE;
    USART_InitStruct.USART_WordLength=USART_WordLength_8b;
    USART_InitStruct.USART_StopBits=USART_StopBits_1;
    USART_InitStruct.USART_Parity=USART_Parity_No;
    USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_Init(USARTx, &USART_InitStruct);
    //使能串口
    USART_Cmd(USARTx, ENABLE);
    //使能串口中断
    //USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    

}
void USARTx_NVIC_Config(void)
{
      NVIC_InitTypeDef NVIC_InitStruct;
      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
      NVIC_InitStruct.NVIC_IRQChannel=USARTx_IRQCHANNEL;
      NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
      NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
      NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
        
      NVIC_Init(&NVIC_InitStruct);  


}
void USARTx_Init(void)
{
    //USARTx_NVIC_Config();
    USARTx_GPIO_Config();
    USARTx_Config();
}


int fputc(int ch,FILE* f)
{
    USART_SendData(USARTx, (uint8_t)ch);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
    return(ch); 
}
int fgetc(FILE* f)
{
     while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)==RESET);
    return (int)USART_ReceiveData(USARTx);
}

