#ifndef GSM_AT_CMDS_H
#define GSM_AT_CMDS_H

#define NEOWAY
#if defined(NEOWAY)
	const char AT_CHECK_PIN[] 				= "AT+CPIN?\r\n";
	const char AT_ECHO_OFF[] 				= "ATE0\r\n";
	const char AT_CHECK_SIGNAL[] 			= "AT+CSQ\r\n";
	const char AT_CHECK_GSM[] 				= "AT+CREG?\r\n";
	const char AT_CHECK_GPRS[] 				= "AT+CGATT?\r\n";
	const char AT_SET_PPP_LINK[]			= "AT+XIIC=1\r\n";
	const char AT_CHECK_PPP_LINK[]			= "AT+XIIC?\r\n";
	const char AT_TCP_TRANS[] 				= "AT+TCPTRANS=";
	const char TCP_CLOSE[]              	= "AT+TCPCLOSE=0\r\n" ;
#endif	// defined(NEOWAY)
#endif	// GSM_AT_CMDS_H