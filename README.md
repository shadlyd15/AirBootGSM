## AirBootGSM : An AVR & Arduino GSM OTA Bootloader

This is proof of concept AVR & Arduino bootloader for upgrading firmware over the air using GSM modem. It is kind of fail-safe. If anything wrong happens and the chip does not boot after the firmware upgrade, the bootloader automatically retries to download and reinstall new firmware. AirBootGSM is based on highly optimized [Optiboot Bootloader.](https://github.com/Optiboot/optiboot)

## Setup Toolchain to Compile :
```bash
sudo apt-get update
sudo apt-get install binutils gcc-avr avr-libc uisp avrdude flex byacc bison
```

## Compile :
Run build.sh
```bash
./build.sh
```
Or select your traget mcu and run the following commands.
```bash
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
```

## To Pragram :
Run build.sh
```bash
./build.sh
```
Or select your programmer and run the following commands.
```bash
PROGRAMMER="usbasp"
# PROGRAMMER="arduino"
# PROGRAMMER="avrisp"
# PROGRAMMER="avrispmkII"
avrdude -c $PROGRAMMER -p $TARGET_MCU -u -U flash:w:"AirBootGSM"_$TARGET.hex
```
## How To Use 
To use the OTA firmware upgrade capabilities from application layer, you can use the provided Arduino library. The library contains a simple example to demonstrate OTA firmware upgrade from application layer. It triggers OTA firmware upgrade if it gets input string "OTA" the from Serial.

```cpp
#include "AirBootGSMLib.h"

void start_ota(){
  eeprom_write_byte(OTA_INIT_SIG_ADDR, 15);
  wdt_enable(WDTO_15MS);
  delay(1000);
}

void stop_ota(){
  if(eeprom_read_byte(OTA_INIT_SIG_ADDR) == 15){
    if(eeprom_read_byte(OTA_STAUS_ADDR) == OTA_COMPLETED){
      Serial.println("Firmware Upgraded");
    }
    else{
      Serial.println("Firmware Upgrade Failed");
    }
    eeprom_write_byte(OTA_INIT_SIG_ADDR, 0xFF);
  }
}

int x = 0;
char buff[128];

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
  Serial.begin(115200);
  delay(100);
  Serial.println("\r\n-- Application Starts --");
  stop_ota();
  memset(buff, 0x00, 128);
}

// the loop function runs over and over again forever
void loop() {
 Serial.println("Loop Starts");
 digitalWrite(LED_BUILTIN, HIGH);
 delay(1000);                       // wait for a second
 digitalWrite(LED_BUILTIN, LOW);
 delay(1000);                       // wait for a second
 digitalWrite(LED_BUILTIN, HIGH);
 delay(1000);                       // wait for a second
 digitalWrite(LED_BUILTIN, LOW);
 delay(1000);                       // wait for a second

  if(Serial.available()){
      char c = (char)Serial.read();
      buff[x] = c;
      x = ((x+1) % 128);
  }

  if(strstr(buff, "OTA")){
    Serial.println("Starting OTA .....\r\n");
    start_ota();
  }
}
```

## Limitations : 
Currently it only supports Neoway GSM modems that support Transparet TCP mode. To keep compiled binary as small as possible to fit with a few KBs, readability of the code is greatly hampared. Still any other modems can easily be adopted.