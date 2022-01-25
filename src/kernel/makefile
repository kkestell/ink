OBJS = nimcache/*.o libnim/bin/libnim.a

all: bin/kernel.elf

bin/kernel.elf:
	mkdir -p $(@D)
	make -C libnim
	nim c main.nim
	ld -o $@ $(OBJS)

clean:
	rm -rf bin nimcache 
	make -C libnim clean
