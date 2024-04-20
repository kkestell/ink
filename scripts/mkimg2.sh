#!/usr/bin/env bash

IMG_FILE="img/ink.img"
IMG_SIZE=64

rm -f "$IMG_FILE"

dd if=/dev/zero of="$IMG_FILE" bs=1M count=$IMG_SIZE status=progress

mkfs.fat -F 32 "$IMG_FILE"

MOUNT_DIR="./tmp"
mkdir -p "$MOUNT_DIR"

LOOP_DEV=$(sudo losetup --find --show "$IMG_FILE")

sudo mount "$LOOP_DEV" "$MOUNT_DIR"

sudo mkdir -p "$MOUNT_DIR/EFI/BOOT"
sudo cp ./src/boot/bin/bootx64.efi "$MOUNT_DIR/EFI/BOOT"
sudo cp ./src/kernel/bin/kernel.elf "$MOUNT_DIR"

sudo umount "$MOUNT_DIR"
sudo losetup -d "$LOOP_DEV"
