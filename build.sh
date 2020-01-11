#!/bin/sh

# Select Your Target MCU
TARGET_SPEED="8MHz"
# TARGET_SPEED="16MHZ"

TARGET_MCU="atmega16"
# TARGET_MCU="atmega168"
# TARGET_MCU="atmega328p"  #TESTED
# TARGET_MCU="atmega1284p"
TARGET=$TARGET_MCU"_"$TARGET_SPEED

make clean
make $TARGET