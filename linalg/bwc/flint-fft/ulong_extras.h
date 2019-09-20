/* 
 * Copyright (C) 2006, 2007, 2008, 2009, 2016 William Hart Copyright (C)
 * 2008, Peter Shrimpton Copyright (C) 2009, Tom Boothby Copyright (C) 2010,
 * Fredrik Johansson
 * 
 * This file is part of FLINT.
 * 
 * FLINT is free software: you can redistribute it and/or modify it under the 
 * terms of the GNU Lesser General Public License (LGPL) as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.  See <http://www.gnu.org/licenses/>. */

#ifndef ULONG_EXTRAS_H
#define ULONG_EXTRAS_H

#ifdef ULONG_EXTRAS_INLINES_C
#define ULONG_EXTRAS_INLINE
#else
#define ULONG_EXTRAS_INLINE static __inline__
#endif

#include <gmp.h>
#include "flint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pair_s {
    ulong x, y;
} n_pair_t;

#define FLINT_MAX_FACTORS_IN_LIMB 15

typedef struct {
    int num;
    int exp[FLINT_MAX_FACTORS_IN_LIMB];
    ulong p[FLINT_MAX_FACTORS_IN_LIMB];
} n_factor_t;

#define FLINT_ODDPRIME_SMALL_CUTOFF 4096
#define FLINT_NUM_PRIMES_SMALL 172
#define FLINT_PRIMES_SMALL_CUTOFF 1030
#define FLINT_PSEUDOSQUARES_CUTOFF 1000

#define FLINT_FACTOR_TRIAL_PRIMES 3000
/* nth_prime(FLINT_FACTOR_TRIAL_PRIMES) */
#define FLINT_FACTOR_TRIAL_PRIMES_PRIME UWORD(27449)
#define FLINT_FACTOR_TRIAL_CUTOFF (UWORD(27449) * UWORD(27449))

#define FLINT_PRIMES_TAB_DEFAULT_CUTOFF 1000000

#define FLINT_FACTOR_SQUFOF_ITERS 50000
#define FLINT_FACTOR_ONE_LINE_MAX (UWORD(1)<<39)
#define FLINT_FACTOR_ONE_LINE_ITERS 40000

#define FLINT_PRIME_PI_ODD_LOOKUP_CUTOFF 311

#define FLINT_SIEVE_SIZE 65536

#if FLINT64
#define UWORD_MAX_PRIME UWORD(18446744073709551557)
#else
#define UWORD_MAX_PRIME UWORD(4294967291)
#endif

#define UWORD_HALF (UWORD_MAX / 2 + 1)

typedef struct {
    slong small_i;
    slong small_num;
    unsigned int *small_primes;

    ulong sieve_a;
    ulong sieve_b;
    slong sieve_i;
    slong sieve_num;
    char *sieve;
} n_primes_struct;

typedef n_primes_struct n_primes_t[1];

void n_primes_init(n_primes_t iter);

void n_primes_clear(n_primes_t iter);

void n_primes_extend_small(n_primes_t iter, ulong bound);

void n_primes_sieve_range(n_primes_t iter, ulong a, ulong b);

void n_primes_jump_after(n_primes_t iter, ulong n);

ULONG_EXTRAS_INLINE ulong n_primes_next(n_primes_t iter)
{
    if (iter->small_i < iter->small_num)
	return iter->small_primes[(iter->small_i)++];

    for (;;) {
	while (iter->sieve_i < iter->sieve_num)
	    if (iter->sieve[iter->sieve_i++] != 0)
		return iter->sieve_a + 2 * (iter->sieve_i - 1);

	if (iter->sieve_b == 0)
	    n_primes_jump_after(iter,
				iter->small_primes[iter->small_num - 1]);
	else
	    n_primes_jump_after(iter, iter->sieve_b);
    }
}

extern const unsigned int flint_primes_small[];

