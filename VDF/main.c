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
        t_exponent = 20;
        overhead = 3.2;
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
    

    
    uint64_t t = 1 << t_exponent;

    
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed); //Use a seed to make output reproducible
    
    mpz_t pk;
    mpz_init(pk);
    // Graph time for squaring, proof generation, total, verification for differents RSA modulus
    FILE *fp;
    fp = fopen("/Users/Adrian/Desktop/graph_modulus.txt", "w");
    fprintf(fp,"modulus,n,input,output,proof,prime,t,gamma,k,bound,t_squaring,t_proof,t_total,t_verify,t_verify_prime\n");
    for (int i = 8; i < 16; ++i) {
        rsa_key_length = 1 << i;
        generate_prime(pk, state, rsa_key_length);
        for (int t_exp = 10; t_exp < 26; ++t_exp) {
            struct timespec start, finish;
            double t_total, t_verify,t_verify_prime;
            
            t = 1 << t_exp;
            ProofData* output = malloc(sizeof(ProofData));
            
            GroupElement input = group_init_set_ui(&pk, 14091996);
            
            clock_gettime(CLOCK_MONOTONIC, &start);
            eval(output, input, t, 42.0, 1);
            clock_gettime(CLOCK_MONOTONIC, &finish);
            t_total = (finish.tv_sec - start.tv_sec);
            t_total += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
            
            
            
            clock_gettime(CLOCK_MONOTONIC, &start);
            assert(verify(output[0]) == 0);
            clock_gettime(CLOCK_MONOTONIC, &finish);
            t_verify = (finish.tv_sec - start.tv_sec);
            t_verify += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
            
            
            
            
            clock_gettime(CLOCK_MONOTONIC, &start);
            assert(verify_prime(output[0])==0);
            clock_gettime(CLOCK_MONOTONIC, &finish);
            t_verify_prime = (finish.tv_sec - start.tv_sec);
            t_verify_prime += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
            
            printf("%u,%llu,%.7f,%.7f,%.7f,%.7f,%.7f\n",
                   rsa_key_length,
                   output[0].t,
                   output[0].time_squaring,
                   output[0].time_proof,
                   t_total,
                   t_verify,
                   t_verify_prime);
            fprintf(fp,"%u,%s,%s,%s,%s,%s,%llu,%llu,%u,%llu,%.7f,%.7f,%.7f,%.7f,%.7f\n",
                    rsa_key_length,
                    mpz_get_str(NULL, 10, pk),
                    group_get_string(output[0].input),
                    group_get_string(output[0].output),
                    group_get_string(output[0].proof),
                    mpz_get_str(NULL, 10, output[0].prime),
                    output[0].t,
                    output[0].gamma,
                    output[0].k,
                    output[0].bound,
                    output[0].time_squaring,
                    output[0].time_proof,
                    t_total,
                    t_verify,
                    t_verify_prime); // printing to file
            vdf_clear_outputs(output, 1);
            free(output);
            
            if (t_total > 600.0) {
                break;
            }
        }
    }
    fclose(fp);
//
//    generate_prime(pk, state, rsa_key_length);
//    gmp_printf("pk: %Zd\n", pk);
//
//    GroupElement input = group_init_set_ui(&pk, input_int);
//
//    ProofData* outputs = malloc(sizeof(ProofData) * segments);
//
//
//
//    for (uint8_t i = 1; i <= segments; ++i) {
//        clock_gettime(CLOCK_MONOTONIC, &start);
//
//        eval(outputs, input, t, overhead, i);
//
//        clock_gettime(CLOCK_MONOTONIC, &finish);
//        elapsed = (finish.tv_sec - start.tv_sec);
//        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//        printf("eval multi %u: \t\t\t%lf\n", i, elapsed);
//
//        for (uint8_t j = 0; j < i; ++j) {
//            assert(verify(outputs[j]) == 0);
//            assert(verify_prime(outputs[j]) == 0);
//        }
//    }
//
//
//
//    vdf_clear_outputs(outputs, segments);
//    free(outputs);
//    outputs = NULL;
    
    mpz_clears(two, pk, NULL);
    return 0;
}
