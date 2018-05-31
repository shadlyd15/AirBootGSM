
#include <avr/io.h>
#include "pin_defs.h"
#include "watchdog.h"

 /* Disable the watchdog timer to prevent
  * eternal reset loop of doom and despair */
void watchdogDisable(void){
	watchdogConfig(WATCHDOG_OFF);
}

// Watchdog functions. These are only safe with interrupts turned off.
void watchdogReset() {
  __asm__ __volatile__ (
    "wdr\n"
  );
}

void watchdogConfig(uint8_t x){
	WDTCSR = _BV(WDCE) | _BV(WDE);
	WDTCSR = x;
}
