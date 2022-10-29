/*
 *	(C) 2022 J. R. Sharp	
 *
 *	Released under MIT License
 *
 *	See LICENSE.txt for License Terms
 *
 *	lcd.c : HX-8347I Driver	
 *
 */

#include "lcd.h"
#include "debug.h"

//LCD pins  |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 | |RD |WR |RS |CS |RST|
//AVR   pin |PD7|PD6|PD5|PD4|PD3|PD2|PB1|PB0| |PC0|PC1|PC2|PC3|PC4|
//UNO pins  |7  |6  |5  |4  |3  |2  |9  |8  | |A0 |A1 |A2 |A3 |A4 |

#define RD_PIN (1<<0)	// nRD : Active low read signal
#define WR_PIN (1<<1)	// nWR : Active low write signal
#define RS_PIN (1<<2)	// DNC : 1 = data, 0 = command
#define CS_PIN (1<<3)	// nCS  : Active low chip select
#define RST_PIN (1<<4)	// nRST : Active low reset

#define COMMAND_PINS (RD_PIN | WR_PIN | RS_PIN | CS_PIN | RST_PIN)
#define DATA_PINS_PORTD 0xfc
#define DATA_PINS_PORTB 0x3

#include <util/delay.h>
#include <avr/pgmspace.h>

void lcd_reset()
{
	PORTC |= RST_PIN;
	DDRC = RST_PIN;
	PORTC &= ~RST_PIN;
	_delay_ms(100);
	PORTC |= RST_PIN;
	_delay_ms(100);
}

uint16_t lcd_read_reg(uint8_t reg)
{
	uint8_t data;
	// Set control signals to outputs	

	PORTC = COMMAND_PINS; 
	DDRC = COMMAND_PINS;

	// Set index to zero
	DDRB |= DATA_PINS_PORTB;
	DDRD |= DATA_PINS_PORTD;	

	PORTB = (PORTB & ~DATA_PINS_PORTB) | (reg & DATA_PINS_PORTB);
	PORTD = (PORTD & ~DATA_PINS_PORTD) | (reg & DATA_PINS_PORTD);

	PORTC &= ~(CS_PIN);

	PORTC &= ~(RS_PIN);
	PORTC &= ~(WR_PIN);
	PORTC |= WR_PIN;
	PORTC |= RS_PIN;

	DDRB &= ~DATA_PINS_PORTB;
	DDRD &= ~DATA_PINS_PORTD;

	_delay_us(1);

	PORTC &= ~(RD_PIN);
	_delay_us(1);
	data = (PINB & DATA_PINS_PORTB) | (PIND & DATA_PINS_PORTD);
	
	PORTC |= RD_PIN;

	_delay_us(1);

	PORTC |= CS_PIN;

	return data;
}

void lcd_write_reg(uint8_t reg, uint8_t data)
{

	// Set control signals to outputs	

	PORTC = COMMAND_PINS; 
	DDRC = COMMAND_PINS;

	// Set index to zero
	DDRB |= DATA_PINS_PORTB;
	DDRD |= DATA_PINS_PORTD;	

	PORTB = (PORTB & ~DATA_PINS_PORTB) | (reg & DATA_PINS_PORTB);
	PORTD = (PORTD & ~DATA_PINS_PORTD) | (reg & DATA_PINS_PORTD);

	PORTC &= ~(CS_PIN);

	PORTC &= ~(RS_PIN);
	PORTC &= ~(WR_PIN);
	PORTC |= WR_PIN;
	PORTC |= RS_PIN;

	_delay_us(1);

	PORTB = (PORTB & ~DATA_PINS_PORTB) | (data & DATA_PINS_PORTB);
	PORTD = (PORTD & ~DATA_PINS_PORTD) | (data & DATA_PINS_PORTD);

	PORTC &= ~(WR_PIN);
	_delay_us(1);
	
	PORTC |= WR_PIN;

	_delay_us(1);

	DDRB &= ~DATA_PINS_PORTB;
	DDRD &= ~DATA_PINS_PORTD;

	PORTC |= CS_PIN;
}

uint16_t lcd_read_id()
{
	lcd_reset();

	return lcd_read_reg(0);
}
#define TFTLCD_DELAY8 0x7F

static const uint8_t HX8347I_inittab[] PROGMEM  = {
            0xEA, 2, 0x00, 0x20,        //PTBA[15:0]
            0xEC, 2, 0x0C, 0xC4,        //STBA[15:0]
            0xE8, 1, 0x38,      //OPON[7:0]
            0xE9, 1, 0x10,      //OPON1[7:0]
            0xF1, 1, 0x01,      //OTPS1B
            0xF2, 1, 0x10,      //GEN
            //Gamma 2.2 Setting
            0x40, 13, 0x01, 0x00, 0x00, 0x10, 0x0E, 0x24, 0x04, 0x50, 0x02, 0x13, 0x19, 0x19, 0x16,
            0x50, 14, 0x1B, 0x31, 0x2F, 0x3F, 0x3F, 0x3E, 0x2F, 0x7B, 0x09, 0x06, 0x06, 0x0C, 0x1D, 0xCC,
            //Power Voltage Setting
            0x1B, 1, 0x1B,      //VRH=4.65V
            0x1A, 1, 0x01,      //BT (VGH~15V,VGL~-10V,DDVDH~5V)
            0x24, 1, 0x2F,      //VMH(VCOM High voltage ~3.2V)
            0x25, 1, 0x57,      //VML(VCOM Low voltage -1.2V)
            //****VCOM offset**///
            0x23, 1, 0x88,      //for Flicker adjust //can reload from OTP
            //Power on Setting
            0x18, 1, 0x34,      //I/P_RADJ,N/P_RADJ, Normal mode 60Hz
            0x19, 1, 0x01,      //OSC_EN='1', start Osc
            0x01, 1, 0x00,      //DP_STB='0', out deep sleep
            0x1F, 1, 0x88,      // GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
            TFTLCD_DELAY8, 5,
            0x1F, 1, 0x80,      // GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
            TFTLCD_DELAY8, 3,
            0x1F, 1, 0x90,      // GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
            TFTLCD_DELAY8, 5,
            0x1F, 1, 0xD0,      // GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
            TFTLCD_DELAY8, 5,
            //262k/65k color selection
            0x17, 1, 0x05,      //default 0x06 262k color // 0x05 65k color
            //SET PANEL
            0x36, 1, 0x00,      //SS_P, GS_P,REV_P,BGR_P
            //Display ON Setting
            0x28, 1, 0x38,      //GON=1, DTE=1, D=1000
            TFTLCD_DELAY8, 40,
            0x28, 1, 0x3F,      //GON=1, DTE=1, D=1100
            0x16, 1, 0x18,
        };


