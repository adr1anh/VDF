//
//  Extra.c
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include "Extra.h"
#include <gmp.h>
#include <openssl/sha.h>
#include <string.h>
#include <math.h>
#include <assert.h>

void mpz_pow2(mpz_t rop, uint64_t exp) {
    if (exp <= UINT32_MAX) {
        mpz_set_ui(rop, 0);
        mpz_setbit(rop, exp);
    } else {
        // TODO: Find an efficient way to do a big power of two
        assert(0);
    }
}

//https://stackoverflow.com/a/2262447

int simpleSHA256(unsigned char* md, void *input, unsigned long length)
{
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return 1;
    
    if(!SHA256_Update(&context, (unsigned char*)input, length))
        return 2;
    
    if(!SHA256_Final(md, &context))
        return 3;
    
    return 0;
}


int hash(mpz_t rop, const mpz_t x)
{
    char residue_string[] = "residue";
    int base = 10;
    size_t residue_len = strlen(residue_string);
    size_t x_len = mpz_sizeinbase(x, base);
    char concatenated_string[residue_len + x_len];
    
    unsigned char md[SHA256_DIGEST_LENGTH]; // 32 bytes
    int error = simpleSHA256(md, concatenated_string,
                             sizeof(concatenated_string) * sizeof(char));
    if (!error) { return error; }
    else {
        mpz_import(rop, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, md);
        return 0;
    }
}


int hash_prime(mpz_t output,
               const GroupElement x,
               const GroupElement y)
{
    // Get the three strings which will be concatenated to produce a 256bit hash
    char* prime_string = "prime";
    char* x_string = group_get_string(x);
    char* y_string = group_get_string(y);
    
    unsigned char md[SHA256_DIGEST_LENGTH];
    
    // We add at every iteration an extra byte to the concatenated string
    // We should never have to go beyond UINT32_MAX
    for (uint32_t i = 0; i < UINT32_MAX; ++i) {
        SHA256_CTX context;
        if(!SHA256_Init(&context))
            return 1;
        
        // Concatenate "prime"||"x"||"y"||"i" by feeding the SHA context with
        // these 4 data inputs
        if(!SHA256_Update(&context, prime_string, strlen(prime_string)))
            return 2;
        if(!SHA256_Update(&context, x_string, strlen(x_string)))
            return 2;
        if(!SHA256_Update(&context, y_string, strlen(y_string)))
            return 2;
        if(!SHA256_Update(&context, &i, sizeof(uint32_t)))
            return 2;
        
        // Calculate hash into md (message digest)
        if(!SHA256_Final(md, &context))
            return 3;
        
        // Convert the message digest
        mpz_import(output, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, md);
        
        // Chech if the output is a probable prime
        int ret = mpz_probab_prime_p(output, HASH_PRIME_TEST_REPETITION);
        if (ret) {
            return ret;
        }
    }
    
    return 0;
}

void generate_prime(mpz_t pk, gmp_randstate_t state, mp_bitcnt_t modulus)
{
    mpz_t p;
    mpz_init(p);
    mpz_set_ui(pk, 1);

    mp_bitcnt_t n = modulus/2;

    while (mpz_probab_prime_p(p, RSA_PRIME_TEST_REPETITION) == 0) {
        mpz_urandomb(p, state, n);
    }
    
    mpz_mul(pk, pk, p);

    n = modulus - n;
    
    while (mpz_probab_prime_p(p, RSA_PRIME_TEST_REPETITION) == 0 ) {
        mpz_urandomb(p, state, n);
    }

    mpz_mul(pk, pk, p);

    mpz_clear(p);
}


uint64_t precomputed_lenth(uint64_t t, uint64_t gamma, uint8_t k) {
    return t / (gamma * k) + (t % (gamma * k) != 0);
}



uint8_t find_optimal_k(uint64_t t, uint64_t gamma) {
    // The choice of gamma should be around t^(1/2) as this is a halfway between
    // memory and computation optimality
    
    // We use Newton's method for finding the minimum of f(k) = t/k + gamma*2^(k+1)
    // That is, a zero of g(k) = f'(k)
    // g(k) = -t/k^2 + gamma*ln(2)*2^(k+1)
    // g'(k) = 2*t/k^3 + gamma*ln(2)^2*2^(k+1)
    //
    // x(n+1) = x(n) - g(x(n))/g'(x(n))
    
    // For t =2^26 and k is opt 31s
    // For t =2^26 and k is opt 32.7s
    
    double xn, xn1, ln2, tmp;
    // We use an int for k because it will never be larger than 64 (not bits!)
    // The zero of g' increases logarithmically in t
    int k = 0;
    xn = 1;
    ln2 = log(2);
    
    // Five iterations are enough, we only need an integer anyway
    for (int i = 0; i<5; ++i){
        tmp = ln2 * gamma * exp2(xn + 1);
        xn1 = xn - (- (double)t/(xn*xn) + tmp) / (2*(double)t/(xn*xn*xn) + ln2*tmp);
        xn = xn1;
        k = rint(xn1);
    }
    
    return (uint8_t)k;
}
