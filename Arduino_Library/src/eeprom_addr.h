#ifndef EEPROM_ADDR_H
#define EEPROM_ADDR_H

/*-----------------------------------------------------*/

#define GSM_BOOTLOADER_MAJVER		(E2END - 0)
#define GSM_BOOTLOADER_MINVER		(E2END - 1)

#define OTA_INIT_SIG_ADDR   		(E2END - 2)
#define OTA_ATTEMPTED_ADDR   		(E2END - 3)

#define OTA_STAUS_ADDR		   		(E2END - 4)

#define GSM_REG_PORT_ADDR		   	(E2END - 5)
#define GSM_REG_PIN_ADDR		   	(E2END - 6)
#define GSM_REG_PIN_ON_LOGIC_ADDR  	(E2END - 7)

#define OTA_SERVER_IP_0  			(E2END - 8)
#define OTA_SERVER_IP_1  			(E2END - 9)
#define OTA_SERVER_IP_2  			(E2END - 10)
#define OTA_SERVER_IP_3  			(E2END - 11)

#define OTA_SERVER_PORT_H  			(E2END - 12)
#define OTA_SERVER_PORT_L  			(E2END - 13)

/*-----------------------------------------------------*/

#define OTA_START_SIG				(uint8_t)(0xBE)
#define OTA_ATTEMPTED_SIG			(uint8_t)(0xBE)
#define OTA_COMPLETED				(uint8_t)(0xBE)

/*-----------------------------------------------------*/

#endif