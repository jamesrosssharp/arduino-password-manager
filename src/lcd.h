/*
 *	(C) 2022 J. R. Sharp	
 *
 *	Released under MIT License
 *
 *	See LICENSE.txt for License Terms
 *
 *	lcd.h : HX-8347I Driver	
 *
 */

#pragma once

#include <stdint.h>
#include <avr/io.h>

uint16_t lcd_read_id();
void lcd_init();

void lcd_clear(uint8_t r, uint8_t g, uint8_t b);
