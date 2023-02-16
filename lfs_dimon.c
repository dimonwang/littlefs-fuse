/*
 * Linux user-space block device wrapper
 *
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "lfs_dimon.h"

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <assert.h>


// Block device wrapper for user-space block devices
int lfs_dimon_create(struct lfs_config *cfg, const char *path) {
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        printf("file %s open error:%s\n", path, strerror(errno));
        return -errno;
    }
    cfg->context = (void*)(intptr_t)fd;

    return 0;
}

void lfs_dimon_destroy(const struct lfs_config *cfg) {
    int fd = (intptr_t)cfg->context;
    close(fd);
}

int lfs_dimon_read(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size) {
    int fd = (intptr_t)cfg->context;

    // check if read is valid
    assert(block < cfg->block_count);

    // go to block
    off_t err = lseek(fd, (off_t)block*cfg->block_size + (off_t)off, SEEK_SET);
    if (err < 0) {
        return -errno;
    }

    // read block
    ssize_t res = read(fd, buffer, (size_t)size);
    if (res < 0) {
        return -errno;
    }

    return 0;
}

int lfs_dimon_prog(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size) {
    int fd = (intptr_t)cfg->context;

    // check if write is valid
    assert(block < cfg->block_count);

    // go to block
    off_t err = lseek(fd, (off_t)block*cfg->block_size + (off_t)off, SEEK_SET);
    if (err < 0) {
        return -errno;
    }

    // write block
    ssize_t res = write(fd, buffer, (size_t)size);
    if (res < 0) {
        return -errno;
    }

    return 0;
}

int lfs_dimon_erase(const struct lfs_config *cfg, lfs_block_t block) {
    // do nothing
    return 0;
}

int lfs_dimon_sync(const struct lfs_config *cfg) {
    int fd = (intptr_t)cfg->context;

    int err = fsync(fd);
    if (err) {
        return -errno;
    }

    return 0;
}

