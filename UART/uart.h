#ifndef __UART_H__
#define __UART_H__

void uart_open(USART_TypeDef* USARTx, uint32_t baud, uint32_t flags);
void uart_close(USART_TypeDef* USARTx);
void uart_putc(USART_TypeDef* USARTx, char c);
char uart_getc(USART_TypeDef* USARTx);

#endif
