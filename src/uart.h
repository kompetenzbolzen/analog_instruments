/*
 * src/uart.h
 * (c) 2021 Jonas Gunz <himself@jonasgunz.de>
 * License: All rights reserved.
 */

/*
 * Interrupt controlled UART
 */

#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

#ifndef BAUD
#warning BAUD "BAUD not defined. Dafaulting to 9600"
#define BAUD 9600
#endif

#ifndef F_CPU
#error "F_CPU not defined"
#endif

#define UBRR F_CPU/16/BAUD-1

void uart_init();

uint8_t uart_putchar(char _c);

uint8_t uart_getchar(char *_c);

#endif
