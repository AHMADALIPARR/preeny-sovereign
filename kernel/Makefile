CC ?= gcc
AS ?= gcc
LD ?= ld
CFLAGS ?= -ffreestanding -O2 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -m32
ASFLAGS ?= -ffreestanding -m32
LDFLAGS ?= -T linker.ld -nostdlib -m elf_i386

all: kernel.elf

boot.o: boot.S
	$(AS) $(ASFLAGS) -c -o $@ $<

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c -o $@ $<

kernel.elf: boot.o kernel.o linker.ld
	$(LD) $(LDFLAGS) -o $@ boot.o kernel.o

# Optional ISO (requires grub-mkrescue)
iso: kernel.elf
	mkdir -p isodir/boot/grub
	cp kernel.elf isodir/boot/
	echo 'menuentry "sovereign" { multiboot /boot/kernel.elf }' > isodir/boot/grub/grub.cfg
	grub-mkrescue -o sovereign.iso isodir

clean:
	rm -f *.o kernel.elf sovereign.iso
	rm -rf isodir

.PHONY: all iso clean
