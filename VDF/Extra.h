//
//  Extra.h
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#ifndef Extra_h
#define Extra_h

#include <gmp.h>
#include <stdint.h>

#include "Group.h"

#define HASH_PRIME_TEST_REPETITION 10
#define RSA_PRIME_TEST_REPETITION 2

extern mpz_t two;

// Function to calculate 2^t using bit setting, should be faster than usual
void mpz_pow2(mpz_t rop, uint64_t exp);


// Given an input pointer of length bytes, will output a 32 byte
// (256 bit) digest in pointer to md
int simpleSHA256(unsigned char* md, void *input, unsigned long length);


// Given an input number x, hash returns the number representation of the SHA256 digest H("residue"||__input__)
int hash(mpz_t rop, const mpz_t x);

// Generate a random prime in output which will depend on input x, y.
// Can specify the number of Miller-Rabin tests we perform
//int hash_prime(mpz_t output,
//               const mpz_t x,
//               const mpz_t y,
//               int reps);
int hash_prime(mpz_t output,
               const GroupElement x,
               const GroupElement y);


void generate_prime(mpz_t pk, gmp_randstate_t state, mp_bitcnt_t modulus);

uint8_t find_optimal_k(uint64_t t, uint64_t gamma);

uint64_t precomputed_lenth(uint64_t t, uint64_t gamma, uint8_t k);

#endif /* Extra_h */
