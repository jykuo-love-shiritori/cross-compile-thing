/* SPDX-License-Identifier: WTFPL */
#ifndef SHREK_FS_WRAPPER_H
#define SHREK_FS_WRAPPER_H

/*
 * kernel fs read/write changed in 4.12
 * https://stackoverflow.com/questions/1184274/read-write-files-within-a-linux-kernel-module
 */

#ifndef KERNEL_4_12_BEFORE
/* kernel 4.12 or later BEGIN */
#define FILP_OPEN(path, flags, rights) file_open(path, flags, rights)
/* kernel 4.12 or later END */

#else /* KERNEL_4_12_BEFORE */
/* kernel 4.12 or before BEGIN */
struct file *_file_open(
		const char *path,
		int flags,
		int rights);
#define FILP_OPEN(path, flags, rights) _file_open(path, flags, rights)

int _file_read(
		struct file *filp,
		unsigned long long offset,
		unsigned char *data,
		unsigned int size);
#define kernel_read(filp, offset, data, size) _file_read(path, flags, rights)

int _file_write(
		struct file *filp,
		unsigned long long offset,
		unsigned char *data,
		unsigned int size);
#define kernel_write(filp, offset, data, size) _file_write(path, flags, rights)
/* kernel 4.12 or before END */

#endif /* KERNEL_4_12_BEFORE */

#endif /* SHREK_FS_WRAPPER_H */

