/* Menu 

(2x8 Display)

- Mode
  - Auto
  - Fixed
  - DMX 3ch
  - DMX 9ch
  - Slave
  - Sound
- Auto Mode
  - Program 1-5
  - Speed
  - Fade Time
  - back
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
  - back
- DMX 3 Channel
  - Address 0-512 (?)
  - back
- DMX 9 Channel
  - Address 0-512 (?)
  - back
- Slave Mode
	- back
- Sound to Light
  - Program 1-5
  - Fade Time
  - back
*/

#include <avr/pgmspace.h>
#include "lcd.h"
#include "settings.h"

const char menuStrings[][9] PROGMEM = {
	"OpenSVLB",
	"by labor",
	"Mode",
	"Auto",
	"Fixed",
	"DMX 3ch",
	"DMX 9ch",
	"Slave",
	"Sound",
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

uint8_t sleepMode = 0;

void menuSleep() {
	if(sleepMode) return;
	sleepMode = 1;
	lcdClear();
	lcdPuts_p(0, 0, &menuStrings[0]);
	lcdPuts_p(0, 1, &menuStrings[1]);
}

void nextMode(){
	if(menuMode <= 6) {
		menuMode++; // go to next Mode
		} else {
		menuMode = 0;
	}
}

void prevMode(){
	if(menuMode > 0) {
		menuMode--; // go to previous Mode
		} else {
		menuMode = 6;
	}
}

void nextSetting(){
	if (menuMode==MODE_SLAVE)
	{
		mode=0;//im slave mode sind keine weiteren einstellungen möglich
	}
	else if((menuMode==MODE_DMX_3 |= menuMode==MODE_DMX_9) && mode<1)
	{
		mode++;
	}
	else if(menuMode==MODE_SOUND && mode<3)
	{
		mode++;
	}
	else if(menuMode==MODE_AUTO && mode<4){
		mode++;
	}
	else{
		mode--;
	}
}

void prevSetting(){
	
}



void menuDraw() {
	sleepMode = 0;
	lcdClear();
	/* TODO: Implement other approach: mode = 0: Mode. mode = 1: Program. mode = 2: ...
	 *		 In this case, the menu structure line 1: title, line 2: value can be achieved.
	 *		 When in menu, first blinks title line, then press on mode lets the second line blink
	 *		 and up down changes the current blinking line.
	 *		 Otherwise I have simply NO idea how to get this better working on 2x8 Characters!!!
	 *		 (Please think about it, sorry if I disturbed your work.)
	 */
	if (menuMode == 0) {
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
		lcdPuts(0, 0, "slave");
		//same as DMX Mode but listening at addr. 54
	}
	else if(menuMode==MODE_SOUND)
	{
		lcdPuts(0, 0, "sound");
	}
}