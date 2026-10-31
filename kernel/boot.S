/* Multiboot2 header + freestanding entry for sovereign kernel */
.set MAGIC,    0xe85250d6
.set ARCH,     0
.set HEADER_LEN, header_end - header_start
.set CHECKSUM, -(MAGIC + ARCH + HEADER_LEN)

.section .multiboot
header_start:
    .long MAGIC
    .long ARCH
    .long HEADER_LEN
    .long CHECKSUM

    /* end tag */
    .short 0
    .short 0
    .long 8
header_end:

.section .text
.global _start
_start:
    /* Multiboot2: EAX = 0x36d76289, EBX = info structure physical address */
    mov %ebx, %eax          /* pass info pointer as first argument */
    mov $stack_top, %esp
    push %eax               /* argument for kernel_main */
    call kernel_main
    add $4, %esp
    cli
1:  hlt
    jmp 1b

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:
