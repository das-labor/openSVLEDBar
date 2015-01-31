/* Menu 

(2x8 Display)

- Mode
  - Fixed
  	- Color 1 / all
      - Red-Blue 0-255
  	- Color 2
      - Red-Blue 0-255
  	- Color 3
      - Red-Blue 0-255
  - DMX
  - Auto
	- Program 1-5
  - Slave Mode
- Division
  - 3 parts
  - 1 part
- Channel (only Mode=DMX)
  - DMX 1-512
- Fade
  - Off-0.1-25.5 sec
- Toggle
  - BPM
  	- 1-256
  - Sound
*/

#include <avr/pgmspace.h>
#include "lcd.h"
#include "settings.h"

const char* const menuStrings[] PROGMEM = {
	"OpenSWLB",
	"by e7p",
	"Division",
	"1 part",
	"3 parts",
	"Mode",
	"Fixed",
	"DMX",
	"Auto",
	"Color",
	"Red",
	"Green",
	"Blue",
	"Channel",
	"Program",
	"Fade",
	"Off",
	"sec",
	"Toggle",
	"BPM",
	"Sound"
};

void menuSleep() {
	lcdClear();
	lcdPuts_p(0, 0, menuStrings[0]);
	lcdPuts_p(1, 1, menuStrings[0]);
}

void menuDraw() {
	//if(settingMode);
}