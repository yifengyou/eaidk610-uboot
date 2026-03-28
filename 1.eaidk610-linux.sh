#!/bin/bash

set -xe

export PATH=`realpath gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu/bin`:$PATH
echo $PATH

make Q= ARCH=arm  CROSS_COMPILE="aarch64-linux-gnu-" rk3399_linux_defconfig
echo " * make rk3399_linux_defconfig done! [$?]"

make Q= ARCH=arm V=1 CROSS_COMPILE="aarch64-linux-gnu-" ARCHV=aarch64 --jobs=`nproc`

echo " * All done!"
