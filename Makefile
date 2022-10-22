CFLAGS ?= -Os -DF_CPU=16000000UL -mmcu=atmega328p
LDFLAGS ?= -mmcu=atmega328p
ARDUINO_USB ?= /dev/ttyACM0

TARGET=pw.hex
TARGET_BIN=$(patsubst %.hex,%.bin,$(TARGET))

BUILD_DIR=build
SRC_DIR=src

all: $(BUILD_DIR)/$(TARGET)

SRC=$(shell find -L $(SRC_DIR) -name '*.c')
OBJS=$(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(SRC)))


$(BUILD_DIR)/$(TARGET): $(BUILD_DIR)/$(TARGET_BIN)
	avr-objcopy -O ihex -R .eeprom $< $@

$(BUILD_DIR)/$(TARGET_BIN): $(OBJS)
	avr-gcc $(LDFLAGS) -o $@ $(OBJS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(addprefix $(BUILD_DIR)/, $(dir $<))
	avr-gcc $(CFLAGS) -c -o $@ $<

prog: $(BUILD_DIR)/$(TARGET)
	avrdude -F -V -c arduino -p ATMEGA328p -P ${ARDUINO_USB} -b 115200 -U flash:w:$(BUILD_DIR)/$(TARGET)

clean: 
	rm -rf build

