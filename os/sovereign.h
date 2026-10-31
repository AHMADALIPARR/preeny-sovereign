/*
 * Sovereign OS substrate header
 * Pure deterministic contracts for the OS layer.
 */

#ifndef SOVEREIGN_H
#define SOVEREIGN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Pure stub contracts (implemented by LD_PRELOAD or linked stubs) */
unsigned int alarm(unsigned int seconds);          /* always 0 */
pid_t fork(void);                                  /* always 0 */
long ptrace(int, int, int, int);                   /* always 0 */
int rand(void);                                    /* always 42 */
unsigned int sleep(unsigned int);                  /* always 0 */
uid_t getuid(void);                                /* always 0 */
uid_t geteuid(void);                               /* always 0 */
time_t time(time_t *);                             /* always 0 */

/* Fortran kernel contracts (BIND(C)) */
int sovereign_rand(void);
int sovereign_triangular(int n);
double sovereign_identity(double x);
double sovereign_zero(void);
double sovereign_square(double x);
double sovereign_phi_scale(double x);
double sovereign_dot(int n, const double *a, const double *b);
double sovereign_sum(int n, const double *a);
void sovereign_fill(int n, double *a, double val);
void sovereign_scale(int n, double *a, double s);

/* OS seal primitive (pure log; replace with WORM chain in full substrate) */
void sovereign_seal(const char *event);

#ifdef __cplusplus
}
#endif

#endif /* SOVEREIGN_H */
