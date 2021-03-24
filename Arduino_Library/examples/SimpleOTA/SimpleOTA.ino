#include "AirBootGSMLib.h"

AirBoot airboot;

int x = 0;
char buff[128];
uint16_t ota_port = 8888;
uint8_t ota_server[4] = { 192, 56, 12, 5 };

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(100);
  Serial.begin(115200);
  delay(100);
  Serial.println("\r\n-- Application Starts --");
  airboot.stop_ota();
  airboot.setGsmEnablePin(PORTB, PB3);
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
    airboot.startOta(ota_server, ota_port);
  }
}
