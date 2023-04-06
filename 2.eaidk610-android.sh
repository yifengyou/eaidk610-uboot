#!/bin/sh

export PATH=/data/armbian/armbian.git/cache/toolchain/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu/bin:$PATH
#export PATH=/data/armbian/kernel_and_uboot/gcc-arm-11.2-2022.02-x86_64-aarch64-none-none-linux-gnu/bin:$PATH
#export PATH=/data/armbian/armbian.git/cache/toolchain/gcc-linaro-7.4.1-2019.02-x86_64_aarch64-none-linux-gnu:$PATH

make distclean

make ARCH=arm  CROSS_COMPILE="aarch64-linux-gnu-" rk3399_defconfig
#make ARCH=arm  CROSS_COMPILE="aarch64-none-linux-gnu-" rk3399_linux_defconfig
echo "make rk3399_linux_defconfig done! [$?]"

echo    "******************************"
echo    "*     Make AArch64 Uboot     *"
echo    "******************************"
#make ARCH=arm V=1 CROSS_COMPILE="aarch64-none-linux-gnu-" ARCHV=aarch64 --jobs=`sed -n "N;/processor/p" /proc/cpuinfo|wc -l`
#make ARCH=arm V=1 CROSS_COMPILE="aarch64-none-linux-gnu-" ARCHV=aarch64 --jobs=`nproc`
make ARCH=arm V=1 CROSS_COMPILE="aarch64-linux-gnu-" ARCHV=aarch64 --jobs=`nproc`
echo "make done! [$?]"

exit 0
