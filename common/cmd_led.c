/*
 * (C) Copyright 2010
 * Jason Kridner <jkridner@beagleboard.org>
 *
 * Based on cmd_led.c patch from:
 * http://www.mail-archive.com/u-boot@lists.denx.de/msg06873.html
 * (C) Copyright 2008
 * Ulf Samuelsson <ulf.samuelsson@atmel.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <status_led.h>

#ifndef STATUS_LED_OFF
#define STATUS_LED_OFF		0
#endif

#ifndef STATUS_LED_ON
#define STATUS_LED_ON		1
#endif

#ifndef STATUS_LED_BLINKING
#define STATUS_LED_BLINKING	2
#endif




struct led_tbl_s {
	char		*string;	/* String for use in the command */
	led_id_t	mask;		/* Mask used for calling __led_set() */
	void		(*off)(void);	/* Optional function for turning LED off */
	void		(*on)(void);	/* Optional function for turning LED on */
	void		(*toggle)(void);/* Optional function for toggling LED */
};

typedef struct led_tbl_s led_tbl_t;

static const led_tbl_t led_commands[] = {
#ifdef CONFIG_BOARD_SPECIFIC_LED

#ifdef STATUS_LED_BIT1
	{ "1", STATUS_LED_BIT1, NULL, NULL, NULL },
#endif

#ifdef STATUS_LED_BIT2
	{ "2", STATUS_LED_BIT2, NULL, NULL, NULL },
#endif

#ifdef STATUS_LED_BIT3
	{ "3", STATUS_LED_BIT3, NULL, NULL, NULL },
#endif

#ifdef STATUS_LED_BIT4
	{ "4", STATUS_LED_BIT4, NULL, NULL, NULL },
#endif

#ifdef STATUS_LED_BIT5
	{ "5", STATUS_LED_BIT5, NULL, NULL, NULL },
#endif

#endif
	{ NULL, 0, NULL, NULL, NULL }
};

enum led_cmd { LED_ON, LED_OFF, LED_TOGGLE, LED_SET, LED_GET };

enum led_cmd get_led_cmd(char *var)
{
	if (strcmp(var, "off") == 0)
		return LED_OFF;
	if (strcmp(var, "on") == 0)
		return LED_ON;
	if (strcmp(var, "toggle") == 0)
		return LED_TOGGLE;
	if (strcmp(var, "set") == 0)
		return LED_SET;
	if (strcmp(var, "get") == 0)
		return LED_GET;
	return -1;
}

int do_led (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i, match = 0, set_value;
	enum led_cmd cmd;
	
	/* Validate arguments */
	if (argc < 3) {
		return CMD_RET_USAGE;
	}

	cmd = get_led_cmd(argv[2]);
	if (cmd < 0) {
		return CMD_RET_USAGE;
	}

	for (i = 0; led_commands[i].string; i++) {
		if ((strcmp("all", argv[1]) == 0) ||
		    (strcmp(led_commands[i].string, argv[1]) == 0)) {
			match = 1;
			switch (cmd) {
			case LED_ON:
				if (led_commands[i].on)
					led_commands[i].on();
				else
					__led_set(led_commands[i].mask,
							  STATUS_LED_ON);
				break;
			case LED_OFF:
				if (led_commands[i].off)
					led_commands[i].off();
				else
					__led_set(led_commands[i].mask,
							  STATUS_LED_OFF);
				break;
			case LED_TOGGLE:
				if (led_commands[i].toggle)
					led_commands[i].toggle();
				else
					__led_toggle(led_commands[i].mask);
			case LED_GET:
				printf("LED 0x%x , now state = 0x%x\n", led_commands[i].mask, __led_get(led_commands[i].mask));
				break;
			case LED_SET:
				// led [1|2|3|4|5|all] [set|off|on|toggle] [value]
				if (argc < 4)
					return CMD_RET_USAGE;
				set_value = simple_strtol(argv[3], NULL, 10);
				debug("[YYF] lcd 0x%lx set value to 0x%x\n", led_commands[i].mask, set_value);
				__led_set(led_commands[i].mask,set_value);
				printf("LED 0x%x , now state = 0x%x\n", led_commands[i].mask, __led_get(led_commands[i].mask));
			}
			if (strcmp("all", argv[1]) != 0)
				break;
		}
	}

	/* If we ran out of matches, print Usage */
	if (!match) {
		return CMD_RET_USAGE;
	}

	return 0;
}

U_BOOT_CMD(
	led, 4, 1, do_led,
	"["
#ifdef CONFIG_BOARD_SPECIFIC_LED

#ifdef STATUS_LED_BIT1
	"1|"
#endif

#ifdef STATUS_LED_BIT2
	"2|"
#endif

#ifdef STATUS_LED_BIT3
	"3|"
#endif

#ifdef STATUS_LED_BIT4
	"4|"
#endif

#ifdef STATUS_LED_BIT5
	"5|"
#endif

#endif
	
	"all] [on|off|toggle|set] [0|1|2|...]",
	"[led_name] [on|off|toggle|set] sets or clears led(s)"
);
