/* SPDX-License-Identifier: WTFPL */
/*
 * Author: Member B <noreply@ntut.edu.tw>
 * Change to kernel module version:
 *         Memner A <noreply@ntut.edu.tw>
 */
#include "gpio.h"

#include "shrek.h"

#include <linux/fs.h>


/*
 * kernel fs read/write changed in 4.14
 * https://stackoverflow.com/questions/1184274/read-write-files-within-a-linux-kernel-module
 */
/*
 * UPDATE: I FOUND INTERESTING API CHANGE
 * on Linux 6.3.2-arch1-1 #1 SMP PREEMPT_DYNAMIC Thu, 11 May 2023 16:40:42 +0000 x86_64 GNU/Linux
 *        ssize_t kernel_write(struct file *, const void *, size_t, loff_t *);
 *                                                   !!!different here!!! v^
 *        ssize_t kernel_write(struct file *, const char *, size_t, loff_t);
 * on TX2 (Linux 4.9.201-tegra #1 SMP PREEMPT Fri Jul 9 08:56:59 PDT 2021 aarch64 aarch64 aarch64 GNU/Linux)
 *   and
 * on 6.3.2
 *        ssize_t kernel_read(struct file *, void *, size_t, loff_t *);
 *                    !!! every thing is differece !!!
 *        int kernel_read(struct file *, loff_t, char *, unsigned long);
 * on TX2
 */

#define GPIO_DIR "/sys/class/gpio/"

bool gpio_export(unsigned int gpio) {
	size_t ret;
	size_t len;
	loff_t off = 0;
	struct file *fp;

	char buf[64];

	fp = filp_open(GPIO_DIR "export", O_WRONLY, 0);
	if (IS_ERR(fp)) {
		pr_err("error: gpio/export open");
		return false;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
#ifdef SHREK_ON_TX2
	ret = kernel_write(fp, buf, len, off);
#else
	ret = kernel_write(fp, buf, len, &off);
#endif /* SHREK_ON_TX2 */
	if (ret != len) {
		pr_err("error: gpio/export write");
		return false;
	}

	filp_close(fp, NULL);

	return true;
}

bool gpio_set_dir(unsigned int gpio, const char *dirStatus) {
	size_t ret;
	size_t len;
	loff_t off = 0;
	struct file *fp;

	char buf[64];

	snprintf(buf, sizeof(buf), GPIO_DIR "gpio%d/direction", gpio);

	fp = filp_open(buf, O_WRONLY, 0);
	if (IS_ERR(fp)) {
		pr_err("error: gpio/direction open");
		return false;
	}

	len = strlen(dirStatus);
#ifdef SHREK_ON_TX2
	ret = kernel_write(fp, dirStatus, len, off);
#else
	ret = kernel_write(fp, dirStatus, len, &off);
#endif /* SHREK_ON_TX2 */
	if (ret != len) {
		pr_err("error: gpio/direction open");
		return false;
	}

	filp_close(fp, NULL);

	return true;
}

bool gpio_set_value(unsigned int gpio, const char *value) {
	size_t ret;
	size_t len;
	loff_t off = 0;
	struct file *fp;

	char buf[64];

	snprintf(buf, sizeof(buf), GPIO_DIR "gpio%d/value", gpio);

	fp = filp_open(buf, O_WRONLY, 0);
	if (IS_ERR(fp)) {
		pr_err("error: gpio/value open");
		return false;
	}

	len = strlen(value);
#ifdef SHREK_ON_TX2
	ret = kernel_write(fp, value, len, off);
#else
	ret = kernel_write(fp, value, len, &off);
#endif /* SHREK_ON_TX2 */
	if (ret != len) {
		pr_err("error: gpio/value write");
		return false;
	}

	filp_close(fp, NULL);

	return true;
}

bool gpio_unexport(unsigned int gpio) {
	size_t ret;
	size_t len;
	loff_t off = 0;
	struct file *fp;

	char buf[64];

	fp = filp_open(GPIO_DIR "unexport", O_WRONLY, 0);
	if (IS_ERR(fp)) {
		pr_err("error: gpio/unexport open");
		return false;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
#ifdef SHREK_ON_TX2
	ret = kernel_write(fp, buf, len, off);
#else
	ret = kernel_write(fp, buf, len, &off);
#endif /* SHREK_ON_TX2 */
	if (ret != len) {
		pr_err("error: gpio/unexport write");
		return false;
	}

	filp_close(fp, NULL);

	return true;
}

int gpio_read_value(unsigned int gpio) {
	size_t ret;
	size_t len;
	loff_t off = 0;
	struct file *fp;

	char buf[64];
	int value;

	snprintf(buf, sizeof(buf), GPIO_DIR "gpio%d/value", gpio);

	fp = filp_open(buf, O_RDONLY, 0);
	if (IS_ERR(fp)) {
		pr_err("error: gpio/value open");
		return -1;
	}

    len = strlen(buf);
#ifdef SHREK_ON_TX2
    ret = kernel_read(fp, off, buf, len);
#else
    ret = kernel_read(fp, buf, len, &off);
#endif /* SHREK_ON_TX2 */
	if (ret != len) {
		pr_err("error: gpio/value read");
		return -1;
	}
	sscanf(buf, "%d", &value);

	filp_close(fp, NULL);

	return value;
}

