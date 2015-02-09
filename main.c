#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "lcd.h"
#include "menu.h"
#include "settings.h"

#define KEY_MENU	(1 << PD4)
#define KEY_UP		(1 << PD5)
#define KEY_DOWN	(1 << PD6)

uint8_t buttonValue, buttonStatus, longEnter, longPrev, longNext;
uint16_t sleepTimer = 0, minuteTimer = 0;

void uartPutc(uint8_t c)
{
	while (!(UCSRA & (1<<UDRE)));
	UDR = c;
}

void uartPuts(char *str)
{
	while (*str) {
		uartPutc(*str++);
	}
}

/* Timer 0: 125kHz Software PWM */
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 0xE8;

	/* TODO: Implement Software PWM for 9 Channels */
}

/* Timer 1: 10ms */
ISR(TIMER1_OVF_vect)
{
	TCNT1H = 0x8A;
	TCNT1L = 0xD0;

	// Simple key debouncing
	buttonValue |= ~(PIND | buttonStatus);
	buttonStatus = (~PIND) | (buttonStatus & buttonValue);

	if (!(PIND & KEY_MENU)) {
		if (longEnter < 50) {
			longEnter++;
		}
	} else {
		longEnter = 0;
	}

	if (!(PIND & KEY_UP)) {
		if (longPrev <= 50) {
			longPrev++;
		}
	} else {
		longPrev = 0;
	}

	if (!(PIND & KEY_DOWN)) {
		if (longNext <= 50) {
			longNext++;
		}
	} else {
		longNext = 0;
	}

	if (sleepTimer > 0) {
		sleepTimer--;
	}

	if (++minuteTimer >= 6000) {
		/* Implement features that need to be run each minute (i.e. update operating minutes) */
		minuteTimer = 0;
	}
}

int main(void)
{
	initLCD();
	menuSleep();

	// Initialize Timers
	TCCR0 = (1 << CS01); // Prescaler 8
	TCNT0 = 0xE8;
	TCCR1A = 0;
	TCCR1B = (1 << CS11); // Prescaler 8
	TCNT1H = 0x8A;
	TCNT1L = 0xD0;
	TIMSK |= (1 << TOIE1) | (1 << TOIE0);
	sei();

	// Enable pull-up resistors for buttons
	PORTD |= (1 << PD4) | (1 << PD5) | (1 << PD6);

	DDRC = 0xFF;
	DDRD |= (1 << PD7);

	while (1) {
		if (buttonValue & (KEY_MENU | KEY_UP | KEY_DOWN)) {
			sleepTimer = 1000; // Set timer to 10 seconds
			if (buttonValue & KEY_MENU) {
				menuEnter();
			}
			if (buttonValue & KEY_UP) {
				menuNext();
			}
			if (buttonValue & KEY_DOWN) {
				menuPrev();
			}
			buttonValue &= ~(KEY_MENU | KEY_UP | KEY_DOWN);
		}
		if (longEnter == 50) {
			menuLongEnter();
			longEnter = 0xFF;
		}
		if (longPrev > 50) {
			menuPrev();
			longPrev = 50;
		}
		if (longNext > 50) {
			menuNext();
			longNext = 50;
		}
		if (sleepTimer == 1) {
			menuSleep();
		}
	}
	
}