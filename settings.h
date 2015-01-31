#ifndef _SETTINGS_H_
#define _SETTINGS_H_

enum MODE {
	MODE_AUTO,
	MODE_FIXED,
	MODE_DMX,
	MODE_SLAVE
};

enum DIVISION {
	DIV_THREE,
	DIV_ONE
};

typedef union {
	struct {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};
	uint8_t rgb[3];
} tColor;

typedef struct {
	enum MODE mode;
	enum DIVISION division;
	uint8_t fade; /* 0 = Off, 1-255 = 0.1-25.5 seconds */
	uint8_t toggleBPM; /* 0 = Off, 1-254 = BPM, 255 = SoundToLight */
	tColor color[3];
	uint16_t dmxChannel;
	uint8_t program;
} tSettings;

union tSettingsData {
	tSettings settings;
	uint8_t data[sizeof(tSettings)];
};

#endif