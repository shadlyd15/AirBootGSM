
#include <avr/eeprom.h>
#include <util/delay.h>

#include "gsm.h"
#include "eeprom_addr.h"
#include "watchdog.h"
#include "debug.h"

int  main(void) __attribute__ ((section (".init9")));
void appStart(void) __attribute__ ((naked));

int main(void){
    uint8_t ch;

    /* This code makes the following assumptions:
     * No interrupts will execute
     * SP points to RAMEND
     * r1 contains zero
     * If not, uncomment the following instructions. */

    // cli();
    asm volatile("clr __zero_reg__");
	#if defined(__AVR_ATmega8__)
	    SP = RAMEND;  // This is done by hardware reset
	#endif

	#if !defined(__AVR_ATmega16__)
	  ch = MCUSR;
	  MCUSR = 0;
	#else
	  ch = MCUCSR;
	  MCUCSR = 0;
	#endif

	uart_init();
	DEBUG(uart_puts("\r\n-- Bootloader Starts --\r\n"));

	if(ch & _BV(WDRF)){
    	watchdogDisable();
		DEBUG(uart_puts("Watchdog Reset\r\n"));
		if(eeprom_read_byte(OTA_INIT_SIG_ADDR) == OTA_START_SIG){
			DEBUG(uart_puts("OTA Signal Found\r\n"));
			if(eeprom_read_byte(OTA_ATTEMPTED_ADDR) == OTA_ATTEMPTED_SIG){
				DEBUG(uart_puts("Optiboot Attempted Before\r\n"));
				eeprom_write_byte(OTA_ATTEMPTED_ADDR, 0xFF);
				DEBUG(uart_puts("Starting Application\r\n"));
				appStart();
			}
			else{
				eeprom_write_byte(OTA_ATTEMPTED_ADDR, OTA_ATTEMPTED_SIG);
				if(eeprom_read_byte(OTA_STAUS_ADDR) == OTA_COMPLETED){
					eeprom_write_byte(OTA_STAUS_ADDR, 0xFF);
				}
				DEBUG(uart_puts("Attempting Optiboot\r\n"));
				DEBUG(uart_puts("GSM Loop\r\n"));
				gsm_loop();
				watchdogConfig(WATCHDOG_1S);
			}
		}
		else{	// on wdt
			DEBUG(uart_puts("OTA Signal Not Found\r\n"));
			DEBUG(uart_puts("Starting Application\r\n"));
			appStart();	
		}
	}
	else if(ch & _BV(BORF)){
		watchdogConfig(WATCHDOG_1S);
		DEBUG(uart_puts("Brown Out Reset\r\n"));
	}
	else if(ch & _BV(EXTRF)){
		watchdogConfig(WATCHDOG_1S);
		DEBUG(uart_puts("External Reset\r\n"));
		DEBUG(uart_puts("Starting OTA\r\n"));		
		if(eeprom_read_byte(OTA_INIT_SIG_ADDR) == OTA_START_SIG){
			DEBUG(uart_puts("OTA Signal Found\r\n"));
			if(eeprom_read_byte(OTA_ATTEMPTED_ADDR) == OTA_ATTEMPTED_SIG){
				DEBUG(uart_puts("Optiboot Attempted Before\r\n"));
				eeprom_write_byte(OTA_ATTEMPTED_ADDR, 0xFF);
				DEBUG(uart_puts("Starting Application\r\n"));
				appStart();
			}
			else{
				eeprom_write_byte(OTA_ATTEMPTED_ADDR, OTA_ATTEMPTED_SIG);
				if(eeprom_read_byte(OTA_STAUS_ADDR) == OTA_COMPLETED){
					eeprom_write_byte(OTA_STAUS_ADDR, 0xFF);
				}
				DEBUG(uart_puts("Attempting Optiboot\r\n"));
				DEBUG(uart_puts("GSM Loop\r\n"));
				gsm_loop();
				watchdogConfig(WATCHDOG_1S);
			}
		}
		else{	// on wdt
			DEBUG(uart_puts("OTA Signal Not Found\r\n"));
			DEBUG(uart_puts("Starting Application\r\n"));
			appStart();	
		}
	}
	else if(ch & _BV(PORF)){
		watchdogConfig(WATCHDOG_1S);
		DEBUG(uart_puts("Power Reset\r\n"));
	}
	else{
		watchdogConfig(WATCHDOG_1S);
		DEBUG(uart_puts("Invalid Firmware\r\n"));
	}

	// DEBUG(uart_puts("-- Optiboot Starts Now --\r\n"));
	optiboot();

	DEBUG(uart_puts("Bootloader Ends Here. Starting App\r\n"));
	/* Exit to user application */
	appStart();

	DEBUG(uart_puts("Main Exited\r\n"));
	return(0);  //never reached 
}

void appStart(void) {
    watchdogDisable();
    asm volatile(
        "clr    r30     \n\t"
        "clr    r31     \n\t"
        "ijmp   \n\t"
    );
}
