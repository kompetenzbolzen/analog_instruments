#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

ISR(TIMER0_OVF_vect) {
	cli();

	sei();
}

int main(void) {
	cli();

	/* Enable TIMER0 with interrupt */
	TCCR0 |= (1<<CS00);
	TIMSK |= (1<<TOIE0);
	
	/* Go! */
	sei();

	while(1){

	}

	return 0;
}
