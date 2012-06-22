/*
 * hserial.c
 *
 *  Created on: Jul 25, 2009
 *      Author: Orlando Arias
 *     License: GPLv3
 *
 *
 *   Based off: HardwareSerial.cpp
 *
 *   Special thanks to RuggedCircuits for fixing bug on hs_writeChar() function.
 *      http://www.ruggedcircuits.com/
 *
 */
#include <inttypes.h>
#include <stdio.h>
// #include "wiring.h"
//#include "wiring_private.h"

#include "hserial.h"

// function prototypes
void hs_parseStr(const int port, const char *str);
inline void store_char(unsigned char c, ring_buffer_t *rx_buffer);

// Initialize rx_buffer arrays accordingly.
#if defined(__AVR_ATmega1280__)
  ring_buffer_t rx_buffer[4] = { { { 0 }, 0, 0 },
                 { { 0 }, 0, 0 }
                 { { 0 }, 0, 0 }
                 { { 0 }, 0, 0 } };
#else
  ring_buffer_t rx_buffer[1] =  { { { 0 }, 0, 0 } };
#endif

// serial macros
#if defined(__AVR_ATmega1280__)

SIGNAL(SIG_USART0_RECV)
{
  unsigned char c = UDR0;
  store_char(c, &rx_buffer[0]);
}

SIGNAL(SIG_USART1_RECV)
{
  unsigned char c = UDR1;
  store_char(c, &rx_buffer[1]);
}

SIGNAL(SIG_USART2_RECV)
{
  unsigned char c = UDR2;
  store_char(c, &rx_buffer[2]);
}

SIGNAL(SIG_USART3_RECV)
{
  unsigned char c = UDR3;
  store_char(c, &rx_buffer[3]);
}

#else

#if defined(__AVR_ATmega8__)
SIGNAL(SIG_UART_RECV)
#else
SIGNAL(USART_RX_vect)
#endif
{
#if defined(__AVR_ATmega8__)
  unsigned char c = UDR;
#else
  unsigned char c = UDR0;
#endif
  store_char(c, &rx_buffer[0]);
}
#endif

// Initialize serial objects according to processor type.
void hs_init(){
  #if defined(__AVR_ATmega8__)
    Serial_[0].rx_buffer = &rx_buffer[0];
    Serial_[0].ubrrh = &UBRRH;
    Serial_[0].ubrrl = &UBRRL;
    Serial_[0].ucsra = &UCSRA;
    Serial_[0].ucsrb = &UCSRB;
    Serial_[0].udr = &UDR;
    Serial_[0].rxen = RXEN;
    Serial_[0].txen = TXEN;
    Serial_[0].rxcie = RXCIE;
    Serial_[0].udre = UDRE;
  #else
    Serial_[0].rx_buffer = &rx_buffer[0];
    Serial_[0].ubrrh = &UBRR0H;
    Serial_[0].ubrrl = &UBRR0L;
    Serial_[0].ucsra = &UCSR0A;
    Serial_[0].ucsrb = &UCSR0B;
    Serial_[0].udr = &UDR0;
    Serial_[0].rxen = RXEN0;
    Serial_[0].txen = TXEN0;
    Serial_[0].rxcie = RXCIE0;
    Serial_[0].udre = UDRE0;
  #endif

  #if defined(__AVR_ATmega1280__)
    Serial_[1].rx_buffer = &rx_buffer[1];
    Serial_[1].ubrrh = &UBRR1H;
    Serial_[1].ubrrl = &UBRR1L;
    Serial_[1].ucsra = &UCSR1A;
    Serial_[1].ucsrb = &UCSR1B;
    Serial_[1].udr = &UDR1;
    Serial_[1].rxen = RXEN1;
    Serial_[1].txen = TXEN1;
    Serial_[1].rxcie = RXCIE1;
    Serial_[1].udre = UDRE1;

    Serial_[2].rx_buffer = &rx_buffer[2];
    Serial_[2].ubrrh = &UBRR2H;
    Serial_[2].ubrrl = &UBRR2L;
    Serial_[2].ucsra = &UCSR2A;
    Serial_[2].ucsrb = &UCSR2B;
    Serial_[2].udr = &UDR2;
    Serial_[2].rxen = RXEN2;
    Serial_[2].txen = TXEN2;
    Serial_[2].rxcie = RXCIE2;
    Serial_[2].udre = UDRE2;

    Serial_[3].rx_buffer = &rx_buffer[3];
    Serial_[3].ubrrh = &UBRR3H;
    Serial_[3].ubrrl = &UBRR3L;
    Serial_[3].ucsra = &UCSR3A;
    Serial_[3].ucsrb = &UCSR3B;
    Serial_[3].udr = &UDR3;
    Serial_[3].rxen = RXEN3;
    Serial_[3].txen = TXEN3;
    Serial_[3].rxcie = RXCIE3;
    Serial_[3].udre = UDRE3;
  #endif
}

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void hs_start(const int port, unsigned long baud){
  uint16_t const ubrr = F_CPU / (8 * baud) - 1;

  *Serial_[port].ubrrh = ubrr >> 8; 
  *Serial_[port].ubrrl = (uint8_t)(ubrr & 0x00FF);

  sbi(*Serial_[port].ucsrb, Serial_[port].rxen);
  sbi(*Serial_[port].ucsrb, Serial_[port].txen);
  sbi(*Serial_[port].ucsrb, Serial_[port].rxcie);
}

uint8_t hs_available(const int port){
  return (RX_BUFFER_SIZE + Serial_[port].rx_buffer->head -
      Serial_[port].rx_buffer->tail) % RX_BUFFER_SIZE;
}

int hs_getChar(const int port){
  /*
   * If the head isn't ahead of the tail,
   * we don't have any characters
   */
  if (Serial_[port].rx_buffer->head == Serial_[port].rx_buffer->tail) {
    return -1;
  } else {
    unsigned char c =
      Serial_[port].rx_buffer->buffer[Serial_[port].rx_buffer->tail];
    Serial_[port].rx_buffer->tail =
      (Serial_[port].rx_buffer->tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}

void hs_flush(const int port){
  Serial_[port].rx_buffer->head =
      Serial_[port].rx_buffer->tail;
}

void hs_writeChar(const int port, uint8_t c){
  while (!((*Serial_[port].ucsra) & (1 << Serial_[port].udre)));
  *(Serial_[port].udr) = c;
}

void hs_writeStr(const int port, const char str[]){
  hs_parseStr(port, str);
}

void hs_parseStr(const int port, const char *str){
    while (*str)
      hs_writeChar(port, *str++);
}

inline void store_char(unsigned char c, ring_buffer_t *rx_buffer){
  int i = (rx_buffer->head + 1) % RX_BUFFER_SIZE;
  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != rx_buffer->tail) {
    rx_buffer->buffer[rx_buffer->head] = c;
    rx_buffer->head = i;
  }
}
