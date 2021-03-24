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
  setOtaServer("192.12.23.44", 8888);
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
