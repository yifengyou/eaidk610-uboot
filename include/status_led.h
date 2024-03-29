/*
 * (C) Copyright 2000-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * The purpose of this code is to signal the operational status of a
 * target which usually boots over the network; while running in
 * PCBoot, a status LED is blinking. As soon as a valid BOOTP reply
 * message has been received, the LED is turned off. The Linux
 * kernel, once it is running, will start blinking the LED again,
 * with another frequency.
 */

#ifndef _STATUS_LED_H_
#define	_STATUS_LED_H_

#ifdef CONFIG_STATUS_LED

#define STATUS_LED_OFF              0
#define STATUS_LED_BLINKING         1
#define STATUS_LED_ON               2

void status_led_tick (unsigned long timestamp);
void status_led_set  (int led, int state);

/*****  TQM8xxL  ********************************************************/
#if defined(CONFIG_TQM8xxL)
# define STATUS_LED_PAR		im_cpm.cp_pbpar
# define STATUS_LED_DIR		im_cpm.cp_pbdir
# define STATUS_LED_ODR		im_cpm.cp_pbodr
# define STATUS_LED_DAT		im_cpm.cp_pbdat

# define STATUS_LED_BIT		0x00000001
# define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE	STATUS_LED_BLINKING

# define STATUS_LED_ACTIVE	1		/* LED on for bit == 1	*/

# define STATUS_LED_BOOT	0		/* LED 0 used for boot status */

/*****  MVS v1  **********************************************************/
#elif (defined(CONFIG_MVS) && CONFIG_MVS < 2)
# define STATUS_LED_PAR		im_ioport.iop_pdpar
# define STATUS_LED_DIR		im_ioport.iop_pddir
# undef  STATUS_LED_ODR
# define STATUS_LED_DAT		im_ioport.iop_pddat

# define STATUS_LED_BIT		0x00000001
# define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE	STATUS_LED_BLINKING

# define STATUS_LED_ACTIVE	1		/* LED on for bit == 1	*/

# define STATUS_LED_BOOT	0		/* LED 0 used for boot status */

/*****  IVMS8  **********************************************************/
#elif defined(CONFIG_IVMS8)

# define STATUS_LED_PAR		im_cpm.cp_pbpar
# define STATUS_LED_DIR		im_cpm.cp_pbdir
# define STATUS_LED_ODR		im_cpm.cp_pbodr
# define STATUS_LED_DAT		im_cpm.cp_pbdat

# define STATUS_LED_BIT		0x00000010	/* LED 0 is on PB.27	*/
# define STATUS_LED_PERIOD	(1 * CONFIG_SYS_HZ)
# define STATUS_LED_STATE	STATUS_LED_OFF
# define STATUS_LED_BIT1	0x00000020	/* LED 1 is on PB.26	*/
# define STATUS_LED_PERIOD1	(1 * CONFIG_SYS_HZ)
# define STATUS_LED_STATE1	STATUS_LED_OFF
/* IDE LED usable for other purposes, too */
# define STATUS_LED_BIT2	0x00000008	/* LED 2 is on PB.28	*/
# define STATUS_LED_PERIOD2	(1 * CONFIG_SYS_HZ)
# define STATUS_LED_STATE2	STATUS_LED_OFF

# define STATUS_LED_ACTIVE	1		/* LED on for bit == 1	*/

# define STATUS_ILOCK_SWITCH	0x00800000	/* ILOCK switch in IRQ4	*/

# define STATUS_ILOCK_PERIOD	(CONFIG_SYS_HZ / 10)	/* about every 100 ms	*/

# define STATUS_LED_YELLOW	0
# define STATUS_LED_GREEN	1
# define STATUS_LED_BOOT	2		/* IDE LED used for boot status */

/*****  IVML24  *********************************************************/
#elif defined(CONFIG_IVML24)

