//
//  VDF.c
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "VDF.h"
#include "Extra.h"



uint64_t precomputed_lenth(uint64_t t, uint64_t gamma, uint8_t k) {
    return t / (gamma * k) + (t % (gamma * k) != 0);
}

uint8_t find_optimal_k(uint64_t t, uint64_t gamma) {
    // The choice of gamma should be around t^(1/2) as this is a halfway between
    // memory and computation optimality
    
    // We use Newton's method for finding the minimum of f(k) = t/k + gamma*2^(k+1)
    // That is, a zero of g(k) = f'(k)
    // g(k) = -t/k^2 + gamma*ln(2)*2^(k+1)
    // g'(k) = 2*t/k^3 + gamma*ln(2)^2*2^(k+1)
    //
    // x(n+1) = x(n) - g(x(n))/g'(x(n))
    
    // For t =2^26 and k is opt 31s
    // For t =2^26 and k is opt 32.7s
//    return log(t)/3;
    double xn, xn1, ln2, tmp;
    // We use an int for k because it will never be larger than 64 (not bits!)
    // The zero of g' increases logarithmically in t
    int k;
    xn = 1;
    k = 0;
    ln2 = log(2);


    // Check if this works
//    while (rint(xn) != k) {
    for (int i = 0; i<5; ++i){
        tmp = ln2 * gamma * exp2(xn + 1);
        xn1 = xn - (- (double)t/(xn*xn) + tmp) / (2*(double)t/(xn*xn*xn) + ln2*tmp);
        xn = xn1;
        k = rint(xn1);
    }
    return (uint8_t)k;
}


void eval(GroupElement output,
          GroupElement proof,
          const GroupElement input,
          uint64_t t)
{
    mpz_t* context = group_get_context(input);
    
    GroupElement input_hashed = group_init(context);
    GroupElement identity = group_get_identity(context);
    
    mpz_t prime;
    mpz_init(prime); //Bane of my existence
    
    uint64_t gamma = sqrt(t);
    uint8_t k = find_optimal_k(t, gamma);
    // This is the size of the array of intermediary calculations of g^2^t
    uint64_t bound = precomputed_lenth(t, gamma, k);
    
    // Hash our input to a random element of the group
    group_hash(input_hashed, input);
    group_set(output, input_hashed);
    
    // Calculate g^(2^t) and save every kappa*gamma steps
    GroupElement precomputed[bound+1];
    for (uint64_t i = 0; i < t; ++i) {
        if (i % (gamma * k) == 0) {
            precomputed[i / (gamma * k)] = group_init_set(output);
        }
        // g = g*g
        group_square(output, output);
    }
    precomputed[bound] = group_init_set(output);
    
    // Find a random prime number in the range of Primes(2k) = Primes(256)
    // depending on the g and y
    // l = Hprime(g||y)
    hash_prime(prime, input, output, 10);
    
    generate_proof(proof, identity, input_hashed, precomputed, prime, t, gamma, k);
    
    // clear mpz
    group_clear(input_hashed);
    group_clear(identity);
    mpz_clear(prime);
    
    for (uint64_t i = 0; i < bound; ++i) {
        group_clear(precomputed[i]);
    }
}

int verify(const GroupElement input,
           const GroupElement output,
           const GroupElement proof,
           uint64_t t)
{
    mpz_t* context = group_get_context(input);
    mpz_t r, prime;
    
    GroupElement input_hashed = group_init(context);
    GroupElement identity = group_get_identity(context);
    GroupElement input_r = group_init(context);
    GroupElement proof_prime = group_init(context);

    mpz_inits(prime, NULL);
    mpz_init_set_ui(r, 2);

    
    group_hash(input_hashed, input);
    
    hash_prime(prime, input, output, 10);
    
    // r = 2^t (mod prime)
    mpz_powm_ui(r, r, t, prime);
    
    // input_r = g^r
    group_pow(input_r, input_hashed, r);
    
    // proof_prime = pi^l
    group_pow(proof_prime, proof, prime);

    group_mul(proof_prime, proof_prime, input_r);
    
    // res = true if g^r * pi^l == y
    int res = group_cmp(proof_prime, output);
    
    mpz_clears(r, prime, NULL);
    group_clear(input_hashed);
    group_clear(identity);
    group_clear(input_r);
    group_clear(proof_prime);

    return res;
}

