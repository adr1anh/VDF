//
//  VDF.c
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include <math.h>

#include "VDF.h"
#include "Extra.h"
#include "Group.h"

void eval(mpz_t output,
          mpz_t proof,
          const mpz_t input,
          unsigned long int t,
          const mpz_t pk)
{
    mpz_t g, l, exp, identity;
    mpz_inits(g, l, exp, identity, NULL);
    mpz_set_ui(identity, 1);
    
    unsigned long int k = log(t)/3;
    if (k == 0) k++;
    unsigned long int gamma = (unsigned long int) sqrt(t);
    unsigned long int bound = t / (k * gamma) + (t % (k * gamma) != 0);
    
    // g = HG(x)
    
    // Group op: output = output * z
    RSA_hash(g, input, pk);
    
    mpz_t precomp[bound + 1];
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_init(precomp[i]);
    }
    RSA_sequential_square_precomp(output, precomp, g, k, gamma, t, pk);
    
    // l = Hprime(g||y)
    hash_prime(l, g, output, 50);
    
    generate_proof(proof, identity, g, l, precomp, bound, k, gamma, t, pk);
    
    // clear mpz
    mpz_clears(g, l, exp, identity, NULL);
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_clear(precomp[i]);
    }
}

int verify(const mpz_t x,
           const mpz_t y,
           const mpz_t proof,
           unsigned long int t,
           const mpz_t pk)
{
    mpz_t g, l, r, tmp;
    mpz_inits(g, l, tmp, NULL);
    mpz_init_set_ui(r, 2);
    
    RSA_hash(g, x, pk);
    
    hash_prime(l, g, y, 50);
    
    // r = 2^t (mod l)
    mpz_powm_ui(r, r, t, l);
    
    // r = g^r
    RSA_pow(r, g, r, pk);
//    mpz_powm(r, g, r, pk);
    
    // tmp = pi^l
    RSA_pow(tmp, proof, l, pk);
//    mpz_powm(tmp, proof, l, pk);
    
    // tmp = g^r * pi^l
    RSA_mul(tmp, tmp, r, pk);
//    mpz_mul(tmp, tmp, r);
//    mpz_mod(tmp, tmp, pk);
    
    // res = true if g^r * pi^l == y
    int res = mpz_cmp(tmp, y);
    mpz_clears(g, l, r, tmp, NULL);
    return res;
}

// We set t to be an unsigned long int. This gives a maximum size of
// 4'294'967'295.kappa, kappa0, kappa1 and gamma are all smaller than this, and
// this can also be unsigned long int
void generate_proof(mpz_t output,
                    const mpz_t id,
                    const mpz_t g,
                    const mpz_t l,
                    const mpz_t* precomp,
                    unsigned long bound,
                    unsigned long k,
                    unsigned long gamma,
                    unsigned long t,
                    const mpz_t pk)
{
    mpz_t block, z, k_exp, k0_exp, k1_exp;
    unsigned long k0, k1, b, b0, b1;
    
    // output = id
    mpz_init_set(output, id);
    
    mpz_inits(block, z, k_exp, k0_exp, k1_exp, NULL);
    
    // else we overflow
    //    assert(k<32);
    
    // floor(k/2)
    k1 = k / 2;
    
    // k0 = k - k1
    k0 = k - k1;
    
    // allocate 2^k ints representing the base
    mpz_t y[(1 << k)];
    for (b = 0; b < (1 << k); ++b) {
        mpz_init_set(y[b], id);
    }
    
    unsigned long int j = gamma;
    // j = gamma - 1 -> 0
    while (j > 0) {
        j--;
        // x = x ^ (2^k)
        RSA_sequential_square(output, k, pk);
//        squaring(output, k, pk);
        
        // y[b] = 1_g
        for (b = 0; b < (1 << k); ++b) {
            mpz_set(y[b], id);
        }
        
        for (unsigned long int i = 0; t >= k * (i * gamma + j + 1) ; ++i) {
            get_block(block, i * gamma + j, t, k, l);
            b = mpz_get_ui(block);
            
            // y_b = y_b * Ci
//            mpz_mul(y[b], y[b], precomp[i]);
            RSA_mul(y[b], y[b], precomp[i], pk);
        }
        
        for (b1 = 0; b1 < (1<<k1); ++b1) {
            mpz_set(z, id);
            for (b0 = 0; b0 < (1<<k0); ++b0) {
                // Group op: z = z * y_(b1*2^k0 + b0)
                RSA_mul(z, z, y[b1 * (1<<k0) + b0], pk);
//                mpz_mul(z, z, y[b1 * (1<<k0) + b0]);
//                mpz_mod(z, z, pk);
            }
            // Group op: z = z * b1^(2^k0)
            RSA_pow_ui(z, z, b1 * (1 << k0), pk);
//            mpz_powm_ui(z, z, b1 * (1 << k0), pk);
            // Group op: output = output * z
            RSA_mul(output, output, z, pk);
//            mpz_mul(output, output, z);
//            mpz_mod(output, output, pk);
        }
        
        for (b0 = 0; b0 < (1<<k0); ++b0) {
            mpz_set(z, id);
            for (b1 = 0; b1 < (1<<k1); ++b1) {
                // Group op: z = z * y_(b1*2^k0 + b0)
                RSA_mul(z, z, y[b1 * (1<<k0) + b0], pk);
//                mpz_mul(z, z, y[b1 * (1<<k0) + b0]);
//                mpz_mod(z, z, pk);
            }
            // Group op: z = z * b0
            RSA_pow_ui(z, z, b0, pk);
//            mpz_powm_ui(z, z, b0, pk);
            // Group op: output = output * z
            RSA_mul(output, output, z, pk);
//            mpz_mul(output, output, z);
//            mpz_mod(output, output, pk);
        }
    }
    
    mpz_clears(block, z, k_exp, k0_exp, k1_exp, NULL);
    
    for (b = 0; b < (1 << k); ++b) {
        mpz_clear(y[b]);
    }
    
}

// returns
// only called once per i
// obtained a 5s speedup on 2^26 by not allocating any vars here
void get_block(mpz_t block,
               unsigned long i,
               unsigned long t,
               unsigned long k,
               const mpz_t l)
{
    // b = 2^exp (mod l)
    mpz_powm_ui(block, two, t - k * (i + 1), l);
    
    // b = 2^k * b
    mpz_mul_2exp(block, block, (mp_bitcnt_t)k);
    
    // b = floor(b/l)
    mpz_fdiv_q(block, block, l);
}