# define STATUS_LED_PAR		im_cpm.cp_pbpar
# define STATUS_LED_DIR		im_cpm.cp_pbdir
# define STATUS_LED_ODR		im_cpm.cp_pbodr
# define STATUS_LED_DAT		im_cpm.cp_pbdat

# define STATUS_LED_BIT		0x00000010	/* LED 0 is on PB.27	*/
# define STATUS_LED_PERIOD	(1 * CONFIG_SYS_HZ)
# define STATUS_LED_STATE	STATUS_LED_OFF
# define STATUS_LED_BIT1	0x00000020	/* LED 1 is on PB.26	*/
# define STATUS_LED_PERIOD1	(1 * CONFIG_SYS_HZ)
# define STATUS_LED_STATE1	STATUS_LED_OFF
/* IDE LED usable for other purposes, too */
# define STATUS_LED_BIT2	0x00000008	/* LED 2 is on PB.28	*/
# define STATUS_LED_PERIOD2	(1 * CONFIG_SYS_HZ)
# define STATUS_LED_STATE2	STATUS_LED_OFF

# define STATUS_LED_ACTIVE	1		/* LED on for bit == 1	*/

# define STATUS_ILOCK_SWITCH	0x00004000	/* ILOCK is on PB.17	*/

# define STATUS_ILOCK_PERIOD	(CONFIG_SYS_HZ / 10)	/* about every 100 ms	*/

# define STATUS_LED_YELLOW	0
# define STATUS_LED_GREEN	1
# define STATUS_LED_BOOT	2		/* IDE LED used for boot status */

/*****  ICU862   ********************************************************/
#elif defined(CONFIG_ICU862)

# define STATUS_LED_PAR		im_ioport.iop_papar
# define STATUS_LED_DIR		im_ioport.iop_padir
# define STATUS_LED_ODR		im_ioport.iop_paodr
# define STATUS_LED_DAT		im_ioport.iop_padat

# define STATUS_LED_BIT		0x4000		/* LED 0 is on PA.1 */
# define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE	STATUS_LED_BLINKING
# define STATUS_LED_BIT1	0x1000		/* LED 1 is on PA.3 */
# define STATUS_LED_PERIOD1	(CONFIG_SYS_HZ)
# define STATUS_LED_STATE1	STATUS_LED_OFF

# define STATUS_LED_ACTIVE	1		/* LED on for bit == 1	*/

# define STATUS_LED_BOOT	0		/* LED 0 used for boot status */

/*****  Someone else defines these  *************************************/
#elif defined(STATUS_LED_PAR)

  /*
   * ADVICE: Define in your board configuration file rather than
   * filling this file up with lots of custom board stuff.
   */

/*****  NetVia   ********************************************************/
#elif defined(CONFIG_NETVIA)

#if !defined(CONFIG_NETVIA_VERSION) || CONFIG_NETVIA_VERSION == 1

#define STATUS_LED_PAR		im_ioport.iop_pdpar
#define STATUS_LED_DIR		im_ioport.iop_pddir
#undef  STATUS_LED_ODR
#define STATUS_LED_DAT		im_ioport.iop_pddat

# define STATUS_LED_BIT		0x0080			/* PD.8 */
# define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE	STATUS_LED_BLINKING

# define STATUS_LED_BIT1	0x0040			/* PD.9 */
# define STATUS_LED_PERIOD1	(CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE1	STATUS_LED_OFF

# define STATUS_LED_ACTIVE	0		/* LED on for bit == 0	*/
# define STATUS_LED_BOOT	0		/* LED 0 used for boot status */

#endif

/*****  CMI   ********************************************************/
#elif defined(CONFIG_CMI)
# define STATUS_LED_DIR		im_mios.mios_mpiosm32ddr
# define STATUS_LED_DAT		im_mios.mios_mpiosm32dr

# define STATUS_LED_BIT		0x2000		/* Select one of the 16 possible*/
						/* MIOS outputs */
# define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 2)	/* Blinking periode is 500 ms */
# define STATUS_LED_STATE	STATUS_LED_BLINKING

