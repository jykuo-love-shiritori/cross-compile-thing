/* SPDX-License-Identifier: WTFPL */

#include "shrek.h"

/*
 * pr_fmt - used by the pr_*() macros to generate the printk format string
 * docs: https://github.com/torvalds/linux/blame/fc4354c6e5c21257cf4a50b32f7c11c7d65c55b3/include/linux/printk.h#L331-L347
 */
#define pr_fmt(fmt) MODULE_NAME ": " fmt
#define BUF_SIZE 64

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>             /* module_init, module_exit         */

#include <linux/fs.h>               /* file_operations */
// #include <linux/cdev.h>
// #include <linux/device.h>
// #include <linux/kdev_t.h>
// #include <linux/mutex.h>

#include <asm/uaccess.h>
//#include <linux/uaccess.h> /* copy_from_user on TX2 */

#include "gpio.h"

MODULE_LICENSE("WTFPL");
MODULE_DESCRIPTION("Shrek module for Web Designing Lab");
MODULE_AUTHOR("Team Leader, Group 15");
MODULE_VERSION("0.69");

#define PIN0 396
#define PIN1 392
#define PIN2 255
#define PIN3 254

#define TEXT_SIZE 1024

//static DEFINE_MUTEX(shrek_mutex);
static char shrek_text[TEXT_SIZE];
static unsigned int shrek_count = 0;
static loff_t lednum;

static int drv_open(struct inode *inode, struct file *file)
{
	pr_debug("open called");
//	if (!mutex_trylock(&shrek_mutex)) {
//		pr_alert("shrekdrv is in use");
//		return -EBUSY;
//	}
	return 0;
}

static int drv_release(struct inode *inode, struct file *file)
{
	pr_debug("release called");
//	mutex_unlock(&shrek_mutex);
	return 0;
}

static ssize_t drv_read(struct file *file, char *_user_buf, size_t size, loff_t *offset)
{
	int ret = 0;
	char value[BUF_SIZE];

	pr_debug("read called");
	if ( ! gpio_read_value(lednum, value) ) {
		pr_err("gpio read error");
		return -1;
	}
	ret = copy_to_user(_user_buf, value, strlen(value));
	if( ret != 0 ) {
		pr_err("read: copy error");
		return -1;
	}

	return 0;
}

static ssize_t drv_write(struct file *file, const char *_user_buf, size_t size, loff_t *offset)
{
	int ret = 0;
	char buf[BUF_SIZE];

	pr_debug("write called");
	ret = copy_from_user(buf, _user_buf, size);
	if( ret != 0 ) {
		pr_err("write: copy error");
	}
	return gpio_set_value(lednum, buf);
}

/* llseek operation is skipped */
static loff_t drv_device_lseek(struct file *file, loff_t offset, int orig)
{
	pr_debug("lseek LED %d", (int)offset);
	switch(offset) {
		case 0: lednum = PIN0; break;
		case 1: lednum = PIN1; break;
		case 2: lednum = PIN2; break;
		case 3: lednum = PIN3; break;
		default:
			lednum = offset;
			break;
	}
	return lednum;
}

/*
 * The file_operations Structure : Character Device Drivers
 * https://tldp.org/LDP/lkmpg/2.4/html/c577.htm
 */
const struct file_operations shrek_fops = {
	.owner = THIS_MODULE,
	.read = drv_read,
	.write = drv_write,
	.open = drv_open,
	.release = drv_release,
	.llseek = drv_device_lseek,
};

static int __init sexy_shrek_init(void)
{
	int rc = 0;
	gpio_export(PIN3);
	gpio_export(PIN2);
	gpio_export(PIN1);
	gpio_export(PIN0);
	gpio_set_dir(PIN3, "out");
	gpio_set_dir(PIN2, "out");
	gpio_set_dir(PIN1, "out");
	gpio_set_dir(PIN0, "out");
	gpio_set_value(PIN3, "1");
	gpio_set_value(PIN2, "1");
	gpio_set_value(PIN1, "1");
	gpio_set_value(PIN0, "1");
	//mutex_init(&shrek_mutex);

    // Let's register the device
    // This will dynamically allocate the major number
	//rc = alloc_chrdev_region(&shrek_dev, MAJOR_NUM, MINOR_NUM, DEV_SHREK_NAME);
	rc =  register_chrdev(MAJOR_NUM, MODULE_NAME, &shrek_fops);
	if (rc < 0) {
		pr_alert("Failed to register the shrek char device. rc = %i",
		       rc);
		return rc;
	}

	pr_info("iM iN\n");
	//pr_info("but this is debug\n");
	return rc;
}

static void __exit sexy_shrek_exit(void)
{
	gpio_set_value(PIN3, "0");
	gpio_set_value(PIN2, "0");
	gpio_set_value(PIN1, "0");
	gpio_set_value(PIN0, "0");
	gpio_unexport(PIN3);
	gpio_unexport(PIN2);
	gpio_unexport(PIN1);
	gpio_unexport(PIN0);
	//mutex_destroy(&shrek_mutex);
    	//device_destroy(shrek_class, shrek_dev);
    	//class_destroy(shrek_class);
    	//cdev_del(shrek_cdev);
	//unregister_chrdev_region(shrek_dev, 1);
	unregister_chrdev(MAJOR_NUM, MODULE_NAME);
	pr_info("Fxck this shit im out\n");
}

module_init(sexy_shrek_init);
module_exit(sexy_shrek_exit);

// https://stackoverflow.com/questions/40012229/opening-a-device-file-fails-due-to-no-such-device-or-address