extern FLINT_TLS_PREFIX ulong *_flint_primes[FLINT_BITS];
extern FLINT_TLS_PREFIX double *_flint_prime_inverses[FLINT_BITS];
extern FLINT_TLS_PREFIX int _flint_primes_used;
#if defined(_OPENMP) && !defined(HAVE_TLS)
#pragma omp threadprivate(_flint_primes, _flint_prime_inverses, _flint_primes_used)
#endif

void n_compute_primes(ulong num_primes);

void n_cleanup_primes(void);

const ulong *n_primes_arr_readonly(ulong n);
const double *n_prime_inverses_arr_readonly(ulong n);

ulong n_randlimb(flint_rand_t state);

ulong n_randint(flint_rand_t state, ulong limit);

ulong n_urandint(flint_rand_t state, ulong limit);

ulong n_randbits(flint_rand_t state, unsigned int bits);

ulong n_randtest_bits(flint_rand_t state, int bits);

ulong n_randtest(flint_rand_t state);

ulong n_randtest_not_zero(flint_rand_t state);

ulong n_randprime(flint_rand_t state, ulong bits, int proved);

ulong n_randtest_prime(flint_rand_t state, int proved);

ulong n_pow(ulong n, ulong exp);

ulong n_flog(ulong n, ulong b);

ulong n_clog(ulong n, ulong b);

ULONG_EXTRAS_INLINE double n_precompute_inverse(ulong n)
{
    return (double) 1 / (double) n;
}

ULONG_EXTRAS_INLINE ulong n_preinvert_limb(ulong n)
{
    ulong norm, ninv;

    count_leading_zeros(norm, n);
    invert_limb(ninv, n << norm);

    return ninv;
}

ulong n_mod_precomp(ulong a, ulong n, double ninv);

ulong n_mod2_precomp(ulong a, ulong n, double ninv);

ulong n_divrem2_precomp(ulong * q, ulong a, ulong n, double npre);

ulong n_divrem2_preinv(ulong * q, ulong a, ulong n, ulong ninv);

ulong n_div2_preinv(ulong a, ulong n, ulong ninv);

ulong n_mod2_preinv(ulong a, ulong n, ulong ninv);

ulong n_ll_mod_preinv(ulong a_hi, ulong a_lo, ulong n, ulong ninv);

ulong n_lll_mod_preinv(ulong a_hi, ulong a_mi,
		       ulong a_lo, ulong n, ulong ninv);

ulong n_mulmod_precomp(ulong a, ulong b, ulong n, double ninv);

ULONG_EXTRAS_INLINE
    ulong n_mulmod2_preinv(ulong a, ulong b, ulong n, ulong ninv)
{
    ulong p1, p2;

    FLINT_ASSERT(n != 0);

    umul_ppmm(p1, p2, a, b);
    return n_ll_mod_preinv(p1, p2, n, ninv);
}

ULONG_EXTRAS_INLINE ulong n_mulmod2(ulong a, ulong b, ulong n)
{
    ulong p1, p2, ninv;

    FLINT_ASSERT(n != 0);

    ninv = n_preinvert_limb(n);
    umul_ppmm(p1, p2, a, b);
    return n_ll_mod_preinv(p1, p2, n, ninv);
}

ulong n_mulmod_preinv(ulong a, ulong b, ulong n, ulong ninv, ulong norm);

ulong n_powmod_ui_precomp(ulong a, ulong exp, ulong n, double npre);

ulong n_powmod_precomp(ulong a, slong exp, ulong n, double npre);

ULONG_EXTRAS_INLINE ulong n_powmod(ulong a, slong exp, ulong n)
{
    double npre = n_precompute_inverse(n);

    return n_powmod_precomp(a, exp, n, npre);
}

/* 
 * This function is in fmpz.h
 * 
 * ulong n_powmod2_fmpz_preinv(ulong a, const fmpz_t exp, ulong n, ulong
 * ninv); */
ulong n_powmod2_preinv(ulong a, slong exp, ulong n, ulong ninv);

ulong n_powmod2_ui_preinv(ulong a, ulong exp, ulong n, ulong ninv);

ulong n_powmod_ui_preinv(ulong a, ulong exp, ulong n, ulong ninv, ulong norm);

