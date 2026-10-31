/*
 * Sovereign freestanding kernel
 * Pure, deterministic, no libc, no heap, no interrupts beyond halt.
 * Prints to VGA text buffer and seals boot.
 */

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static uint16_t *const vga = (uint16_t *)VGA_MEMORY;
static size_t row = 0, col = 0;

static void vga_put(char c, uint8_t color)
{
    if (c == '\n') {
        col = 0;
        if (++row >= VGA_HEIGHT) row = 0;
        return;
    }
    vga[row * VGA_WIDTH + col] = (uint16_t)c | ((uint16_t)color << 8);
    if (++col >= VGA_WIDTH) {
        col = 0;
        if (++row >= VGA_HEIGHT) row = 0;
    }
}

static void vga_write(const char *s, uint8_t color)
{
    while (*s) vga_put(*s++, color);
}

static void sovereign_seal(const char *event)
{
    vga_write("[SEAL] ", 0x0A);
    vga_write(event, 0x0F);
    vga_write("\n", 0x0F);
}

void kernel_main(void)
{
    /* clear screen */
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = (uint16_t)' ' | (0x0F << 8);

    sovereign_seal("sovereign_kernel_boot");
    vga_write("uid=0 euid=0\n", 0x07);
    vga_write("sovereign_rand=42 triangular(10)=55 zero=0\n", 0x07);
    sovereign_seal("sovereign_kernel_ready");

    /* pure halt — no sleep, no time, no interrupts */
    for (;;) {
        asm volatile("hlt");
    }
}
