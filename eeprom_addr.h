#ifndef EEPROM_ADDR_H
#define EEPROM_ADDR_H

/*-----------------------------------------------------*/

#define EEPROM_MAJVER   			((uint8_t*)0)
#define EEPROM_MINVER   			((uint8_t*)1)

#define OTA_INIT_SIG_ADDR   		((uint8_t*)2)
#define OTA_ATTEMPTED_ADDR   		((uint8_t*)3)

#define OTA_STAUS_ADDR		   		((uint8_t*)4)

#define GSM_REG_PORT_ADDR		   	((uint8_t*)5)
#define GSM_REG_PIN_ADDR		   	((uint8_t*)6)
#define GSM_REG_PIN_ON_LOGIC_ADDR  	((uint8_t*)7)

#define OTA_SERVER_IP_1  			((uint8_t*)8)
#define OTA_SERVER_IP_2  			((uint8_t*)9)
#define OTA_SERVER_IP_3  			((uint8_t*)10)
#define OTA_SERVER_IP_4  			((uint8_t*)11)

#define OTA_SERVER_PORT_H  			((uint8_t*)12)
#define OTA_SERVER_PORT_L  			((uint8_t*)13)

/*-----------------------------------------------------*/

#define OTA_START_SIG				(uint8_t)(0xBE)
#define OTA_ATTEMPTED_SIG			(uint8_t)(0xBE)
#define OTA_COMPLETED				(uint8_t)(0xBE)

#define GSM_BOOTLOADER_MAJVER		((uint8_t*)0x00)
#define GSM_BOOTLOADER_MINVER		((uint8_t*)0x01)

/*-----------------------------------------------------*/

#endif