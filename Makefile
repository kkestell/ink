.PHONY: boot kernel img qemu clean default

boot:
	@$(MAKE) -C src/boot

kernel:
	@$(MAKE) -C src/kernel

img: boot kernel
	scripts/mkimg.sh

qemu: img
	scripts/qemu.sh

clean:
	@$(MAKE) -C src/boot clean
	@$(MAKE) -C src/kernel clean
	rm -f img/ink.img

default: img
