# eaidk610-uboot

## quick start

0. setup compiler


```
# md5sum: c7e2ae4fd6a66df642d59e8453775b4c
wget -c https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/8.3-2019.03/binrel/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu.tar.xz
tar -xvf gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu.tar.xz

# don't put compiler in uboot src dir, make distclean will clean .a and etc file
mv gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu ../
```

1. build linux

```
./1.eaidk610-linux.sh
```

2. build android

```
./2.eaidk610-android.sh
```


