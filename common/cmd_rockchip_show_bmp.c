/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>

#include <../drivers/video/rockchip_display.h>

static int do_rockchip_show_bmp(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    // 如果用户输入了kernel作为参数，则打印kernel
    if (argc > 1 && strcmp(argv[1], "kernel") == 0) {
        printf("[YYF] show logo_kernel.bmp\n");
        rockchip_show_bmp("logo_kernel.bmp");
    }
    else {
        printf("[YYF] show logo.bmp\n");
        rockchip_show_bmp("logo.bmp");
    }
    return 0;
}

U_BOOT_CMD(
	rockchip_show_bmp,	CONFIG_SYS_MAXARGS,	1,	do_rockchip_show_bmp,
	"show bmp",
	"[args..]\n"
	"    - if kernel , show logo_kernel.bmp, else show logo.bmp"
);
