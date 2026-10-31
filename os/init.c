/*
 * Sovereign OS — process 1 (init)
 * Pure, deterministic, zero external dependencies beyond the pure stubs.
 * No fork, no alarm, no rand, no time progression unless explicitly sealed.
 */

#include <stdio.h>
#include <unistd.h>

/* Declarations matching pure Fortran kernels (BIND(C)) */
extern int sovereign_rand(void);
extern int sovereign_triangular(int n);
extern double sovereign_zero(void);

static void sovereign_seal(const char *event)
{
    /* Minimal WORM-style seal — in a real substrate this would hash to a chain.
       Here it is a pure log to stdout so the OS remains observable and deterministic. */
    printf("[SEAL] %s\n", event);
    fflush(stdout);
}

int main(void)
{
    sovereign_seal("sovereign_os_boot");

    /* Demonstrate pure stubs + kernels under the OS */
    printf("uid=%d euid=%d\n", getuid(), geteuid());
    printf("sovereign_rand=%d triangular(10)=%d zero=%g\n",
           sovereign_rand(), sovereign_triangular(10), sovereign_zero());

    sovereign_seal("sovereign_os_ready");

    /* Idle forever — no sleep, no alarm. Pure halt. */
    for (;;) {
        /* busy wait or pause; pause is pure enough under the stubs */
        pause();
    }

    return 0;
}
