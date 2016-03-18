#include <linux/init.h>
#include <linux/module.h>


static int __init dedupfs_init(void)
{
	printk(KERN_INFO "Hello man!\n");
	return 0;
}


static void __exit dedupfs_exit(void)
{
	printk(KERN_INFO "Goodbye, man!\n");
}

module_init(dedupfs_init);
module_exit(dedupfs_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Shalymov");


