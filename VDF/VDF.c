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
#include <stdlib.h>
#include <pthread.h>

#include "VDF.h"
#include "Extra.h"

int verify(const ProofData data)
{
    mpz_t* context = group_get_context(data.input);
    mpz_t r, prime;
    mpz_init(prime);
    mpz_init_set_ui(r, 2);
    
    GroupElement identity = group_get_identity(context);
    // input ^ r
    GroupElement input_r = group_init(context);
    // proof ^ prime
    GroupElement proof_prime = group_init(context);

    hash_prime(prime, data.input, data.output);
    
    // r = 2^t (mod prime)
    // Because mpz_powm_ui takes an unsigned long int (same as uint32_t) we
    // must seperate t into managable bits
    if (data.t > UINT32_MAX) {
        uint64_t t_temp = data.t;
        while (t_temp > UINT32_MAX) {
            mpz_powm_ui(r, r, UINT32_MAX, prime);
            t_temp -= UINT32_MAX;
        }
        mpz_powm_ui(r, r, t_temp, prime);
    } else {
        mpz_powm_ui(r, r, data.t, prime);
    }
    
    
    // input_r = g^r
    group_pow(input_r, data.input, r);
    
    // proof_prime = pi^l
    group_pow(proof_prime, data.proof, prime);
    group_mul(proof_prime, proof_prime, input_r);
    
    // res = true if g^r * pi^l == y
    int res = group_cmp(proof_prime, data.output);

    mpz_clears(r, prime, NULL);
    group_clear(identity);
    group_clear(input_r);
    group_clear(proof_prime);
    
    return res;
}

int verify_prime(const ProofData data)
{
    mpz_t* context = group_get_context(data.input);
    
    mpz_t r, prime;
    mpz_init(prime);
    mpz_init_set_ui(r, 2);
    
    GroupElement output = group_init(context);
    GroupElement identity = group_get_identity(context);
    GroupElement input_exp_r = group_init(context);
    GroupElement proof_exp_prime = group_init(context);
    
    // r = 2^t (mod prime)
    if (data.t > UINT32_MAX) {
        uint64_t t_temp = data.t;
        while (t_temp > UINT32_MAX) {
            mpz_powm_ui(r, r, UINT32_MAX, data.prime);
            t_temp -= UINT32_MAX;
        }
        mpz_powm_ui(r, r, t_temp, data.prime);
    } else {
        mpz_powm_ui(r, r, data.t, data.prime);
    }
    
    // input_r = g^r
    group_pow(input_exp_r, data.input, r);
    
    // proof_prime = pi^l
    group_pow(proof_exp_prime, data.proof, data.prime);
    
    // y = pi^l * g^r
    group_mul(output, proof_exp_prime, input_exp_r);
    
    // Verify prime
    hash_prime(prime, data.input, output);
    
    // res = true if g^r * pi^l == y
    int res = mpz_cmp(prime, data.prime);
    
    mpz_clears(r, prime, NULL);
    group_clear(output);
    group_clear(identity);
    group_clear(input_exp_r);
    group_clear(proof_exp_prime);
    
    return res;
}


