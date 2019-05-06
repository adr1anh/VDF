//
//  Group.h
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#ifndef Group_h
#define Group_h

#include <gmp.h>


// TODO set inline?

// Set pk to a composite prime pk = p*q, pk has is modulus-bits long.
// Must include a gmp_randstate_t.
// Specify number of iterations of Miller-Rabin tests
void RSA_generate_prime(mpz_t pk, gmp_randstate_t state, int rep, mp_bitcnt_t modulus);

// Hash g to an element of the RSA group using a SHA256
// Hmmm, should probably hash to a larger output
void RSA_hash(mpz_t rop, const mpz_t g, const mpz_t pk);

// Set rop to rop ^ (2^t) in the RSA group
void RSA_sequential_square(mpz_t rop, unsigned long int t, const mpz_t mod);

// TODO find bound
// Same as RSA_sequential_square except we also compute a table precomp of every TODO step.
void RSA_sequential_square_precomp(mpz_t rop,
                                   mpz_t precomp[],
                                   mpz_t g,
                                   unsigned long int k,
                                   unsigned long int gamma,
                                   unsigned long int t,
                                   const mpz_t mod);

// Set rop = op1 * op2
void RSA_mul(mpz_t rop, const mpz_t op1, const mpz_t op2, const mpz_t pk);

// Set rop = op1 ^ exp
void RSA_pow(mpz_t rop, const mpz_t op1, const mpz_t exp, const mpz_t pk);

// Set rop = op1 ^ exp
void RSA_pow_ui(mpz_t rop, const mpz_t op1, unsigned long int exp, const mpz_t pk);

//int setup_group();

#endif /* Group_h */
