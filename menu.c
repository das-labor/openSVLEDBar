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
#include <stdbool.h>
#include "menu.h"
#include "lcd.h"
#include "settings.h"
#include "main.h"
#include "animations.h"

const char sleepStrings[][9] PROGMEM = {
	"OpenSVLB",
	"by labor"
};

const char modeString[] PROGMEM = "Mode";

const char menuModeStrings[][9] PROGMEM = {
	"Auto",
	"Fixed",
	"DMX 3ch",
	"DMX 9ch",
	"Slave",
	"Sound"
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

const char backString[] PROGMEM = "\x7f back";

const uint8_t modeSettings[] = {3, 9, 1, 1, 0, 2};

enum MENU_SETTING menuSetting;
enum MENU_STATUS menuStatus, lastMenuStatus;

extern tSettings settings;
extern tColor color[3];
extern bool settingsChanged;

/*
 * status = 0: Sleep
 * status = 1: Select Mode e.g. DMX 9ch
 * status = 2: Select Setting e.g. Fade time, DMX address
 * status = 3: Set value
 */

void menuSleep(void)
{
	menuStatus = STATUS_SLEEP;
	menuDraw();
}

void menuEnter(void)
{
	lastMenuStatus = menuStatus;
	switch (menuStatus) {
		case STATUS_SLEEP:
		menuStatus = STATUS_MODE;
		break;

		case STATUS_MODE:
		if (settings.mode == MODE_SLAVE) {
			menuSetting = SETTING_BACK; // Slave mode only has back
		} else {
			menuSetting = 1; // Skip back
		}

		// Validity check of the selected DMX-Channel
		if (settings.mode == MODE_DMX9CH && settings.dmxAddress > 504) {
			settings.dmxAddress = 504;
			settingsChanged = true;
		}

		menuStatus = STATUS_SETTING;
		break;

		case STATUS_SETTING:
		if (menuSetting == SETTING_BACK) {
			menuStatus = STATUS_MODE;
		} else {
			menuStatus = STATUS_VALUE;
		}
		break;

		case STATUS_VALUE:
		menuStatus = STATUS_SETTING;
		// Extra save  
		storeSettings();
		break;
	}
	menuDraw();
}

void menuLongEnter(void)
{
	// Long Enter goes back to the main Menu or to Sleep mode when there
	if (lastMenuStatus > STATUS_MODE) {
		menuStatus = STATUS_MODE;
	} else {
		menuStatus = STATUS_SLEEP;
	}
	lastMenuStatus = menuStatus;
	menuDraw();
}

void menuNext(void)
{
	switch (menuStatus) {
		case STATUS_SLEEP:
			menuStatus = STATUS_MODE;
			break;

		case STATUS_MODE:
			settings.mode++;
			if (settings.mode == NUMBER_MODES) {
				settings.mode = 0;
			}
			settingsChanged = true;
			break;

		case STATUS_SETTING:
			menuSetting++;
			if (menuSetting > modeSettings[settings.mode]) {
				menuSetting = 0;
			}
			break;

		case STATUS_VALUE:
			switch (settings.mode) {
				case MODE_AUTO:
				case MODE_SOUND:
					if (menuSetting == SETTING_PROGRAM) {
						if (settings.program < NUMBER_PROGRAMS - 1) {
							settings.program++;
						} else {
							settings.program = 0;
						}
						settingsChanged = true;
					} else if (menuSetting == SETTING_FADE) {
						if (settings.fade < 255) {
							settings.fade++;
							settingsChanged = true;
						}
					} else if (menuSetting == SETTING_SPEED) {
						if (settings.toggleBPM < 255) {
							settings.toggleBPM++;
							invalidateBPMTime();
							settingsChanged = true;
						}
					}
					break;

				case MODE_FIXED:
					if (color[(menuSetting - 1) / 3].rgb[(menuSetting - 1) % 3] < 255) {
						color[(menuSetting - 1) / 3].rgb[(menuSetting - 1) % 3]++;
						settingsChanged = true;
					}
					break;

				case MODE_DMX3CH:
					if (settings.dmxAddress < 510) {
						settings.dmxAddress++;
						settingsChanged = true;
					}
					break;

				case MODE_DMX9CH:
					if (settings.dmxAddress < 504) {
						settings.dmxAddress++;
						settingsChanged = true;
					}
					break;

				default:
					break;
			}
			break;
	}
	menuDraw();
}

void menuPrev(void)
{
	switch (menuStatus) {
		case STATUS_SLEEP:
			menuStatus = STATUS_MODE;
			break;

		case STATUS_MODE:
			if (settings.mode == 0) {
				settings.mode = NUMBER_MODES;
			}
			settings.mode--;
			settingsChanged = true;
			break;

		case STATUS_SETTING:
			if (menuSetting == 0) {
				menuSetting = modeSettings[settings.mode] + 1;
			}
			menuSetting--;
			break;

		case STATUS_VALUE:
			switch (settings.mode) {
				case MODE_AUTO:
				case MODE_SOUND:
					if (menuSetting == SETTING_PROGRAM) {
						if (settings.program > 0) {
							settings.program--;
						} else {
							settings.program = NUMBER_PROGRAMS - 1;
						}
						settingsChanged = true;
					} else if (menuSetting == SETTING_FADE) {
						if (settings.fade > 0) {
							settings.fade--;
						}
					} else if (menuSetting == SETTING_SPEED) {
						if (settings.toggleBPM > 1) {
							settings.toggleBPM--;
							invalidateBPMTime();
							settingsChanged = true;
						}
					}
					break;

				case MODE_FIXED:
					if (color[(menuSetting - 1) / 3].rgb[(menuSetting - 1) % 3] > 0) {
						color[(menuSetting - 1) / 3].rgb[(menuSetting - 1) % 3]--;
						settingsChanged = true;
					}
					break;

				case MODE_DMX3CH:
				case MODE_DMX9CH:
					if (settings.dmxAddress > 1) {
						settings.dmxAddress--;
						settingsChanged = true;
					}
					break;

				default:
					break;
			}
			break;
	}
	menuDraw();
}

void menuDraw(void)
{
	lcdSetCursor(0);
	lcdClear();
	if (menuStatus == STATUS_SLEEP) {
		// Draw Sleep
		lcdPuts_p(0, 0, sleepStrings[0]);
		lcdPuts_p(0, 1, sleepStrings[1]);
	} else {
		uint8_t value = 0;
		for (uint8_t y = 0; y < 2; y++) {
			switch (menuStatus+y) {
				case STATUS_MODE: // Draw Mode Menu
				lcdPuts_p(2, y, modeString);
				break;

				case STATUS_MODE+1: // Draw Mode
				lcdPuts_p(0, y, menuModeStrings[settings.mode]);
				value = 8 * settings.mode / NUMBER_MODES;
				break;

				case STATUS_SETTING+1: // Draw Setting
				if (menuSetting == SETTING_BACK) {
					// Back setting
					lcdPuts_p(1, y, backString);
				} else {
					// Other settings
					switch (settings.mode) {
						case MODE_AUTO:
						case MODE_SOUND:
						lcdPuts_p(0, y, autoModeStrings[menuSetting - 1]);
						break;

						case MODE_FIXED:
						lcdPuts_p(0, y, colorStrings[(menuSetting - 1) % 3]);
						lcdPutn(7, y, 1, (menuSetting - 1) / 3);
						break;

						case MODE_DMX3CH:
						case MODE_DMX9CH:
						lcdPuts_p(0, y, addressString);
						break;

						default:
						break;
					}
				}
				break;

				case STATUS_VALUE+1: // Draw Value
				switch (settings.mode) {
					case MODE_AUTO:
					case MODE_SOUND:
					if (menuSetting == SETTING_PROGRAM) {
						lcdPutn(0, y, 8, settings.program + 1);
					} else if (menuSetting == SETTING_FADE) {
						if (settings.fade == 0) {
							lcdPuts_p(5, y, fadeStrings[0]);
						} else {
							lcdPutf(0, y, 4, 1, (float)settings.fade / 10.0f);
							lcdPuts_p(5, y, fadeStrings[1]);
						}
					} else if (menuSetting == SETTING_SPEED) {
						lcdPutn(0, y, 4, settings.toggleBPM);
						lcdPuts_p(5, y, BPMString);
						value = settings.toggleBPM / 32;
					}
					break;

					case MODE_FIXED:
 					lcdPutn(0, y, 8, color[(menuSetting - 1) / 3].rgb[(menuSetting - 1) % 3]);
					break;

 					case MODE_DMX3CH:
					case MODE_DMX9CH:
 					lcdPutw(0, y, 8, settings.dmxAddress);
					break;

					default:
					break;
 				}
				break;

				default:
				break;
			}
		}
		lcdSetPosition(value, 1);
		lcdSetCursor(1);
	}
}