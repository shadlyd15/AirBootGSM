#ifndef GSM_OTA_H
#define GSM_OTA_H

typedef enum otaStatus{
	OTA_FAILED = 0,
	OTA_SUCCESS = 1,
    FIRMWARE_OK = 2
} otaStatus_t;

class GSM_OTA
{
public:
	GSM_OTA();
	~GSM_OTA();
	void setGsmEnablePin(uint8_t * port, uint8_t pin);
	void setOtaServer(uint8_t * ip_addr, uint16_t port);
	int getOtaStatus();	
};

#endif // GSM_OTA_H