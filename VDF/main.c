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
#include <math.h>

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
    
    mpz_init_set_ui(two, 2);
    
    if (argc != 7) {
        printf("Must provide arguments seed, rsa_key_length, input_int, t_exponent, overhead, segments\n");
        printf("if overhead ==0 we use time squaring / time proof\n");
        seed = 42;
        rsa_key_length = 4096;
        input_int = 3;
        t_exponent = 4;
        overhead = 3.2;
        segments = 1;
//        return 1;
    } else {
        seed = atoi(argv[1]);
        rsa_key_length = atoi(argv[2]);
        input_int = atoi(argv[3]);
        t_exponent = atoi(argv[4]);
        overhead = atof(argv[5]);
        segments = atoi(argv[6]);
    }
    

    

// Graph time for squaring, proof generation, total, verification for differents RSA modulus
//    FILE *fp;
//    fp = fopen("/Users/Adrian/Desktop/graph_modulus.txt", "w");
//    fprintf(fp,"modulus,n,input,output,proof,prime,t,gamma,k,bound,t_squaring,t_proof,t_total,t_verify,t_verify_prime\n");
//    for (int i = 8; i < 16; ++i) {
//        rsa_key_length = 1 << i;
//        gmp_randstate_t state;
//        gmp_randinit_default(state);
//        gmp_randseed_ui(state, seed); //Use a seed to make output reproducible
//
//        mpz_t pk;
//        mpz_init(pk);
//
//        generate_prime(pk, state, rsa_key_length);
//        for (int t_exp = 10; t_exp < 32; ++t_exp) {
//            struct timespec start, finish;
//            double t_total, t_verify,t_verify_prime;
//
//            t = 1 << t_exp;
//            ProofData* output = malloc(sizeof(ProofData));
//
//            GroupElement input = group_init_set_ui(&pk, 14091996);
//
//            clock_gettime(CLOCK_MONOTONIC, &start);
//            eval(output, input, t, 42.0, 1);
//            clock_gettime(CLOCK_MONOTONIC, &finish);
//            t_total = (finish.tv_sec - start.tv_sec);
//            t_total += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//
//
//
//            clock_gettime(CLOCK_MONOTONIC, &start);
//            assert(verify(output[0]) == 0);
//            clock_gettime(CLOCK_MONOTONIC, &finish);
//            t_verify = (finish.tv_sec - start.tv_sec);
//            t_verify += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//
//
//
//
//            clock_gettime(CLOCK_MONOTONIC, &start);
//            assert(verify_prime(output[0])==0);
//            clock_gettime(CLOCK_MONOTONIC, &finish);
//            t_verify_prime = (finish.tv_sec - start.tv_sec);
//            t_verify_prime += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//
//            printf("%u,%llu,%.7f,%.7f,%.7f,%.7f,%.7f\n",
//                   rsa_key_length,
//                   output[0].t,
//                   output[0].time_squaring,
//                   output[0].time_proof,
//                   t_total,
//                   t_verify,
//                   t_verify_prime);
//            fprintf(fp,"%u,%s,%s,%s,%s,%s,%llu,%llu,%u,%llu,%.7f,%.7f,%.7f,%.7f,%.7f\n",
//                    rsa_key_length,
//                    mpz_get_str(NULL, 10, pk),
//                    group_get_string(output[0].input),
//                    group_get_string(output[0].output),
//                    group_get_string(output[0].proof),
//                    mpz_get_str(NULL, 10, output[0].prime),
//                    output[0].t,
//                    output[0].gamma,
//                    output[0].k,
//                    output[0].bound,
//                    output[0].time_squaring,
//                    output[0].time_proof,
//                    t_total,
//                    t_verify,
//                    t_verify_prime); // printing to file
//            vdf_clear_outputs(output, 1);
//            free(output);
//
//            if (t_total > 300.0) {
//                break;
//            }
//        }
//        mpz_clear(pk);
//    }
//    fclose(fp);

