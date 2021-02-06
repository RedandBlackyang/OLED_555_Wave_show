#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#include "stm32f10x.h"
#include <stdio.h>


#define USARTx USART1
#define USARTx_BAUD_RATE 115200#define USARTx_CLK  RCC_APB2Periph_USART1
#define USARTx_CLK_CMD  RCC_APB2PeriphClockCmd

#define USARTx_IRQCHANNEL USART1_IRQn
#define USARTx_IRQHANDLER USART1_IRQHandler


#define USART_Rx_GPIO_PORT  GPIOA
#define USART_Rx_GPIO_PIN   GPIO_Pin_10
#define USART_Rx_GPIO_CLK   RCC_APB2Periph_GPIOA
#define USART_Rx_GPIO_CLK_CMD  RCC_APB2PeriphClockCmd

#define USART_Tx_GPIO_PORT GPIOA
#define USART_Tx_GPIO_PIN  GPIO_Pin_9
#define USART_Tx_GPIO_CLK  RCC_APB2Periph_GPIOA
#define USART_Tx_GPIO_CLK_CMD  RCC_APB2PeriphClockCmd


void USARTx_GPIO_Config(void);
void USARTx_Config(void);
void USARTx_NVIC_Config(void);
void USARTx_Init(void);

#endif //_BSP_USART_H_
