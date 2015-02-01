/* Menu 

(2x8 Display)

- Mode
  - DMX 3 Channel
    - Address 0-255
  - DMX 9 Channel
    - Address 0-255
  - Fixed Color Mode
  	- Color Segment 1 Red 0-255
	- Color Segment 1 Green 0-255
	- Color Segment 1 Blue 0-255
    - Color Segment 2 Red 0-255
    - Color Segment 2 Green 0-255
    - Color Segment 2 Blue 0-255
   	- Color Segment 3 Red 0-255
   	- Color Segment 3 Green 0-255
   	- Color Segment 3 Blue 0-255
  - Auto Mode
	- Program 1-5
	- Speed
	- Fade Time
  - Slave Mode
  - Sound to Light
    - Program 1-5
	- Speed
	- Fade Time
*/

#include <avr/pgmspace.h>
#include "lcd.h"
#include "settings.h"

const char* const menuStrings[] PROGMEM = {
	"OpenSVLB",
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