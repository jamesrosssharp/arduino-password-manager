/*
 *	(C) 2022 J. R. Sharp	
 *
 *	Released under MIT License
 *
 *	See LICENSE.txt for License Terms
 *
 *	main.c : Program entry point
 *
 */

#include <avr/io.h>

#define BLINK_DELAY_MS 1000

#include <util/delay.h>

#include "uart.h"
#include "debug.h"
#include "gpio.h"
#include "lcd.h"

#define LED_PORT GPIO_PORTB
#define LED_PIN	 5

int main (void)
{
	gpio_set_output(LED_PORT, LED_PIN);

	uart_initialize();

	DEBUG("Hello world! LCD ID = %x\n", lcd_read_id());

	lcd_init();
	
	lcd_clear(0x00, 0xff, 0x00);

	while(1) 
	{

		// turn LED on
		gpio_set(LED_PORT, LED_PIN);
		_delay_ms(BLINK_DELAY_MS);

		// turn LED off
		gpio_clear(LED_PORT, LED_PIN);
		_delay_ms(BLINK_DELAY_MS);
	}
}
