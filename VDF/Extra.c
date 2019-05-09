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

void mpz_pow2(mpz_t rop, unsigned long int exp) {
    mpz_set_ui(rop, 0);
    mpz_setbit(rop, exp);
}

// Prints a mpz_t
void printz(mpz_t const out, char name[]) {
    //    std::cout << s << ": " << mpz_get_str (NULL, 10, out) << std::endl;
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
               const GroupElement y,
               int reps)
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
        int ret = mpz_probab_prime_p(output, reps);
        if (ret) {
            return ret;
        }
    }
    
    return 0;
}

void generate_prime(mpz_t pk, gmp_randstate_t state, int rep, mp_bitcnt_t modulus)
{
    mpz_t p;
    mpz_init(p);
    mpz_set_ui(pk, 1);

    mp_bitcnt_t n;

    n = modulus/2;

    while (mpz_probab_prime_p(p, rep) == 0) {
        mpz_urandomb(p, state, n);
    }
    
    mpz_mul(pk, pk, p);

    n = modulus - n;
    
    while (mpz_probab_prime_p(p, rep) == 0 ) {
        mpz_urandomb(p, state, n);
    }

    mpz_mul(pk, pk, p);

    mpz_clear(p);
}
