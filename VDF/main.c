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

#include "Extra.h"
#include "Group.h"
#include "VDF.h"
#include "Testing.h"

mpz_t two;

int main(int argc, const char * argv[]) {
    //set initial static vars
    mpz_init_set_ui(two, 2);
    
    clock_t start, end;
    double cpu_time_used;

    unsigned long int t = (1<<28);

//    mpz_t p1,p2;
//    GroupElement tmp1,tmp2;
//    mpz_inits(p1, p2,tmp1->el,tmp2->el, NULL);
//    mpz_init_set_ui(tmp1->el, 5);
//    mpz_init_set_ui(tmp2->el, 3);
//    hash_prime(p1, tmp1, tmp2, 50);
//    hash_prime(p2, tmp2, tmp1, 50);
//    mpz_mul(pk, p1, p2);
    gmp_randstate_t state;
    gmp_randinit_default(state);
    mpz_t pk;
    mpz_init(pk);
    generate_prime(pk, state, 100, 512);
    
//    gmp_printf("pk %Zd\n ", pk);
    
    GroupElement y = group_init(&pk);
    GroupElement proof = group_init(&pk);
    GroupElement x = group_init_set_ui(&pk, 2);
    
    start = clock();
    eval(y, proof, x, t);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time used for eval: %lf\n", cpu_time_used);


    start = clock();
    assert(verify(x, y, proof, t) == 0);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("time used for vefify: %lf\n", cpu_time_used);
    
//    assert(test_generate_proof(x, id, t, k, gamma, pk));
    
//    unsigned long int k = log(t)/3;
//    mpz_set_ui(tmp1, 17);
//    test_get_block(t, k, tmp1);
    
    group_clear(y);
    group_clear(x);
    group_clear(proof);
    mpz_clears(two, pk, NULL);
    return 0;
}
