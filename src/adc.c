/*
 *	(C) 2022 J. R. Sharp	
 *
 *	Released under MIT License
 *
 *	See LICENSE.txt for License Terms
 *
 *	adc.c : ADC driver
 *
 */

#include "adc.h"

#include <util/delay.h>

uint16_t adc_read_analog_pin(int8_t analog_pin)
{

	// Configure pin
	ADMUX = 0x40 | analog_pin;
	ADCSRA |=  (1<<ADEN) | (1 << ADSC) | 7;
	ADCSRB = 0x00;
	while (bit_is_set(ADCSRA, ADSC));
	return ADC;
}

