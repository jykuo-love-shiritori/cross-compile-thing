/* SPDX-License-Identifier: WTFPL */

#define MODULE_NAME "sexy_shrek"
#define MAJOR_NUM 69
#define MINOR_NUM 69

#define DEV_SHREK_NAME "sexyshrek"


/**
 * pr_fmt - used by the pr_*() macros to generate the printk format string
 * docs: https://github.com/torvalds/linux/blame/fc4354c6e5c21257cf4a50b32f7c11c7d65c55b3/include/linux/printk.h#L331-L347
 */
#define pr_fmt(fmt) MODULE_NAME ": " fmt

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>

#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Shrek module for Web Designing Lab");
MODULE_AUTHOR("Team Leader, Group 15");
MODULE_VERSION("0.69");


#define TEXT_SIZE 1024

static dev_t shrek_dev = 0;
//static struct cdev *shrek_cdev;
//static struct class *shrek_class;
//static DEFINE_MUTEX(shrek_mutex);
static char shrek_text[TEXT_SIZE];
static unsigned int shrek_count = 0;


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
	pr_info("read!");
	__arch_copy_from_user(buf, shrek_text, size);
	return (ssize_t)69;
}

static ssize_t drv_write(struct file *file,
                         const char *buf,
                         size_t size,
                         loff_t *offset)
{
	pr_info("write called!");
	if(size >= TEXT_SIZE) {
		size = TEXT_SIZE;
	}
	
	shrek_count++;
	__arch_copy_from_user((shrek_text, buf, size);
	shrek_text[size-1] = '\0';
	pr_info("write %d time!", shrek_count);
	pr_info("text: %s", shrek_text);
	pr_info("size: %d", size);
	return size;
}

/* llseek operation is skipped */
static loff_t drv_device_lseek(struct file *file, loff_t offset, int orig)
{
	pr_info("lseek %d", (int)offset);
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

static int __init sexy_shrek_init(void)
{
	int rc = 0;

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
	goto return_section;

//	shrek_cdev = cdev_alloc();
//    if (shrek_cdev == NULL) {
//        pr_alert("Failed to alloc cdev");
//        rc = -1;
//        goto failed_cdev;
//    }
//
//    shrek_cdev->ops = &drv_fops;
//    rc = cdev_add(shrek_cdev, shrek_dev, 69);
//    if (rc < 0) {
//        pr_alert("Failed to add cdev");
//        rc = -2;
//        goto failed_cdev;
//    }
//
//    shrek_class = class_create(THIS_MODULE, DEV_SHREK_NAME);
//    if (!shrek_class) {
//        pr_alert("Failed to create device class");
//        rc = -3;
//        goto failed_class_create;
//    }
//
//	if (!device_create(shrek_class, NULL, shrek_dev, NULL, DEV_SHREK_NAME)) {
//        pr_alert("Failed to create device");
//        rc = -4;
//        goto failed_device_create;
//    }
//
  return_section:
	pr_info("iM iN\n");
	//pr_info("but this is debug\n");
	return rc;
//  failed_device_create:
//    class_destroy(shrek_class);
//  failed_class_create:
//    cdev_del(shrek_cdev);
//  failed_cdev:
//    unregister_chrdev_region(shrek_dev, 1);
//    return rc;
}

static void __exit sexy_shrek_exit(void)
{
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
