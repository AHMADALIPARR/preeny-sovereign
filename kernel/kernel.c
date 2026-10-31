/*
 * Sovereign freestanding kernel
 * Pure, deterministic, no libc, no heap.
 * Parses Multiboot2 memory map and prints to VGA.
 */

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289
#define MULTIBOOT_TAG_TYPE_END      0
#define MULTIBOOT_TAG_TYPE_MMAP     6

struct multiboot_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot_mmap_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t zero;
};

struct multiboot_tag_mmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    struct multiboot_mmap_entry entries[];
};

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

static void vga_hex(uint64_t val, uint8_t color)
{
    const char *hex = "0123456789ABCDEF";
    char buf[17];
    buf[16] = 0;
    for (int i = 15; i >= 0; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }
    vga_write(buf, color);
}

static void sovereign_seal(const char *event)
{
    vga_write("[SEAL] ", 0x0A);
    vga_write(event, 0x0F);
    vga_write("\n", 0x0F);
}

static const char *mmap_type_str(uint32_t t)
{
    switch (t) {
    case 1: return "available";
    case 2: return "reserved";
    case 3: return "acpi_reclaim";
    case 4: return "acpi_nvs";
    case 5: return "bad";
    default: return "unknown";
    }
}

static void parse_mmap(void *mbi)
{
    if (!mbi) {
        vga_write("no multiboot info\n", 0x0C);
        return;
    }

    uint32_t total = *(uint32_t *)mbi;
    struct multiboot_tag *tag = (struct multiboot_tag *)((uint8_t *)mbi + 8);
    struct multiboot_tag *end = (struct multiboot_tag *)((uint8_t *)mbi + total);

    vga_write("memory map:\n", 0x07);

    while (tag < end && tag->type != MULTIBOOT_TAG_TYPE_END) {
        if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
            struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
            uint32_t entry_size = mmap->entry_size;
            uint8_t *entry_ptr = (uint8_t *)mmap->entries;
            uint8_t *mmap_end = (uint8_t *)mmap + mmap->size;

            while (entry_ptr + sizeof(struct multiboot_mmap_entry) <= mmap_end) {
                struct multiboot_mmap_entry *e = (struct multiboot_mmap_entry *)entry_ptr;
                vga_write("  ", 0x07);
                vga_hex(e->addr, 0x0B);
                vga_write(" len ", 0x07);
                vga_hex(e->len, 0x0B);
                vga_write(" ", 0x07);
                vga_write(mmap_type_str(e->type), 0x0E);
                vga_write("\n", 0x07);
                entry_ptr += entry_size;
            }
        }
        tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7));
    }
}

void kernel_main(void *mbi)
{
    /* clear screen */
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = (uint16_t)' ' | (0x0F << 8);

    sovereign_seal("sovereign_kernel_boot");
    parse_mmap(mbi);
    sovereign_seal("sovereign_kernel_ready");

    for (;;) {
        asm volatile("hlt");
    }
}
