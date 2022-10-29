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

#include "ts.h"
#include "gpio.h"

#include "adc.h"

#define XP_PORT GPIO_PORTB
#define XP_PIN	0

#define YM_PORT GPIO_PORTB
#define YM_PIN	1

// XM is ADC1
#define XM_PORT GPIO_PORTC
#define XM_PIN  2

// XP is ADC2
#define YP_PORT GPIO_PORTC
#define YP_PIN 3

void ts_read(uint16_t* x, uint16_t* y, uint16_t* z)
{

	// Read X

	//   Set YP and YM to inputs and low (no pullup)
	gpio_set_input(YP_PORT, YP_PIN);
	gpio_clear(YP_PORT, YP_PIN);

	gpio_set_input(YM_PORT, YM_PIN);
	gpio_clear(YM_PORT, YM_PIN);

	// Set XP and XM to outputs (XP <- HIGH, XM <- low)

	gpio_set_output(XP_PORT, XP_PIN);
	gpio_set_output(XM_PORT, XM_PIN);
	gpio_set(XP_PORT, XP_PIN);
	gpio_clear(XM_PORT, XM_PIN);

	*x = 1023 - adc_read_analog_pin(YP_PIN);	

	// Read Y

	//   Set YP and YM to inputs and low (no pullup)
	gpio_set_input(XP_PORT, XP_PIN);
	gpio_clear(XP_PORT, XP_PIN);

	gpio_set_input(XM_PORT, XM_PIN);
	gpio_clear(XM_PORT, XM_PIN);

	// Set XP and XM to outputs (XP <- HIGH, XM <- low)

	gpio_set_output(YP_PORT, YP_PIN);
	gpio_set_output(YM_PORT, YM_PIN);
	gpio_set(YP_PORT, YP_PIN);
	gpio_clear(YM_PORT, YM_PIN);

	*y = 1023 - adc_read_analog_pin(XM_PIN);	

	// Read Z

	gpio_set_output(XP_PORT, XP_PIN);
	gpio_set_output(YM_PORT, YM_PIN);
	gpio_set(YM_PORT, YM_PIN);
	gpio_clear(XP_PORT, XP_PIN);

	uint16_t z1 = adc_read_analog_pin(XM_PIN);
	uint16_t z2 = adc_read_analog_pin(YP_PIN);

	*z = 1023 - (z2 - z1);

}
