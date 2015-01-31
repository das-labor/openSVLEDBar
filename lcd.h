#ifndef _LCD_H_
#define _LCD_H_

#include <avr/pgmspace.h>

void initLCD();

void lcdWaitBusyFlag();
void lcdWriteCommand(uint8_t);
void lcdWriteData(uint8_t);

void lcdClear();
void lcdPuts(uint8_t, uint8_t, const char*);
void lcdPuts_p(uint8_t, uint8_t, const char* PROGMEM);

#endif