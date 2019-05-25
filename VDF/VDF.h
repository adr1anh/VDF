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

typedef struct ProofData {
    GroupElement input; // g
    GroupElement* precomputed; // g^(i*2^(κ*γ)) for i = 0, ..., bound
    GroupElement proof; // g^floor(2^t/l)
    GroupElement output; // g^(2^t)
    mpz_t prime;
    uint64_t t;
    uint64_t gamma;
    uint8_t k;
    uint64_t bound; // Length of precomputed array
    double time_squaring;
    double time_proof;
} ProofData;

// eval:
// Evaluate the VDF, optimized to minimize the overhead
//
// ouputs:
// A ProofData struct with all fields computed (except precomputed) computed
//
// input:
// Initial group element to be evaluated
//
// t:
// The number of squarings to perform
//
// overhead:
// When we split t into different segments, t_(i-1) = ω * t_i
//
// segments:
// The amount of splitting we do to t
void eval(ProofData* outputs,
          const GroupElement input,
          uint64_t t,
          double overhead,
          uint8_t segments);

// verify / verify_prime:
// Verification of a computed output and proof using either the output, or the
// prime challenge
//
// data:
// The proof data computed by eval
//
// return:
// 0 if the test passe, anything else otherwise
int verify(const ProofData data);
int verify_prime(const ProofData data);

// vdf_clear_outputs:
// Clears a ProofData struct
void vdf_clear_outputs(ProofData* outputs, uint8_t segments);

#endif /* VDF_h */
