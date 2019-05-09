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
#include <stdint.h>

#include "Group.h"


// eval:
// Evaluate the VDF
//
// ouput:
// Calculates g^(2^t)
//
// proof:
// Calculates g^floor(2^t/l) in O(t/ln(t))
//
// input:
//
// t:
//
void eval(GroupElement output,
          GroupElement proof,
          const GroupElement input,
          uint64_t t);

// verify:
// Verification of a computed output and proof
//
// input:
//
// output:
//
// proof:
//
// t:
//
// return:
// 0 if the test passe, anything else otherwise
int verify(const GroupElement input,
           const GroupElement output,
           const GroupElement proof,
           uint64_t t);

// generate_proof:
// Verification of a computed output and proof
//
// input:
//
// output:
//
// proof:
//
// t:
//
// return:
// 0 if the test passe, anything else otherwise
void generate_proof(GroupElement proof,
                    const GroupElement identity,
                    const GroupElement input_hashed,
                    const GroupElement* precomputed,
                    const mpz_t prime,
                    uint64_t t,
                    uint64_t gamma,
                    uint8_t k);

// get_block:
// Verification of a computed output and proof
//
// block:
//
// i:
//
// t:
//
// k:
//
// prime:
//
void get_block(mpz_t block,
               uint64_t i,
               uint64_t t,
               uint8_t k,
               const mpz_t prime);


#endif /* VDF_h */
