//
//  Testing.c
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include "Testing.h"
#include "Extra.h"
#include "Group.h"
#include "VDF.h"

int test_generate_proof(mpz_t x, mpz_t id, unsigned long int t, unsigned long int k, unsigned long int gamma, const mpz_t pk)
{
    mpz_t y, l, exp, proof;
    mpz_inits(y, l, exp, proof, NULL);
    
    unsigned long int bound = t / (k * gamma) + (t % (k * gamma) != 0);
    
    mpz_t precomp[bound + 1];
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_init(precomp[i]);
    }
    
    RSA_sequential_square_precomp(y, precomp, x, k, gamma, t, pk);
    
    // l = Hprime(x||y)
    hash_prime(l, x, y, 50);
    
    // exp = floor(2^t/l)
    mpz_pow2(exp, t);
    mpz_fdiv_q(exp, exp, l);
    
    // y = g^floor(2^t/l)
    // Group op: y = x ^ exp
    mpz_powm(y, x, exp, pk);
    
    generate_proof(proof, id, x, l, precomp, bound, k, gamma, t, pk);
    
    int res = mpz_cmp(proof, y);
    mpz_clears(y, l, exp, proof, NULL);
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_clear(precomp[i]);
    }
    return res;
}

//verify that sum_i(get_block(i)*2^(ki)) = floor(2^t/l)
int test_get_block(unsigned long t, unsigned long k, mpz_t const l)
{
    mpz_t exp, test, pow_two, block;
    mpz_inits(exp, pow_two, test, block, NULL);
    
    // exp = floor(2^t/l)
    mpz_pow2(exp, t);
    mpz_fdiv_q(exp, exp, l);
    
    unsigned long int i = 0;
    unsigned long int b;
    while (t > k * (i + 1)) {
        get_block(block, i, t, k, l);
        b = mpz_get_ui(block);
        
        mpz_pow2(pow_two, k * i);
        mpz_mul_ui(pow_two, pow_two, b);
        mpz_add(test, test, pow_two);
        ++i;
    }
    
    int res = mpz_cmp(test, exp);
    
    mpz_clears(exp, pow_two, test, NULL);
    return res;
}
