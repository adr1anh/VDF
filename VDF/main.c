//
//  main.cpp
//  VDF
//
//  Created by Adrian Hamelink on 20.03.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include <stdio.h>
#include <gmp.h>
#include <openssl/sha.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
// TODO: Implement same size int
#include <stdint.h>

#include "Extra.h"
#include "Group.h"
#include "VDF.h"
#include "Testing.h"

int main(int argc, const char * argv[]) {
    //set initial static vars
    mpz_init_set_ui(one, 1);
    mpz_init_set_ui(two, 2);
    
//    clock_t start, end;
//    double cpu_time_used;
    
    //before get block change
    // t = 2^26: 46s
    // t = 2^27: 98s
    // t = 2^28: 207s
    
    
    //after get block change
    // t = 2^26: 40s
    // t = 2^27: 85s
    // t = 2^28: 183s
    unsigned long int t = 1<14;
//    unsigned long int k = log(t)/3;
//    unsigned long int gamma = sqrt(t);
//    unsigned long int bound = t / (k * gamma) + (t % (k * gamma) != 0);
    
    
    mpz_t tmp1, tmp2, p1, p2, pk, x, y, proof, id;
    mpz_init_set_ui(tmp1, 5);
    mpz_init_set_ui(tmp2, 3);
    mpz_inits(p1, p2, pk, y, proof, NULL);
    mpz_init_set_ui(x, 2);
    mpz_init_set_ui(id, 1);

    hash_prime(p1, tmp1, tmp2, 50);
    hash_prime(p2, tmp2, tmp1, 50);
    
    //pk = p1 * p2
    mpz_mul(pk, p1, p2);
    
    
//    start = clock();
    eval(y, proof, x, t, pk);
//    end = clock();
//    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//    printf("time used for eval: %lf\n", cpu_time_used);


//    start = clock();
    int i = verify(x, y, proof, t, pk);
    
    printf("res: %u", i);
    
//    end = clock();
//    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
//    printf("time used for vefify: %lf\n", cpu_time_used);
    
//    assert(test_generate_proof(x, id, t, k, gamma, pk));
    
//    unsigned long int k = log(t)/3;
//    mpz_set_ui(tmp1, 17);
//    test_get_block(t, k, tmp1);
    
    mpz_clears(one, two, tmp1, tmp2, p1, p2, pk, x, y, proof, id, NULL);
    return 0;
}
