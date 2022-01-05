/*
 * src/cmd.c
 * (c) 2021 Jonas Gunz <himself@jonasgunz.de>
 * License: All rights reserved.
 */

#include "cmd.h"
#include "helpers.h"
#include <stdint.h>

struct {
	uint8_t state;
	uint8_t cmd;
	uint8_t argv[_CMD_MAX_ARGC];
} cmd_state;

const struct {
	char literal;
	void (*fkt)(uint8_t[]);
	uint8_t argc;
} cmd[_CMD_CNT] = {
	{0,cmd_err,0},
	{'p',cmd_set_pwm,3}
};

void cmd_init() {
	cmd_state.state = 0;
	cmd_state.cmd = 0;
}

void cmd_tick(char _c) {
	uint8_t i;

	if(_c == '\r') {
		/* if cmd is not cmd_err and argc do not match */
		if( cmd_state.cmd && ( cmd[cmd_state.cmd].argc != cmd_state.state - 1 ) ) {
			/* set cmd_err to _ERR_ARGC */
			cmd_state.cmd = 0;
			cmd_state.argv[0] = _ERR_ARGC;
		}

		(*cmd[cmd_state.cmd].fkt)(cmd_state.argv);
		cmd_state.state   = 0;
		cmd_state.cmd     = 0;
		cmd_state.argv[0] = 0;
		return;
	}

	if(_c == '\n')
		return;

	if(!cmd_state.state) {
		cmd_state.state = 1;
		for( i=0; i<_CMD_CNT; i++ ) {
			if( _c != cmd[i].literal )
				continue;

			cmd_state.cmd = i;
			return;
		}

		cmd_state.cmd = 0;
		cmd_state.argv[0] = _ERR_CMD;
	}

	/* Check, if maximum argc is reached */
	if (cmd_state.state >= _CMD_MAX_ARGC) {
		cmd_state.cmd = 0;
		cmd_state.argv[0] = _ERR_ARGC;
	}

	cmd_state.argv[ (cmd_state.state++) - 1 ] = _c;
}

void cmd_set_pwm(uint8_t _argv[]) {
	uint8_t duty = hex_to_byte((char*)&(_argv[1]));
	uint8_t pin = _argv[0] - 48;

	pwm_set_pin(pin, duty);
}

void cmd_err( uint8_t _argv[] ) {
	switch(_argv[0]) {
		case _ERR_ARGC:
			uart_putstring("E:ARG\r\n");
			break;
		case _ERR_CMD:
			uart_putstring("E:CMD\r\n");
			break;
		default:
			uart_putstring("E\r\n");

	}
}
