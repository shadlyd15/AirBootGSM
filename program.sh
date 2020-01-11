#!/bin/sh

# Select Your Programmer
PROGRAMMER="usbasp"
avrdude -c $PROGRAMMER -p $TARGET_MCU -u -U flash:w:"AirBootGSM"_$TARGET.hex