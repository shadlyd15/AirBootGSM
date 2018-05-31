# program name should not be changed ...
PROGRAM    = gsm_bootloader
DEBUG 	   = enable
# DEBUG 	   = disable

# The default behavior is to build using tools that are in the users
# current path variables, but we can also build using an installed
# Arduino user IDE setup, or the Arduino source tree.
# Uncomment this next lines to build within the arduino environment,
# using the arduino-included avrgcc toolset (mac and pc)
# ENV ?= arduino
# ENV ?= arduinodev
# OS ?= macosx
# OS ?= windows

MCU_TARGET = atmega328p
LDSECTIONS = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe

# Build environments
# Start of some ugly makefile-isms to allow optiboot to be built
# in several different environments.  See the README.TXT file for
# details.

# default
fixpath = $(1)

ifeq ($(ENV), arduino)
# For Arduino, we assume that we're connected to the optiboot directory
# included with the arduino distribution, which means that the full set
# of avr-tools are "right up there" in standard places.
TOOLROOT = ../../../tools
GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

ifeq ($(OS), windows)
# On windows, SOME of the tool paths will need to have backslashes instead
# of forward slashes (because they use windows cmd.exe for execution instead
# of a unix/mingw shell?)  We also have to ensure that a consistent shell
# is used even if a unix shell is installed (ie as part of WINAVR)
fixpath = $(subst /,\,$1)
SHELL = cmd.exe
endif

else ifeq ($(ENV), arduinodev)
# Arduino IDE source code environment.  Use the unpacked compilers created
# by the build (you'll need to do "ant build" first.)
ifeq ($(OS), macosx)
TOOLROOT = ../../../../build/macosx/work/Arduino.app/Contents/Resources/Java/hardware/tools
endif
ifeq ($(OS), windows)
TOOLROOT = ../../../../build/windows/work/hardware/tools
endif

GCCROOT = $(TOOLROOT)/avr/bin/
AVRDUDE_CONF = -C$(TOOLROOT)/avr/etc/avrdude.conf

else
GCCROOT =
AVRDUDE_CONF =
endif

# End of build environment code.

OBJ      = main.o watchdog.o gsm.o optiboot.o uart.o
OPTIMIZE = -Os -flto -fuse-linker-plugin -fno-inline-small-functions -fno-split-wide-types -mno-interrupts -mrelax
OPTIMIZE += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -fno-jump-tables -std=gnu99 #-save-temps

DEFS     =
LIBS     =

CC       = $(GCCROOT)avr-gcc

# Override is only needed by avr-lib build system.

override CFLAGS  = -g -Wall -w -Wextra -Wstrict-prototypes $(OPTIMIZE) -mmcu=$(MCU_TARGET) -DF_CPU=$(AVR_FREQ) $(DEFS)
override LDFLAGS = $(LDSECTIONS) -Wl,--relax -Wl,--gc-sections -flto -fuse-linker-plugin

OBJCOPY = $(GCCROOT)avr-objcopy
OBJDUMP = $(call fixpath,$(GCCROOT)avr-objdump)

SIZE    = $(GCCROOT)avr-size

# Build with the debug messages enabled
# Use it only if you have already seen the source code

ifeq ($(DEBUG), enable)
override CFLAGS += -DENABLE_DEBUG
endif

## ATmega8
atmega8_8MHz: TARGET = atmega8_8MHz
atmega8_8MHz: MCU_TARGET = atmega8
atmega8_8MHz: CFLAGS += 
atmega8_8MHz: AVR_FREQ = 8000000L
atmega8_8MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega8_8MHz: $(PROGRAM)_atmega8_8MHz.hex
atmega8_8MHz: $(PROGRAM)_atmega8_8MHz.lst

atmega8_16MHz: TARGET = atmega8_16MHz
atmega8_16MHz: MCU_TARGET = atmega8
atmega8_16MHz: CFLAGS += 
atmega8_16MHz: AVR_FREQ = 16000000L
atmega8_16MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega8_16MHz: $(PROGRAM)_atmega8_16MHz.hex
atmega8_16MHz: $(PROGRAM)_atmega8_16MHz.lst

