/*
 * (C) Copyright 2008-2015 Fuzhou Rockchip Electronics Co., Ltd
 * Peter, Software Engineering, <superpeter.cai@gmail.com>.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <version.h>
#include <errno.h>
#include <fastboot.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <power/pmic.h>

#include <asm/io.h>
#include <asm/arch/rkplat.h>

#include "../common/config.h"
#ifdef CONFIG_OPTEE_CLIENT
#include "../common/rkloader/attestation_key.h"
#endif

DECLARE_GLOBAL_DATA_PTR;

void __weak rkclk_set_apll_high(void)
{

}

static ulong get_sp(void)
{
	ulong ret;

	asm("mov %0, sp" : "=r"(ret) : );
	return ret;
}

void board_lmb_reserve(struct lmb *lmb) {
	ulong sp;
	sp = get_sp();
	debug("## Current stack ends at 0x%08lx ", sp);

	/* adjust sp by 64K to be safe */
	sp -= 64<<10;
	lmb_reserve(lmb, sp,
			gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size - sp);

	//reserve 48M for kernel & 8M for nand api.
	lmb_reserve(lmb, gd->bd->bi_dram[0].start, CONFIG_LMB_RESERVE_SIZE);
}

int board_storage_init(void)
{
	debug("[YYF] %s:%s:%d\n", __FILE__, __func__, __LINE__);
	if (StorageInit() == 0) {
		puts("storage init OK!\n");
		return 0;
	} else {
		puts("storage init fail!\n");
		return -1;
	}
}


/*****************************************
 * Routine: board_init
 * Description: Early hardware init.
 *****************************************/
int board_init(void)
{
	/* Set Initial global variables */

	gd->bd->bi_arch_number = MACH_TYPE_RK30XX;
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x88000;

	return 0;
}


#ifdef CONFIG_DISPLAY_BOARDINFO
/**
 * Print board information
 */
int checkboard(void)
{
	puts("Board:\tRockchip platform Board\n");
#ifdef CONFIG_SECOND_LEVEL_BOOTLOADER
	printf("Uboot as second level loader\n");
#endif
	return 0;
}
#endif


#ifdef CONFIG_ARCH_EARLY_INIT_R
int arch_early_init_r(void)
{
	debug("arch_early_init_r\n");

	 /* set up exceptions */
	interrupt_init();
	/* enable exceptions */
	enable_interrupts();

	/* rk pl330 dmac init */
#ifdef CONFIG_RK_PL330_DMAC
	rk_pl330_dmac_init_all();
#endif /* CONFIG_RK_PL330_DMAC */

#ifdef CONFIG_RK_MCU
	rk_mcu_init();
#endif

	return 0;
}
#endif


#define RAMDISK_ZERO_COPY_SETTING	"0xffffffffffffffff=n\0"
static void board_init_adjust_env(void)
{
	bool change = false;
	
	debug("[YYF] %s:%s:%d\n", __FILE__, __func__, __LINE__);
	
	char *s = getenv("bootdelay");
	if (s != NULL) {
		unsigned long bootdelay = 0;

		bootdelay = simple_strtoul(s, NULL, 16);
		debug("getenv: bootdelay = %lu\n", bootdelay);
#if (CONFIG_BOOTDELAY <= 0)
		if (bootdelay > 0) {
			setenv("bootdelay", simple_itoa(0));
			change = true;
			debug("setenv: bootdelay = 0\n");
		}
#else
		if (bootdelay != CONFIG_BOOTDELAY) {
			setenv("bootdelay", simple_itoa(CONFIG_BOOTDELAY));
			change = true;
			debug("setenv: bootdelay = %d\n", CONFIG_BOOTDELAY);
		}
#endif
	}

	s = getenv("bootcmd");
	if (s != NULL) {
		debug("getenv: bootcmd = %s\n", s);
		if (strcmp(s, CONFIG_BOOTCOMMAND) != 0) {
			setenv("bootcmd", CONFIG_BOOTCOMMAND);
			change = true;
			debug("setenv: bootcmd = %s\n", CONFIG_BOOTCOMMAND);
		}
	}

	s = getenv("initrd_high");
	if (s != NULL) {
		debug("getenv: initrd_high = %s\n", s);
		if (strcmp(s, RAMDISK_ZERO_COPY_SETTING) != 0) {
			setenv("initrd_high", RAMDISK_ZERO_COPY_SETTING);
			change = true;
			debug("setenv: initrd_high = %s\n", RAMDISK_ZERO_COPY_SETTING);
		}
	}

	if (change) {
#ifdef CONFIG_CMD_SAVEENV
		debug("board init saveenv.\n");
		saveenv();
#endif
	}
}

#define CONFIG_GPIO_LED_INVERTED_TABLE {}
static led_id_t gpio_led_inv[] = CONFIG_GPIO_LED_INVERTED_TABLE;

static int gpio_led_gpio_value(led_id_t mask, int state)
{
	int i, gpio_value = (state == STATUS_LED_ON);
	
	for (i = 0; i < ARRAY_SIZE(gpio_led_inv); i++) {
		if (gpio_led_inv[i] == mask)
			gpio_value = !gpio_value;
	}
	return gpio_value;
}

void __led_init(led_id_t mask, int state)
{
	int gpio_value;
	if (gpio_request(mask, "gpio_led") != 0) {
			printf("%s: failed requesting GPIO%lu!\n", __func__, mask);
			return;
	}

//	gpio_value = gpio_led_gpio_value(mask, state);
	gpio_direction_output(mask, state);
	debug("[YYF] %s:%s:%d gpio:0x%x state:0x%x\n", __FILE__, __func__, __LINE__, mask, state);
}

