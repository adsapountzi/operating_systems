# include <linux/module.h>
# include <linux/printk.h>
# include <linux/kobject.h>
# include <linux/sysfs.h>
# include <linux/init.h>
# include <linux/fs.h>
# include <linux/string.h>
# include <linux/kernel.h>
# include  <linux/sched.h>
#include <linux/syscalls.h>

long find_roots(void){
    long int id;
	struct task_struct* curr;

	curr = current;
    id = curr->pid;

	printk("find_roots system call called by process %ld\n", (long)curr->pid);

	for (; curr->pid != 1; curr = curr->real_parent){
		printk("id: %ld, name: %s\n", (long)curr->pid, curr->comm);
	}
	printk("id: %ld, name: %s\n", (long)curr->pid, curr->comm);

	return id;
}

static struct kobject *example_kobject;
volatile int roots = 0;

static ssize_t  foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
    

    return sprintf(buf ,"%ld\n", find_roots());
}

struct kobj_attribute foo_attribute = __ATTR( foo , 0660 , foo_show, NULL);

static int __init mymodule_init (void) {
    int error = 0;
    example_kobject = kobject_create_and_add("kobject_example",kernel_kobj);
    if( !example_kobject )
        return -ENOMEM;

    error = sysfs_create_file(example_kobject, &foo_attribute.attr);
    
    if(error) {
        printk("failed to create the foo file in /sys/kernel/kobject_example\n");
    }
    return error;
}

static void __exit mymodule_exit(void){
    printk("Module uninitialized successfully\n");
    kobject_put(example_kobject);
}

module_init(mymodule_init);
module_exit(mymodule_exit);
MODULE_LICENSE("GPL");
