#!/bin/sh

case $1 in
	android)
		make rk3399_defconfig
		./mkv8.sh
		;;
	linux)
		make rk3399_linux_defconfig
		./mkv8.sh
		;;
	*)
		echo please select build target: android or linux
		;;
esac

