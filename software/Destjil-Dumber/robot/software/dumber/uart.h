/**
 * \file uart.h
 * \brief Header for uart driver
 * \author Sebastien DI MERCURIO
 * 
 * uart.h file is header for uart driver
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>

#ifdef F_CPU
#undef F_CPU
#define F_CPU 7372800UL
#endif

#define UART_BAUDRATE 9600
#define UART_RX_BUFFER_SIZE 100

void UARTInit(void);
void UARTPutchar(char c);
char UARTGetchar(void);
unsigned char UARTGetBufferSize(void);

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

#endif /* __UART_H__ */
