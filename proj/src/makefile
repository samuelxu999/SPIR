DIR_INC = ./include
DIR_DRIVER = ./driver
DIR_CORE = ./core
DIR_LIB = ./lib
DIR_BIN = ./bin

INC = $(wildcard ${DIR_INC}/*.h)
SRC_DRIVER = $(wildcard ${DIR_DRIVER}/*.c)
LIB_DRIVER = $(patsubst %.c,${DIR_LIB}/%.o,$(notdir ${SRC_DRIVER}))


LIB_LINK = ${DIR_LIB}/pushbutton_driver.o \
	${DIR_LIB}/motor_driver.o \
	${DIR_LIB}/lcd_driver.o \
	${DIR_LIB}/ADC_driver.o \
	${DIR_LIB}/menu_driver.o
	
#LIB_OBJ=$(patsubst %.c,${DIR_LIB}/%.o,$(notdir ${LIB_SRC}))

#OBJ = $(SRC_DRIVER:.c=.o)

TARGET = main

BIN_TARGET = ${DIR_BIN}/${TARGET}

CC=avr-gcc
F_CPU = 20000000
MCU = atmega328p
CCOPTS= -g -Wall -Os -std=c99
CCOPTS += $(addprefix -I,$(DIR_INC))

AVR_STRIP=avr-strip

AVR_OBJCOPY=avr-objcopy
OBJCOPY_OPT=-R .eeprom -O ihex

AVR_DUDE=avrdude
DUDE_OPT=-p m328p -c avrisp2 -P
DIDE_DEV=/dev/ttyACM0
OPERATE_CONF=-U flash:w:

#compile core
${BIN_TARGET}:${LIB_DRIVER} lcd_driver.o
	$(CC) $(CCOPTS) -mmcu=$(MCU) -DF_CPU=$(F_CPU) -o $@ -g ${DIR_CORE}/main.c ${LIB_LINK}

#compile all the driver	
${DIR_LIB}/%.o:${DIR_DRIVER}/%.c
	$(CC) $(CCOPTS) -mmcu=$(MCU) -DF_CPU=$(F_CPU) -c  $< -o $@

#import external lib
lcd_driver.o:
	cp ${DIR_DRIVER}/lcd_driver.o ${DIR_LIB}/lcd_driver.o
	


default: compile

All:program
#	@echo $(LIB_SRC)
#	@echo $(LIB_OBJ)
#	@echo $(BIN_TARGET)
#	@echo ${OBJ}

compile:${BIN_TARGET}

hex: compile
	#avr-strip pro_final
	${AVR_STRIP} ${BIN_TARGET}
	#avr-objcopy -R .eeprom -O ihex pro_final pro_final.hex
	${AVR_OBJCOPY} ${OBJCOPY_OPT} ${BIN_TARGET} ${BIN_TARGET}.hex
	
program: hex
	#avrdude -p m328p -c avrisp2 -P /dev/ttyACM0 -U flash:w:pro_final.hex
	${AVR_DUDE} ${DUDE_OPT} ${DIDE_DEV} ${OPERATE_CONF}${BIN_TARGET}.hex

clean:
	rm ${DIR_LIB}/*.o ${DIR_BIN}/*

#compile: main.c pushbutton_driver.o motor_driver.o ADC_driver.o menu_driver.o
#	avr-gcc  $(CCOPTS) -mmcu=$(MCU) -DF_CPU=$(F_CPU) -o pro_final -g main.c pushbutton_driver.o motor_driver.o lcd_driver.o ADC_driver.o menu_driver.o
#	avr-gcc -c $(CCOPTS) -mmcu=$(MCU) -DF_CPU=$(F_CPU) pushbutton_driver.c -o pushbutton_driver.o
