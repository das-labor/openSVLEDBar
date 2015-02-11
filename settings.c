#include "settings.h"

tSettings settings = {MODE_AUTO, 5, 120, 1, 0, {{{{0, 0, 0}}}, {{{0, 0, 0}}}, {{{0, 0, 0}}}}};

uint16_t bpmTime;

void setupSettings()
{
	invalidateBPMTime();
}

void invalidateBPMTime()
{
	bpmTime = (uint16_t)(0.5 + (60.0 * 1000.0 / 2.0) / (double)settings.toggleBPM);
}