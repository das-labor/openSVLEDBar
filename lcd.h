#ifndef _LCD_H_
#define _LCD_H_

#include <avr/pgmspace.h>

void initLCD(void);

void lcdWaitBusyFlag(void);
void lcdWriteCommand(uint8_t command);
void lcdWriteData(uint8_t data);

void lcdClear(void);
void lcdPutc(uint8_t x, uint8_t y, char c);
void lcdPuts(uint8_t x, uint8_t y, const char* string);
void lcdPuts_p(uint8_t x, uint8_t y, const char* string);

void lcdPutn(uint8_t x, uint8_t y, uint8_t width, uint8_t n);
void lcdPutw(uint8_t x, uint8_t y, uint8_t width, uint16_t n);
void lcdPutf(uint8_t x, uint8_t y, uint8_t width, uint8_t decimals, float n);

#endif