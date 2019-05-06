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

void RSA_hash(mpz_t rop, const mpz_t g, const mpz_t pk);
void RSA_sequential_square(mpz_t rop, unsigned long int t, const mpz_t mod);
void RSA_sequential_square_precomp(mpz_t rop,
                                   mpz_t precomp[],
                                   mpz_t g,
                                   unsigned long int k,
                                   unsigned long int gamma,
                                   unsigned long int t,
                                   const mpz_t mod);
void RSA_mul(mpz_t rop, const mpz_t op1, const mpz_t op2, const mpz_t pk);
void RSA_pow(mpz_t rop, const mpz_t op1, const mpz_t exp, const mpz_t pk);
void RSA_pow_ui(mpz_t rop, const mpz_t op1, unsigned long int exp, const mpz_t pk);

//int setup_group();

#endif /* Group_h */
