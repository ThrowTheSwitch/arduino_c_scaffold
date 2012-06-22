#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "hserial.h"

#include "main.h"

static uint16_t tick = 0;

int main(void)
{
  configure();

  while(1)
  {
    int16_t c;
    while ((c = hs_getChar(0)) != -1)
    {
      hs_writeChar(0, c);
    }
  }

  return 0;
}

void configure(void)
{
  /* disable interrupts */
  cli();

  /* configure TIMER0 to use the CLK/64 prescaler. */
  TCCR0B = _BV(CS00) | _BV(CS01);

  /* enable the TIMER0 overflow interrupt */
  TIMSK0 = _BV(TOIE0);

  /* set the initial timer counter value. */
  TCNT0 = TIMER_RESET_VAL;

  /* confiure PB5 as an output. */
  DDRB |= _BV(DDB5);

  /* turn off surface mount LED on */
  PORTB &= ~_BV(PORTB5);


  hs_init();
  hs_start(0, 115200);

  /* enable interrupts. */
  sei();
}

void task(void)
{
  /* toggle every thousand ticks */
  if (tick >= 1000)
  {
    /* toggle the LED */
    PORTB ^= _BV(PORTB5);

    /* reset the tick */
    tick = 0;
  }

  tick++;
}

ISR(TIMER0_OVF_vect)
{
  /* preload the timer. */
  TCNT0 = TIMER_RESET_VAL;
  
  /* call our periodic task. */
  task();
}

