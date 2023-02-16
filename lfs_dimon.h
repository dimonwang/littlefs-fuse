/*
 * Linux user-space block device wrapper
 *
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef LFS_FUSE_BD_H
#define LFS_FUSE_BD_H

#include "lfs.h"


// Create a block device with path to dev block device
int lfs_dimon_create(struct lfs_config *cfg, const char *path);

// Clean up memory associated with emu block device
void lfs_dimon_destroy(const struct lfs_config *cfg);

// Read a block
int lfs_dimon_read(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size);

// Program a block
//
// The block must have previously been erased.
int lfs_dimon_prog(const struct lfs_config *cfg, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size);

// Erase a block
//
// A block must be erased before being programmed. The
// state of an erased block is undefined.
int lfs_dimon_erase(const struct lfs_config *cfg, lfs_block_t block);

// Sync the block device
int lfs_dimon_sync(const struct lfs_config *cfg);


#endif
