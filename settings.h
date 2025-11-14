#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdint.h>

typedef enum {
	MODE_AUTO,
	MODE_FIXED,
	MODE_DMX3CH,
	MODE_DMX9CH,
	MODE_SLAVE,
	MODE_SOUND
} tMode;

#define NUMBER_MODES 6

#define SET_COLOR(c, r, g, b) c.red = (r); c.green = (g); c.blue = (b);

typedef struct {
	union {
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
		};
		uint8_t rgb[3];
		uint32_t data : 24;
	};
} tColor;

typedef struct {
	tMode mode;
	uint8_t fade; /* 0 = Off, 1-255 = 0.1-25.5 seconds */
	uint8_t toggleBPM; /* 1-255 = BPM */
	uint16_t dmxAddress;
	uint8_t program;
	tColor color[3];
} tSettings;

void setupSettings(void);
void invalidateBPMTime(void);
void storeSettings(void);

#endif