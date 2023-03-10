#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);

int gpio_set_dir(unsigned int gpio, const char *dirStatus);
int gpio_set_value(unsigned int gpio, const char *value);

#endif
