#include "shrek.h"

#include "fs.h"
#include "gpio.h"

/*
 * fake function for compile test
 */
#define KERNEL_4_12_BEFORE

#ifdef KERNEL_4_12_BEFORE
struct file *_file_open(
		const char *path,
		int flags,
		int rights)
{
	pr_debug("_file_open called");
	return NULL;
}

int _file_read(
		struct file *filp,
		unsigned long long offset,
		unsigned char *data,
		unsigned int size)
{
	pr_debug("_file_read called");
	return 0;
}

int _file_write(
		struct file *filp,
		unsigned long long offset,
		unsigned char *data,
		unsigned int size)
{
	pr_debug("_file_write called");
	return 0;
}
#endif /* KERNEL_4_12_BEFORE */

bool gpio_export(unsigned int pin) {
	pr_debug("gpio_unexport called");
	return false;
}

bool gpio_unexport(unsigned int pin) {
	pr_debug("gpio_unexport called");
	return false;
}

bool gpio_set_dir(unsigned int gpio, const char *dirStatus) {
	pr_debug("gpio_set_dir called");
	return false;
}

bool gpio_set_value(unsigned int gpio, const char *value) {
	pr_debug("gpio_set_value called");
	return false;
}

int gpio_read_value(unsigned int gpio) {
	pr_debug("gpio_read_value called");
	return 0;
}

