#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uart.h"

uint8_t t0_ovf_cnt = 0;

uint8_t pb0_thresh = 128;

ISR(TIMER0_OVF_vect) {
	cli();
	/*TCNT0 = (1<<7);*/ /* Hack-increase Interrupt trigger freq */

	t0_ovf_cnt ++;

	if ( t0_ovf_cnt >= pb0_thresh )
		PORTB &= ~(1<<PINB0);
	else
		PORTB |= (1<<PINB0);

	sei();
}

ISR(TIMER2_OVF_vect) {
	cli();
	pb0_thresh ++;
	sei();
}

int main(void) {
	char c;

	cli();

	/* Pins */
	DDRB |= (1<<PINB0);

	/* Enable TIMER0 with interrupt, no prescaler */
	TCCR0 |= (1<<CS00);
	TIMSK |= (1<<TOIE0);

	/* Enable TIMER2 with interrupt, Clk divide by 1024 */
	/*
	TCCR2 |= (7<<CS20);
	TIMSK |= (1<<TOIE2);
	*/

	/* Uart */
	uart_init();
	
	/* Go! */
	sei();

	while(1){
		if( uart_getchar(&c) )
			continue;

		uart_putchar(c);
		pb0_thresh = c;
	}

	return 0;
}
