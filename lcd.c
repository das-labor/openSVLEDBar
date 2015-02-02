#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "lcd.h"

#define LCD_RW (1 << PB0)
#define LCD_RS (1 << PB1)
#define LCD_EN (1 << PB2)

#define LCD_ENABLE_US 20

void initLCD() {
	/* initialize Ports */
	PORTA = 0x00;
	DDRA = 0xFF;
	PORTB &= ~(LCD_RW | LCD_RS | LCD_EN);
	DDRB |= LCD_RW | LCD_RS | LCD_EN;

	/* Bootup of the display */
	_delay_ms(15);
	PORTA = 0x38;
	PORTB |= LCD_EN;
	_delay_us(LCD_ENABLE_US);
	PORTB &= ~LCD_EN;
	_delay_ms(5);
    PORTB |= LCD_EN;
    _delay_us(LCD_ENABLE_US);
    PORTB &= ~LCD_EN;
	_delay_ms(1);
    PORTB |= LCD_EN;
    _delay_us(LCD_ENABLE_US);
    PORTB &= ~LCD_EN;
	_delay_ms(1);

	lcdClear(); /* Display clear */
	lcdWriteCommand(0x38); /* Two lines, 8-bit mode */
	lcdWriteCommand(0x08); /* Display all off */
	lcdWriteCommand(0x06); /* Entry Increment no shift */
	lcdWriteCommand(0x0C); /* Display on */
}

void lcdWaitBusyFlag() {
	uint8_t i = 0;
	/* Wait for busy flag */
	while(i++ < 255) {
		PORTA = 0xFF;
		PORTB &= ~LCD_RS;
		PORTB |= LCD_RW;
		DDRA = 0x00;
		PORTB |= LCD_EN;
		_delay_ms(1);
		if(!(PINA & (1 << PA7))) {
			break;
		}
	}
	PORTB &= ~(LCD_EN | LCD_RW);
	DDRA = 0xFF;
}

void lcdWriteCommand(uint8_t command) {
	lcdWaitBusyFlag();

	PORTB &= ~LCD_RS;
	PORTA = command;
	PORTB |= LCD_EN;
	_delay_us(LCD_ENABLE_US);
	PORTB &= ~LCD_EN;
}

void lcdWriteData(uint8_t data) {
	lcdWaitBusyFlag();

	PORTB |= LCD_RS;
	PORTA = data;
	PORTB |= LCD_EN;
	_delay_us(LCD_ENABLE_US);
	PORTB &= ~LCD_EN;
}

void lcdClear() {
	lcdWriteCommand(0x01);
}

void lcdPuts(uint8_t x, uint8_t y, const char* string) {
	/* Simplified set address */
	if(y) y = 0x40;
	lcdWriteCommand(0x80 | y | x);

	while(*string != 0) {
		lcdWriteData(*string++);
	}
}

void lcdPuts_p(uint8_t x, uint8_t y, const char* string PROGMEM) {
	/* Simplified set address */
	if(y) y = 0x40;
	lcdWriteCommand(0x80 | y | x);

	uint8_t c;
	while(c = pgm_read_byte(string++)) {
		lcdWriteData(c);
	}
}