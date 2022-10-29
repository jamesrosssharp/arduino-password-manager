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

#pragma once

#include <stdint.h>
#include <avr/io.h>

uint16_t adc_read_analog_pin(int8_t analog_pin);
