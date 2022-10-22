#include <avr/io.h>

#include "uart.h"

#define BAUD_RATE 9600
#define BAUD_PRESCALE (F_CPU/16/BAUD_RATE-1)

FILE uart_output = FDEV_SETUP_STREAM(uart_send_byte, NULL, _FDEV_SETUP_WRITE);

void uart_initialize(void) 
{
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	stdout = &uart_output;
}

int uart_send_byte(char byte, FILE *stream) 
{
	if (byte == '\n') {
		uart_send_byte('\r',stream);
	}

	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0 = byte;
	return 0;
}

