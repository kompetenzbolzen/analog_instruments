/*
 * src/uart.c
 * (c) 2021 Jonas Gunz <himself@jonasgunz.de>
 * License: All rights reserved.
 */

#include "uart.h"

struct ringbuff_s {
	uint8_t buffer[256];
	uint8_t read;
	uint8_t write;
} rxc_buf, txc_buf ;

ISR(USART_RXC_vect) {
	cli();

	/* Discard if buffer full */
	if( rxc_buf.read - 1 == rxc_buf.write )
		goto end;

	rxc_buf.buffer[rxc_buf.write] = UDR;
	rxc_buf.write++;

end:
	sei();
}

ISR(USART_UDRE_vect) {
	cli();

	/* Prevent interrupt loops */
	if( txc_buf.read == txc_buf.write ) {
		UCSRB &= ~(1<<UDRIE);
		goto end;
	}

	UDR = txc_buf.buffer[txc_buf.read];
	txc_buf.read++;

end:
	sei();
}

void uart_init(){
	memset( &txc_buf, 0, sizeof(txc_buf) );
	memset( &rxc_buf, 0, sizeof(rxc_buf) );

	UBRRH = (uint8_t)((UBRR)>>8);
	UBRRL = (uint8_t)(UBRR);

	UCSRB |= (1<<RXEN)|(1<<RXCIE)|(1<<TXEN);
}

uint8_t uart_putchar(char _c) {
	if( txc_buf.read - 1 == txc_buf.write )
		return 1;

	txc_buf.buffer[txc_buf.write] = _c;
	txc_buf.write ++;

	UCSRB |= (1<<UDRIE);

	return 0;
}

uint8_t uart_getchar(char *_c) {
	if( rxc_buf.read == rxc_buf.write )
		return 1;

	*_c = rxc_buf.buffer[rxc_buf.read];
	rxc_buf.read++;

	return 0;
}
