#!/bin/sh

set -e

export PATH=`realpath ../gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu/bin`:$PATH
echo $PATH

CURRENTDIR=`pwd`
echo "CURRENTDIR: ${CURRENTDIR}"
BUILDDIR=${CURRENTDIR}-build-linux
echo "BUILDDIR: ${BUILDDIR}"
if [ -d ${BUILDDIR} ]; then
	rm -rf ${BUILDDIR}
fi
mkdir -p ${BUILDDIR}

echo    "******************************"
echo    "*     Clean Uboot Config     *"
echo    "******************************"
make distclean
echo " * make distclean done! [$?]"

echo    "******************************"
echo    "*     Make Uboot Config      *"
echo    "******************************"
make O=${BUILDDIR} ARCH=arm  CROSS_COMPILE="aarch64-linux-gnu-" rk3399_linux_defconfig
echo " * make rk3399_linux_defconfig done! [$?]"

echo    "******************************"
echo    "*     Make AArch64 Uboot     *"
echo    "******************************"
make O=${BUILDDIR} ARCH=arm V=1 CROSS_COMPILE="aarch64-linux-gnu-" ARCHV=aarch64 --jobs=`nproc`
echo " * make done! [$?]"

echo " * All done!"
