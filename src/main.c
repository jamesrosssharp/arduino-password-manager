#include <avr/io.h>

#define BLINK_DELAY_MS 1000

#include <util/delay.h>

#include "uart.h"

#include "debug.h"

int a;

int main (void)
{
	// Arduino digital pin 13 (pin 5 of PORTB) for output
	DDRB |= 0B100000; // PORTB5

	uart_initialize();

	while(1) 
	{
		DEBUG("Hello world! %x\n", 0x69);

		// turn LED on
		PORTB |= 0B100000; // PORTB5
		_delay_ms(BLINK_DELAY_MS);

		// turn LED off
		PORTB &= ~ 0B100000; // PORTB5
		_delay_ms(BLINK_DELAY_MS);
	}
}
