#ifndef GSM_SERIAL_H
#define GSM_SERIAL_H

#include <inttypes.h>

#define SIZE_RX_BUFFER 		32
#define SIZE_RESPONSE		SIZE_RX_BUFFER

#define OK 1
#define NOT_OK 0
#define UNKNOWN 255

#define UART_BUFFER_EMPTY 0
#define UART_NO_CHARACTER -1

#define INITIAL						0
#define CHECK_PIN					1
#define TURN_OFF_ECHO				2
#define	CHECK_SIGNAL				3
#define	CHECK_GPRS					4
#define	CHECK_GSM					5
#define SET_PPP_LINK				6
#define CHECK_PPP_LINK				7
#define CONNECT_TCP					8
#define IDLE						9

uint8_t gsm_init(void);

#endif	// GSM_SERIAL_H