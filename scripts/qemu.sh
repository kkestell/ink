#!/bin/bash

qemu-system-x86_64 \
  -enable-kvm \
  -cpu host \
  -chardev stdio,id=char0,logfile=serial.log,signal=off \
  -serial chardev:char0 \
  -bios bios/ovmf.fd \
  -usb \
  -hda img/ink.img \
  -net none
