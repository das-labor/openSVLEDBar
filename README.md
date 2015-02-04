OpenSVLEDBar
============

This repository was set up for the development of a fully open source firmware for LED Bars sold by a popular PA-/music reseller in Germany.

Supported LED Bars
------------------

For now, only one LED Bar is supported:
* 252 LEDs
* DMX
* Sound to Light
* 1.1 metres wide
* Three segments of equal width
* Available for ~80 Euros
Each of its three individually controllable segments contain 84 LEDs (28 of red, green and blue LEDs) ordered up diagonally on the bar.
This gives a fairly good color result.

There is another version, which is only third wide as the version stated above, which may has also the same hardware specifications (beside the LED distribution and complete width).

Hardware specifications
-----------------------
* LED voltage 24 VDC
* Microcontroller: ATmega8515L @ 24 MHz (!)
* LCD Display on the back (HD44780, 2x8 Characters)
* DMX (SN75176 differential transceiver) connected to USART
* Microphone In (LM358 VCO) connected to INT1
* Three Buttons (Enter, Up, Down) low active
* 9 LED-Segments connected to Inverters / Drivers

Pinout of the ISP connector on the main board
---------------------------------------------
(TODO)