/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>

#include <../drivers/video/rockchip_display.h>

static int do_yifengyou(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int i;
	int putnl = 1;

	for (i = 1; i < argc; i++) {
		char *p = argv[i];
		char *nls; /* new-line suppression */

		if (i > 1)
			putc(' ');

		nls = strstr(p, "\\c");
		if (nls) {
			char *prenls = p;

			putnl = 0;
			/*
			 * be paranoid and guess that someone might
			 * say \c more than once
			 */
			while (nls) {
				*nls = '\0';
				puts(prenls);
				*nls = '\\';
				prenls = nls + 2;
				nls = strstr(prenls, "\\c");
			}
			puts(prenls);
		} else {
			puts(p);
		}
	}

	if (putnl)
		putc('\n');

	return 0;
}

U_BOOT_CMD(
	yifengyou,	CONFIG_SYS_MAXARGS,	1,	do_yifengyou,
	"echo args to console",
	"[args..]\n"
	"    - echo args to console; \\c suppresses newline"
);
