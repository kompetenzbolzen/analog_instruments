/*
 * src/pwm.h
 * (c) 2022 Jonas Gunz <himself@jonasgunz.de>
 * License: All rights reserved.
 */

#ifndef _PWM_H_
#define _PWM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

extern uint8_t t0_ovf_cnt;
extern uint8_t pb0_thresh;

void pwm_init();

void pwm_set_pin(uint8_t _pin, uint8_t _duty);

#endif /* _PWM_H_ */
