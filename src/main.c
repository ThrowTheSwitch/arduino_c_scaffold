/*
 * This example was borrowed from:
 *   http://balau82.wordpress.com/2011/03/29/programming-arduino-uno-in-pure-c/
 */

#include <avr/io.h>
#include <util/delay.h>

enum {
	BLINK_DELAY_MS = 1000,
};

int main (void)
{
	/* set pin 5 of PORTB for output*/
	DDRB |= _BV(DDB5);

	while(1) {
		/* set pin 5 high to turn led on */
		PORTB |= _BV(PORTB5);
		_delay_ms(BLINK_DELAY_MS);
		
		/* set pin 5 low to turn led off */
		PORTB &= ~_BV(PORTB5);
		_delay_ms(BLINK_DELAY_MS);
	}

	return 0;
}

