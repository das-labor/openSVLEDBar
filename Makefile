MMCU=atmega8515
F_CPU=24000000
PROG=usbasp
PROG_PORT=usb

TARGET=main

OBJ=menu.o lcd.o settings.o animations.o

FORMAT=ihex
OBJCOPY=avr-objcopy
OBJSIZE=avr-size
CC=avr-gcc
CFLAGS=-mmcu=$(MMCU) -I. -gdwarf-2 -DF_CPU=$(F_CPU)UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wextra -pedantic -Wundef -std=gnu11 -Wundef -lm
CFLAGS+=-ffunction-sections -fdata-sections -mstrict-X -maccumulate-args -Wl,--gc-sections

all: $(TARGET).hex $(TARGET).eep

clean:
	rm -f $(TARGET).hex $(TARGET).eep $(TARGET).elf $(TARGET).o $(OBJ)

flash: $(TARGET).hex $(TARGET).eep
	sudo avrdude -U flash:w:$(TARGET).hex:i -U eeprom:w:$(TARGET).eep:i -C /etc/avrdude.conf -v -p $(MMCU) -b 57600 -c $(PROG) -P $(PROG_PORT)

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

%.elf: %.o $(OBJ)
	$(CC) $(CFLAGS) $^ --output $@
	$(OBJSIZE) --format=avr --mcu=$(MMCU) $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@