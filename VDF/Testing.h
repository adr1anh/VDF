//
//  Testing.h
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#ifndef Testing_h
#define Testing_h

#include <gmp.h>

int test_get_block(unsigned long t, unsigned long k, mpz_t const l);

int test_generate_proof(mpz_t x, mpz_t id, unsigned long int t, unsigned long int k, unsigned long int gamma, const mpz_t pk);



#endif /* Testing_h */
