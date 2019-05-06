//
//  VDF.h
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#ifndef VDF_h
#define VDF_h

#include <gmp.h>

// RSA Methods

// rop = input^(2^t)
// proof = 
void eval(mpz_t rop,
          mpz_t proof,
          const mpz_t input,
          unsigned long int t,
          const mpz_t pk);

//
int verify(const mpz_t x,
           const mpz_t y,
           const mpz_t proof,
           unsigned long int t,
           const mpz_t pk);

//
void generate_proof(mpz_t output,
                    const mpz_t id,
                    const mpz_t g,
                    const mpz_t l,
                    const mpz_t* precomp,
                    unsigned long bound,
                    unsigned long k,
                    unsigned long gamma,
                    unsigned long t,
                    const mpz_t pk);

void get_block(mpz_t block,
               unsigned long i,
               unsigned long t,
               unsigned long k,
               const mpz_t l);


#endif /* VDF_h */
