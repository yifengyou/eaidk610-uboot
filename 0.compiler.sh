#!/bin/bash

set -e

if [ ! -f ../gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc ] ; then
	# md5sum: c7e2ae4fd6a66df642d59e8453775b4c
	wget -c https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/8.3-2019.03/binrel/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu.tar.xz
	tar -vf gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu.tar.xz
	mv gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu ../
else
	echo "cross-compiler gcc already ok!"
fi

echo "All done!"
