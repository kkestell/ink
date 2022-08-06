CC = clang
CFLAGS = -O3 -Iinclude -target x86_64-none-elf -std=c11 -ffreestanding -fomit-frame-pointer -Wall -Wextra -pedantic -Werror

LD = ld.lld
LFLAGS = --script=kernel.ld 

obj/%.o: src/%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

CFILES  = $(wildcard src/*.c)
SFILES  = $(wildcard src/*.S)
OFILES  = $(CFILES:src/%.c=obj/%.o)
OFILES += $(SFILES:src/%.S=obj/%.o)

DEPS = $(OFILES:%.o=%.d)
-include $(DEPS)

all: bin/kernel.elf

bin/kernel.elf: $(OFILES)
	mkdir -p $(@D)
	$(LD) $(LFLAGS) $(OFILES) -o $@

clean:
	rm -rf bin obj > /dev/null 2> /dev/null || true
