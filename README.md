

# error: redefinition of ‘fdt_property_cell’

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
/data/u-boot/tools/../include/libfdt.h:1199:19: note: previous definition of ‘fdt_property_cell’ with type ‘int(void *, const char *, uint32_t)’ {aka ‘int(void *, const char *, unsigned int)’}
 1199 | static inline int fdt_property_cell(void *fdt, const char *name, uint32_t val)
      |                   ^~~~~~~~~~~~~~~~~
/usr/include/libfdt.h:1908:19: error: redefinition of ‘fdt_appendprop_cell’
 1908 | static inline int fdt_appendprop_cell(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1538:19: note: previous definition of ‘fdt_appendprop_cell’ with type ‘int(void *, int,  const char *, uint32_t)’ {aka ‘int(void *, int,  const char *, unsigned int)’}
 1538 | static inline int fdt_appendprop_cell(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~~
/usr/include/libfdt.h:1677:19: error: redefinition of ‘fdt_setprop_u32’
 1677 | static inline int fdt_setprop_u32(void *fdt, int nodeoffset, const char *name,
      |                   ^~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1349:19: note: previous definition of ‘fdt_setprop_u32’ with type ‘int(void *, int,  const char *, uint32_t)’ {aka ‘int(void *, int,  const char *, unsigned int)’}
 1349 | static inline int fdt_setprop_u32(void *fdt, int nodeoffset, const char *name,
      |                   ^~~~~~~~~~~~~~~
/usr/include/libfdt.h:1890:19: error: redefinition of ‘fdt_appendprop_u64’
 1890 | static inline int fdt_appendprop_u64(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~
tools/../include/libfdt.h:1526:19: note: previous definition of ‘fdt_appendprop_u64’ with type ‘int(void *, int,  const char *, uint64_t)’ {aka ‘int(void *, int,  const char *, long unsigned int)’}
 1526 | static inline int fdt_appendprop_u64(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~
/usr/include/libfdt.h:1712:19: error: redefinition of ‘fdt_setprop_u64’
 1712 | static inline int fdt_setprop_u64(void *fdt, int nodeoffset, const char *name,
      |                   ^~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1384:19: note: previous definition of ‘fdt_setprop_u64’ with type ‘int(void *, int,  const char *, uint64_t)’ {aka ‘int(void *, int,  const char *, long unsigned int)’}
 1384 | static inline int fdt_setprop_u64(void *fdt, int nodeoffset, const char *name,
      |                   ^~~~~~~~~~~~~~~
/usr/include/libfdt.h:1908:19: error: redefinition of ‘fdt_appendprop_cell’
 1908 | static inline int fdt_appendprop_cell(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~~
tools/../include/libfdt.h:1538:19: note: previous definition of ‘fdt_appendprop_cell’ with type ‘int(void *, int,  const char *, uint32_t)’ {aka ‘int(void *, int,  const char *, unsigned int)’}
 1538 | static inline int fdt_appendprop_cell(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~~
/usr/include/libfdt.h:1730:19: error: redefinition of ‘fdt_setprop_cell’
 1730 | static inline int fdt_setprop_cell(void *fdt, int nodeoffset, const char *name,
      |                   ^~~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1396:19: note: previous definition of ‘fdt_setprop_cell’ with type ‘int(void *, int,  const char *, uint32_t)’ {aka ‘int(void *, int,  const char *, unsigned int)’}
 1396 | static inline int fdt_setprop_cell(void *fdt, int nodeoffset, const char *name,
      |                   ^~~~~~~~~~~~~~~~
/usr/include/libfdt.h:1855:19: error: redefinition of ‘fdt_appendprop_u32’
 1855 | static inline int fdt_appendprop_u32(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1491:19: note: previous definition of ‘fdt_appendprop_u32’ with type ‘int(void *, int,  const char *, uint32_t)’ {aka ‘int(void *, int,  const char *, unsigned int)’}
 1491 | static inline int fdt_appendprop_u32(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~
/usr/include/libfdt.h:1890:19: error: redefinition of ‘fdt_appendprop_u64’
 1890 | static inline int fdt_appendprop_u64(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1526:19: note: previous definition of ‘fdt_appendprop_u64’ with type ‘int(void *, int,  const char *, uint64_t)’ {aka ‘int(void *, int,  const char *, long unsigned int)’}
 1526 | static inline int fdt_appendprop_u64(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~
/usr/include/libfdt.h:1908:19: error: redefinition of ‘fdt_appendprop_cell’
 1908 | static inline int fdt_appendprop_cell(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~~
/data/u-boot/tools/../include/libfdt.h:1538:19: note: previous definition of ‘fdt_appendprop_cell’ with type ‘int(void *, int,  const char *, uint32_t)’ {aka ‘int(void *, int,  const char *, unsigned int)’}
 1538 | static inline int fdt_appendprop_cell(void *fdt, int nodeoffset,
      |                   ^~~~~~~~~~~~~~~~~~~

```
