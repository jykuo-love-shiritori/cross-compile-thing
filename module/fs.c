/* SPDX-License-Identifier: WTFPL */
/* WIP */
#include "fs.h"

#include <linux/fs.h>

#ifdef KERNEL_4_12_BEFORE
struct file *_file_open(const char *path, int flags, int rights) 
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

int _file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

int _file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

#endif /* KERNEL_4_12_BEFORE */

// reference
// ssize_t read(const char *path, const void *buf, size_t count, loff_t *pos);
//     int ret = 0;
// 	struct file* fp = NULL;
// 	loff_t pos = 0;
// #ifdef KERNEL_4_12_BEFORE
// 	mm_segment_t oldfs;
// 
//     oldfs = get_fs();
//     set_fs(get_ds());
// 
// 	fp = filp_open(path, O_WRONLY, 0);
//     ret = vfs_write(fp, buf, count, &pos);
// 
//     set_fs(oldfs);
// #else /* kernel 4.12 or later */
// 	fp = filp_open(path, O_WRONLY, 0);
// 	ret = kernel_write(fp, buf, count, &pos);
// #endif /* KERNEL_4_12_BEFORE */
// 	filp_close(fp, NULL);
// 	return ret
// }

