.PHONY: boot kernel img clean

boot:
	$(MAKE) -C src/boot

kernel:
	$(MAKE) -C src/kernel

img: boot kernel
	scripts/mkimg2.sh

qemu: img
	scripts/qemu.sh

clean:
	$(MAKE) -C src/boot clean
	$(MAKE) -C src/kernel clean
	rm -f img/ink.img