void lcd_init()
{

	int size = sizeof(HX8347I_inittab);
	const uint8_t* tab = HX8347I_inittab;

	while (size)
	{
		uint8_t reg, len;

		reg = pgm_read_byte(tab++);
		size --;
		len = pgm_read_byte(tab++);
		size --;

		if (reg == TFTLCD_DELAY8)
		{	
			for (int i = 0; i < len; i++)
				_delay_us(1);
		}
		else
		{
			for (int i = 0; i < len; i++)
			{
				lcd_write_reg(reg, pgm_read_byte(tab++));
				DEBUG("Reg %x = %x\n", reg, lcd_read_reg(reg));
				reg++;
				size --;
			}
		}
	}

}

static void lcd_setup_gram()
{
	PORTC = COMMAND_PINS; 
	DDRC = COMMAND_PINS;

	// Set index to zero
	DDRB |= DATA_PINS_PORTB;
	DDRD |= DATA_PINS_PORTD;	

	PORTB = (PORTB & ~DATA_PINS_PORTB) | (0x22 & DATA_PINS_PORTB);
	PORTD = (PORTD & ~DATA_PINS_PORTD) | (0x22 & DATA_PINS_PORTD);

	PORTC &= ~(CS_PIN);

	PORTC &= ~(RS_PIN);
	PORTC &= ~(WR_PIN);
	PORTC |= WR_PIN;
	PORTC |= RS_PIN;
}

void lcd_wr_gram(uint8_t hi, uint8_t lo)
{
	PORTB = (PORTB & ~DATA_PINS_PORTB) | (hi & DATA_PINS_PORTB);
	PORTD = (PORTD & ~DATA_PINS_PORTD) | (hi & DATA_PINS_PORTD);
	PORTC &= ~(WR_PIN);
	PORTC |= WR_PIN;
	PORTB = (PORTB & ~DATA_PINS_PORTB) | (lo & DATA_PINS_PORTB);
	PORTD = (PORTD & ~DATA_PINS_PORTD) | (lo & DATA_PINS_PORTD);
	PORTC &= ~(WR_PIN);
	PORTC |= WR_PIN;
}

void lcd_finish_gram()
{
	DDRB &= ~DATA_PINS_PORTB;
	DDRD &= ~DATA_PINS_PORTD;

	PORTC |= CS_PIN;
}

void lcd_clear(uint16_t col)
{
	lcd_fill_rect(0, 0, 240, 320, col);
}

void lcd_setup_area(int x, int y, int w, int h)
{
	lcd_write_reg(2, x>>8);
	lcd_write_reg(3, x&0xff);
	lcd_write_reg(4, (x+w-1)>>8);
	lcd_write_reg(5, (x+w-1)&0xff);
	lcd_write_reg(6, y>>8);
	lcd_write_reg(7, y&0xff);
	lcd_write_reg(8, (y+h-1)>>8);
	lcd_write_reg(9, (y+h-1)&0xff);
}

void lcd_fill_rect(int x, int y, int w, int h, uint16_t col)
{
	lcd_setup_area(x, y, w, h);

	lcd_setup_gram();

	uint8_t hi = col >> 8;
	uint8_t lo = col & 0xff;

	for (int yy = 0; yy < h; yy++)
	{
		for (int xx = 0; xx < w; xx++)
		{
			lcd_wr_gram(hi, lo);
		}
	}	

	lcd_finish_gram();
}

void lcd_draw_bitmap(int x, int y, int w, int h, uint16_t fg_col, uint16_t bg_col, const uint8_t* bitmap)
{
	lcd_setup_area(x, y, w, h);

	lcd_setup_gram();

	uint8_t hi_fg = fg_col >> 8;
	uint8_t lo_fg = fg_col & 0xff;
	uint8_t hi_bg = bg_col >> 8;
	uint8_t lo_bg = bg_col & 0xff;

	uint8_t curbyte = 0;
	uint8_t bits = 0;

	for (int yy = 0; yy < h; yy++)
	{
		for (int xx = 0; xx < w; xx++)
		{
			if (bits == 0)
			{
				curbyte = pgm_read_byte(bitmap++);
				bits = 8;
			}

			if (curbyte & 0x80)
				lcd_wr_gram(hi_fg, lo_fg);
			else
				lcd_wr_gram(hi_bg, lo_bg);
			
			curbyte <<= 1;
			bits --;
		}
	}	

	lcd_finish_gram();
}




