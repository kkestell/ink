#!/bin/bash

mkdir -p iso

mkdir -p disk/EFI/BOOT
cp src/boot/bin/bootx64.efi disk/EFI/BOOT/BOOTX64.EFI
cp src/kernel/bin/kernel.elf disk/kernel.elf
hdiutil create -fs fat32 -ov -size 48m -volname UEFI -format UDTO -srcfolder disk iso/ink.cdr
rm -rf disk

qemu-system-x86_64 \
  -chardev stdio,id=char0,logfile=serial.log,signal=off \
  -serial chardev:char0 \
  -bios bios/ovmf.fd \
  -drive format=raw,file=iso/ink.cdr \
  -net none
