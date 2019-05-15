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
    mpz_init_set_ui(two, 2);
    
    clock_t start, end;
    double cpu_time_used;
    
    uint64_t t = (1<<15) +1 ;

    
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, 42); //Use a seed to make output reproducible
    mpz_t pk;
    mpz_init(pk);
    generate_prime(pk, state, 3, 8000);
    gmp_printf("pk: %Zd\n", pk);
    
    GroupElement input = group_init_set_ui(&pk, 4);
    
    uint8_t segments = 3;
    double overhead = 20.0;
    ProofData* outputs = malloc(sizeof(ProofData) * segments);
    
    start = clock();
    eval(outputs, input, t, overhead, segments);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time used for eval multi %u: \t\t %lf\n", segments, cpu_time_used);
    
    segments = 2;
    start = clock();
    eval(outputs, input, t, overhead, segments);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time used for eval multi %u: \t\t %lf\n", segments, cpu_time_used);
    
    segments = 1;
    start = clock();
    eval(outputs, input, t, overhead, segments);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time used for eval multi %u: \t\t %lf\n", segments, cpu_time_used);
    
//    start = clock();
//    assert(verify_short(x, prime1, proof1, t1) == 0);
    for (uint8_t i = 0; i < segments; ++i) {
        assert(verify(outputs[i]) == 0);
        assert(verify_prime(outputs[i]) == 0);
    }

    vdf_clear_outputs(outputs, segments);
    free(outputs);
    outputs = NULL;
    
    mpz_clears(two, pk, NULL);
    return 0;
}
