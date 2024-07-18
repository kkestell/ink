boot:
    @make -C src/boot

kernel:
    @make -C src/kernel

img: boot kernel
    scripts/mkimg.sh

qemu: img
    scripts/qemu.sh

clean:
    @make -C src/boot clean
    @make -C src/kernel clean
    rm -f img/ink.img

default:
    just img
