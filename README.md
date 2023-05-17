# eaidk610-uboot



## QA

### error: redefinition of ‘fdt_property_cell’

Q:

```
      |                   ^~~~~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1526:19: note: previous definition of ‘fdt_appendprop_u64’ with type ‘int(void *, int,  const char *, uint64_t)’ {aka ‘int(void *, int,  const char *, long unsigned int)’}
 1526 | static inline int fdt_appendprop_u64(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~
make[1]: *** [scripts/Makefile.host:134: tools/ublimage.o] Error 1
make[1]: *** [scripts/Makefile.host:134: tools/common/bootm.o] Error 1
/usr/include/libfdt.h:1478:19: error: redefinition of ‘fdt_property_cell’
 1478 | static inline int fdt_property_cell(void *fdt, const char *name, uint32_t val)
      |                   ^~~~~~~~~~~~~~~~~
```

A:

```
sudo apt-get purge -y libfdt-dev 
```

* <https://github.com/hardkernel/u-boot/issues/73>


### 














