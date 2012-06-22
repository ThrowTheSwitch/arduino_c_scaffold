/*
 * hserial.h
 *
 *  Created on: Jul 25, 2009
 *      Author: Orlando Arias
 *     License: GPLv3
 *
 *   Based off: HardwareSerial.h
 */

#ifndef HSERIAL_H_
#define HSERIAL_H_

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
// #include "wiring.h"
// #include "wiring_private.h"

#define RX_BUFFER_SIZE 128

typedef struct _ring_buffer {
  uint8_t buffer[RX_BUFFER_SIZE];
  int16_t head;
  int16_t tail;
} ring_buffer_t;

typedef struct _hserial {
  ring_buffer_t *rx_buffer;
    volatile uint8_t *ubrrh;
    volatile uint8_t *ubrrl;
    volatile uint8_t *ucsra;
    volatile uint8_t *ucsrb;
    volatile uint8_t *udr;
    uint8_t rxen;
    uint8_t txen;
    uint8_t rxcie;
    uint8_t udre;
} hserial;

/* The ATmega1280 has 4 serial ports available
 * thus we check whether we are cross compiling to
 * this CPU and create as many serial ports as needed.
 */
#if defined(__AVR_ATmega1280__)
  hserial Serial_[4];
#else
  hserial Serial_[1];
#endif

void hs_init();                   // Initializes all available serial ports.
void hs_start(const int16_t port, uint32_t baud);  // Sets up a serial port.
uint8_t hs_available(const int16_t port);       // Checks whether data is available on the port
void hs_flush(const int16_t port);            // Flushes the contents on the serial port.
int16_t hs_getChar(const int16_t port);           // Gets next character (rx) on serial port.
void hs_writeChar(const int16_t port, uint8_t c);   // Write a character (tx) on serial port.
void hs_writeStr(const int16_t port, const char str[]); // Write a string to serial port (tx).

#endif /* HSERIAL_H_ */

