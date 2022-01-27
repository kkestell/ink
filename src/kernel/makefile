bin/kernel.elf: kernel
	mkdir -p $(@D)
	ld -o $@ $(shell find obj/ -type f -name '*.o')

obj/%.o: data/%
	mkdir -p $(@D)
	objcopy -I binary --output elf64-x86-64 --binary-architecture i386:x86-64 $< $@

obj/%.o: src/%.s
	mkdir -p $(@D)
	as -c $< -o $@

obj/%.o: src/%.c
	mkdir -p $(@D)
	gcc -Iinclude -ffreestanding -fno-pie -mcmodel=kernel -std=gnu11 -O3 -c $< -o $@

CFILES = $(shell find src/ -type f -name '*.c')
SFILES = $(shell find src/ -type f -name '*.s')
DFILES = $(shell find data/ -type f)
OFILES = $(CFILES:src/%.c=obj/%.o) $(SFILES:src/%.s=obj/%.o) $(DFILES:data/%=obj/%.o)

kernel: $(OFILES)
	nim c src/main.nim

format:
	/nim/bin/nimpretty --maxLineLen:90 $(shell find src/ -type f -name '*.nim')

clean:
	rm -rf obj bin 