# define STATUS_LED_ACTIVE	1		/* LED on for bit == 0	*/
# define STATUS_LED_BOOT	0		/* LED 0 used for boot status */

/*****  KUP4K, KUP4X  ****************************************************/
#elif defined(CONFIG_KUP4K) || defined(CONFIG_KUP4X)

# define STATUS_LED_PAR		im_ioport.iop_papar
# define STATUS_LED_DIR		im_ioport.iop_padir
# define STATUS_LED_ODR		im_ioport.iop_paodr
# define STATUS_LED_DAT		im_ioport.iop_padat

# define STATUS_LED_BIT		0x00000300  /*  green + red    PA[8]=yellow,  PA[7]=red,  PA[6]=green */
# define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE	STATUS_LED_BLINKING

# define STATUS_LED_ACTIVE	1		/* LED on for bit == 1	*/

# define STATUS_LED_BOOT	0		/* LED 0 used for boot status */

#elif defined(CONFIG_V38B)

# define STATUS_LED_BIT		0x0010			/* Timer7 GPIO */
# define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE	STATUS_LED_BLINKING

# define STATUS_LED_ACTIVE	0		/* LED on for bit == 0 */
# define STATUS_LED_BOOT	0		/* LED 0 used for boot status */

#elif defined(CONFIG_MOTIONPRO)

#define STATUS_LED_BIT		((vu_long *) MPC5XXX_GPT6_ENABLE)
#define STATUS_LED_PERIOD	(CONFIG_SYS_HZ / 10)
#define STATUS_LED_STATE	STATUS_LED_BLINKING

#define STATUS_LED_BIT1		((vu_long *) MPC5XXX_GPT7_ENABLE)
#define STATUS_LED_PERIOD1	(CONFIG_SYS_HZ / 10)
#define STATUS_LED_STATE1	STATUS_LED_OFF

#define STATUS_LED_BOOT		0	/* LED 0 used for boot status */




/* [YYF] extra add start ------------*/
/*****  EAIDK610  *********************************************************/
#elif defined(CONFIG_ROCKCHIP)

typedef unsigned long led_id_t;
/*
 * RK3399提供5组GPIO（GPIO0~GPIO4）一共122个，所有GPIO都可以作为中断

EAIDK610 一共有5个LED灯，定义如下：
gpio-leds {
	compatible = "gpio-leds";
	pinctrl-names = "default";
	pinctrl-0 = <0xd3>;

	led@1 {
		gpios = <0x3c 0x02 0x00>;  // gpios = <&gpio>
		label = "system_work_led1";
		retain-state-suspended;
	};

	led@2 {
		gpios = <0x3c 0x0c 0x00>;
		label = "system_work_led2";
		retain-state-suspended;
	};

	led@3 {
		gpios = <0x3c 0x0d 0x00>;
		label = "system_work_led3";
		retain-state-suspended;
	};

	led@4 {
		gpios = <0xcf 0x1b 0x00>;
		label = "wifi_work_led";
	};

	led@5 {
		gpios = <0xcf 0x1c 0x00>;
		label = "bt_work_led";
	};
};

leds: gpio-leds {
	compatible = "gpio-leds";
	pinctrl-names = "default";
	pinctrl-0 =<&leds_gpio>;

	led@1 {
		gpios = <&gpio0 2 GPIO_ACTIVE_HIGH>;
		label = "system_work_led1";
		retain-state-suspended;
	};

	led@2 {
		gpios = <&gpio0 12 GPIO_ACTIVE_HIGH>;
		label = "system_work_led2";
		retain-state-suspended;
	};

	led@3 {
		gpios = <&gpio0 13 GPIO_ACTIVE_HIGH>;
		label = "system_work_led3";
		retain-state-suspended;
	};

	led@4 {
		gpios = <&gpio2 27 GPIO_ACTIVE_HIGH>;
		label = "wifi_work_led";
	};

	led@5 {
		gpios = <&gpio2 28 GPIO_ACTIVE_HIGH>;
		label = "bt_work_led";
	};
};
 */