//    fp = fopen("/Users/Adrian/Desktop/graph_overhead.csv", "w");
//    fprintf(fp,"modulus,n,input,output,proof,prime,t,gamma,k,bound,segments,overhead,t_squaring,t_proof,t_total,t_verify,t_verify_prime\n");
//    for (int i = 8; i < 16; ++i) {
//        rsa_key_length = 1 << i;
//        gmp_randstate_t state;
//        gmp_randinit_default(state);
//        gmp_randseed_ui(state, seed); //Use a seed to make output reproducible
//
//        mpz_t pk;
//        mpz_init(pk);
//        generate_prime(pk, state, rsa_key_length);
//        for (int segment = 0; segment < 3; ++segment) {
//            struct timespec start, finish;
//            double t_total, t_verify,t_verify_prime;
//
//            t = 1 << 25;
//            double omega = log((double)t);
//            ProofData* output = malloc(sizeof(ProofData)*(1+segment));
//
//            GroupElement input = group_init_set_ui(&pk, 14091996);
//
//            clock_gettime(CLOCK_MONOTONIC, &start);
//            eval(output, input, t, omega, segment+1);
//            clock_gettime(CLOCK_MONOTONIC, &finish);
//            t_total = (finish.tv_sec - start.tv_sec);
//            t_total += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//
//
//
//            clock_gettime(CLOCK_MONOTONIC, &start);
//            for (int j =0; j<segment+1;j++){
//                assert(verify(output[j]) == 0);
//            }
//            clock_gettime(CLOCK_MONOTONIC, &finish);
//            t_verify = (finish.tv_sec - start.tv_sec);
//            t_verify += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//
//
//
//
//            clock_gettime(CLOCK_MONOTONIC, &start);
//            for (int j =0; j<segment+1;j++){
//                assert(verify_prime(output[j])==0);
//            }
//            clock_gettime(CLOCK_MONOTONIC, &finish);
//            t_verify_prime = (finish.tv_sec - start.tv_sec);
//            t_verify_prime += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
//
//
//
//            printf("%u,%llu,%.7f,%.7f,%.7f,%.7f,%.7f\n",
//                   rsa_key_length,
//                   output[segment].t,
//                   output[segment].time_squaring,
//                   output[segment].time_proof,
//                   t_total,
//                   t_verify,
//                   t_verify_prime);
//            fprintf(fp,"%u,%s,%s,%s,%s,%s,%llu,%llu,%u,%llu,%u,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f\n",
//                    rsa_key_length,
//                    mpz_get_str(NULL, 10, pk),
//                    group_get_string(output[segment].input),
//                    group_get_string(output[segment].output),
//                    group_get_string(output[segment].proof),
//                    mpz_get_str(NULL, 10, output[segment].prime),
//                    output[segment].t,
//                    output[segment].gamma,
//                    output[segment].k,
//                    output[segment].bound,
//                    segment,
//                    overhead,
//                    output[segment].time_squaring,
//                    output[segment].time_proof,
//                    t_total,
//                    t_verify,
//                    t_verify_prime); // printing to file
//            vdf_clear_outputs(output, 1);
//            free(output);
//
////            if (t_total > 600.0) {
////                break;
////            }
//        }
//        mpz_clear(pk);
//    }
//    fclose(fp);

    FILE *fp;
    fp = fopen("/Users/Adrian/Desktop/graph_overhead_opt.csv", "w");
    fprintf(fp,"modulus,n,input,output,proof,prime,t,gamma,k,bound,segments,current_segment,overhead,t_squaring,t_proof,t_total,t_verify,t_verify_prime\n");
    for (uint8_t i = 12; i < 15; ++i) {
        printf("RSA: %llu\n",1<<i);
        rsa_key_length = 1 << i;
        gmp_randstate_t state;
        gmp_randinit_default(state);
        gmp_randseed_ui(state, seed); //Use a seed to make output reproducible

        mpz_t pk;
        mpz_init(pk);
        generate_prime(pk, state, rsa_key_length);
        double overhead = 42.0;
        for (int segment = 0; segment < 6; ++segment) {
            struct timespec start, finish;
            double t_total, t_verify,t_verify_prime;

            uint64_t t = 1 << 20;
            ProofData* output = malloc(sizeof(ProofData)*(1+segment));

            GroupElement input = group_init_set_ui(&pk, 14091996);

            clock_gettime(CLOCK_MONOTONIC, &start);
            eval(output, input, t, overhead, segment+1);
            clock_gettime(CLOCK_MONOTONIC, &finish);
            t_total = (finish.tv_sec - start.tv_sec);
            t_total += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


            if (segment == 0) {
                overhead = output[0].time_squaring / output[0].time_proof;
            }


            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j =0; j<segment+1;j++){
                assert(verify(output[j]) == 0);
            }
            clock_gettime(CLOCK_MONOTONIC, &finish);
            t_verify = (finish.tv_sec - start.tv_sec);
            t_verify += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;




            clock_gettime(CLOCK_MONOTONIC, &start);
            for (int j =0; j<segment+1;j++){
                assert(verify_prime(output[j])==0);
            }
            clock_gettime(CLOCK_MONOTONIC, &finish);
            t_verify_prime = (finish.tv_sec - start.tv_sec);
            t_verify_prime += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


            printf("\n\n Segments:%d\n",segment);
            for (int j =0; j<segment+1;j++){
                printf("RSA: %u \t t: %lu \t sq: %.7f \t pr: %.7f \t tot: %.7f\n",
                       rsa_key_length,
                       (unsigned long)log2(output[j].t),
                       output[j].time_squaring,
                       output[j].time_proof,
                       t_total);
                fprintf(fp,"%u,%s,%s,%s,%s,%s,%llu,%llu,%u,%llu,%u,%u,%.7f,%.7f,%.7f,%.7f,%.7f,%.7f\n",
                        rsa_key_length,
                        mpz_get_str(NULL, 10, pk),
                        group_get_string(output[j].input),
                        group_get_string(output[j].output),
                        group_get_string(output[j].proof),
                        mpz_get_str(NULL, 10, output[j].prime),
                        output[j].t,
                        output[j].gamma,
                        output[j].k,
                        output[j].bound,
                        segment,
                        j,
                        overhead,
                        output[j].time_squaring,
                        output[j].time_proof,
                        t_total,
                        t_verify,
                        t_verify_prime); // printing to file
            }
            vdf_clear_outputs(output, 1);
            free(output);

//            if (t_total > 600.0) {
//                break;
//            }
        }
        mpz_clear(pk);
    }
    fclose(fp);

    // Setup timers
    struct timespec start, finish;
    double elapsed = 0.0;
    
    // Set correct t
    uint64_t t = 1 << t_exponent;
    
    
    // Setup random generator with seed
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    
    // Find public key N=pq
    mpz_t pk;
    mpz_init(pk);
    generate_prime(pk, state, rsa_key_length);
    gmp_printf("pk: %Zd\n", pk);

    
    // Create group element g = H(x) where H==ID
    GroupElement input = group_init_set_ui(&pk, input_int);

    
    // Create output data buffer [array of proof data, one for each segment]
    ProofData* outputs = malloc(sizeof(ProofData) * segments);



    for (uint8_t i = 1; i <= segments; ++i) {
        
        printf("---------------------------------\n");
        printf("With %u segment(s) \n\n", i);
        

        printf("t is: %llu \n", t);
        
        printf("input is: %llu \n", input_int);
        
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        eval(outputs, input, t, overhead, i);
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        
        if (overhead == 0 && i == 1) {
            overhead = outputs[0].time_squaring / outputs[0].time_proof;
        } else if (i >1) {
            printf("omega is: %f \n", overhead);
        }
        
        if (t < 100) {
            printf("output is: %s \n\n", group_get_string(outputs[i-1].output));
        }
        
//        printf("\n");
        for (uint8_t j = 0; j < i; ++j) {
            printf("Segment %u of %u\n", j+1,i);
            printf("t%u = %llu\n", j+1, outputs[j].t);
            printf("Time elapsed for segment %u squaring: \t\t%lf\n", j, outputs[j].time_squaring);
            printf("Time elapsed for segment %u proof : \t\t%lf\n", j, outputs[j].time_proof);
        }
        
        printf("\n\nTotal time elapsed: \t\t\t\t%lf\n", elapsed);
        
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (uint8_t j = 0; j < i; ++j) {
            assert(verify(outputs[j]) == 0);
            assert(verify_prime(outputs[j]) == 0);
        }
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed = (finish.tv_sec - start.tv_sec);
        elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        printf("Time spent for verify: \t\t\t\t%lf\n\n\n\n", elapsed);
    }



    vdf_clear_outputs(outputs, segments);
    free(outputs);
    outputs = NULL;
    
    mpz_clears(two, pk, NULL);
    return 0;
}
