#include "uart.h"
#include "watchdog.h"

void uart_init(){
#ifndef SOFT_UART

	#if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega16__)
	  UCSRA = _BV(U2X); //Double speed mode USART
	  UCSRB = _BV(RXEN) | _BV(TXEN);  // enable Rx & Tx
	  UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);  // config USART; 8N1
	  UBRRL = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );
	#else
	  UART_SRA = _BV(U2X0); //Double speed mode USART0
	  UART_SRB = _BV(RXEN0) | _BV(TXEN0);
	  UART_SRC = _BV(UCSZ00) | _BV(UCSZ01);
	  UART_SRL = (uint8_t)( (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 );
	#endif

#else
// AVR305 equation: #define UART_B_VALUE (((F_CPU/BAUD_RATE)-23)/6)
// Adding 3 to numerator simulates nearest rounding for more accurate baud rates
	#define UART_B_VALUE (((F_CPU/BAUD_RATE)-20)/6)
	#if UART_B_VALUE > 255
	#error Baud rate too slow for soft UART
  #endif
	  /* Set TX pin as output */
	UART_DDR |= _BV(UART_TX_BIT);

#endif
}

void putch(char ch) {
#ifndef SOFT_UART
  while (!(UART_SRA & _BV(UDRE0)));
  UART_UDR = ch;
#else
  __asm__ __volatile__ (
    "   com %[ch]\n" // ones complement, carry set
    "   sec\n"
    "1: brcc 2f\n"
    "   cbi %[uartPort],%[uartBit]\n"
    "   rjmp 3f\n"
    "2: sbi %[uartPort],%[uartBit]\n"
    "   nop\n"
    "3: rcall uartDelay\n"
    "   rcall uartDelay\n"
    "   lsr %[ch]\n"
    "   dec %[bitcnt]\n"
    "   brne 1b\n"
    :
    :
      [bitcnt] "d" (10),
      [ch] "r" (ch),
      [uartPort] "I" (_SFR_IO_ADDR(UART_PORT)),
      [uartBit] "I" (UART_TX_BIT)
    :
      "r25"
  );
#endif
}

uint8_t getch(void) {
  uint8_t ch;

#ifdef LED_DATA_FLASH
  #if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega16__)
    LED_PORT ^= _BV(LED);
  #else
    LED_PIN |= _BV(LED);
  #endif
#endif

#ifdef SOFT_UART
    watchdogReset();
  __asm__ __volatile__ (
    "1: sbic  %[uartPin],%[uartBit]\n"  // Wait for start edge
    "   rjmp  1b\n"
    "   rcall uartDelay\n"          // Get to middle of start bit
    "2: rcall uartDelay\n"              // Wait 1 bit period
    "   rcall uartDelay\n"              // Wait 1 bit period
    "   clc\n"
    "   sbic  %[uartPin],%[uartBit]\n"
    "   sec\n"
    "   dec   %[bitCnt]\n"
    "   breq  3f\n"
    "   ror   %[ch]\n"
    "   rjmp  2b\n"
    "3:\n"
    :
      [ch] "=r" (ch)
    :
      [bitCnt] "d" (9),
      [uartPin] "I" (_SFR_IO_ADDR(UART_PIN)),
      [uartBit] "I" (UART_RX_BIT)
    :
      "r25"
);
#else
  while(!(UART_SRA & _BV(RXC0)))
    ;
  if (!(UART_SRA & _BV(FE0))) {
      /*
       * A Framing Error indicates (probably) that something is talking
       * to us at the wrong bit rate.  Assume that this is because it
       * expects to be talking to the application, and DON'T reset the
       * watchdog.  This should cause the bootloader to abort and run
       * the application "soon", if it keeps happening.  (Note that we
       * don't care that an invalid char is returned...)
       */
    watchdogReset();
  }

  ch = UART_UDR;
#endif

#ifdef LED_DATA_FLASH
  #if defined(__AVR_ATmega8__) || defined (__AVR_ATmega32__) || defined (__AVR_ATmega16__)
    LED_PORT ^= _BV(LED);
  #else
    LED_PIN |= _BV(LED);
  #endif
#endif

  return ch;
}

#ifdef SOFT_UART
  // AVR305 equation: #define UART_B_VALUE (((F_CPU/BAUD_RATE)-23)/6)
  // Adding 3 to numerator simulates nearest rounding for more accurate baud rates
  #define UART_B_VALUE (((F_CPU/BAUD_RATE)-20)/6)
  #if UART_B_VALUE > 255
    #error Baud rate too slow for soft UART
  #endif

void uartDelay() {
  __asm__ __volatile__ (
    "ldi r25,%[count]\n"
    "1:dec r25\n"
    "brne 1b\n"
    "ret\n"
    ::[count] "M" (UART_B_VALUE)
  );
}
#endif

uint8_t uart_puts(const char * buf){
  while (*buf) {
    putch(*buf++);
  }
}

int get_non_blocking_ch(){
  if(UART_SRA & _BV(RXC0)){
    if (!(UART_SRA & _BV(FE0))) {
        /*
         * A Framing Error indicates (probably) that something is talking
         * to us at the wrong bit rate.  Assume that this is because it
         * expects to be talking to the application, and DON'T reset the
         * watchdog.  This should cause the bootloader to abort and run
         * the application "soon", if it keeps happening.  (Note that we
         * don't care that an invalid char is returned...)
         */
      watchdogReset();
    }
    return(UART_UDR);
  }
  return -1;
}