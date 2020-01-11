#ifndef GSM_OTA_H
#define GSM_OTA_H

#define OTA_COMPLETED       	((uint8_t)(32))

#define OTA_INIT_SIG_ADDR       ((uint8_t*)0)
#define OTA_STAUS_ADDR          ((uint8_t*)2)

typedef enum otaStatus{
	NO_OTA_PARAMETER = -1
	OTA_FAILED = 0,
	OTA_SUCCESS = 1,
    FIRMWARE_OK = 2
} otaStatus_t;

class GSM_OTA{
	public:
		int startOta();
		int getOtaStatus();	
		void setGsmEnablePin(uint8_t * port, uint8_t pin);
		void setOtaServer(uint8_t * ip_addr, uint16_t port);
	private:
		uint8_t isGsmEnablePinSet = 0;
		uint8_t isOtaServerSet = 0;
};

#endif // GSM_OTA_H