/* Menu:
- Mode
  - Auto
    - Program 1-5
    - Fade Time
    - Speed
    - back
  - Fixed
    - Segment 1 Red 0-255
    - Segment 1 Green 0-255
    - Segment 1 Blue 0-255
    - Segment 2 Red 0-255
    - Segment 2 Green 0-255
    - Segment 2 Blue 0-255
    - Segment 3 Red 0-255
    - Segment 3 Green 0-255
    - Segment 3 Blue 0-255
    - back
  - DMX 3ch
    - Address 1-510
    - back
  - DMX 9ch
    - Address 1-504
    - back
  - Slave
    - back
  - Sound
    - Program 1-5
    - Fade Time
    - back
*/

#include <avr/pgmspace.h>
#include "lcd.h"
#include "settings.h"

const char sleepStrings[][9] PROGMEM = {
	"OpenSVLB",
	"by labor",
};

const char modeString[] PROGMEM = "Mode";

const char menuModeStrings[][9] PROGMEM = {
	"Auto",
	"Fixed",
	"DMX 3ch",
	"DMX 9ch",
	"Slave",
	"Sound",
};

const char autoModeStrings[][9] PROGMEM = {
	"Program",
	"Speed",
	"Fade"
};

const char BPMString[] PROGMEM = "BPM";

const char fadeStrings[][9] PROGMEM = {
	"Off",
	"sec"
};

const char colorStrings[][9] PROGMEM = {
	"Red",
	"Green",
	"Blue"
};

const char addressString[] PROGMEM = "Address";

const char backString[] PROGMEM = "\x1e back";

#define MENU_STRING_FIRST_MODE	3
#define MENU_STRING_BACK		9
#define MENU_STRING_FIRST_AUTO	10

enum MODE menuMode;
uint8_t menuSetting;
uint8_t menuStatus;
/*
 * status = 0: Sleep
 * status = 1: Select Mode e.g. DMX 9ch
 * status = 2: Select Setting e.g. Fade time, DMX address
 * status = 3: Set value
 */

void menuSleep(void)
{
	menuStatus = 0;
	menuDraw();
}

void menuEnter(void)
{
	if(menuStatus == 0) {
		menuStatus = 1;
	} else if(menuStatus == 1) {
		menuSetting = 1; // Skip back
		menuStatus = 2;
	}
	menuDraw();
}

void menuNext(void)
{

}

void menuPrev(void)
{

}

void menuDraw(void)
{
	lcdClear();
	switch(menuStatus) {
		case 0: // Draw Sleep
		lcdPuts_p(0, 0, &sleepStrings[0]);
		lcdPuts_p(0, 1, &sleepStrings[1]);
		break;

		case 1: // Draw Mode
		lcdPuts_p(2, 0, &modeString);
		lcdPuts_p(0, 1, &menuModeStrings[menuMode]);
		break;

		case 2: // Draw Setting
		lcdPuts_p(0, 1, &menuModeStrings[menuMode]);
		if(menuSetting == 0) {
			// Back setting
			lcdPuts_p(1, 1, &backString);
		} else {
			// Other settings
			if(menuMode == MODE_AUTO || menuMode == MODE_SOUND) {
				lcdPuts_p(0, 1, &autoModeStrings[menuSetting - 1]);
			} else if(menuMode == MODE_FIXED) {
				lcdPuts_p(0, 1, &colorStrings[(menuSetting - 1) % 3]);
				lcdPutc(7, 1, '1' + (menuSetting - 1) / 3);
			} else if(menuMode == MODE_DMX3CH || menuMode == MODE_DMX9CH) {
				lcdPuts_p(0, 1, &addressString);
			}
		}
		break;

		case 3: // Draw Value
		// TODO
	}
}