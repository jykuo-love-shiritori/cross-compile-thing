/* SPDX-License-Identifier: WTFPL */
/*
 * Author: Member B <noreply@ntut.edu.tw>
 * Change to kernel module version:
 *         Memner A <noreply@ntut.edu.tw>
 */
#include "gpio.h"

#include <linux/fs.h>
#include "fs.h"       /* FILE_OPEN() */

#define GPIO_DIR "/sys/class/gpio/"

bool gpio_export(unsigned int gpio) {
	int ret;
	int len;
	char buf[64];
	struct file *fp;

	fp = FILP_OPEN(GPIO_DIR "export", O_WRONLY,0);
	if (fp < 0) {
		pr_err("error: gpio/export open");
		return false;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	ret = kernel_write(fp, buf, len,NULL);
	if (ret != len) {
		pr_err("error: gpio/export write");
		return false;
	}

	filp_close(fp,NULL);

	return true;
}

bool gpio_set_dir(unsigned int gpio, const char *dirStatus) {
	int ret;
	char buf[64];
	struct file *fp;

	snprintf(buf, sizeof(buf), GPIO_DIR "gpio%d/direction", gpio);

	fp = FILP_OPEN(buf, O_WRONLY,0);
	if (fp < 0) {
		pr_err("error: gpio/direction open");
		return false;
	}

	ret = kernel_write(fp, dirStatus, 4,NULL);
	if (ret != strlen(dirStatus)) {
		pr_err("error: gpio/direction open");
		return false;
	}

	filp_close(fp,NULL);

	return true;
}

bool gpio_set_value(unsigned int gpio, const char *value) {
	int ret;
	struct file *fp;
	char buf[64];

	snprintf(buf, sizeof(buf), GPIO_DIR "gpio%d/value", gpio);

	fp = FILP_OPEN(buf, O_WRONLY,0);
	if (fp < 0) {
		pr_err("error: gpio/value open");
		return false;
	}

	ret = kernel_write(fp, value, 2,NULL);
	if (ret != strlen(value)) {
		pr_err("error: gpio/value write");
		return false;
	}

	filp_close(fp,NULL);

	return true;
}

bool gpio_unexport(unsigned int gpio) {
	int ret;
	int len;
	char buf[64];
	struct file *fp;

	fp = FILP_OPEN(GPIO_DIR "unexport", O_WRONLY,0);
	if (fp< 0) {
		pr_err("gpio/unexport open");
		return false;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);

	ret = kernel_write(fp, buf, len,NULL);
	if (ret != len) {
		pr_err("error: gpio/export write");
		return false;
	}

	filp_close(fp,NULL);

	return true;
}

