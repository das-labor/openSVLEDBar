/* Menu 

(2x8 Display)

- Mode
  - DMX 3 Channel
    - Address 0-255
  - DMX 9 Channel
    - Address 0-255
  - Fixed Color Mode
  	- Segment 1 Red 0-255
	- Segment 1 Green 0-255
	- Segment 1 Blue 0-255
    - Segment 2 Red 0-255
    - Segment 2 Green 0-255
    - Segment 2 Blue 0-255
   	- Segment 3 Red 0-255
   	- Segment 3 Green 0-255
   	- Segment 3 Blue 0-255
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



const char menuStrings[][9] PROGMEM = {
	"OpenSVLB",
	"by labor",
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
	lcdPuts_p(0, 0, &menuStrings[0]);
	lcdPuts_p(0, 1, &menuStrings[1]);
}

void menuDraw() {
	if (menuMode==MODE_AUTO)
	{
		//Automodus
	}
	else if(menuMode==MODE_FIXED)
	{
		lcdPuts(0, 0, "fixed co");
	}
	else if(menuMode==MODE_DMX_3)
	{
		lcdPuts(0, 0, "DMX 3ch");
	}
	else if(menuMode==MODE_DMX_9)
	{
		lcdPuts(0, 0, "DMX 9ch");
	}
	else if(menuMode==MODE_SLAVE)
	{
		//same as DMX Mode but listening at addr. 54
	}
	else if(menuMode==MODE_SOUND)
	{
		
	}
}