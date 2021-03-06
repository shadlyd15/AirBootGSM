#include <avr/eeprom.h>
#include <avr/wdt.h>
#include "AirBootGSMLib.h"
#include "eeprom_addr.h"

void GSM_OTA::setGsmEnablePin(uint8_t * port, uint8_t pin){
	this->isGsmEnablePinSet = 1;
	if(eeprom_read_byte(GSM_REG_PORT_ADDR) != port){
		eeprom_write_byte(GSM_REG_PORT_ADDR, port);	
	}

	if(eeprom_read_byte(GSM_REG_PIN_ADDR) != pin){
		eeprom_write_byte(GSM_REG_PIN_ADDR, pin);
	}
}

void GSM_OTA::setOtaServer(uint8_t * ip_addr, uint16_t port){
	this->isOtaServerSet = 1;
	if(eeprom_read_byte(OTA_SERVER_IP_0) != ip_addr[0])
		eeprom_write_byte(OTA_SERVER_IP_0, ip_addr[0]);

	if(eeprom_read_byte(OTA_SERVER_IP_1) != ip_addr[1])
		eeprom_write_byte(OTA_SERVER_IP_1, ip_addr[1]);
	
	if(eeprom_read_byte(OTA_SERVER_IP_2) != ip_addr[2])
		eeprom_write_byte(OTA_SERVER_IP_2, ip_addr[2]);
	
	if(eeprom_read_byte(OTA_SERVER_IP_3) != ip_addr[3])
		eeprom_write_byte(OTA_SERVER_IP_3, ip_addr[3]);

	if(eeprom_read_byte(OTA_SERVER_PORT_H) != (port & 0xFF))
		eeprom_write_byte(OTA_SERVER_PORT_H, port & 0xFF);

	if(eeprom_read_byte(OTA_SERVER_PORT_L) != (port >> 8))
		eeprom_write_byte(OTA_SERVER_PORT_L, port >> 8);
}

int GSM_OTA::startOta(){
	if(isOtaServerSet && isGsmEnablePinSet){
		if(eeprom_read_byte(OTA_INIT_SIG_ADDR) != OTA_START_SIG){
	  		eeprom_write_byte(OTA_INIT_SIG_ADDR, OTA_START_SIG);
	  	}
		wdt_enable(WDTO_15MS);
		delay(1000);	
	}
	return -1;
}

int GSM_OTA::getOtaStatus(){
	if(eeprom_read_byte(OTA_INIT_SIG_ADDR) == OTA_START_SIG){
		eeprom_write_byte(OTA_INIT_SIG_ADDR, 0xFF);
		return OTA_SUCCESS;
	}
	return FIRMWARE_OK;
}