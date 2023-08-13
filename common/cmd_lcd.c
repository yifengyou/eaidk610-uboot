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

// cmd_lcd.c
#include <common.h>
#include <command.h>

#include <../drivers/video/rockchip_display.h>

static int colors[] = {
        0xFF0000,// 红
        0xFF7D00,// 橙
        0xFFFF00,// 黄
        0x00FF00,// 绿
        0x00FFFF,// 青
        0x0000FF,// 蓝
        0xFF00FF,// 紫
        0xFFFFFF,// 白
        0x000000,// 黑
};

// 定义一个变量，记录当前的颜色索引
static int color_index = 0;

static int do_lcd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]) {

    int color = colors[color_index];

    rockchip_show_color(color);

    printf("LCD color: 0x%06X\n", color);

    color_index++;
    if (color_index >= ARRAY_SIZE(colors)) {
        color_index = 0;
    }

    return 0;
}

// 定义扩展命令lcd_cmd
U_BOOT_CMD(
        lcd, CONFIG_SYS_MAXARGS,1, do_lcd,
"change LCD color",
" - change LCD color in a loop"
);
