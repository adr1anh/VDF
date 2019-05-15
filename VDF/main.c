//
//  main.cpp
//  VDF
//
//  Created by Adrian Hamelink on 20.03.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>

#include "Extra.h"
#include "Group.h"
#include "VDF.h"

mpz_t two;

int main(int argc, const char * argv[]) {
    uint8_t seed;
    uint32_t rsa_key_length;
    uint64_t input_int;
    uint8_t t_exponent;
    double overhead;
    uint8_t segments;
    
    if (argc != 7) {
        printf("Must provide arguments seed, rsa_key_length, input_int, t_exponent, overhead, segments\n");
        seed = 42;
        rsa_key_length = 8192;
        input_int = 3;
        t_exponent = 17;
        overhead = 20.0;
        segments = 3;
    } else {
        seed = atoi(argv[1]);
        rsa_key_length = atoi(argv[2]);
        input_int = atoi(argv[3]);
        t_exponent = atoi(argv[4]);
        overhead = atof(argv[5]);
        segments = atoi(argv[6]);
    }
    
    mpz_init_set_ui(two, 2);
    
    clock_t start, end;
    double cpu_time_used;
    
    uint64_t t = 1 << t_exponent;

    
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed); //Use a seed to make output reproducible
    mpz_t pk;
    mpz_init(pk);
    generate_prime(pk, state, 2, rsa_key_length);
    gmp_printf("pk: %Zd\n", pk);
    
    GroupElement input = group_init_set_ui(&pk, input_int);
    
    ProofData* outputs = malloc(sizeof(ProofData) * segments);
    
    for (uint8_t i = 1; i <= segments; ++i) {
        start = clock();
        eval(outputs, input, t, overhead, i);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("time used for eval multi %u: \t\t %lf\n", i, cpu_time_used);
        for (uint8_t j = 0; j < i; ++j) {
            assert(verify(outputs[j]) == 0);
            assert(verify_prime(outputs[j]) == 0);
        }
    }
    


    vdf_clear_outputs(outputs, segments);
    free(outputs);
    outputs = NULL;
    
    mpz_clears(two, pk, NULL);
    return 0;
}
