/*
 * src/pwm.h
 * (c) 2022 Jonas Gunz <himself@jonasgunz.de>
 * License: MIT
 */

#ifndef _PWM_H_
#define _PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define PWM_PINS 3
#define PWM_DDR DDRB
#define PWM_PORT PORTB

void pwm_init();

void pwm_set_pin(uint8_t _pin, uint8_t _duty);

void pwm_fade_pin(uint8_t _pin, uint8_t _duty);

#endif /* _PWM_H_ */