void generate_proof(GroupElement proof,
                    const GroupElement identity,
                    const GroupElement input_hashed,
                    const GroupElement* precomputed,
                    const mpz_t prime,
                    uint64_t t,
                    uint64_t gamma,
                    uint8_t k)
{
    mpz_t* context = group_get_context(input_hashed);
    mpz_t block, exp;
    uint8_t k0, k1;
    uint64_t b, bound, exp_uint64;
    
    // output = id
    group_set(proof, identity);
    GroupElement tmp = group_init(context);
    
    mpz_inits(block, exp, NULL);
    
    // Number of precomputed squarings of input
    bound = precomputed_lenth(t, gamma, k);
    
    // k1 = floor(k/2)
    k1 = k / 2;
    
    // k0 = k - k1
    k0 = k - k1;
    
    // allocate 2^k GrouoElements representing the base
    GroupElement y[1<<k];
    for (b = 0; b < (1 << k); ++b) {
        y[b] = group_init(context);
    }

    uint64_t j = gamma;
    while (j > 0) {
        j--;
        // x = x ^ (2^k)
        group_seq_square(proof, proof, k);
        
        // y[b] = id
        for (b = 0; b < (1 << k); ++b) {
            group_set(y[b], identity);
        }
    
        for (uint64_t i = 0; t >= k * (i * gamma + j + 1) ; ++i) {
            get_block(block, i * gamma + j, t, k, prime);
            b = mpz_get_ui(block);
            // y_b = y_b * Ci
            group_mul(y[b], y[b], precomputed[i]);
        }
        
        // We should be able to do both of these two following loops in parallel
        for (uint64_t b1 = 0; b1 < (1<<k1); ++b1) {
            group_set(tmp, identity);
            for (uint64_t b0 = 0; b0 < (1<<k0); ++b0) {
                group_mul(tmp, tmp, y[b1 * (1<<k0) + b0]);
            }
            exp_uint64 = b1 * (1 << k0);
            // Set exp to exp_uint64
            mpz_import(exp, 1, -1, sizeof(uint64_t), 0, 0, &exp_uint64);
            group_pow(tmp, tmp, exp);
            
            group_mul(proof, proof, tmp);
        }
        
        for (uint64_t b0 = 0; b0 < (1<<k0); ++b0) {
            group_set(tmp, identity);
            for (uint64_t b1 = 0; b1 < (1<<k1); ++b1) {
                group_mul(tmp, tmp, y[b1 * (1<<k0) + b0]);
            }
            // set exp to b0
            mpz_import(exp, 1, -1, sizeof(uint64_t), 0, 0, &b0);
            group_pow(tmp, tmp, exp);
            
            group_mul(proof, proof, tmp);
        }
    }
    
    //debug
    if (0) {
        mpz_t proof2;
        mpz_init(proof2);
        // exp = floor(2^t/l)
        mpz_pow2(exp, t);
        mpz_fdiv_q(exp, exp, prime);

        // y = g^floor(2^t/l)
        // Group op: y = x ^ exp
//        mpz_powm(proof2, input_hashed->el, exp, *(input_hashed->mod));
//        assert(mpz_cmp(proof->elr, proof2)==0);
//        gmp_printf("calc proof: %Zd\nreal proof: %Zd\n", proof->el, proof2);
    }
    
    
    mpz_clears(block, exp, NULL);
    group_clear(tmp);
    
    for (b = 0; b < (1 << k); ++b) {
        group_clear(y[b]);
    }
    
}

void get_block(mpz_t block,
               uint64_t i,
               uint64_t t,
               uint8_t k,
               const mpz_t prime)
{
    // b = 2^exp (mod l)
    mpz_powm_ui(block, two, t - k * (i + 1), prime);

    // b = 2^k * b
    // conversion from uint8_t to mp_bitcnt_t (unsigned long) is ok
    mpz_mul_2exp(block, block, (mp_bitcnt_t)k);
    
    // b = floor(b/l)
    mpz_fdiv_q(block, block, prime);
}
