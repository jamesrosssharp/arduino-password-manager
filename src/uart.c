#include <avr/io.h>

#define BAUD_RATE 9600
#define BAUD_PRESCALE (F_CPU/16/BAUD_RATE-1)

void uart_initialize(void) 
{
	UBRR0H = (BAUD_PRESCALE >> 8);
	UBRR0L = BAUD_PRESCALE;
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
}

int uart_send_byte(char byte) 
{
	loop_until_bit_is_set(UCSR0A,UDRE0);
	UDR0 = byte;
	return 0;
}

