/*
 * src/main.c
 * (c) 2021 Jonas Gunz <himself@jonasgunz.de>
 * License: All rights reserved.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uart.h"
#include "cmd.h"
#include "pwm.h"

int main(void) {
	cli();

	uart_init();
	cmd_init();
	pwm_init();
	
	/* Go! */
	sei();

	uart_putstring("START\r\n");

	while(1){
		char c;
		if( uart_getchar(&c) )
			continue;

		uart_putchar(c);
		cmd_tick(c);
	}

	return 0;
}
