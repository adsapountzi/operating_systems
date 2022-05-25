#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/mm.h>

SYSCALL_DEFINE1(my_kfree, void*, ptr){
	return (kfree(ptr));
}