#!/usr/bin/env bash

pushd src
pushd boot
make clean && make
popd

pushd kernel
make clean && make
popd
popd

rm img/ink.img
dd if=/dev/zero of=img/ink.img bs=1k count=1440
mformat -i img/ink.img -f 1440 ::
mmd -i img/ink.img ::/EFI
mmd -i img/ink.img ::/EFI/BOOT
mcopy -i img/ink.img src/boot/bin/bootx64.efi ::/EFI/BOOT
mcopy -i img/ink.img src/kernel/bin/kernel.elf ::/
