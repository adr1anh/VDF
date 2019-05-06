//
//  Group.c
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include "Group.h"
#include "Extra.h"

void RSA_generate_prime(mpz_t pk, gmp_randstate_t state, int rep, mp_bitcnt_t modulus)
{
    mpz_t p, q, bound;
    mpz_inits(p, q, bound, NULL);
    
    mp_bitcnt_t n;
    
    n = modulus/2;
    mpz_pow2(bound, n - 1);
    
    while (mpz_probab_prime_p(p, rep) == 0 || mpz_cmp(p, bound) < 0) {
        mpz_urandomb(p, state, n);
    }
    
    n = modulus - n;
    mpz_pow2(bound, n - 1);
    
    while (mpz_probab_prime_p(q, rep) == 0 || mpz_cmp(q, bound) < 0) {
        mpz_urandomb(q, state, n);
    }
    
    mpz_mul(pk, p, q);
    
    mpz_clears(p, q, bound, NULL);
}

void RSA_hash(mpz_t rop, const mpz_t g, const mpz_t mod)
{
    hash(rop, g);
    mpz_mod(rop, rop, mod);
}

// Do t squarings of rop
void RSA_sequential_square(mpz_t rop, unsigned long int t, const mpz_t mod) {
    for (unsigned long int i = 0; i < t; ++i) {
        mpz_mul(rop, rop, rop);
        mpz_mod(rop, rop, mod);
    }
}

void RSA_sequential_square_precomp(mpz_t rop,
                                   mpz_t precomp[],
                                   mpz_t g,
                                   unsigned long int k,
                                   unsigned long int gamma,
                                   unsigned long int t,
                                   const mpz_t mod)
{
    mpz_set(rop, g);
    for (unsigned long int i = 0; i < t; ++i) {
        if (i % (k * gamma) == 0) {
            mpz_set(precomp[i / (k * gamma)], rop);
        }
        mpz_mul(rop, rop, rop);
        mpz_mod(rop, rop, mod);
    }
}

void RSA_mul(mpz_t rop, const mpz_t op1, const mpz_t op2, const mpz_t pk)
{
    mpz_mul(rop, op1, op2);
    mpz_mod(rop, rop, pk);
}

void RSA_pow(mpz_t rop, const mpz_t op1, const mpz_t exp, const mpz_t pk)
{
     mpz_powm(rop, op1, exp, pk);
}

void RSA_pow_ui(mpz_t rop, const mpz_t op1, unsigned long int exp, const mpz_t pk)
{
    mpz_powm_ui(rop, op1, exp, pk);
}

