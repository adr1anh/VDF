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

static mpz_t one;
static mpz_t two;

// Function to calculate 2^t using bit setting, should be faster than usual
void mpz_pow2(mpz_t rop, unsigned long int exp);
// Given an input pointer of length bytes, will output a 32 byte
// (256 bit) digest in pointer to md
int simpleSHA256(unsigned char* md, void *input, unsigned long length);
// Given an input number x, hash returns the number representation of the SHA256 digest H("residue"||__input__)

int hash(mpz_t rop, const mpz_t x);

int hash_prime(mpz_t output,
               const mpz_t x,
               const mpz_t y,
               int reps);

#endif /* Extra_h */
