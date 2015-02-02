#ifndef _MENU_H_
#define _MENU_H_

void menuSleep();
void menuDraw();

void nextMode();
void prevMode();

void nextSetting();
void prevSetting();

extern uint8_t menuMode;
extern uint8_t mode; 
/*
*mode=0: Select Mode i.e. DMX 9ch
*mode=1: Select Setting i.e. fade time, dmx addr.
*mode=2: Set value	
*/
#endif