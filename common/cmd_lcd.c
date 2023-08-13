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
#include <lcd.h>

// 定义一个颜色数组，包含红、绿、蓝、白、黑五种颜色
static int colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 0x000000};

// 定义一个变量，记录当前的颜色索引
static int color_index = 0;

static int do_lcd(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    // 获取LCD的宽度和高度
    int width = lcd_get_pixel_width();
    int height = lcd_get_pixel_height();

    // 获取当前的颜色值
    int color = colors[color_index];

//    // 填充LCD的缓冲区为当前的颜色值
//    lcd_fill(0, 0, width, height, color);

    // 刷新LCD的显示
    lcd_sync();

    // 打印当前的颜色信息
    printf("LCD color: 0x%06X\n", color);

    // 更新颜色索引，如果超过数组长度，就从头开始
    color_index++;
    if (color_index >= ARRAY_SIZE(colors)) {
        color_index = 0;
    }

    return 0;
}

// 定义扩展命令lcd_cmd
U_BOOT_CMD(
        lcd, CONFIG_SYS_MAXARGS,	1, do_lcd,
"change LCD color",
" - change LCD color in a loop"
);
