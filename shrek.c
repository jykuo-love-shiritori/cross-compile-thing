#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#define pr_fmt(fmt) KBUILD_MODNAME “: ” fmt

#define MODULE_NAME "sexy_shrek"
#define MAJOR_NUM 69

MODULE_DESCRIPTION("My kernel module");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

#define REMOVE_DEVICE

static int __init sexy_shrek_init(void)
{
#ifndef REMOVE_DEVICE
	if ( register_chrdev(MAJOR_NUM, MODULE_NAME, &drv_fops) < 0 ) {
		printk("<1>%s: can't get major %d\n", MODULE_NAME, MAJOR_NUM);
		return (-EBUSY);
	}
	printk("<1>%s: iM iN\n", MODULE_NAME);
#endif /* REMOVE_DEVICE */
	return 0;
}

static void __exit sexy_shrek_exit(void)
{
#ifndef REMOVE_DEVICE
	unregister_chrdev(MAJOR_NUM, MODULE_NAME);
#endif /* REMOVE_DEVICE */
	printk("<1>%s: Fxck this shit im out\n", MODULE_NAME);
}

module_init(sexy_shrek_init);
module_exit(sexy_shrek_exit);

//struct file_operations drv_fops = {
//	read: drv_read,
//	write: drv_read,
//	unlocked: drv_read,
//	open: drv_read,
//	release: drv_read,
//}

