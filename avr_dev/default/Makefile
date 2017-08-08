###############################################################################
# Makefile for the project avr_dev
###############################################################################

## General Flags
PROJECT = avr_dev
MCU = atmega128
TARGET = avr_dev.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                                                                                                                                       -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=avr_dev.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\Github\SPIR\avr_dev\src\include" 

## Objects that must be built in order to link
OBJECTS = main.o ADC_driver.o USART_driver.o SPI_driver.o Sensor.o Queues.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) avr_dev.hex avr_dev.eep avr_dev.lss## Compile
main.o: ../src/core/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ADC_driver.o: ../src/driver/ADC_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

USART_driver.o: ../src/driver/USART_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

SPI_driver.o: ../src/driver/SPI_driver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Sensor.o: ../src/core/Sensor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Queues.o: ../src/core/Queues.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) avr_dev.elf dep/* avr_dev.hex avr_dev.eep avr_dev.lss avr_dev.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

