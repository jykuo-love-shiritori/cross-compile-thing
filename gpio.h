/* SPDX-License-Identifier: WTFPL */
/*
 * GPIO Interface for Kernel Module
 *
 * Author: Team Leader <>
 */

#ifndef SHREK_GPIO_H
#define SHREK_GPIO_H

#include <linux/kernel.h> /* include stdbool.h */

/*
 * GPIO Sysfs Interface for Userspace:
 * https://www.kernel.org/doc/html/latest/admin-guide/gpio/sysfs.html
 *
 * stackoverflow: https://stackoverflow.com/a/63812033
 */

/**
 * gpio_export - export control of a GPIO to userspace
 * @pin: GPIO Pin#
 *
 * Return: true if success. false if not, and errno is set to indicate the error
 * for write().
 */
bool gpio_export(unsigned int pin);

/**
 * gpio_unexport - reverses the effect of exporting to userspace.
 * @pin: GPIO Pin#
 *
 * Return: true if success. false if not, and errno is set to indicate the error
 * for write().
 */
bool gpio_unexport(unsigned int pin);

/**
 * gpio_set_dir - set direction of GPIO
 * @pin: GPIO Pin#
 * @dirStatus: direction, reads as either "in" or "out".
 *
 * Return: true if success. false if not, and errno is set to indicate the error
 * for write().
 */
bool gpio_set_dir(unsigned int gpio, const char *dirStatus);

/**
 * gpio_set_value - set value of GPIO
 * @pin: GPIO Pin#
 * @value: reads as either 0 (low) or 1 (high).
 *
 * If the GPIO is configured as an output, this value may be written;
 * any nonzero value is treated as high.
 *
 * Return: true if success. false if not, and errno is set to indicate the error
 * for write().
 */
bool gpio_set_value(unsigned int gpio, const char *value);

/**
 * gpio_read_value - read value of GPIO
 * @pin: GPIO Pin#
 *
 * Return: true if success. false if not, and errno is set to indicate the error
 * for read().
 */
bool gpio_read_value(const unsigned int gpio, char *value);

#endif /* SHREK_GPIO_H */

