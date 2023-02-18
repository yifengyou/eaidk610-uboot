#!/bin/bash

set -xe

export PS1='\[\e[32;1m\][\[\e[31;1m\]\u\[\e[33;1m\]@\[\e[35;1m\]\h\[\e[36;1m\] \w\[\e[32;1m\]]\[\e[37;1m\]\$\[\e[0m\] '
alias egrep='egrep --color=auto'
alias fgrep='fgrep --color=auto'
alias grep='grep --color=auto'
alias l.='ls -d .* -a --color=auto'
alias ll='ls -l -h -a --color=auto'
alias ls='ls -a --color=auto'
alias cp='cp -i'
alias mv='mv -i'
alias rm='rm -i'
alias xzegrep='xzegrep --color=auto'
alias xzfgrep='xzfgrep --color=auto'
alias xzgrep='xzgrep --color=auto'
alias zegrep='zegrep --color=auto'
alias zfgrep='zfgrep --color=auto'
alias zgrep='zgrep --color=auto'
alias push='git push'



if [ ! -f /.dockerenv ] ; then
	echo "not in docker env"
	exit 1
fi


rm -f uboot.img idbloader.bin trust.bin || : 

export CCACHE_BASEDIR=/root/armbian/cache/sources/u-boot/v2022.07 
export PATH=/root/armbian/cache/toolchain/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin::/usr/lib/ccache:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
make distclean
echo " # yifengyou: distclean ok"

echo ""
make -j24 eaidk-610-rk3399_defconfig CROSS_COMPILE="ccache aarch64-none-linux-gnu-"
echo "ret=[$?]"
echo " # yifengyou: make eaidk-610-rk3399_defconfig ok"
echo ""

TIMESTAMP=`date +"%Y%m%d%H%M%S"`
sed -i "s#CONFIG_LOCALVERSION=\"\"#CONFIG_LOCALVERSION=\"-armbian_yyf_${TIMESTAMP}\"#g" .config
sed -i 's/CONFIG_LOCALVERSION_AUTO=.*/# CONFIG_LOCALVERSION_AUTO is not set/g' .config
sed -i "s/^CONFIG_BOOTDELAY=.*/CONFIG_BOOTDELAY=30/" .config

make u-boot-dtb.bin -j24 CROSS_COMPILE="ccache aarch64-none-linux-gnu-"
echo "ret=[$?]"
echo " # yifengyou: make u-boot-dtb.bin ok"
echo ""


# yifengyou: rockchip64_common.inc [/root/armbian/cache/sources/u-boot/v2022.07]
tools/mkimage -n rk3399 -T rksd -d /root/armbian/cache/sources/rkbin-tools/rk33/rk3399_ddr_933MHz_v1.25.bin idbloader.bin
#Image Type:   Rockchip RK33 (SD/MMC) boot image
#Init Data Size: 131072 bytes

# yifengyou: rockchip64_common.inc [/root/armbian/cache/sources/u-boot/v2022.07]
cat /root/armbian/cache/sources/rkbin-tools/rk33/rk3399_miniloader_v1.26.bin >> idbloader.bin

# yifengyou: rockchip64_common.inc [/root/armbian/cache/sources/u-boot/v2022.07]
/root/armbian/cache/sources/rkbin-tools/tools/loaderimage --pack --uboot ./u-boot-dtb.bin uboot.img 0x200000

#load addr is 0x200000!
#pack input ./u-boot-dtb.bin 
#pack file size: 625912 
#crc = 0x5c52ec76
#pack uboot.img success! 

# yifengyou: rockchip64_common.inc [/root/armbian/cache/sources/u-boot/v2022.07]
/root/armbian/cache/sources/rkbin-tools/tools/trust_merger --replace bl31.elf /root/armbian/cache/sources/rkbin-tools/rk33/rk3399_bl31_v1.35.elf trust.ini
#out:trust.bin
#merge success(trust.bin)

echo "now `date`"
ls -alh uboot.img idbloader.bin trust.bin
md5sum uboot.img idbloader.bin trust.bin


echo "All done!"


