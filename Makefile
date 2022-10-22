CFLAGS ?= -Os -DF_APU=16000000UL -mmcu=atmega328p
LDFLAGS ?= -mmcu=atmega328p
# Path where your Arduino Uno is connected to
ARDUINO_USB ?= /dev/ttyACM0

all: build/led.hex

build/led.hex: build/led.bin
	avr-objcopy -O ihex -R .eeprom $< $@

build/led.bin: build/led.o
	avr-gcc $(LDFLAGS) -o $@ $<

build/led.o: src/main.c
	mkdir -p build
	avr-gcc $(CFLAGS) -c -o $@ $<

deploy: build/led.hex
	avrdude -F -V -c arduino -p ATMEGA328p -P ${ARDUINO_USB} -b 115200 -U flash:w:build/led.hex  

clean: 
	rm -rf build

