/*
 *	(C) 2022 J. R. Sharp	
 *
 *	Released under MIT License
 *
 *	See LICENSE.txt for License Terms
 *
 *	ts.c: touchscreen interface 
 *
 */

#pragma once

#include <stdint.h>

void ts_read(uint16_t* x, uint16_t* y, uint16_t* z);
