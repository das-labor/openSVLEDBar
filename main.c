#include "main.h"
#include "lcd.h"
#include "menu.h"
#include "settings.h"

uint8_t menuSleepCounter = 0;
/*
	Button K1	PD4		Menu/Enter
	Button K2	PD5		up
	Button K3	PD6		down
*/
int main() {
	initLCD();
	lcdPuts(0, 0, "foo");
	menuSleep();
	lcdPuts(0, 0, "bar");
	//enable pullups for buttons
	PORTD |= (1<<PD4);
	PORTD |= (1<<PD5);
	PORTD |= (1<<PD6);
	
	while(1)
	{
		if(PIND & (1<<PD4))
		{
			//enter menu
			menuDraw();
		}
		if(PIND & (1<<PD4))
		{
			if(menuMode<=6){menuMode++;} //go to next menu item
				else{menuMode=0;}
			menuDraw();
		}
		if(PIND & (1<<PD4))
		{
			if(menuMode>0){menuMode--;}	//go to previous menu item
			else{menuMode=6;}
			menuDraw();
		}
	}
	
}