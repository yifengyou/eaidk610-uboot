// SPDX-License-Identifier: GPL-2.0+
/*
 * eFuse driver for Rockchip devices
 *
 * Copyright 2017, Theobroma Systems Design und Consulting GmbH
 * Written by Philipp Tomsich <philipp.tomsich@theobroma-systems.com>
 */

#include <common.h>
#include <asm/io.h>
#include <command.h>
#include <display_options.h>
#include <dm.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <malloc.h>
#include <misc.h>

#define RK3328_INT_STATUS	0x0018
#define RK3328_DOUT		0x0020
#define RK3328_AUTO_CTRL	0x0024
#define RK3328_INT_FINISH	BIT(0)
#define RK3328_AUTO_ENB		BIT(0)
#define RK3328_AUTO_RD		BIT(1)
#define RK3328_NO_SECURE_BYTES	32
#define RK3328_SECURE_BYTES	96

#define RK3399_A_SHIFT          16
#define RK3399_A_MASK           0x3ff
#define RK3399_NFUSES           32
#define RK3399_BYTES_PER_FUSE   4
#define RK3399_STROBSFTSEL      BIT(9)
#define RK3399_RSB              BIT(7)
#define RK3399_PD               BIT(5)
#define RK3399_PGENB            BIT(3)
#define RK3399_LOAD             BIT(2)
#define RK3399_STROBE           BIT(1)
#define RK3399_CSB              BIT(0)

typedef int (*EFUSE_READ)(struct udevice *dev, int offset, void *buf,
			  int size);

struct rockchip_efuse_regs {
	u32 ctrl;      /* 0x00  efuse control register */
	u32 dout;      /* 0x04  efuse data out register */
	u32 rf;        /* 0x08  efuse redundancy bit used register */
	u32 _rsvd0;
	u32 jtag_pass; /* 0x10  JTAG password */
	u32 strobe_finish_ctrl;
		       /* 0x14	efuse strobe finish control register */
	u32 int_status;/* 0x18 */
	u32 reserved;  /* 0x1c */
	u32 dout2;     /* 0x20 */
	u32 auto_ctrl; /* 0x24 */
};

struct rockchip_efuse_plat {
	void __iomem *base;
	struct clk *clk;
};

#if defined(DEBUG)
static int dump_efuses(struct cmd_tbl *cmdtp, int flag,
		       int argc, char *const argv[])
{
	/*
	 * N.B.: This function is tailored towards the RK3399 and assumes that
	 *       there's always 32 fuses x 32 bits (i.e. 128 bytes of data) to
	 *       be read.
	 */

	struct udevice *dev;
	u8 fuses[128];
	int ret;

	/* retrieve the device */
	ret = uclass_get_device_by_driver(UCLASS_MISC,
					  DM_DRIVER_GET(rockchip_efuse), &dev);
	if (ret) {
		printf("%s: no misc-device found\n", __func__);
		return 0;
	}

	ret = misc_read(dev, 0, &fuses, sizeof(fuses));
	if (ret < 0) {
		printf("%s: misc_read failed\n", __func__);
		return 0;
	}

	printf("efuse-contents:\n");
	print_buffer(0, fuses, 1, 128, 16);

	return 0;
}

U_BOOT_CMD(
	rk3399_dump_efuses, 1, 1, dump_efuses,
	"Dump the content of the efuses",
	""
);
#endif

