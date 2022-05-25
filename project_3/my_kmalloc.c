#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/mm.h>

SYSCALL_DEFINE1(my_kmalloc, size_t, size){
	return (kmalloc(size, GFP_KERNEL));
}