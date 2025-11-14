#include <avr/eeprom.h>
#include <stdbool.h>
#include "settings.h"

const tSettings defaultSettings = {MODE_AUTO, 5, 120, 1, 0, {{{{0, 0, 0}}}, {{{0, 0, 0}}}, {{{0, 0, 0}}}}};
tSettings settings = defaultSettings;
tSettings storedSettings EEMEM = defaultSettings;

#define VALID_EEPROM 0xAA
uint8_t validEepromFlag EEMEM = VALID_EEPROM;

uint16_t bpmTime;

extern uint16_t bpmTimer;

extern tColor color[3];

bool settingsChanged = false;

void setupSettings()
{
	// Load from eeprom
	if (eeprom_read_byte(&validEepromFlag) == VALID_EEPROM) {
		eeprom_read_block(&settings, &storedSettings, sizeof(tSettings));
	}

	// Update color if in fixed mode
	if (settings.mode == MODE_FIXED) {
		color[0] = settings.color[0];
		color[1] = settings.color[1];
		color[2] = settings.color[2];
	}

	invalidateBPMTime();
}

void invalidateBPMTime()
{
	uint16_t newTime = (uint16_t)(0.5 + (60.0 * 1000.0 / 2.0) / (double)settings.toggleBPM);
	if(bpmTime > newTime) {
		bpmTimer = 0;
	}
	bpmTime = newTime;
}

void storeSettings()
{
	// Only store color if in fixed mode
	if (settings.mode == MODE_FIXED) {
		settings.color[0] = color[0];
		settings.color[1] = color[1];
		settings.color[2] = color[2];
	}

	// Store to eeprom
	eeprom_update_block(&settings, &storedSettings, sizeof(tSettings));

	settingsChanged = false;
}