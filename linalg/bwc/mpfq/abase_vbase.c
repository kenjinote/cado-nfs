/* MPFQ generated file -- do not edit */

#define _POSIX_C_SOURCE 200112L
#include <stdarg.h>
#include "abase_vbase.h"
#include "abase_u64k1.h"
#include "abase_u64k1_t.h"
#include "abase_u64k2.h"
#include "abase_u64k2_t.h"
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_1
#include "abase_p_1.h"
#include "abase_p_1_t.h"
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_1 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_2
#include "abase_p_2.h"
#include "abase_p_2_t.h"
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_2 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_3
#include "abase_p_3.h"
#include "abase_p_3_t.h"
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_3 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_4
#include "abase_p_4.h"
#include "abase_p_4_t.h"
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_4 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_8
#include "abase_p_8.h"
#include "abase_p_8_t.h"
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_8 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_pz
#include "abase_pz.h"
#include "abase_pz_t.h"
#endif /* COMPILE_MPFQ_PRIME_FIELD_pz */
void abase_vbase_oo_field_init_byfeatures(abase_vbase_ptr v, ...)
{
        va_list ap;
        va_start(ap, v);
        mpz_t p;
        mpz_init_set_ui(p, 2);
        int groupsize = 1;
        for(int a ; (a = va_arg(ap, int)) != 0 ; ) {
            if (a == MPFQ_PRIME_MPZ) {
                mpz_set(p, va_arg(ap, mpz_srcptr));
            } else if (a == MPFQ_GROUPSIZE) {
                groupsize = va_arg(ap, int);
            } else {
                /* We do not support MPFQ_PRIME_MPN. Only MPFQ_PRIME_MPZ*/
                fprintf(stderr, "Feature code %d unsupported\n", a);
                exit(1);
            }
        }
        va_end(ap);
        if (0) {
        } else if (groupsize == 64 && mpz_cmp_ui(p, 2) == 0) {
            abase_u64k1_oo_field_init(v);
        } else if (groupsize == 128 && mpz_cmp_ui(p, 2) == 0) {
            abase_u64k2_oo_field_init(v);
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_1
        } else if (groupsize == 1 && mpz_size(p) == 1) {
            abase_p_1_oo_field_init(v);
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_1 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_2
        } else if (groupsize == 1 && mpz_size(p) == 2) {
            abase_p_2_oo_field_init(v);
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_2 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_3
        } else if (groupsize == 1 && mpz_size(p) == 3) {
            abase_p_3_oo_field_init(v);
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_3 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_4
        } else if (groupsize == 1 && mpz_size(p) == 4) {
            abase_p_4_oo_field_init(v);
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_4 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_p_8
        } else if (groupsize == 1 && mpz_size(p) == 8) {
            abase_p_8_oo_field_init(v);
#endif /* COMPILE_MPFQ_PRIME_FIELD_p_8 */
#ifdef COMPILE_MPFQ_PRIME_FIELD_pz
        } else if (groupsize == 1 && 1) {
            abase_pz_oo_field_init(v);
#endif /* COMPILE_MPFQ_PRIME_FIELD_pz */
        } else {
            gmp_fprintf(stderr, "Unsupported combination: group size = %d, p = %Zd, %zu limbs\n", groupsize, p, mpz_size(p));
            exit(1);
        }
        v->field_specify(v, MPFQ_PRIME_MPZ, p);
        v->field_specify(v, MPFQ_GROUPSIZE, &groupsize);
        mpz_clear(p);
}

void abase_vbase_oo_init_templates(abase_vbase_tmpl_ptr w, abase_vbase_ptr v0, abase_vbase_ptr v1)
{
    const char * s0 = v0->impl_name();
    const char * s1 = v1->impl_name();
    if (0) {
    } else if (strcmp(s0, "u64k1") == 0 && strcmp(s1, "u64k1") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_u64k1_u64k1_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_u64k1_u64k1_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_u64k1_u64k1_transpose;
    } else if (strcmp(s0, "u64k1") == 0 && strcmp(s1, "u64k2") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_u64k1_u64k2_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_u64k1_u64k2_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_u64k1_u64k2_transpose;
    } else if (strcmp(s0, "u64k2") == 0 && strcmp(s1, "u64k1") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_u64k2_u64k1_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_u64k2_u64k1_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_u64k2_u64k1_transpose;
    } else if (strcmp(s0, "u64k2") == 0 && strcmp(s1, "u64k2") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_u64k2_u64k2_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_u64k2_u64k2_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_u64k2_u64k2_transpose;
#if defined(COMPILE_MPFQ_PRIME_FIELD_p_1)
    } else if (strcmp(s0, "p_1") == 0 && strcmp(s1, "p_1") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_p_1_p_1_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_p_1_p_1_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_p_1_p_1_transpose;
#endif /* defined(COMPILE_MPFQ_PRIME_FIELD_p_1) */
#if defined(COMPILE_MPFQ_PRIME_FIELD_p_2)
    } else if (strcmp(s0, "p_2") == 0 && strcmp(s1, "p_2") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_p_2_p_2_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_p_2_p_2_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_p_2_p_2_transpose;
#endif /* defined(COMPILE_MPFQ_PRIME_FIELD_p_2) */
#if defined(COMPILE_MPFQ_PRIME_FIELD_p_3)
    } else if (strcmp(s0, "p_3") == 0 && strcmp(s1, "p_3") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_p_3_p_3_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_p_3_p_3_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_p_3_p_3_transpose;
#endif /* defined(COMPILE_MPFQ_PRIME_FIELD_p_3) */
#if defined(COMPILE_MPFQ_PRIME_FIELD_p_4)
    } else if (strcmp(s0, "p_4") == 0 && strcmp(s1, "p_4") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_p_4_p_4_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_p_4_p_4_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_p_4_p_4_transpose;
#endif /* defined(COMPILE_MPFQ_PRIME_FIELD_p_4) */
#if defined(COMPILE_MPFQ_PRIME_FIELD_p_8)
    } else if (strcmp(s0, "p_8") == 0 && strcmp(s1, "p_8") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_p_8_p_8_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_p_8_p_8_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_p_8_p_8_transpose;
#endif /* defined(COMPILE_MPFQ_PRIME_FIELD_p_8) */
#if defined(COMPILE_MPFQ_PRIME_FIELD_pz)
    } else if (strcmp(s0, "pz") == 0 && strcmp(s1, "pz") == 0) {
        w->dotprod = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, const void *, unsigned int)) abase_pz_pz_dotprod;
        w->addmul_tiny = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *, void *, unsigned int)) abase_pz_pz_addmul_tiny;
        w->transpose = (void (*) (abase_vbase_ptr, abase_vbase_ptr, void *, const void *)) abase_pz_pz_transpose;
#endif /* defined(COMPILE_MPFQ_PRIME_FIELD_pz) */
    } else {
        abort();
    }
}


/* vim:set ft=cpp: */