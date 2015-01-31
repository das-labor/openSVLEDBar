#include "main.h"
#include "lcd.h"
#include "menu.h"

uint8_t menuSleepCounter = 0;

int main() {
	initLCD();
	menuSleep();
	
}