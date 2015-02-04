#ifndef _MENU_H_
#define _MENU_H_

enum MENU_SETTING {
	SETTING_BACK = 0,

	SETTING_PROGRAM = 1,
	SETTING_FADE,
	SETTING_SPEED,

	SETTING_RED_1 = 1,
	SETTING_GREEN_1,
	SETTING_BLUE_1,
	SETTING_RED_2,
	SETTING_GREEN_2,
	SETTING_BLUE_2,
	SETTING_RED_3,
	SETTING_GREEN_3,
	SETTING_BLUE_3,

	SETTING_ADDRESS = 1
};

void menuDraw(void);

void menuSleep(void);

void menuEnter(void);
void menuNext(void);
void menuPrev(void);

#endif