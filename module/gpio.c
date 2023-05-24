/* SPDX-License-Identifier: WTFPL */
/*
 * Author: Member B <noreply@ntut.edu.tw>
 * Change to kernel module version:
 *         Memner A <noreply@ntut.edu.tw>
 */
/* WIP by Team Leader */

#include "gpio.h"

#include "fs.h"

#define GPIO_DIR "/sys/class/gpio/"

int gpio_export(unsigned int gpio) {
	int len;
	char buf[64];
	struct file *fp;
	fp = filp_open(GPIO_DIR "export", O_WRONLY,0);
	if (fp < 0) {
		pr_err("gpio/export");
		return 1;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);

	vfs_write(fp, buf, len,NULL);
	filp_close(fp,NULL);

	return 0;
}

int gpio_set_dir(unsigned int gpio, const char *dirStatus) {
	char buf[64];

	snprintf(buf, sizeof(buf), GPIO_DIR "gpio%d/direction", gpio);

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

int gpio_set_value(unsigned int gpio, const char *value) {
	struct file *fp;
	char buf[64];

	snprintf(buf, sizeof(buf), GPIO_DIR "gpio%d/value", gpio);

	fp = filp_open(buf, O_WRONLY,0);

	if (fp< 0) {
		pr_err("gpio/direction");
		return fp;
	}

	

	vfs_write(fp, value, 2,NULL);

	filp_close(fp,NULL);
	return 0;
}

int gpio_unexport(unsigned int gpio) {
	int len;
	char buf[64];
	struct file *fp;
	fp = filp_open(GPIO_DIR "unexport", O_WRONLY,0);
	if (fp< 0) {
		pr_err("gpio/unexport");
		return fp;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);

	vfs_write(fp, buf, len,NULL);
	filp_close(fp,NULL);
	return 0;
}

