CC = clang
CFLAGS = -Iinclude -target x86_64-pc-win32-coff -std=c11 -fno-stack-protector -fshort-wchar -mno-red-zone -Wall -Wextra -pedantic -Werror

LD = lld
LFLAGS = -flavor link -subsystem:efi_application -nodefaultlib -dll -entry:efi_main

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

CFILES = $(wildcard src/*.c)
OFILES  = $(CFILES:src/%.c=obj/%.o)

DEPS = $(OFILES:%.o=%.d)
-include $(DEPS)

all: bin/bootx64.efi

bin/bootx64.efi: $(OFILES)
	mkdir -p $(@D)
	$(LD) $(LFLAGS) $(OFILES) -out:$@

clean:
	rm -rf bin obj > /dev/null 2> /dev/null || true
