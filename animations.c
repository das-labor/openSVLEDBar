#include <stdlib.h>
#include "settings.h"

extern tColor color[3];
extern tColor fadeColor[3];
extern tSettings settings;

uint8_t animationState = 0;

void animate(void)
{
	switch (settings.program) {
		case 0:
			animationState++;
			switch (animationState) {
				case 0:
					SET_COLOR(fadeColor[0], 255, 0, 0);
					SET_COLOR(color[1], 0, 0, 0);
					SET_COLOR(fadeColor[2], 0, 0, 0);
					break;

				case 1:
					SET_COLOR(fadeColor[0], 0, 0, 0);
					SET_COLOR(fadeColor[1], 0, 255, 0);
					SET_COLOR(color[2], 0, 0, 0);
					break;

				case 2:
					SET_COLOR(color[0], 0, 0, 0);
					SET_COLOR(fadeColor[1], 0, 0, 0);
					SET_COLOR(fadeColor[2], 0, 0, 255);
					__attribute__((__fallthrough__));

				default:
					animationState = -1;
			}
			break;

		case 1:
			animationState++;
			switch (animationState) {
				case 0:
					SET_COLOR(fadeColor[0], rand(), rand(), rand());
					SET_COLOR(color[1], 0, 0, 0);
					SET_COLOR(fadeColor[2], 0, 0, 0);
					break;

				case 1:
					SET_COLOR(fadeColor[0], 0, 0, 0);
					SET_COLOR(fadeColor[1], rand(), rand(), rand());
					SET_COLOR(color[2], 0, 0, 0);
					break;

				case 2:
					SET_COLOR(color[0], 0, 0, 0);
					SET_COLOR(fadeColor[1], 0, 0, 0);
					SET_COLOR(fadeColor[2], rand(), rand(), rand());
					__attribute__((__fallthrough__));

				default:
					animationState = -1;
			}
			break;

		case 2:
			animationState++;
			switch (animationState) {
				case 0:
					SET_COLOR(fadeColor[0], rand(), rand(), rand());
					break;

				case 1:
					SET_COLOR(fadeColor[1], rand(), rand(), rand());
					break;

				case 2:
					SET_COLOR(fadeColor[2], rand(), rand(), rand());
					break;

				case 3:
					SET_COLOR(fadeColor[0], rand(), rand(), rand());
					SET_COLOR(fadeColor[1], rand(), rand(), rand());
					SET_COLOR(fadeColor[2], rand(), rand(), rand());
					__attribute__((__fallthrough__));

				default:
					animationState = -1;
			}
			break;

		case 3:
			animationState++;
			switch (animationState) {
				case 0:
					SET_COLOR(fadeColor[0], rand(), rand(), rand());
					SET_COLOR(fadeColor[2], 0, 0, 0);
					break;

				case 1:
					fadeColor[1].data = fadeColor[0].data;
					SET_COLOR(fadeColor[0], 0, 0, 0);
					break;

				case 2:
					fadeColor[2].data = fadeColor[1].data;
					SET_COLOR(fadeColor[1], 0, 0, 0);
					__attribute__((__fallthrough__));

				default:
					animationState = -1;
			}
			break;

		case 4:
			SET_COLOR(fadeColor[0], rand(), rand(), rand());
			SET_COLOR(fadeColor[1], rand(), rand(), rand());
			SET_COLOR(fadeColor[2], rand(), rand(), rand());
			break;
	}
}