void __led_set(led_id_t mask, int state)
{
	gpio_set_value(mask, state);
	debug("[YYF] %s:%s:%d gpio:0x%x set state:0x%x\n", __FILE__, __func__, __LINE__, mask, state);
}

int __led_get(led_id_t mask)
{
	return gpio_get_value(mask);
}

void __led_toggle(led_id_t mask)
{
	gpio_set_value(mask, !gpio_get_value(mask));
	debug("[YYF] %s:%s:%d gpio:0x%x set state:0x%x\n", __FILE__, __func__, __LINE__, mask, !gpio_get_value(mask));
}

#ifdef CONFIG_BOARD_LATE_INIT
extern char bootloader_ver[24];
int board_late_init(void)
{
	debug("[YYF] %s:%s:%d\n", __FILE__, __func__, __LINE__);

	board_init_adjust_env();
	
	load_disk_partitions();

#ifdef CONFIG_RK_PWM_REMOTE
        RemotectlInit();
#endif

	rkimage_prepare_fdt();

#ifdef CONFIG_RK_KEY
	key_init();
#endif

#ifdef CONFIG_RK_POWER
	debug("fixed_init\n");
	fixed_regulator_init();
	debug("pmic_init\n");
	pmic_init(0);
#if defined(CONFIG_POWER_PWM_REGULATOR)
	debug("pwm_regulator_init\n");
	pwm_regulator_init();
#endif
	rkclk_set_apll_high();
	rkclk_dump_pll();
	debug("fg_init\n");
	fg_init(0); /*fuel gauge init*/
	debug("charger init\n");
	plat_charger_init();
#endif /* CONFIG_RK_POWER */

#ifdef CONFIG_OPTEE_CLIENT
       load_attestation_key();
#endif

	debug("idb init\n");
	//TODO:set those buffers in a better way, and use malloc?
	rkidb_setup_space(gd->arch.rk_global_buf_addr);

	/* after setup space, get id block data first */
	rkidb_get_idblk_data();

	/* Secure boot check after idb data get */
	SecureBootCheck();

	if (rkidb_get_bootloader_ver() == 0) {
		printf("\n#Boot ver: %s\n", bootloader_ver);
	}

	char tmp_buf[32];
	/* rk sn size 30bytes, zero buff */
	memset(tmp_buf, 0, 32);
	if (rkidb_get_sn(tmp_buf)) {
		setenv("fbt_sn#", tmp_buf);
	}

	debug("fbt preboot\n");
	board_fbt_preboot();

	return 0;
}
#endif

#ifdef CONFIG_CMD_NET
/*
 * Initializes on-chip ethernet controllers.
 * to override, implement board_eth_init()
 */
int board_eth_init(bd_t *bis)
{
	__maybe_unused int rc;

	debug("board_eth_init\n");

#ifdef CONFIG_RK_GMAC
	char macaddr[6];
	char ethaddr[20];
	char *env_str = NULL;

	memset(ethaddr, sizeof(ethaddr), 0);
	env_str = getenv("ethaddr");
	if (rkidb_get_mac_address(macaddr) == true) {
		sprintf(ethaddr, "%02X:%02X:%02X:%02X:%02X:%02X",
			macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

		printf("mac address: %s\n", ethaddr);

		if (env_str == NULL)
			setenv ((char *)"ethaddr", (char *)ethaddr);
		else if (strncmp(env_str, ethaddr, strlen(ethaddr)) != 0)
			setenv ((char *)"ethaddr", (char *)ethaddr);
	} else {
		uint16_t v;

		v = (rand() & 0xfeff) | 0x0200;
		macaddr[0] = (v >> 8) & 0xff;
		macaddr[1] = v & 0xff;
		v = rand();
		macaddr[2] = (v >> 8) & 0xff;
		macaddr[3] = v & 0xff;
		v = rand();
		macaddr[4] = (v >> 8) & 0xff;
		macaddr[5] = v & 0xff;

		sprintf(ethaddr, "%02X:%02X:%02X:%02X:%02X:%02X",
			macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);

		if (env_str == NULL) {
			printf("mac address: %s\n", ethaddr);
			setenv ((char *)"ethaddr", (char *)ethaddr);
		} else {
			printf("mac address: %s\n", env_str);
		}
	}

	rc = rk_gmac_initialize(bis);
	if (rc < 0) {
		printf("rockchip: failed to initialize gmac\n");
		return rc;
	}
#endif /* CONFIG_RK_GMAC */

	return 0;
}
#endif

#ifdef CONFIG_ROCKCHIP_DISPLAY
extern void rockchip_display_fixup(void *blob);
#endif

#if defined(CONFIG_RK_DEVICEINFO)
extern bool g_is_devinfo_load;
#endif

#if defined(CONFIG_OF_LIBFDT) && defined(CONFIG_OF_BOARD_SETUP)
void ft_board_setup(void *blob, bd_t * bd)
{
#ifdef CONFIG_ROCKCHIP_DISPLAY
	rockchip_display_fixup(blob);
#endif
#ifdef CONFIG_ROCKCHIP
#if defined(CONFIG_LCD) || defined(CONFIG_VIDEO)
	u64 start, size;
	int offset;

	if (!gd->uboot_logo)
		return;

	start = gd->fb_base;
	offset = gd->fb_offset;
	if (offset > 0)
		size = CONFIG_RK_LCD_SIZE;
	else
		size = CONFIG_RK_FB_SIZE;

	fdt_update_reserved_memory(blob, "rockchip,fb-logo", start, size);

#if defined(CONFIG_RK_DEVICEINFO)
	if (g_is_devinfo_load)
		fdt_update_reserved_memory(blob, "rockchip,stb-devinfo",
					   CONFIG_RKHDMI_PARAM_ADDR,
					   SZ_8K);
#endif

#endif
#endif
}
#endif
