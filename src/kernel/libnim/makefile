obj/%.o: src/%.s
	mkdir -p $(@D)
	as -c $< -o $@

obj/%.o: src/%.c
	mkdir -p $(@D)
	gcc -Iinclude -ffreestanding -fno-pie -mcmodel=kernel -c $< -o $@

CFILES = $(shell find src/ -type f -name '*.c')
SFILES = $(shell find src/ -type f -name '*.s')
OFILES  = $(CFILES:src/%.c=obj/%.o)
OFILES += $(SFILES:src/%.s=obj/%.o)

DEPS = $(OFILES:%.o=%.d)
-include $(DEPS)

all: bin/libnim.a

bin/libnim.a: $(OFILES)
	mkdir -p $(@D)
	ar rvs $@ $^

clean:
	rm -rf obj bin
