# preeny-sovereign

**Sovereign OS** — pure, deterministic, freestanding, bootable kernel.

No libc. No heap. No interrupts beyond halt. No non-deterministic time, random, or process creation.

## Layers

1. **Pure C stubs** (`src/`) — constant replacements for classic LD_PRELOAD hooks.
2. **Sovereign Fortran kernels** (`kernels/`) — pure `BIND(C)` numeric substrate.
3. **Userspace init** (`os/`) — process-1 style runner on the pure stubs.
4. **Bootable kernels** — Multiboot2 freestanding kernels (32-bit and 64-bit) that write to VGA, parse the memory map, and seal boot.

## Bootable kernels

### 32-bit (`kernel/`)
- `kernel/boot.S` — Multiboot2 header + entry; passes info structure pointer to `kernel_main`
- `kernel/kernel.c` — VGA text output, Multiboot2 tag walker, memory map (type 6) parser, pure seal, `hlt` loop
- `kernel/linker.ld` — load at 1 MiB

```bash
make kernel
qemu-system-i386 -kernel kernel/kernel.elf
```

### 64-bit (`kernel64/`)
Long-mode kernel: Multiboot2 entry in 32-bit, then switches to x86_64 (identity-mapped 2 MiB pages, GDT, EFER.LME, paging).
- `kernel64/boot.S` — Multiboot2 header, page tables, long-mode switch, passes MBI in `%rdi`
- `kernel64/kernel.c` — same VGA + memory map parser, 64-bit seals
- `kernel64/linker.ld` — load at 1 MiB

```bash
make kernel64
qemu-system-x86_64 -kernel kernel64/kernel64.elf
```

Screen output includes the memory map (addresses, lengths, types) between the boot and ready seals, then pure `hlt`.

## Pure C stubs

| Module       | Behavior |
|--------------|----------|
| `dealarm.so` | `alarm()` → 0 |
| `defork.so`  | `fork()` → 0 |
| `deptrace.so`| `ptrace()` → 0 |
| `derand.so`  | `rand()` / `random()` → 42 |
| `desigact.so`| `sigaction()` → 0 |
| `desleep.so` | `sleep()` / `usleep()` / `nanosleep()` → 0 |
| `deuid.so`   | `getuid()` / `geteuid()` → 0 |
| `detime.so`  | `time()` / `gettimeofday()` → 0 |
| `startstop.so`| SIGSTOP on load |

## Fortran kernels

| Symbol                  | Meaning |
|-------------------------|---------|
| `sovereign_rand`        | 42 |
| `sovereign_triangular`  | n(n+1)/2 |
| `sovereign_identity`    | x |
| `sovereign_zero`        | 0.0 |
| `sovereign_square`      | x² |
| `sovereign_phi_scale`   | x / φ |
| `sovereign_dot`         | dot product |
| `sovereign_sum`         | sum |
| `sovereign_fill`        | constant fill |
| `sovereign_scale`       | in-place scale |

## Full build

```bash
make              # stubs + kernels + os + kernel + kernel64
make kernel       # 32-bit freestanding kernel.elf
make kernel64     # 64-bit freestanding kernel64.elf
make iso          # sovereign.iso (grub-mkrescue)
```

Requires `gcc`, `gfortran`, and (for ISO) `grub-mkrescue` / `xorriso`.

## License

C stubs derived from zardus/preeny (BSD-2-Clause). Fortran kernels, userspace init, and freestanding kernels are original sovereign pure code, provided as-is for deterministic environments.