static int rockchip_rk3328_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct rockchip_efuse_plat *plat = dev_get_plat(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;
	unsigned int addr_start, addr_end, addr_offset, addr_len;
	u32 out_value, status;
	u8 *buffer;
	int ret = 0, i = 0, j = 0;

	/* Max non-secure Byte */
	if (size > RK3328_NO_SECURE_BYTES)
		size = RK3328_NO_SECURE_BYTES;

	/* 128 Byte efuse, 96 Byte for secure, 32 Byte for non-secure */
	offset += RK3328_SECURE_BYTES;
	addr_start = rounddown(offset, RK3399_BYTES_PER_FUSE) /
			       RK3399_BYTES_PER_FUSE;
	addr_end = roundup(offset + size, RK3399_BYTES_PER_FUSE) /
			   RK3399_BYTES_PER_FUSE;
	addr_offset = offset % RK3399_BYTES_PER_FUSE;
	addr_len = addr_end - addr_start;

	buffer = calloc(1, sizeof(*buffer) * addr_len * RK3399_BYTES_PER_FUSE);
	if (!buffer)
		return -ENOMEM;

	for (j = 0; j < addr_len; j++) {
		writel(RK3328_AUTO_RD | RK3328_AUTO_ENB |
		       ((addr_start++ & RK3399_A_MASK) << RK3399_A_SHIFT),
		       &efuse->auto_ctrl);
		udelay(5);
		status = readl(&efuse->int_status);
		if (!(status & RK3328_INT_FINISH)) {
			ret = -EIO;
			goto err;
		}
		out_value = readl(&efuse->dout2);
		writel(RK3328_INT_FINISH, &efuse->int_status);

		memcpy(&buffer[i], &out_value, RK3399_BYTES_PER_FUSE);
		i += RK3399_BYTES_PER_FUSE;
	}
	memcpy(buf, buffer + addr_offset, size);
err:
	free(buffer);

	return ret;
}

static int rockchip_rk3399_efuse_read(struct udevice *dev, int offset,
				      void *buf, int size)
{
	struct rockchip_efuse_plat *plat = dev_get_plat(dev);
	struct rockchip_efuse_regs *efuse =
		(struct rockchip_efuse_regs *)plat->base;

	unsigned int addr_start, addr_end, addr_offset;
	u32 out_value;
	u8  bytes[RK3399_NFUSES * RK3399_BYTES_PER_FUSE];
	int i = 0;
	u32 addr;

	addr_start = offset / RK3399_BYTES_PER_FUSE;
	addr_offset = offset % RK3399_BYTES_PER_FUSE;
	addr_end = DIV_ROUND_UP(offset + size, RK3399_BYTES_PER_FUSE);

	/* cap to the size of the efuse block */
	if (addr_end > RK3399_NFUSES)
		addr_end = RK3399_NFUSES;

	writel(RK3399_LOAD | RK3399_PGENB | RK3399_STROBSFTSEL | RK3399_RSB,
	       &efuse->ctrl);
	udelay(1);
	for (addr = addr_start; addr < addr_end; addr++) {
		setbits_le32(&efuse->ctrl,
			     RK3399_STROBE | (addr << RK3399_A_SHIFT));
		udelay(1);
		out_value = readl(&efuse->dout);
		clrbits_le32(&efuse->ctrl, RK3399_STROBE);
		udelay(1);

		memcpy(&bytes[i], &out_value, RK3399_BYTES_PER_FUSE);
		i += RK3399_BYTES_PER_FUSE;
	}

	/* Switch to standby mode */
	writel(RK3399_PD | RK3399_CSB, &efuse->ctrl);

	memcpy(buf, bytes + addr_offset, size);

	return 0;
}

static int rockchip_efuse_read(struct udevice *dev, int offset,
			       void *buf, int size)
{
	EFUSE_READ efuse_read = NULL;

	efuse_read = (EFUSE_READ)dev_get_driver_data(dev);
	if (!efuse_read)
		return -EINVAL;

	return (*efuse_read)(dev, offset, buf, size);
}

static const struct misc_ops rockchip_efuse_ops = {
	.read = rockchip_efuse_read,
};

static int rockchip_efuse_of_to_plat(struct udevice *dev)
{
	struct rockchip_efuse_plat *plat = dev_get_plat(dev);

	plat->base = dev_read_addr_ptr(dev);
	return 0;
}

static const struct udevice_id rockchip_efuse_ids[] = {
	{
		.compatible = "rockchip,rk3328-efuse",
		.data = (ulong)&rockchip_rk3328_efuse_read,
	},
	{
		.compatible = "rockchip,rk3399-efuse",
		.data = (ulong)&rockchip_rk3399_efuse_read,
	},
	{}
};

U_BOOT_DRIVER(rockchip_efuse) = {
	.name = "rockchip_efuse",
	.id = UCLASS_MISC,
	.of_match = rockchip_efuse_ids,
	.of_to_plat = rockchip_efuse_of_to_plat,
	.plat_auto	= sizeof(struct rockchip_efuse_plat),
	.ops = &rockchip_efuse_ops,
};
