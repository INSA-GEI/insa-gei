/**
 * \file uart.c
 * \brief Driver for uart
 * \author Sebastien DI MERCURIO
 * 
 * uart.c file contains the driver for UART
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

/**
 * \brief Buffer for received data
 *
 * This buffer is used by the interrupt function, when receiving data. Data are stored in this buffer
 * and UART_RX_BufferLength is increased. When UART_RX_BufferLength reaches UART_RX_BUFFER_SIZE, no
 * more data are stored and new data are lost.
 *
 * When calling UARTGetchar, data are read from this buffer at index UART_RX_BufferIndex and UART_RX_BufferLength
 * is decreased. When UART_RX_BufferLength reaches 0, void char is returned. 
 * @see UART_RX_BufferLength
 * @see UART_RX_BUFFER_SIZE
 * @see UARTGetchar
 * @see UART_RX_BufferIndex
 */
unsigned char UART_RX_Buffer[UART_RX_BUFFER_SIZE];

/**
 * \brief Index for data to read from UART_RX_Buffer
 *
 * When reading received data from UART_RX_Buffer, oldest char is pointed in the array by this index.
 * After a read, this index is incremented, and wrapped to the beginning of the array if the pointer was outside the array.
 * @see UART_RX_Buffer
 */
unsigned char UART_RX_BufferIndex;

/**
 * \brief Indicate amount of data received (and not read)
 *
 * This counter store the number of data currently received and still not read.
 * Used with UART_RX_BufferIndex, this counter indicate next free entry in UART_RX_Buffer for storing new data.
 * @see UART_RX_BufferIndex
 * @see UART_RX_Buffer
 */
unsigned char UART_RX_BufferLength;

unsigned char UARTChecksum;

/**
 * \brief Baud rate constant
 *
 * This constant is used to set the baud rate timer: this value enables a baud rate of 9600 baud with a main clock of 7372800 hz.
 */
#define MYUBRR 103

/**
 * \brief UART initialization
 *
 * Initialization of the UART: set a baud rate of 9600 baud, 8 bits , no parity checking and 1 stop bit.
 * Also enable interrupts for receiving data (transmitting data is done using polling).
 */
void UARTInit(void)
{	
	/*Set baud rate */
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;

	UCSR0A = (1<<U2X0);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
	
	UARTChecksum=0;
} 

/**
 * \brief Wrapper for UARTPutchar
 *
 * Wrapper function to be used with FDEV_SETUP_STREAM macro for building a FILE handler. This function wraps UARTPutchar function
 * used to send data on UART.
 * @see UARTPutchar
 * @see FDEV_SETUP_STREAM
 */
int uart_putchar(char c, FILE *stream)
{
	if (c!='\n')
	{
		UARTChecksum = UARTChecksum ^ (unsigned char)c;	
		UARTPutchar(c);	
	}
	else
	{
		if (UARTChecksum == '\n') UARTChecksum = UARTChecksum +1;
		UARTPutchar(UARTChecksum);
		UARTPutchar(c);
		
		UARTChecksum =0;
	}
	
	return 1;	
}

/**
 * \brief Send a char on UART
 *
 * This function send one char using polling, on UART.
 * @param[in] c Character to send
 */
void UARTPutchar(char c)
{
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));

	/* Put data into buffer, sends the data */
	UDR0 = c;
}

/**
 * \brief Wrapper for UARTGetchar
 *
 * Wrapper function to be used with FDEV_SETUP_STREAM macro for building a FILE handler. This function wraps UARTGetchar function
 * used to read data from UART_RX_Buffer array.
 * @see UARTGetchar
 * @see FDEV_SETUP_STREAM
 * @see UART_RX_Buffer
 */
int uart_getchar(FILE *stream)
{
	return ((int)UARTGetchar());	
}

/**
 * \brief Read a char in UART_RX_Buffer
 *
 * This function read one char from UART_RX_Buffer array, previously filled by interrupt.
 * If UART_RX_Buffer is empty, this function returns 0. In order to avoid this problem, one should calls UARTGetBufferSize
 * previously, to check if data are available for read or not.
 * @return Character read
 */
char UARTGetchar(void)
{
char tmp;

	tmp =0;

	if (UART_RX_BufferLength!=0)
	{
		tmp =UART_RX_Buffer[UART_RX_BufferIndex];

		UART_RX_BufferIndex++;
		UART_RX_BufferLength--;

		if (UART_RX_BufferIndex>UART_RX_BUFFER_SIZE) UART_RX_BufferIndex =0;
	}

	return tmp;
}

/**
 * \brief Return amount of data currently received.
 *
 * This function returns to callee the amount of data currently stored in UART_RX_Buffer array and waiting to be read.
 * This function is useful before calling UARTGetchar function in order to be sure to read available data, as UARTGetchar 
 * returns '0' when no data is available for reading. 
 * @return Number of data received and still not read 
 */
unsigned char UARTGetBufferSize(void)
{
	
	return UART_RX_BufferLength;
}

/**
 * \brief Interrupt function for received data.
 *
 * Each time a data is received on UART, this interrupt function is called.
 * Received data is the stored in UART_RX_Buffer array and UART_RX_BufferLength is increased. When array is full
 * no more data are stored, silently.
 * @see UART_RX_Buffer
 * @see UART_RX_BufferLength 
 */
ISR(USART_RX_vect, ISR_BLOCK)
{
unsigned char receive_data;
unsigned char index;


	receive_data=UDR0;
	
	if (receive_data != 0)
	{
		if (UART_RX_BufferLength <UART_RX_BUFFER_SIZE)
		{
			index = UART_RX_BufferIndex + UART_RX_BufferLength;
			if (index >= UART_RX_BUFFER_SIZE) index = index - UART_RX_BUFFER_SIZE;
			UART_RX_Buffer[index]=receive_data;
			UART_RX_BufferLength ++;
		}
	}
}

