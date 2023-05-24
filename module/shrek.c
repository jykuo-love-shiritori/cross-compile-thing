/* SPDX-License-Identifier: WTFPL */

#include "shrek.h"

/*
 * pr_fmt - used by the pr_*() macros to generate the printk format string
 * docs: https://github.com/torvalds/linux/blame/fc4354c6e5c21257cf4a50b32f7c11c7d65c55b3/include/linux/printk.h#L331-L347
 */
#define pr_fmt(fmt) MODULE_NAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>             /* module_init, module_exit         */

#include <linux/fs.h>               /* copy_from_user                   */
// #include <linux/cdev.h>
// #include <linux/device.h>
// #include <linux/kdev_t.h>
// #include <linux/mutex.h>

// #include <asm/uaccess.h>
// #include <linux/uaccess.h>

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
	pr_info("open!");

	//if (!mutex_trylock(&shrek_mutex)) {
	//	pr_alert("shrekdrv is in use");
	//	return -EBUSY;
	//}
	return 0;
}

static int drv_release(struct inode *inode, struct file *file)
{
	pr_info("release!");
	//mutex_unlock(&shrek_mutex);
	return 0;
}

static ssize_t drv_read(struct file *file,
                        char *buf,
                        size_t size,
                        loff_t *offset)
{
	int ret = 0;

	pr_info("read!");
	ret = copy_from_user(buf, shrek_text, size);
	if( ret != 0 ) {
		pr_err("copy error");
	}

	return (ssize_t)69;
}

static ssize_t drv_write(struct file *file,
                         const char *buf,
                         size_t size,
                         loff_t *offset)
{
	int ret = 0;
	pr_info("write called!");
	if(size >= TEXT_SIZE) {
		size = TEXT_SIZE;
	}
	
	shrek_count++;
	ret = copy_from_user(shrek_text, buf, size);
	if( ret != 0 ) {
		pr_err("copy error");
	}

	shrek_text[size-1] = '\0';
	pr_info("write %d time!", shrek_count);
	pr_info("text: %s", shrek_text);
	pr_info("size: %d", (int)size);
	return size;
}

/* llseek operation is skipped */
static loff_t drv_device_lseek(struct file *file, loff_t offset, int orig)
{
	pr_info("lseek LED %d", (int)offset);
	lednum= offset;
	return 1;
}

/**
 * The file_operations Structure : Character Device Drivers
 * https://tldp.org/LDP/lkmpg/2.4/html/c577.htm
 */
const struct file_operations drv_fops = {
	.owner = THIS_MODULE,
	.read = drv_read,
	.write = drv_write,
	.open = drv_open,
	.release = drv_release,
	.llseek = drv_device_lseek,
};
static int gpio_export(unsigned int gpio) {
	int len;
	char buf[64];
	struct file *fp;
	fp = filp_open("/sys/class/gpio/export", O_WRONLY,0);
	if (fp < 0) {
		pr_err("gpio/export");
		return 1;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);

	vfs_write(fp, buf, len,NULL);
	filp_close(fp,NULL);

	return 0;
}
static int gpio_set_dir(unsigned int gpio, const char *dirStatus) {
	char buf[64];

	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);

	struct file *fp;
	fp = filp_open(buf, O_WRONLY,0);

	if (fp< 0) {
		pr_err("gpio/direction");
		return fp;
	}

	

	vfs_write(fp, dirStatus, 4,NULL);

	filp_close(fp,NULL);
	return 0;
}
static int gpio_set_value(unsigned int gpio, const char *value) {
	struct file *fp;
	char buf[64];

	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

	fp = filp_open(buf, O_WRONLY,0);

	if (fp< 0) {
		pr_err("gpio/direction");
		return fp;
	}

	

	vfs_write(fp, value, 2,NULL);

	filp_close(fp,NULL);
	return 0;
}
static int gpio_unexport(unsigned int gpio) {
	int len;
	char buf[64];
	struct file *fp;
	fp = filp_open("/sys/class/gpio/unexport", O_WRONLY,0);
	if (fp< 0) {
		pr_err("gpio/unexport");
		return fp;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);

	vfs_write(fp, buf, len,NULL);
	filp_close(fp,NULL);
	return 0;
}
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
	rc =  register_chrdev(MAJOR_NUM, MODULE_NAME, &drv_fops);
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

