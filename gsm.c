
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "gsm.h"
#include "uart.h"
#include "watchdog.h"
#include "eeprom_addr.h"
#include "gsm_at_cmds.h"

volatile unsigned long g_seconds;
//NOTE: A unsigned long holds values from 0 to 4,294,967,295 (2^32 - 1). It will roll over to 0 after reaching its maximum value.

void set_gsm_enable_config(uint8_t * port, uint8_t pin){
    eeprom_write_byte(GSM_REG_PORT_ADDR, port);
    eeprom_write_byte(GSM_REG_PIN_ADDR, pin);
}

void set_ota_server_config(uint8_t * ip_addr, uint16_t port){
  eeprom_write_byte(OTA_SERVER_IP_1, ip_addr[0]);
  eeprom_write_byte(OTA_SERVER_IP_2, ip_addr[1]);
  eeprom_write_byte(OTA_SERVER_IP_3, ip_addr[2]);
  eeprom_write_byte(OTA_SERVER_IP_4, ip_addr[3]);
  
  eeprom_write_byte(OTA_SERVER_PORT_H, port & 0xFF);
  eeprom_write_byte(OTA_SERVER_PORT_L, port >> 8);
}

void change_pin_state(uint8_t *port, uint8_t pin, uint8_t value){
    unsigned char * ddr = port - 1;
    *ddr |= (1 << pin);
    if(value){
      *port |= (1 << pin);//make pin 13 high and power on the led
    }
    else{
      *port &= ~(1 <<pin);//make pin 13 low and power off the led
    }
}

void init_millis(){
	TCCR1B |= 1<<CS10 | 1<<CS12;
	TCNT1 = 0; //15625
}

unsigned long elapsed_seconds(){
	if(TCNT1 > F_CPU / 1024){
		g_seconds = g_seconds + 1;
		TCNT1 = 0;
	}
	return g_seconds;
}

uint8_t find_sub_string(char* mainStr, uint8_t mainLen, char* subStr, uint8_t subLen){
  uint8_t ans = NOT_OK;
  uint8_t count = 0;
  uint8_t offset = 0;
  uint8_t i = 0;
  
  for(i = 0; i < mainLen; i++){
	  if(mainStr[i] == subStr[count]){
	  	count++;
	  	offset++;
	  }
	  else{
	  	count = 0;
	  }
	    
	  if(count == (subLen-1)){
		  ans = OK;
		  return ans;
	  }
  }
  return ans;
}

uint8_t send_at_command(char * ATcommand, char * expected_resp, unsigned long timeout){
	uint8_t x=0;
	uint8_t answer = NOT_OK;
	char response[SIZE_RESPONSE];
	memset(response,'\0',SIZE_RESPONSE);
	unsigned long start_time = elapsed_seconds();
	 
	uart_puts(ATcommand);    // Send the AT command
	do{                      // this loop waits for the answer until timeouts
		int ch = get_non_blocking_ch();
		if(ch > -1){
			char c = (char)ch;
			response[x] = c;
			x = ((x+1) % SIZE_RESPONSE);
			if(strstr(response, expected_resp)){
				answer = OK;
				return answer;
			}
			else if(strstr(response, "ERROR")){
				answer = NOT_OK;
				return answer;
			}
		}
	}
	while(((unsigned long)(elapsed_seconds() - start_time) < timeout) || answer); 
	return answer;	
}

void turn_modem_on(void){
	uint8_t * port_addr 	= eeprom_read_byte(GSM_REG_PORT_ADDR);
	uint8_t port_pin	 	= eeprom_read_byte(GSM_REG_PIN_ADDR);
	uint8_t high_level	 	= eeprom_read_byte(GSM_REG_PIN_ON_LOGIC_ADDR);
	
	change_pin_state(port_addr, port_pin, high_level);	
}

void turn_modem_off(void){
	uint8_t * port_addr 	= eeprom_read_byte(GSM_REG_PORT_ADDR);
	uint8_t port_pin	 	= eeprom_read_byte(GSM_REG_PIN_ADDR);
	uint8_t high_level	 	= eeprom_read_byte(GSM_REG_PIN_ON_LOGIC_ADDR);

	change_pin_state(port_addr, port_pin, !high_level);		
}

void get_ota_server_ip_str(char * ip_str){
	uint8_t ip_1 = eeprom_read_byte(OTA_SERVER_IP_1);
	uint8_t ip_2 = eeprom_read_byte(OTA_SERVER_IP_2);
	uint8_t ip_3 = eeprom_read_byte(OTA_SERVER_IP_3);
	uint8_t ip_4 = eeprom_read_byte(OTA_SERVER_IP_4);

	sprintf(ip_str, "%d.%d.%d.%d", ip_1, ip_2, ip_3, ip_4);
}

void get_server_port_str(char * port_str){
  uint8_t port_h = eeprom_read_byte(OTA_SERVER_PORT_H);
  uint8_t port_l = eeprom_read_byte(OTA_SERVER_PORT_L);
  uint16_t port = ((uint16_t)port_l << 8) | port_h;

  sprintf(port_str, "%d", port);
}

uint8_t gsm_loop(void){
	init_millis();
	uint8_t ret_val = NOT_OK;
	uint8_t gsm_state = INITIAL;
	while(gsm_state != IDLE){
		watchdogReset();
		switch(gsm_state){
			case INITIAL:{
				turn_modem_on();
				send_at_command(TCP_CLOSE, "OK", 2);
				gsm_state = CHECK_PIN;
				break;
			}

			case CHECK_PIN:{
				if(send_at_command(AT_CHECK_PIN, "+CPIN: READY", 2)){
					gsm_state = TURN_OFF_ECHO;
				}
				
				_delay_ms(1000);
				break;
			}

			case TURN_OFF_ECHO:{
				if(send_at_command(AT_ECHO_OFF, "OK", 2)){
					gsm_state = CHECK_GSM;
				}
				
				_delay_ms(1000);
				break;
			}

			case CHECK_SIGNAL:{
				if(send_at_command(AT_CHECK_SIGNAL, "OK", 2)){
					gsm_state = CHECK_GSM;
				}
				
				_delay_ms(1000);
				break;
			}

			case CHECK_GSM:{
				if(send_at_command(AT_CHECK_GSM, "+CREG: 0,1", 2)){
					gsm_state = CHECK_GPRS;
				}
				
				_delay_ms(1000);
				break;
			}

			case CHECK_GPRS:{
				if(send_at_command(AT_CHECK_GPRS, "+CGATT: 1", 2)){
					gsm_state = SET_PPP_LINK;
				}
				
				_delay_ms(1000);
				break;
			}

			case SET_PPP_LINK:{
				if(send_at_command(AT_SET_PPP_LINK, "OK", 2)){
					gsm_state = CONNECT_TCP;
				}
				
				_delay_ms(1000);
				break;
			}

			case CHECK_PPP_LINK:{
				if(send_at_command(AT_CHECK_PPP_LINK, "+XIIC:    1,", 2)){
					gsm_state = CONNECT_TCP;
				}
				
				_delay_ms(1000);
				break;
			}

			case CONNECT_TCP:{
				if(send_at_command(AT_TCP_TRANS, "+TCPTRANS: OK", 5)){
					gsm_state = IDLE;
					return OK;
				}
				else{
					turn_modem_off();
					_delay_ms(1000);
					gsm_state = INITIAL;
				}
				break;
			}

			case IDLE:{
				// uart_puts("GSM Loop Exits !!!\r\n");
				ret_val = OK;
				break;
			}

			default:{
				break;
			}
		}
	}
	
	watchdogReset();
	return NOT_OK;
}
