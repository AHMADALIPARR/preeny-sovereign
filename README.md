# preeny-sovereign

**Sovereign OS substrate** — pure, deterministic, zero-dependency foundation.

No `dlsym`. No logging side-channels. No environment variables. No non-deterministic time, random, or process creation. No external libraries.

Three layers:

1. **Pure C stubs** (`src/`) — LD_PRELOAD or linked replacements for `alarm`, `fork`, `ptrace`, `rand`, `sleep`, `getuid`, `time`, etc. Every call is a constant.
2. **Sovereign Fortran kernels** (`kernels/`) — pure `BIND(C)` numeric substrate (`sovereign_rand`, reductions, vector ops, φ-scale). Caller owns memory. All functions `pure`.
3. **Sovereign OS** (`os/`) — process 1 (`sovereign-init`) that boots, seals events, and runs on the pure stubs + kernels. Header `sovereign.h` defines the contracts.

Derived from the simplest modules of [zardus/preeny](https://github.com/zardus/preeny), reduced to pure stubs. Fortran kernels and OS layer are original sovereign code.

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

## Build

```bash
make              # stubs + kernels + sovereign-init
make stubs
make kernels
make os
```

Requires `gcc` and `gfortran`.

## Run the sovereign OS

```bash
make
LD_PRELOAD=./derand.so:./desleep.so:./dealarm.so:./deuid.so:./detime.so \
  ./os/sovereign-init
```

Output is deterministic:

```
[SEAL] sovereign_os_boot
uid=0 euid=0
sovereign_rand=42 triangular(10)=55 zero=0
[SEAL] sovereign_os_ready
```

The process then pauses. No sleep, no fork, no time progression.

## License

C stubs derived from zardus/preeny (BSD-2-Clause). Fortran kernels and OS layer are original sovereign pure code, provided as-is for deterministic environments.
