/*
 * src/cmd.h
 * (c) 2021 Jonas Gunz <himself@jonasgunz.de>
 * License: All rights reserved.
 */

#ifndef _CMD_H_
#define _CMD_H_

#include <stdint.h>

#include "uart.h"
#include "pwm.h"
#include "helpers.h"

#define _CMD_CNT	2
#define _CMD_MAX_ARGC	4

#define _ERR_CMD	1
#define _ERR_ARGC	2

void cmd_init();

void cmd_tick(char _c);

/**
 * argc=3
 *
 * 0: Pin Num
 * 1-2: Dutycycle in HEX (00-FF)
 */
void cmd_set_pwm(uint8_t _argv[]);

/**
 * argc=1
 *
 * 0: Error type
 */
void cmd_err(uint8_t _argv[]);

#endif
