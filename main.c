#include <avr/io.h>
#include <avr/interrupt.h>

#include "settings.h"
#include "main.h"
#include "lcd.h"
#include "menu.h"
#include "animations.h"

#define KEY_MENU	(1 << PD4)
#define KEY_UP		(1 << PD5)
#define KEY_DOWN	(1 << PD6)

uint8_t buttonValue, buttonStatus, longEnter, longPrev, longNext;
uint16_t sleepTimer = 0, minuteTimer = 0, bpmTimer = 0;
uint8_t pwmClock, timerTicks = 0;

extern tMode menuMode;
extern tSettings settings;
extern uint16_t bpmTime;

tColor color[3], fadeColor[3];

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

	if (pwmClock < color[0].rgb[0]) {
		PORTC |= (1 << PC7);
	} else {
		PORTC &= ~(1 << PC7);
	}
	if (pwmClock < color[0].rgb[1]) {
		PORTC |= (1 << PC6);
	} else {
		PORTC &= ~(1 << PC6);
	}
	if (pwmClock < color[0].rgb[2]) {
		PORTC |= (1 << PC5);
	} else {
		PORTC &= ~(1 << PC5);
	}
	if (pwmClock < color[1].rgb[0]) {
		PORTC |= (1 << PC4);
	} else {
		PORTC &= ~(1 << PC4);
	}
	if (pwmClock < color[1].rgb[1]) {
		PORTC |= (1 << PC3);
	} else {
		PORTC &= ~(1 << PC3);
	}
	if (pwmClock < color[1].rgb[2]) {
		PORTC |= (1 << PC2);
	} else {
		PORTC &= ~(1 << PC2);
	}
	if (pwmClock < color[2].rgb[0]) {
		PORTC |= (1 << PC1);
	} else {
		PORTC &= ~(1 << PC1);
	}
	if (pwmClock < color[2].rgb[1]) {
		PORTC |= (1 << PC0);
	} else {
		PORTC &= ~(1 << PC0);
	}
	if (pwmClock < color[2].rgb[2]) {
		PORTD |= (1 << PD7);
	} else {
		PORTD &= ~(1 << PD7);
	}

	pwmClock++;
}

/* Timer 1: 2ms */
ISR(TIMER1_OVF_vect)
{
	TCNT1H = 0xE8;
	TCNT1L = 0x90;

	timerTicks++;
}

int main(void)
{
	// Enable LED Ports
	DDRC = 0xFF;
	DDRD |= (1 << PD7);

	initLCD();
	menuSleep();
	setupSettings();

	// Initialize Timers
	TCCR0 = (1 << CS01); // Prescaler 8
	TCNT0 = 0xE8;
	TCCR1A = 0;
	TCCR1B = (1 << CS11); // Prescaler 8
	TCNT1H = 0xE8;
	TCNT1L = 0x90;
	TIMSK |= (1 << TOIE1) | (1 << TOIE0);
	sei();

	// Enable pull-up resistors for buttons
	PORTD |= KEY_MENU | KEY_UP | KEY_DOWN;

	while (1) {
		if (timerTicks > 0) {
			// Simple key debouncing
			buttonValue |= ~(PIND | buttonStatus);
			buttonStatus = (~PIND) | (buttonStatus & buttonValue);

			while (timerTicks > 0) {
				timerTicks--;

				if (!(PIND & KEY_MENU)) {
					if (longEnter < 250) {
						longEnter++;
					}
				} else {
					longEnter = 0;
				}

				if (!(PIND & KEY_UP)) {
					if (longNext < 250) {
						longNext++;
					}
				} else {
					longNext = 0;
				}

				if (!(PIND & KEY_DOWN)) {
					if (longPrev < 250) {
						longPrev++;
					}
				} else {
					longPrev = 0;
				}

				if (sleepTimer > 0) {
					sleepTimer--;
				}

				if (++minuteTimer >= 30000) {
					/* Implement features that need to be run each minute (i.e. update operating minutes) */
					minuteTimer = 0;
				}

				if (menuMode == MODE_AUTO) {
					for (uint8_t i = 0; i < 3; i++) {
						for (uint8_t j = 0; j < 3; j++) {
							if (color[i].rgb[j] < fadeColor[i].rgb[j]) {
								color[i].rgb[j]++;
							} else if (color[i].rgb[j] > fadeColor[i].rgb[j]) {
								color[i].rgb[j]--;
							}
						}
					}
				}

				if (bpmTimer > 0) {
					bpmTimer--;
				}
			}
		}

		if (buttonValue & (KEY_MENU | KEY_UP | KEY_DOWN)) {
			//sleepTimer = 5000; // Set timer to 10 seconds
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
		} else {
			if ((~PIND) & (KEY_MENU | KEY_UP | KEY_DOWN)) {
				//sleepTimer = 5000; // Set timer to 10 seconds
			} else if (sleepTimer == 1) {
				menuSleep();
			}
			if (longEnter == 250) {
				menuLongEnter();
				longEnter = 0xFF;
			}
			if (longPrev >= 250) {
				menuPrev();
				longPrev = 245;
			}
			if (longNext >= 250) {
				menuNext();
				longNext = 245;
			}
		}

		if (bpmTimer == 0) {
			if (menuMode == MODE_AUTO) {
				bpmTimer = bpmTime;
				animate();
			}
		}

		/* TODO
			if (menuStatus <= STATUS_SETTING) {
				// save settings
				settings.mode = menuMode;
				settings.color[0] = color[0];
				settings.color[1] = color[1];
				settings.color[2] = color[2];
			}
		*/
	}
	
}