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

void lcd_clear(uint16_t col);
void lcd_fill_rect(int x, int y, int w, int h, uint16_t col);
void lcd_draw_bitmap(int x, int y, int w, int h, uint16_t fg_col, uint16_t bg_col, const uint8_t* bitmap);
