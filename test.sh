#!/bin/sh

sudo losetup -d /dev/loop12
sudo chmod 777 /dev/loop12
dd if=/dev/zero of=image bs=512 count=2048
sudo losetup /dev/loop12 image

./lfs_fuse --format /dev/loop12

mkdir -p ~/l_d
./lfs_fuse /dev/loop12 ~/l_d -o nonempty
sudo losetup -d /dev/loop12
