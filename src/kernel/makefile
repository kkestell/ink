bin/kernel.elf: kernel
	mkdir -p $(@D)
	ld -o $@ $(shell find obj/ -type f -name '*.o')

obj/%.o: src/%.s
	mkdir -p $(@D)
	as -c $< -o $@

obj/%.o: src/%.c
	mkdir -p $(@D)
	gcc -Iinclude -ffreestanding -fno-pie -mcmodel=kernel -c $< -o $@

CFILES = $(shell find src/ -type f -name '*.c')
SFILES = $(shell find src/ -type f -name '*.s')
OFILES = $(CFILES:src/%.c=obj/%.o) $(SFILES:src/%.s=obj/%.o)

kernel: $(OFILES)
	nim c src/main.nim

clean:
	rm -rf obj bin 
