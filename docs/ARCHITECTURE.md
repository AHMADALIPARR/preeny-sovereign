# Architecture

Visual reference for the Sovereign OS stack.

---

## System map

File: [images/system-map.svg](images/system-map.svg)

```mermaid
flowchart TB
    subgraph Userspace[Userspace]
        STUBS[Pure C stubs]
        FORT[Fortran kernels]
        INIT[OS init]
    end
    subgraph Kernels[Bootable kernels]
        K32[32-bit Multiboot2]
        K64[64-bit long mode]
    end
    STUBS --> INIT
    FORT --> INIT
    K32 --> VGA[VGA seals and halt]
    K64 --> VGA
```

---

## Boot path

File: [images/boot-path.svg](images/boot-path.svg)

```mermaid
flowchart TD
    subgraph K32[32-bit]
        A1[Multiboot entry] --> A2[kernel_main]
        A2 --> A3[VGA mmap seal]
        A3 --> A4[halt]
    end

    subgraph K64[64-bit]
        B1[Multiboot 32-bit entry] --> B2[Page tables]
        B2 --> B3[GDT LME paging]
        B3 --> B4[Far jump to 64-bit]
        B4 --> B5[kernel_main]
        B5 --> B6[VGA mmap seal]
        B6 --> B7[halt]
    end
```

**32-bit** — Multiboot2 enters protected mode and jumps straight to kernel_main.  
**64-bit** — same entry, then builds identity maps, loads GDT, enables long mode, then kernel_main with MBI in rdi.

Both paths clear VGA, emit SEAL sovereign_kernel_boot, walk Multiboot tag type 6, emit SEAL sovereign_kernel_ready, then halt forever.

---

## Component graph

```mermaid
flowchart TB
    src_c[src C files] --> stubs[shared object stubs]
    kernels_f[kernels Fortran] --> fkernels[Fortran objects]
    fkernels --> init[os init]
    os_c[os init.c] --> init
    k_boot[kernel boot.S] --> k32[kernel.elf]
    k_c[kernel kernel.c] --> k32
    k64_boot[kernel64 boot.S] --> k64[kernel64.elf]
    k64_c[kernel64 kernel.c] --> k64
    k32 --> iso[sovereign.iso]
```

**Layout**

- `src/` — pure deterministic C stubs  
- `kernels/` — Fortran 2008 math and vector kernels  
- `os/` — init and sovereign.h contracts  
- `kernel/` — 32-bit Multiboot2 freestanding kernel  
- `kernel64/` — 64-bit long-mode freestanding kernel  
- `docs/images/` — SVG assets  

---

## Memory and I/O

```mermaid
flowchart LR
    MAIN[kernel_main] --> SEAL[sovereign_seal]
    MAIN --> PARSE[parse_mmap]
    PARSE --> RAM[Physical RAM via Multiboot]
    SEAL --> VGA[VGA text buffer]
    PARSE --> VGA
    MAIN --> HLT[halt loop]
```

- **0xB8000** — VGA text 80 by 25  
- **Multiboot2 info** — total size and tags from boot.S  
- **Tag type 6** — memory map entries addr, len, type  
- **No heap** — stack only; static VGA pointer  

Mmap type names: available, reserved, acpi_reclaim, acpi_nvs, bad, unknown.

---

## Stubs and contracts

| Module | Behavior |
| --- | --- |
| dealarm.c | alarm returns 0 |
| defork.c | fork returns 0 |
| deptrace.c | ptrace returns 0 |
| derand.c | rand random return 42 |
| desigact.c | sigaction returns 0 |
| desleep.c | sleep usleep nanosleep return 0 |
| detime.c | time gettimeofday return 0 |
| deuid.c | getuid geteuid return 0 |
| startstop.c | SIGSTOP on load |

Fortran surface in sovereign.h: scalars rand, triangular, identity, zero, square, phi_scale; vectors dot, sum, fill, scale. All BIND(C).

---

## Seal protocol

```mermaid
sequenceDiagram
    participant K as Kernel
    participant V as VGA

    K->>V: clear screen
    K->>V: SEAL sovereign_kernel_boot
    K->>V: memory map lines
    K->>V: SEAL sovereign_kernel_ready
    Note over K: No further output pure halt
```

Seals are the only intentional side-effect channel after boot. No wall clock, no random.

---

## Related

- [README](../README.md)  
- Upstream stubs: [zardus/preeny](https://github.com/zardus/preeny)
