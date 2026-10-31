# preeny-sovereign

**Sovereign OS** — pure, deterministic, freestanding, bootable kernel.

No libc. No heap. No interrupts beyond halt. No non-deterministic time, random, or process creation.

## Layers

1. **Pure C stubs** (`src/`) — constant replacements for classic LD_PRELOAD hooks.
2. **Sovereign Fortran kernels** (`kernels/`) — pure `BIND(C)` numeric substrate.
3. **Userspace init** (`os/`) — process-1 style runner on the pure stubs.
4. **Bootable kernel** (`kernel/`) — Multiboot2 freestanding kernel that writes to VGA text memory, parses the memory map, and seals boot.

## Bootable kernel

Freestanding x86 Multiboot2 kernel:
- `kernel/boot.S` — Multiboot2 header + entry; passes info structure pointer to `kernel_main`
- `kernel/kernel.c` — VGA text output, Multiboot2 tag walker, memory map (type 6) parser, pure seal, `hlt` loop
- `kernel/linker.ld` — load at 1 MiB

### Build & boot

```bash
make kernel
# or full ISO (requires grub-mkrescue)
make iso

# QEMU
qemu-system-i386 -kernel kernel/kernel.elf
# or
qemu-system-i386 -cdrom kernel/sovereign.iso
```

Screen output includes the memory map (addresses, lengths, types: available / reserved / acpi_reclaim / …) between the boot and ready seals, then pure `hlt`.

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
make              # stubs + kernels + os + kernel
make kernel       # freestanding kernel.elf
make iso          # sovereign.iso (grub-mkrescue)
```

Requires `gcc`, `gfortran`, and (for ISO) `grub-mkrescue` / `xorriso`.

## License

C stubs derived from zardus/preeny (BSD-2-Clause). Fortran kernels, userspace init, and freestanding kernel are original sovereign pure code, provided as-is for deterministic environments.
