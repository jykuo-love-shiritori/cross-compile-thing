#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define PIN0 396 // VCC  7 GND  6
#define PIN1 392 // VCC 12 GND  9
#define PIN2 255 // VCC 15 GND 14
#define PIN3 254 // VCC 22 GND 20

int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);

int gpio_set_dir(unsigned int gpio, const char *dirStatus);
int gpio_set_value(unsigned int gpio, const char *value);

#endif
