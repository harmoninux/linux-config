#!/bin/sh

rm -f rootfs.img
qemu-img create -f raw rootfs.img 128G
mkfs.ext4 rootfs.img
sudo mkdir -p /mnt/rootfs
sudo mount rootfs.img /mnt/rootfs
sudo cp -r rootfs/* /mnt/rootfs
sudo umount /mnt/rootfs
qemu-img convert -p -f raw -O qcow2 -o lazy_refcounts=on -o extended_l2=on rootfs.img rootfs.qcow2