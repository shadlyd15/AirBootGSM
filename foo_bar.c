
void bar(uint8_t *port, uint8_t pin, bool value){
    unsigned char * ddr = port - 1;
    *ddr |= (1 << pin);
    if(value){
      *port |= (1 << pin);//make pin 13 high and power on the led
    }
    else{
      *port &= ~(1 <<pin);//make pin 13 low and power off the led
    }
}

// (*(&x - 1)) 
// the setup function runs once when you press reset or power the board
void setup() {
}

// the loop function runs over and over again forever
void loop() {
  bar(&PORTB, 5, true);
  delay(1000);
  bar(&PORTB, 5, false);
  delay(1000);
}