ULONG_EXTRAS_INLINE ulong n_powmod2(ulong a, slong exp, ulong n)
{
    ulong ninv;

    FLINT_ASSERT(n != 0);

    ninv = n_preinvert_limb(n);

    return n_powmod2_preinv(a, exp, n, ninv);
}

ULONG_EXTRAS_INLINE ulong n_addmod(ulong x, ulong y, ulong n)
{
    FLINT_ASSERT(x < n);
    FLINT_ASSERT(y < n);
    FLINT_ASSERT(n != 0);

    return (n - y > x ? x + y : x + y - n);
}

ULONG_EXTRAS_INLINE ulong n_submod(ulong x, ulong y, ulong n)
{
    FLINT_ASSERT(x < n);
    FLINT_ASSERT(y < n);
    FLINT_ASSERT(n != 0);

    return (y > x ? x - y + n : x - y);
}

ULONG_EXTRAS_INLINE ulong n_negmod(ulong x, ulong n)
{
    FLINT_ASSERT(x < n);
    FLINT_ASSERT(n != 0);

    return n_submod(0, x, n);
}

ulong n_sqrtmod(ulong a, ulong p);

slong n_sqrtmod_2pow(ulong ** sqrt, ulong a, slong exp);

slong n_sqrtmod_primepow(ulong ** sqrt, ulong a, ulong p, slong exp);

slong n_sqrtmodn(ulong ** sqrt, ulong a, n_factor_t * fac);

ulong n_gcd(ulong x, ulong y);

#define n_gcd_full n_gcd

ulong n_xgcd(ulong * a, ulong * b, ulong x, ulong y);

ulong n_gcdinv(ulong * a, ulong x, ulong y);

ULONG_EXTRAS_INLINE ulong n_invmod(ulong x, ulong y)
{
    ulong r, g;

    g = n_gcdinv(&r, x, y);
    if (g != 1)
	flint_throw(FLINT_IMPINV, "Cannot invert modulo %wd*%wd\n", g, g / y);

    return r;
}

ulong n_revbin(ulong in, ulong bits);

int n_jacobi(slong x, ulong y);

int n_jacobi_unsigned(ulong x, ulong y);

ulong n_sqrt(ulong a);

ulong n_sqrtrem(ulong * r, ulong a);

int n_is_square(ulong x);

double n_cbrt_estimate(double a);

ulong n_cbrt(ulong a);

ulong n_cbrt_binary_search(ulong x);

ulong n_cbrt_newton_iteration(ulong n);

ulong n_cbrt_chebyshev_approx(ulong n);

ulong n_cbrtrem(ulong * remainder, ulong n);

int n_is_perfect_power235(ulong n);

int n_is_perfect_power(ulong * root, ulong n);

int n_is_oddprime_small(ulong n);

int n_is_oddprime_binary(ulong n);

int n_is_probabprime_fermat(ulong n, ulong i);

int n_is_probabprime_fibonacci(ulong n);

int n_is_probabprime_lucas(ulong n);

int n_is_probabprime_BPSW(ulong n);

int n_is_strong_probabprime_precomp(ulong n, double npre, ulong a, ulong d);

int n_is_strong_probabprime2_preinv(ulong n, ulong ninv, ulong a, ulong d);

int n_is_probabprime(ulong n);

int n_is_prime_pseudosquare(ulong n);

int n_is_prime_pocklington(ulong n, ulong iterations);

int n_is_prime(ulong n);

ulong n_nth_prime(ulong n);

void n_nth_prime_bounds(ulong * lo, ulong * hi, ulong n);

ulong n_prime_pi(ulong n);

void n_prime_pi_bounds(ulong * lo, ulong * hi, ulong n);

int n_remove(ulong * n, ulong p);

int n_remove2_precomp(ulong * n, ulong p, double ppre);

ULONG_EXTRAS_INLINE void n_factor_init(n_factor_t * factors)
{
    factors->num = UWORD(0);
}

void n_factor_insert(n_factor_t * factors, ulong p, ulong exp);

ulong n_factor_trial_range(n_factor_t * factors,
			   ulong n, ulong start, ulong num_primes);

