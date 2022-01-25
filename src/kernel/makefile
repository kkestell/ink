OBJS = nimcache/*.o libnim/bin/libnim.a

bin/kernel.elf:
	mkdir -p bin
	make -C libnim
	nim c --cpu:amd64 --os:any --mm:arc -d:useMalloc --nimcache=nimcache --noLinking --noMain --threads:off --checks:off --assertions:off \
		--passC:"-Ilibnim/include -I/usr/include -ffreestanding -fno-pie -mcmodel=kernel -std=gnu11" -d:release --opt:size --header:main.h main.nim
	ld -o bin/kernel.elf $(OBJS)

clean:
	rm -rf bin nimcache 
	make -C libnim clean

# CC	= gcc

# %.o:%.c
# 	$(CC) -g -c $<

# NIMSRC	= main.nim
# NIMOBJS	= nimcache/*.o
# LNIMOBJS= libnim/print.o
# OBJS	= $(NIMOBJS) libnim/libnim.a entry.o

# clean:
# 	rm -rf nimcache
# 	make -C libnim clean

# $(TARGET):$(OBJS)
# 	$(CC) -o $@ $^ -nostdlib -lgcc

# entry.o : nimcache/main.h

# $(NIMOBJS):
# 	make nim

# nimcache/main.h:
# 	make nim

# nim:$(NIMSRC)
# 	nim c --cpu:amd64 --os:linux --nimcache=nimcache --noLinking --noMain --passC:"-g" --passC:"-Ilibnim" --passC:"-ffreestanding" --passC:"-fno-pie" --passC:"-mcmodel=kernel" -d:release --header:main.h $(NIMSRC)

# libnim/libnim.a:
# 	make -C libnim

# bin/kernel: $(OBJS)
# 	ld -m64 -o $@ $^