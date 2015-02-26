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
tDMXstatus dmxStatus;
uint16_t dmxCounter;
uint8_t dmxResetCounter, dmxOffset;
uint8_t soundTrigger;

extern tMode menuMode;
extern tSettings settings;
extern uint16_t bpmTime;

tColor color[3], fadeColor[3];
double fadeFrameDiff[][3]	= {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};
double fadeFrameCarry[][3]	= {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};

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

/* DMX Data Received */
ISR(USART_RX_vect)
{
	uint8_t rxStatus = UCSRB;
	uint8_t rxData = UDR;
	if (rxStatus & (1 << RXB8)) {
		if (!(dmxStatus & DMX_FINISHED)) {
			if (!(dmxStatus & DMX_STARTED)) {
				if (rxData == 0x00) {
					dmxStatus |= DMX_STARTED;
					dmxOffset = 0;
					dmxCounter = settings.dmxAddress - 1;
				}
			} else {
				if (dmxCounter == 0) {
					if (menuMode == MODE_DMX9CH) {
						color[dmxOffset / 3].rgb[dmxOffset % 3] = rxData;
						dmxOffset++;
						if (dmxOffset > 8) {
							dmxStatus |= DMX_FINISHED;
						}
					} else if (menuMode == MODE_DMX3CH) {
						color[0].rgb[dmxOffset] = color[1].rgb[dmxOffset] = color[2].rgb[dmxOffset] = rxData;
						dmxOffset++;
						if (dmxOffset > 2) {
							dmxStatus |= DMX_FINISHED;
						}
					}
				} else {
					dmxCounter--;
				}
			}
		}
	} else {
		dmxStatus = 0;
		dmxResetCounter = 40;
	}
}

ISR(INT1_vect) {
	soundTrigger++;
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

	// Enable DMX UART
	DDRD |= (1 << PD2);
	PORTD &= ~(1 << PD2);
	UBRRH = 0x00;
	UBRRL = 0x05; // Set Baudrate to 125kHz
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
	UCSRB = (1 << RXEN) | (1 << UCSZ2);
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

				if (menuMode == MODE_AUTO || menuMode == MODE_SOUND) {
					for (uint8_t i = 0; i < 3; i++) {
						for (uint8_t j = 0; j < 3; j++) { 
							if (color[i].rgb[j] != fadeColor[i].rgb[j]) {
								double newDiff = fadeFrameDiff[i][j] + fadeFrameCarry[i][j];
								if (newDiff == 0.0)
									continue;
								if (color[i].rgb[j] > fadeColor[i].rgb[j] && color[i].rgb[j] < (uint8_t)(-newDiff)) {
									color[i].rgb[j] = 0;
								} else if (color[i].rgb[j] < fadeColor[i].rgb[j] && color[i].rgb[j] > (uint8_t)(255.0 - newDiff)) {
									color[i].rgb[j] = 255;
								} else {
									color[i].rgb[j] += (uint8_t)newDiff;
									fadeFrameCarry[i][j] = newDiff - (int16_t)(newDiff);
								}
							}
						}
					}
				} else if (menuMode == MODE_DMX3CH || menuMode == MODE_DMX9CH) {
					if(dmxResetCounter > 1) {
						dmxResetCounter--;
					} else if(dmxResetCounter == 1) {
						dmxStatus |= DMX_FINISHED;
						SET_COLOR(color[0], 0, 0, 0);
						SET_COLOR(color[1], 0, 0, 0);
						SET_COLOR(color[2], 0, 0, 0);
						dmxResetCounter = 0;
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

		if (menuMode == MODE_DMX3CH || menuMode == MODE_DMX9CH) {
			UCSRB |= (1 << RXCIE);
		} else {
			UCSRB &= ~(1 << RXCIE);
		}

		if (menuMode == MODE_SOUND) {
			GICR |= (1 << INT1);
		} else {
			GICR &= ~(1 << INT1);
		}

		switch (menuMode) {
			case MODE_AUTO:
			case MODE_SOUND:
				if (menuMode == MODE_AUTO && bpmTimer == 0) {
					bpmTimer = bpmTime;
				} else if (menuMode == MODE_SOUND && soundTrigger > 0) {
					soundTrigger = 0;
				} else {
					break;
				}

				int16_t fadeDiff;
				double fadeSlots = (double)settings.fade * 50.0;
				bpmTimer = bpmTime;
				animate();
				fadeDiff = (int16_t)fadeColor[0].red - (int16_t)color[0].red;
				if (fadeDiff != 0) {
					fadeFrameDiff[0][0] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[0][0] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[0].green - (int16_t)color[0].green;
				if (fadeDiff != 0) {
					fadeFrameDiff[0][1] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[0][1] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[0].blue - (int16_t)color[0].blue;
				if (fadeDiff != 0) {
					fadeFrameDiff[0][2] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[0][2] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[1].red - (int16_t)color[1].red;
				if (fadeDiff != 0) {
					fadeFrameDiff[1][0] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[1][0] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[1].green - (int16_t)color[1].green;
				if (fadeDiff != 0) {
					fadeFrameDiff[1][1] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[1][1] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[1].blue - (int16_t)color[1].blue;
				if (fadeDiff != 0) {
					fadeFrameDiff[1][2] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[1][2] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[2].red - (int16_t)color[2].red;
				if (fadeDiff != 0) {
					fadeFrameDiff[2][0] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[2][0] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[2].green - (int16_t)color[2].green;
				if (fadeDiff != 0) {
					fadeFrameDiff[2][1] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[2][1] = 0.0;
				}
				fadeDiff = (int16_t)fadeColor[2].blue - (int16_t)color[2].blue;
				if (fadeDiff != 0) {
					fadeFrameDiff[2][2] = (double)fadeDiff / fadeSlots;
				} else {
					fadeFrameDiff[2][2] = 0.0;
				}

				break;

			default:
				break;
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