ulong n_factor_trial_partial(n_factor_t * factors, ulong n,
			     ulong * prod, ulong num_primes, ulong limit);

ulong n_factor_trial(n_factor_t * factors, ulong n, ulong num_primes);

ulong n_factor_partial(n_factor_t * factors,
		       ulong n, ulong limit, int proved);

ulong n_factor_power235(ulong * exp, ulong n);

ulong n_factor_one_line(ulong n, ulong iters);

ulong n_factor_lehman(ulong n);

ulong n_factor_SQUFOF(ulong n, ulong iters);

void n_factor(n_factor_t * factors, ulong n, int proved);

ulong n_factor_pp1(ulong n, ulong B1, ulong c);

int n_factor_pollard_brent_single(ulong * factor, ulong n,
				  ulong ninv, ulong ai,
				  ulong xi, ulong normbits, ulong max_iters);

int n_factor_pollard_brent(ulong * factor, flint_rand_t state,
			   ulong n_in, ulong max_tries, ulong max_iters);

int n_is_squarefree(ulong n);

int n_moebius_mu(ulong n);

void n_moebius_mu_vec(int *mu, ulong len);

ulong n_euler_phi(ulong n);

int n_sizeinbase(ulong n, int base);

ulong n_nextprime(ulong n, int proved);

ulong n_factorial_mod2_preinv(ulong n, ulong p, ulong pinv);

ulong n_factorial_fast_mod2_preinv(ulong n, ulong p, ulong pinv);

ulong n_primitive_root_prime_prefactor(ulong p, n_factor_t * factors);

ulong n_primitive_root_prime(ulong p);

ulong n_discrete_log_bsgs(ulong b, ulong a, ulong n);

ulong n_root_estimate(double a, int n);

ulong n_rootrem(ulong * remainder, ulong n, ulong root);

ulong n_root(ulong n, ulong root);

/***** ECM functions *********************************************************/

typedef struct n_ecm_s {

    ulong x, z;			/* the coordinates */
    ulong a24;			/* value (a + 2)/4 */
    ulong ninv;
    ulong normbits;
    ulong one;

    unsigned char *GCD_table;	/* checks whether baby step int is coprime to 
				 * Primorial or not */

    unsigned char **prime_table;

} n_ecm_s;

typedef n_ecm_s n_ecm_t[1];

void n_factor_ecm_double(ulong * x, ulong * z, ulong x0,
			 ulong z0, ulong n, n_ecm_t n_ecm_inf);

void n_factor_ecm_add(ulong * x, ulong * z, ulong x1,
		      ulong z1, ulong x2, ulong z2,
		      ulong x0, ulong z0, ulong n, n_ecm_t n_ecm_inf);

void n_factor_ecm_mul_montgomery_ladder(ulong * x, ulong * z,
					ulong x0, ulong z0,
					ulong k, ulong n, n_ecm_t n_ecm_inf);

int n_factor_ecm_select_curve(ulong * f, ulong sig, ulong n,
			      n_ecm_t n_ecm_inf);

int n_factor_ecm_stage_I(ulong * f, const ulong * prime_array,
			 ulong num, ulong B1, ulong n, n_ecm_t n_ecm_inf);

int n_factor_ecm_stage_II(ulong * f, ulong B1, ulong B2,
			  ulong P, ulong n, n_ecm_t n_ecm_inf);

int n_factor_ecm(ulong * f, ulong curves, ulong B1,
		 ulong B2, flint_rand_t state, ulong n);

mp_limb_t n_mulmod_precomp_shoup(mp_limb_t w, mp_limb_t p);

static __inline__
    mp_limb_t
n_mulmod_shoup(mp_limb_t w, mp_limb_t t, mp_limb_t w_precomp, mp_limb_t p)
{
    mp_limb_t q, r, p_hi, p_lo;

    umul_ppmm(p_hi, p_lo, w_precomp, t);
    q = p_hi;


    r = w * t;
    r -= q * p;

    if (r >= p) {
	r -= p;
    }

    return r;
}

#ifdef __cplusplus
}
#endif

#endif
