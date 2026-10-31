/* Multiboot2 entry + switch to long mode (x86_64) */
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
    .short 0
    .short 0
    .long 8
header_end:

.section .rodata
gdt64:
    .quad 0                         /* null */
    .quad (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)  /* code 64-bit */
gdt64_end:

gdt64_pointer:
    .word gdt64_end - gdt64 - 1
    .quad gdt64

.section .text
.code32
.global _start
_start:
    cli
    mov $stack_top, %esp

    /* save Multiboot2 info pointer */
    mov %ebx, mbi_ptr

    /* check for long mode support (CPUID) */
    mov $0x80000001, %eax
    cpuid
    test $(1 << 29), %edx
    jz no_long_mode

    /* set up identity-mapped page tables */
    mov $p4_table, %eax
    mov %eax, %cr3

    mov $p3_table, %eax
    or $0x3, %eax
    mov %eax, p4_table

    mov $p2_table, %eax
    or $0x3, %eax
    mov %eax, p3_table

    /* map first 2 MiB with 2 MiB pages */
    mov $0, %ecx
map_loop:
    mov %ecx, %eax
    shl $21, %eax
    or $0x83, %eax          /* present + writable + huge */
    mov %eax, p2_table(,%ecx,8)
    inc %ecx
    cmp $512, %ecx
    jne map_loop

    /* enable PAE */
    mov %cr4, %eax
    or $(1 << 5), %eax
    mov %eax, %cr4

    /* enable long mode in EFER */
    mov $0xC0000080, %ecx
    rdmsr
    or $(1 << 8), %eax
    wrmsr

    /* enable paging */
    mov %cr0, %eax
    or $(1 << 31), %eax
    mov %eax, %cr0

    /* load GDT and far jump to 64-bit code */
    lgdt gdt64_pointer
    ljmp $0x08, $long_mode_start

no_long_mode:
    /* hang */
    cli
1:  hlt
    jmp 1b

.code64
long_mode_start:
    mov $0, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %fs
    mov %ax, %gs

    /* pass mbi pointer */
    mov mbi_ptr, %rdi
    call kernel_main

    cli
1:  hlt
    jmp 1b

.section .bss
.align 4096
p4_table:
    .skip 4096
p3_table:
    .skip 4096
p2_table:
    .skip 4096
stack_bottom:
    .skip 16384
stack_top:

mbi_ptr:
    .skip 4
