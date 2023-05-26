/* SPDX-License-Identifier: WTFPL */

#include "shrek.h"

/*
 * pr_fmt - used by the pr_*() macros to generate the printk format string
 * docs: https://github.com/torvalds/linux/blame/fc4354c6e5c21257cf4a50b32f7c11c7d65c55b3/include/linux/printk.h#L331-L347
 */
#define pr_fmt(fmt) MODULE_NAME ": " fmt
#define DEBUG

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>             /* module_init, module_exit         */

#include <linux/fs.h>               /* file_operations */
#include <linux/cdev.h>
// #include <linux/device.h>
// #include <linux/kdev_t.h>
// #include <linux/mutex.h>

// #include <asm/uaccess.h>
#include <linux/uaccess.h> /* copy_from_user on TX2 */

#include "gpio.h"


MODULE_LICENSE("GPL and additional rights");
MODULE_DESCRIPTION("Shrek module for Web Designing Lab");
MODULE_AUTHOR("Team Leader, Group 15");
MODULE_VERSION("0.69");

#define PIN0 396
#define PIN1 392
#define PIN2 255
#define PIN3 254

#define BUF_SIZE 8

static int shrek_major = 69;
static int shrek_minor = 69;
static int shrek_nr_devs = 1;

static dev_t shrek_dev = 0;
static struct cdev *shrek_cdev;
static struct class *shrek_class;
//static DEFINE_MUTEX(shrek_mutex);

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

static ssize_t drv_read(struct file *file, char *buf, size_t size, loff_t *offset)
{
	int ret = 0;
	char value[BUF_SIZE];

	pr_debug("read called");
	if ( ! gpio_read_value(lednum, value) ) {
		pr_err("gpio read error");
		return -1;
	}
	ret = copy_to_user(buf, value, strlen(value));
	if( ret != 0 ) {
		pr_err("read: copy error");
		return -1;
	}

	return 0;
}

static ssize_t drv_write(struct file *file, const char *buf, size_t size, loff_t *offset)
{
	int ret = 0;
	char buf2[BUF_SIZE];

	pr_debug("write called");
	ret = copy_from_user(buf2, buf, size);
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

static bool gpio_setup(unsigned int pin) {
	bool rc = false;
	rc = gpio_export(pin);
	if (!rc) return false;
	rc = gpio_set_dir(pin, "out");
	if (!rc) return false;
	//rc = gpio_set_value(pin, "1");
	//if (!rc) return false;
	return true;
}

static int __init sexy_shrek_init(void)
{
	int rc = 0;

	gpio_setup(PIN0);
//	gpio_setup(PIN1);
//	gpio_setup(PIN2);
//	gpio_setup(PIN3);

//	mutex_init(&shrek_mutex);

	/*
	 * the code below is copy from:
	 * https://static.lwn.net/images/pdf/LDD3/ch03.pdf
	 */
	if (shrek_major) { // default is <69,69>
		shrek_dev = MKDEV(shrek_major, shrek_minor);
		rc = register_chrdev_region(shrek_dev, shrek_nr_devs, DEV_SHREK_NAME);
	} else {
		rc = alloc_chrdev_region(&shrek_dev, shrek_minor, shrek_nr_devs, DEV_SHREK_NAME);
		shrek_major = MAJOR(shrek_dev);
	}
	/* It is emulated in the 2.6 kernel but should not be used for new code */
    // rc = register_chrdev(shrek_major, DEV_SHREK_NAME, shrek_fops);
	if (rc < 0) {
		pr_warn("Failed to register the shrek char device. rc = %i\n", rc);
		pr_warn("can't get major %d\n", shrek_major);
		return rc;
	}

	/*
	 * the code below is copy from:
	 * https://github.com/sysprog21/fibdrv/blob/a4dfbedf2dc45d0f7e6e456b16ecbf291fcae1b5/fibdrv.c#L107-L169
	 */
    shrek_cdev = cdev_alloc();
    if (shrek_cdev == NULL) {
        pr_warn("Failed to alloc cdev");
        rc = -1;
        goto failed_cdev;
    }
    shrek_cdev->ops = &shrek_fops;
    rc = cdev_add(shrek_cdev, shrek_dev, 1);

    if (rc < 0) {
        pr_warn("Failed to add cdev");
        rc = -2;
        goto failed_cdev;
    }

    shrek_class = class_create(THIS_MODULE, DEV_SHREK_NAME);

    if (!shrek_class) {
        pr_warn("Failed to create device class");
        rc = -3;
        goto failed_class_create;
    }

    if (!device_create(shrek_class, NULL, shrek_dev, NULL, DEV_SHREK_NAME)) {
        pr_warn("Failed to create device");
        rc = -4;
        goto failed_device_create;
    }
	pr_info("iM iN\n");
    return rc;
failed_device_create:
    class_destroy(shrek_class);
failed_class_create:
    cdev_del(shrek_cdev);
failed_cdev:
    unregister_chrdev_region(shrek_dev, 1);
    return rc;
}

static void __exit sexy_shrek_exit(void)
{
//	gpio_set_value(PIN3, "0");
//	gpio_set_value(PIN2, "0");
//	gpio_set_value(PIN1, "0");
//	gpio_set_value(PIN0, "0");
//	gpio_unexport(PIN3);
//	gpio_unexport(PIN2);
//	gpio_unexport(PIN1);
	gpio_unexport(PIN0);
//	mutex_destroy(&shrek_mutex);
    device_destroy(shrek_class, shrek_dev);
    class_destroy(shrek_class);
    cdev_del(shrek_cdev);
	/* Function that undoes a registration made with register_chrde */
	// unregister_chrdev(shrek_major, DEV_SHREK_NAME);
	unregister_chrdev_region(shrek_dev, shrek_nr_devs);

	pr_info("Fxck this shit im out\n");
}

module_init(sexy_shrek_init);
module_exit(sexy_shrek_exit);

