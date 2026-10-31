CC ?= gcc
AS ?= gcc
LD ?= ld
CFLAGS ?= -ffreestanding -O2 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -m64 -mcmodel=kernel -mno-red-zone
ASFLAGS ?= -ffreestanding -m64
LDFLAGS ?= -T linker.ld -nostdlib -m elf_x86_64

all: kernel64.elf

boot.o: boot.S
	$(AS) $(ASFLAGS) -c -o $@ $<

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c -o $@ $<

kernel64.elf: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) -o $@ boot.o kernel.o

clean:
	rm -f *.o kernel64.elf

.PHONY: all clean
