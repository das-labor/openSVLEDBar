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
#include "menu.h"
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
	"Fade",
	"Speed"
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
enum MENU_SETTING menuSetting;
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
	switch(menuStatus) {
		case 0:
		menuStatus = 1;
		break;

		case 1:
		if(menuMode == MODE_SLAVE) {
			menuSetting = SETTING_BACK; // Slave mode only has back
		} else {
			menuSetting = 1; // Skip back
		}
		menuStatus = 2;
		break;

		case 2:
		// Anything else to do?
		if(menuSetting == SETTING_BACK) {
			menuStatus = 1;
		} else {
			menuStatus = 3;
		}
		break;

		case 3:
		// Anything else to do?
		menuStatus = 2;
		break;
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
	if(menuStatus == 0) {
		// Draw Sleep
		lcdPuts_p(0, 0, sleepStrings[0]);
		lcdPuts_p(0, 1, sleepStrings[1]);
	} else {
		for(uint8_t y = 0; y < 2; y++) {
			switch(menuStatus+y) {
				case 1: // Draw Mode Menu
				lcdPuts_p(2, y, modeString);
				break;

				case 2: // Draw Mode
				lcdPuts_p(0, y, menuModeStrings[menuMode]);
				break;

				case 3: // Draw Setting
				if(menuSetting == SETTING_BACK) {
					// Back setting
					lcdPuts_p(1, y, backString);
				} else {
					// Other settings
					if(menuMode == MODE_AUTO || menuMode == MODE_SOUND) {
						lcdPuts_p(0, y, autoModeStrings[menuSetting - 1]);
					} else if(menuMode == MODE_FIXED) {
						lcdPuts_p(0, y, colorStrings[(menuSetting - 1) % 3]);
						lcdPutn(7, y, 1, (menuSetting - 1) / 3);
					} else if(menuMode == MODE_DMX3CH || menuMode == MODE_DMX9CH) {
						lcdPuts_p(0, y, addressString);
					}
				}
				break;

				case 4: // Draw Value
				if(menuMode == MODE_AUTO || menuMode == MODE_SOUND) {
					if(menuSetting == SETTING_PROGRAM) {
						lcdPutn(0, y, 8, settings.program);
					} else if(menuSetting == SETTING_FADE) {
						if(settings.fade == 0) {
							lcdPuts_p(5, y, fadeStrings[0]);
						} else {
							lcdPutf(0, y, 4, 1, (float)settings.fade / 10.0f);
							lcdPuts_p(5, y, fadeStrings[1]);
						}
					} else if(menuSetting == SETTING_SPEED) {
						lcdPutn(0, y, 4, settings.toggleBPM);
						lcdPuts_p(5, y, BPMString);
					}
 				} else if(menuMode == MODE_FIXED) {
 					lcdPutn(0, y, 8, settings.color[(menuSetting - 1) / 3].rgb[(menuSetting - 1) % 3]);
 				} else if(menuMode == MODE_DMX3CH || menuMode == MODE_DMX9CH) {
 					lcdPutw(0, y, 8, settings.dmxAddress);
 				}
				break;
			}
		}
	}
}