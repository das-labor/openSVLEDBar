#include "main.h"
#include "lcd.h"
#include "menu.h"
#include "settings.h"

#define KEY_MENU	(1 << PD4)
#define KEY_UP		(1 << PD5)
#define KEY_DOWN	(1 << PD6)

/* Timer 0: 125kHz Software PWM */
ISR(TIMER0_OVF_vect) {
	TCNT0 = 0xE8;

	/* TODO: Implement Software PWM for 9 Channels */
}

uint8_t buttonValue, buttonStatus;
uint16_t timerCount = 0;

/* Timer 1: 10ms */
ISR(TIMER1_OVF_vect) {
	TCNT1H = 0x8A;
	TCNT1L = 0xD0;

	// Simple key debouncing
	buttonValue |= ~(PIND | buttonStatus);
	buttonStatus = ~PIND | (buttonStatus & buttonValue);

	timerCount++;
	if(timerCount >= 1000) { // 10 seconds
		menuSleep();
		timerCount = 0;
	}
}

int main() {
	initLCD();
	menuSleep();

	// Initialize Timers
	TCCR0 = (1 << CS01); // Prescaler 8
	TCNT0 = 0xE8;
	TCCR1A = 0;
	TCCR1B = (1 << CS11); // Prescaler 8
	TCNT1H = 0x8A;
	TCNT1L = 0xD0;
	TIMSK1 |= (1 << TOIE1) | (1 << TOIE0);
	sei();

	// Enable pull-up resistors for buttons
	PORTD |= (1 << PD4) | (1 << PD5) | (1 << PD6);
	
	while(1) {
		if(buttonValue & KEY_MENU) {
			// Enter menu
			menuDraw();
		}
		if(buttonValue & KEY_UP) {
			if(menuMode <= 6) {
				menuMode++; // go to next menu item
			} else {
				menuMode = 0;
			}
			menuDraw();
		}
		if(buttonValue & KEY_DOWN) {
			if(menuMode > 0) {
				menuMode--; // go to previous menu item
			} else {
				menuMode = 6;
			}
			menuDraw();
		}
	}
	
}