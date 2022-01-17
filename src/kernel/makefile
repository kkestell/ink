CC = clang
CFLAGS = -Iinclude -target x86_64-none-elf -Wall -Wextra -pedantic

LD = ld.lld
LFLAGS = -nostdlib --script=kernel.ld

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

CFILES = $(wildcard src/*.c)
OFILES  = $(CFILES:src/%.c=obj/%.o)

DEPS = $(OFILES:%.o=%.d)
-include $(DEPS)

all: bin/kernel.elf

bin/kernel.elf: $(OFILES)
	mkdir -p $(@D)
	$(LD) $(LFLAGS) $(OFILES) -o $@

clean:
	rm -rf bin obj > /dev/null 2> /dev/null || true
