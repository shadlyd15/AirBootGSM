/*
 * pin_defs.h
 * This contains most of the rather ugly defines that implement our
 * ability to use UART=n and LED=D3, and some avr family bit name differences.
 */
#include "pin_defs.h"
#include <avr/io.h>

/* set the UART baud rate defaults */
#ifndef BAUD_RATE
  #if F_CPU >= 8000000L
    #define BAUD_RATE   115200L // Highest rate Avrdude win32 will support
  #elif F_CPU >= 1000000L
    #define BAUD_RATE   9600L   // 19200 also supported, but with significant error
  #elif F_CPU >= 128000L
    #define BAUD_RATE   4800L   // Good for 128kHz internal RC
  #else
    #define BAUD_RATE 1200L     // Good even at 32768Hz
  #endif
#endif

#ifndef UART
  #define UART 0
#endif

#define BAUD_SETTING (( (F_CPU + BAUD_RATE * 4L) / ((BAUD_RATE * 8L))) - 1 )
#define BAUD_ACTUAL (F_CPU/(8 * ((BAUD_SETTING)+1)))

#if BAUD_ACTUAL <= BAUD_RATE
  #define BAUD_ERROR (( 100*(BAUD_RATE - BAUD_ACTUAL) ) / BAUD_RATE)
  #if BAUD_ERROR >= 5
    #error BAUD_RATE error greater than -5%
  #elif BAUD_ERROR >= 2
    #warning BAUD_RATE error greater than -2%
  #endif
#else
  #define BAUD_ERROR (( 100*(BAUD_ACTUAL - BAUD_RATE) ) / BAUD_RATE)
  #if BAUD_ERROR >= 5
    #error BAUD_RATE error greater than 5%
  #elif BAUD_ERROR >= 2
    #warning BAUD_RATE error greater than 2%
  #endif
#endif

#if (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 > 250
  #error Unachievable baud rate (too slow) BAUD_RATE 
#endif // baud rate slow check

#if (F_CPU + BAUD_RATE * 4L) / (BAUD_RATE * 8L) - 1 < 3
  #if BAUD_ERROR != 0 // permit high bitrates (ie 1Mbps@16MHz) if error is zero
    #error Unachievable baud rate (too fast) BAUD_RATE 
  #endif
#endif // baud rate fastn check

void __attribute__((noinline)) putch(char);
uint8_t __attribute__((noinline)) getch(void);
uint8_t uart_puts(const char * buf);
int get_non_blocking_ch();

#ifdef SOFT_UART
  void uartDelay() __attribute__ ((naked));
#endif