## ATmega168
atmega16_8MHz: TARGET = atmega16_8MHz
atmega16_8MHz: MCU_TARGET = atmega16
atmega16_8MHz: CFLAGS += 
atmega16_8MHz: AVR_FREQ = 8000000L
atmega16_8MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega16_8MHz: $(PROGRAM)_atmega8_8MHz.hex
atmega16_8MHz: $(PROGRAM)_atmega8_8MHz.lst

atmega16_16MHz: TARGET = atmega16_16MHz
atmega16_16MHz: MCU_TARGET = atmega16
atmega16_16MHz: CFLAGS += 
atmega16_16MHz: AVR_FREQ = 16000000L
atmega16_16MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega16_16MHz: $(PROGRAM)_atmega8_16MHz.hex
atmega16_16MHz: $(PROGRAM)_atmega8_16MHz.lst

## ATmega168
atmega168_8MHz: TARGET = atmega168_8MHz
atmega168_8MHz: MCU_TARGET = atmega168
atmega168_8MHz: CFLAGS += 
atmega168_8MHz: AVR_FREQ = 8000000L
atmega168_8MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega168_8MHz: $(PROGRAM)_atmega8_8MHz.hex
atmega168_8MHz: $(PROGRAM)_atmega8_8MHz.lst

atmega168_16MHz: TARGET = atmega168_16MHz
atmega168_16MHz: MCU_TARGET = atmega168
atmega168_16MHz: CFLAGS += 
atmega168_16MHz: AVR_FREQ = 16000000L
atmega168_16MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e00 -Wl,--section-start=.version=0x1ffe
atmega168_16MHz: $(PROGRAM)_atmega8_16MHz.hex
atmega168_16MHz: $(PROGRAM)_atmega8_16MHz.lst

## ATmega328p
atmega328p_8MHz: TARGET = atmega328p_8MHz
atmega328p_8MHz: MCU_TARGET = atmega328p
atmega328p_8MHz: CFLAGS += 
atmega328p_8MHz: AVR_FREQ = 8000000L
atmega328p_8MHz: LDSECTIONS = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega328p_8MHz: $(PROGRAM)_atmega328p_8Mhz.hex
atmega328p_8MHz: $(PROGRAM)_atmega328p_8Mhz.lst

atmega328p_16MHz: TARGET = atmega328p_16MHz
atmega328p_16MHz: MCU_TARGET = atmega328p
atmega328p_16MHz: CFLAGS += 
atmega328p_16MHz: AVR_FREQ = 16000000L
atmega328p_16MHz: LDSECTIONS = -Wl,--section-start=.text=0x7000 #-Wl,--section-start=.version=0x7ffe
atmega328p_16MHz: $(PROGRAM)_atmega328p_8Mhz.hex
atmega328p_16MHz: $(PROGRAM)_atmega328p_8Mhz.lst

## Atmega1284p
atmega1284p_8MHz: TARGET = atmega1284p_8MHz
atmega1284p_8MHz: MCU_TARGET = atmega1284p
atmega1284p_8MHz: CFLAGS += 
atmega1284p_8MHz: AVR_FREQ = 8000000L
atmega1284p_8MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
atmega1284p_8MHz: $(PROGRAM)_atmega1284p_8MHz.hex
atmega1284p_8MHz: $(PROGRAM)_atmega1284p_8MHz.lst

atmega1284p_16MHz: TARGET = atmega1284p_16MHz
atmega1284p_16MHz: MCU_TARGET = atmega1284p
atmega1284p_16MHz: CFLAGS += 
atmega1284p_16MHz: AVR_FREQ = 16000000L
atmega1284p_16MHz: LDSECTIONS  = -Wl,--section-start=.text=0x1e000
atmega1284p_16MHz: $(PROGRAM)_atmega1284p_8MHz.hex
atmega1284p_16MHz: $(PROGRAM)_atmega1284p_8MHz.lst

## Generic build instructions

%.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(SIZE) $@

clean:
	rm -rf *.o *.i *.s *.elf *.lst *.map *.sym *.lss *.eep *.srec *.bin *.hex *~

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -j .version --set-section-flags .version=alloc,load -O binary $< $@
