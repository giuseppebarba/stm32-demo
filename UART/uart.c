#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include "uart.h"

void uart_open(USART_TypeDef* USARTx, uint32_t baud, uint32_t flags) {
	USART_InitTypeDef USART_InitStructure;

	/*
	 * Init USART GPIO_InitStructure
	 */
	USART_StructInit(&USART_InitStructure);

	/*
	 * Configure USART parameter to 9600 baud, 8 data bits, 1 stop bit,
	 * no parity and no flow control
	 */
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_Mode = flags;
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx, ENABLE);
}

void uart_close(USART_TypeDef* USARTx) {
	USART_DeInit(USARTx);
}


void uart_putc(USART_TypeDef* USARTx, char c) {
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USARTx->DR = (c & 0xFF);
}

char uart_getc(USART_TypeDef* USARTx) {
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);

	return (char)(USARTx->DR & 0xFF);
}