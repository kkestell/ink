OBJS = obj/*.o src/libnim/bin/libnim.a

all: bin/kernel.elf

bin/kernel.elf:
	mkdir -p $(@D)
	make -C src/libnim
	nim c src/main.nim
	ld -o $@ $(OBJS)

clean:
	rm -rf obj bin 
	make -C src/libnim clean
