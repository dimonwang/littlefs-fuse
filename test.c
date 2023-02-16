#include "lfs.h"
#include "lfs_dimon.h"
#include <errno.h>

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

// configuration of the filesystem is provided by this struct
static struct lfs_config cfg = {
    // block device operations
    .read  = lfs_dimon_read,
    .prog  = lfs_dimon_prog,
    .erase = lfs_dimon_erase,
    .sync  = lfs_dimon_sync,

    // block device configuration
    .read_size = 512,
    .prog_size = 512,
    .block_size = 512,
    .block_count = 2048,
    .cache_size = 512,
    .lookahead_size = 512,
    .block_cycles = 500,
};

// entry point
int main(void) {
    if (lfs_dimon_create(&cfg, "../littlefs-fuse/image") < 0) {
        printf("lfs_fuse_bd_create error:%s\n",  strerror(errno));
        return -1;
    }

    // mount the filesystem
    int err = lfs_mount(&lfs, &cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        printf("remount\n");
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }

    // read current count
    /* uint32_t boot_count = 0; */
    char boot_count[124];
    lfs_file_open(&lfs, &file, "lfs.txt", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &boot_count, 11);

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);

    // release any resources we were using
    lfs_unmount(&lfs);
    printf("%s\n", boot_count);
    return 0;
}
