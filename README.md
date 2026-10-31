# preeny-sovereign

Sovereign minimal pure-stub LD_PRELOAD hooks and pure Fortran compute kernels.

Zero-dependency, fully deterministic. No `dlsym`, no logging, no environment variables, no external libraries, no non-deterministic behavior.

Derived from the simplest modules of [zardus/preeny](https://github.com/zardus/preeny), reduced to pure stubs. Fortran kernels added as a sovereign numeric substrate.

## Pure C stubs

| Module       | Behavior |
|--------------|----------|
| `dealarm.so` | `alarm()` always returns 0 |
| `defork.so`  | `fork()` always returns 0 |
| `deptrace.so`| `ptrace()` always returns 0 |
| `derand.so`  | `rand()` / `random()` always return 42 |
| `desigact.so`| `sigaction()` always returns 0 |
| `desleep.so` | `sleep()` / `usleep()` / `nanosleep()` return immediately |
| `deuid.so`   | `getuid()` / `geteuid()` always return 0 |
| `detime.so`  | `time()` / `gettimeofday()` return 0 |
| `startstop.so`| Process receives SIGSTOP on load |

Complex modules requiring `dlsym(RTLD_NEXT, …)` are omitted.

## Sovereign Fortran kernels

Pure, deterministic, `BIND(C)` interfaces in `kernels/`:

| Symbol                  | Meaning |
|-------------------------|---------|
| `sovereign_rand`        | Always returns 42 |
| `sovereign_triangular`  | n(n+1)/2 |
| `sovereign_identity`    | Identity |
| `sovereign_zero`        | Zero |
| `sovereign_square`      | x² |
| `sovereign_phi_scale`   | x / φ |
| `sovereign_dot`         | Dot product |
| `sovereign_sum`         | Sum reduction |
| `sovereign_fill`        | Constant fill |
| `sovereign_scale`       | In-place scale |

All functions are `pure`. No allocation inside kernels. Caller owns memory for vector routines.

## Build

```bash
make          # C stubs + Fortran kernels
make stubs    # C only
make kernels  # Fortran only
```

Requires `gcc` and `gfortran`.

## Usage

```bash
# LD_PRELOAD pure stubs
LD_PRELOAD=./derand.so:./desleep.so:./dealarm.so ./target

# Link or dlopen Fortran kernels (BIND(C) symbols)
gcc -o demo demo.c kernels/sovereign_math.so -ldl
```

## License

C stubs derived from zardus/preeny (BSD-2-Clause). Fortran kernels are original sovereign pure code, provided as-is for deterministic environments.