/* system_work_led1 蓝色 */
/* GPIO0_A2
 * 0*32+0*8+2
 * 属于第二组GPIO A2是2
 * 0x002中第三位是GPIO0组，02是组内编号，也就是0*8+2，十六进制0x2
 * */
# define STATUS_LED_BIT1           0x002
# define STATUS_LED_PERIOD1        (CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE1         STATUS_LED_OFF
/* system_work_led2 红色 */
/* GPIO0_B4
 * 0*32+1*8+4
 * 属于第二组GPIO B4是12
 * 0x00c中第三位是GPIO0组，0c是组内编号，也就是1*8+4=12，十六进制0xc
 * */
# define STATUS_LED_BIT2           0x00c
# define STATUS_LED_PERIOD2        (CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE2         STATUS_LED_OFF
/* system_work_led3 绿色 */
/* GPIO0_B5
 * 0*32 + 1*8 +5
 * 属于第二组GPIO B5是13
 * 0x00d中第三位是GPIO0组，0d是组内编号，也就是1*8+5=13，十六进制0xd
 * */
# define STATUS_LED_BIT3           0x00d
# define STATUS_LED_PERIOD3        (CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE3         STATUS_LED_OFF
/* wifi_work_led 橙色 */
/* GPIO2_D3
 * 2*32+3*8+3
 * 属于第二组GPIO D3是27
 * 0x21b中第三位是GPIO2组，1b是组内编号，也就是3*8+3=27，十六进制0x1b
 * */
# define STATUS_LED_BIT4           0x21b
# define STATUS_LED_PERIOD4        (CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE4         STATUS_LED_OFF
/* bt_work_led 蓝色 */
/* GPIO2_D4
 * 2*32 + 3*8 +4
 * 属于第二组GPIO D4是28
 * 0x21c中第三位是GPIO2组，1c是组内编号，也就是3*8+4=28，十六进制0x1c
 * */
# define STATUS_LED_BIT5           0x21c
# define STATUS_LED_PERIOD5        (CONFIG_SYS_HZ / 2)
# define STATUS_LED_STATE5         STATUS_LED_OFF

/* 默认高电平使能 */
# define STATUS_LED_ACTIVE          0 /* LED on for bit == 0 */
/* 默认开启第一个led灯 */
# define STATUS_LED_BOOT            0 /* 第0个(1个)用于boot状态灯 */

extern void __led_toggle (led_id_t mask);
extern int __led_get (led_id_t mask);
extern void __led_init (led_id_t mask, int state);
extern void __led_set (led_id_t mask, int state);

/* [YYF] extra add end ------------*/

#elif defined(CONFIG_BOARD_SPECIFIC_LED)
/* led_id_t is unsigned long mask */
typedef unsigned long led_id_t;

extern void __led_toggle (led_id_t mask);
extern void __led_init (led_id_t mask, int state);
extern void __led_set (led_id_t mask, int state);
#else
# error Status LED configuration missing
#endif
/************************************************************************/

#ifndef CONFIG_BOARD_SPECIFIC_LED
# include <asm/status_led.h>
#endif

#endif	/* CONFIG_STATUS_LED	*/

/*
 * Coloured LEDs API
 */
#ifndef	__ASSEMBLY__
void coloured_LED_init(void);
void red_led_on(void);
void red_led_off(void);
void green_led_on(void);
void green_led_off(void);
void yellow_led_on(void);
void yellow_led_off(void);
void blue_led_on(void);
void blue_led_off(void);
#else
	.extern LED_init
	.extern red_led_on
	.extern red_led_off
	.extern yellow_led_on
	.extern yellow_led_off
	.extern green_led_on
	.extern green_led_off
	.extern blue_led_on
	.extern blue_led_off
#endif

#endif	/* _STATUS_LED_H_	*/
