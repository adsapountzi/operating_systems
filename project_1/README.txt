 -----------------------------------------------------------------
| Eleni Xochelli                ||   2761  ||   xeleni@uth.gr     |
| Athanasia Despoina Sapountzi  ||   2624  ||   asapountzi@uth.gr |
| Iordanis Lilitsis             ||   2867  ||   liordanis@uth.gr  |
 -----------------------------------------------------------------

Part1:
	Compile roots.c root.h   : gcc -Wall -g -c roots.c -o roots.o
	Compile libroots.a       : ar rcs libroots.a roots.o
    Compile find_roots_lib.c : gcc -Wall -g find_roots_lib.c -o find_roots_lib -lroots -L. -static
    Run as ./find_roots_lib and expect the output at the tail of dmesg command.

Part2:
	Enter project1_module directory and press make to compile the module. 
	Then, insert the module as such: sudo insmod project1_kyber.ko

Part3:
	Enter sysfs_module directory and press make to compile the module. 
	Then, insert the module as such: sudo insmod part3.ko