// generate_proof_parallel:
// Calulates the proof attribute for a give ProofData
//
// ptr:
// A pointer to a ProofData struct for which all attributes but proof have been set
void* generate_proof_parallel(void* ptr) {
    struct timespec start, finish;
    ProofData* data;
    data = (ProofData*)ptr;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    mpz_t* context = group_get_context(data->input);
    
    hash_prime(data->prime, data->input, data->output);
    
    uint64_t t = data->t;
    uint64_t gamma = data->gamma;
    uint8_t k = data->k;
    
    uint8_t k0, k1;
    k1 = k / 2;
    k0 = k - k1;
    
    uint64_t b, exp_uint64;
    
    // Used to calculate blocks on the fly
    mpz_t block, res, exp_k, exp_kgamma;
    mpz_t exp;
    mpz_inits(block, res, exp, exp_k, exp_kgamma, NULL);
    mpz_pow2(exp_k, k);
    mpz_pow2(exp_kgamma, gamma * k);
    
    // output = id
    GroupElement identity = group_get_identity(context);
    data->proof = group_init_set(identity);
    
    GroupElement tmp = group_init(context);
    
    
    // allocate 2^k GroupElements representing the base
    GroupElement y[1 << k];
    for (b = 0; b < (1 << k); ++b) {
        y[b] = group_init(context);
    }
    
    uint64_t j = gamma;
    while (j > 0) {
        j--;
        // x = x ^ (2^k)
        group_seq_square(data->proof, data->proof, k);
        
        // y[b] = id
        for (b = 0; b < (1 << k); ++b) {
            group_set(y[b], identity);
        }
        
        // Number of k*gamma steps between k*(j+1) and t
        uint64_t bound = (t - (j+1)*k)/(gamma*k);
        // Set remainder to 2^(t-k(bound*gamma + j + 1) mod l
        mpz_pow2(res, t - k *(bound * gamma + j + 1));
        mpz_mod(res, res, data->prime);
        
        // This bound makes sure that t >= k * (i * gamma + j + 1)
        for (uint64_t i = 0; i < bound + 1 ; ++i) {
            // b = 2^k * r
            mpz_mul(block, res, exp_k);
            // b = floor(b/l) = floor((2^k * r)/l)
            mpz_fdiv_q(block, block, data->prime);
            // r = r * 2^(k*gamma)
            mpz_mul(res, res, exp_kgamma);
            // r = r mod l
            mpz_mod(res, res, data->prime);
            
            b = mpz_get_ui(block);
            
            // y_b = y_b * Ci
            group_mul(y[b], y[b], data->precomputed[bound-i]);
        }
        
        // We should be able to do both of these two following loops in parallel
        for (uint64_t b1 = 0; b1 < (1<<k1); ++b1) {
            group_set(tmp, identity);
            for (uint64_t b0 = 0; b0 < (1<<k0); ++b0) {
                group_mul(tmp, tmp, y[b1 * (1<<k0) + b0]);
            }
            exp_uint64 = b1 * (1 << k0);
            // Set exp to exp_uint64
            mpz_import(exp, 1, 1, sizeof(uint64_t), 0, 0, &exp_uint64);
            group_pow(tmp, tmp, exp);
            
            group_mul(data->proof, data->proof, tmp);
        }
        
        for (uint64_t b0 = 0; b0 < (1<<k0); ++b0) {
            group_set(tmp, identity);
            for (uint64_t b1 = 0; b1 < (1<<k1); ++b1) {
                group_mul(tmp, tmp, y[b1 * (1<<k0) + b0]);
            }
            // set exp to b0
            mpz_import(exp, 1, 1, sizeof(uint64_t), 0, 0, &b0);
            group_pow(tmp, tmp, exp);
            
            group_mul(data->proof, data->proof, tmp);
        }
    }
    
    mpz_clears(block, res, exp_k, exp_kgamma, exp, NULL);
    group_clear(tmp);
    group_clear(identity);
    
    for (b = 0; b < (1 << k); ++b) {
        group_clear(y[b]);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    data->time_proof = (finish.tv_sec - start.tv_sec);
    data->time_proof += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    
    return NULL;
}

void eval(ProofData* outputs,
          const GroupElement input,
          uint64_t t,
          double overhead,
          uint8_t segments)
{
    mpz_t* context = group_get_context(input);
    assert(overhead >= 1);
    assert(segments > 0);

    // Initialize the return structure for each segement
    uint64_t total = 0;
    for (uint8_t i = 0; i < segments; ++i) {
        // Set each ti to t*w^(n-1)*(w-1)/(w^n-1)
        if (i < (segments - 1)) {
            outputs[i].t = t * (pow(overhead, segments - (i+1)) * (overhead - 1) / (pow(overhead, segments) - 1));
            total += outputs[i].t;
        } else {
            // compensate for some rounding
            outputs[i].t = t - total;
        }
        outputs[i].gamma = sqrt(outputs[i].t);
        outputs[i].k = find_optimal_k(outputs[i].t, outputs[i].gamma);
        outputs[i].bound = precomputed_lenth(outputs[i].t, outputs[i].gamma, outputs[i].k);
        outputs[i].precomputed = malloc(sizeof(GroupElement) * outputs[i].bound);
        mpz_init(outputs[i].prime);
    }
    
    // Hash the input
    GroupElement input_hashed = group_init(context);
    group_hash(input_hashed, input);
    
    GroupElement identity = group_get_identity(context);
    GroupElement y = group_init_set(input_hashed);

    pthread_t thread_id[segments];
    
    for (uint8_t i = 0; i < segments; ++i) {
        struct timespec start, finish;
        
        outputs[i].input = group_init_set(y);
        uint64_t t_i = outputs[i].t;
        uint64_t gamma = outputs[i].gamma;
        uint8_t k = outputs[i].k;

        // Time the squaring
        clock_gettime(CLOCK_MONOTONIC, &start);
        // Compute the sequential squaring of the input, and memoizing
        // every κ*γ steps
        for (uint64_t j = 0; j < t_i; ++j) {
            if (j % (gamma * k) == 0) {
                outputs[i].precomputed[j / (gamma * k)] = group_init_set(y);
            }
            // g = g*g
            group_square(y, y);
        }
        outputs[i].output = group_init_set(y);
        
        clock_gettime(CLOCK_MONOTONIC, &finish);
        outputs[i].time_squaring = (finish.tv_sec - start.tv_sec);
        outputs[i].time_squaring += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        // Compute the proof of y_i while calculating y_(i+1)
        pthread_create(&thread_id[i], NULL, generate_proof_parallel, &(outputs[i]));
    }
    
    
    for (uint8_t i = 0; i < segments; ++i) {
        pthread_join(thread_id[i], NULL);
    }
    
    group_clear(input_hashed);
    group_clear(identity);
    group_clear(y);
    
    for (uint8_t i = 0; i < segments; ++i) {
        for (uint64_t j = 0; j < outputs[i].bound; ++j) {
            group_clear(outputs[i].precomputed[j]);
        }
        free(outputs[i].precomputed);
        outputs[i].precomputed = NULL;
    }
}

void vdf_clear_outputs(ProofData* outputs, uint8_t segments) {
    for (uint8_t i = 0; i < segments; ++i) {
        group_clear(outputs[i].input);
        group_clear(outputs[i].proof);
        group_clear(outputs[i].output);
        mpz_clear(outputs[i].prime);
    }
}
