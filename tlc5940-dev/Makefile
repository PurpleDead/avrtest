
all : test.hex burn

PART=atmega328p
PROGPART=m328p

CFLAGS=-g -Os -mmcu=$(PART) -DF_CPU=8000000UL -flto -fwhole-program -DMUTE_PRINTF -std=c99
CC=avr-gcc
AS=avr-gcc

#The avr-objcopy utility is what translates a binary GNU executable file into a .hex file.
test.elf : test.c
	avr-gcc -I. $(CFLAGS) -Wl,--relax -mcall-prologues -Wl,-Map,test.map -o $@ $^ -L/usr/lib/avr/include/avr

test.hex : test.elf
	avr-objcopy -j .text -j .data -O ihex test.elf test.hex

burn : test.hex
	sudo avrdude -c usbasp -p $(PROGPART) -U flash:w:test.hex -v

#This sets the part to its fabric state
deffuses:
	sudo avrdude -c usbasp -p $(PROGPART) -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

fastclk:
	sudo avrdude -c usbasp -p $(PROGPART) -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m

readfuses:
	sudo avrdude -c usbasp -p $(PROGPART) -U lfuse:r:highfuse.txt:b -U hfuse:r:lowfuse.txt:b

clean:
	rm -f *~ highfuse.txt lowfuse.txt test.hex test.map test.elf *.o
