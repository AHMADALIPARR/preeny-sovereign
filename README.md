# preeny-sovereign

Sovereign minimal pure-stub LD_PRELOAD hooks derived from [zardus/preeny](https://github.com/zardus/preeny).

These are zero-dependency, deterministic replacements for the simplest interception modules. No `dlsym`, no logging, no environment-variable configuration, no glibc-specific workarounds.

## Pure stubs included

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

Complex modules that require `dlsym(RTLD_NEXT, …)` (desock, desock_dup, mallocwatch, etc.) are intentionally omitted. They cannot be reduced to pure stubs without losing their semantics.

## Build

```bash
make
```

Produces one `.so` per source file.

## Usage

```bash
LD_PRELOAD=./derand.so:./desleep.so:./dealarm.so ./target
```

## License

Derived from zardus/preeny (BSD-2-Clause). These minimal stubs are provided as-is for sovereign / deterministic environments.
