/*
 * src/pwm.c
 * (c) 2022 Jonas Gunz <himself@jonasgunz.de>
 * License: MIT
 */

#include "pwm.h"
#include <stdint.h>

uint8_t t0_ovf_cnt = 0;

uint8_t pwm_duty[PWM_PINS];
uint8_t pwm_target[PWM_PINS];

ISR(TIMER0_OVF_vect) {
	uint8_t i;
	cli();
	/*TCNT0 = (1<<7);*/ /* Hack-increase Interrupt trigger freq */

	t0_ovf_cnt ++;

	for( i=0; i<PWM_PINS; i++) {
		if(t0_ovf_cnt <= pwm_duty[i])
			PWM_PORT |= (1<<i);
		else
			PWM_PORT &= ~(1<<i);
	}

	sei();
}

ISR(TIMER2_OVF_vect) {
	uint8_t i;
	cli();

	for( i=0; i<PWM_PINS; i++) {
		if (pwm_duty[i] < pwm_target[i])
			pwm_duty[i]++;
		else if (pwm_duty[i] > pwm_target[i])
			pwm_duty[i]--;
	}

	sei();
}

void pwm_init() {
	uint8_t i;

	PWM_DDR  |=  (0xff >> (8-PWM_PINS));
	PWM_PORT &= ~(0xff >> (8-PWM_PINS));

	for ( i=0; i<PWM_PINS; i++ )
		pwm_duty[i] = pwm_target[i] = 0;

	/* Enable TIMER0 with interrupt, no prescaler */
	TCCR0 |= (1<<CS00);
	TIMSK |= (1<<TOIE0);

	/* Enable TIMER2 with interrupt, Clk divide by 1024 */
	TCCR2 |= (7<<CS20);
	TIMSK |= (1<<TOIE2);
}

void pwm_set_pin(uint8_t _pin, uint8_t _duty) {
	if(_pin < PWM_PINS) {
		pwm_duty[_pin] = _duty;
		pwm_target[_pin] = _duty;
	}
}

void pwm_fade_pin(uint8_t _pin, uint8_t _duty) {
	if(_pin < PWM_PINS)
		pwm_target[_pin] = _duty;
}
