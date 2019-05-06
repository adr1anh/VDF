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

// Function to calculate 2^t using bit setting, should be faster than usual
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
               const mpz_t x,
               const mpz_t y,
               int reps)
{
    char prime_string[] = "prime";
    int base = 10;
    size_t prime_len = strlen(prime_string);
    size_t x_len = mpz_sizeinbase(x, base);
    size_t y_len = mpz_sizeinbase(y, base);
    
    char concatenated_string[prime_len + x_len + y_len + 32];
    // Concat the string
    strcpy(concatenated_string, prime_string);
    mpz_get_str(&concatenated_string[prime_len], base, x);
    mpz_get_str(&concatenated_string[prime_len + x_len], base, y);
    
    for (int i = 0; i < INT32_MAX; ++i) {
        int count, d, c;
        count = 0;
        
        // Append binary string of i until we find a prime
        for (c = 31 ; c >= 0 ; c--)
        {
            d = i >> c;
            if (d & 1)
                concatenated_string[prime_len + x_len + y_len + count] = 1 + '0';
            else
                concatenated_string[prime_len + x_len + y_len + count] = 0 + '0';
            count++;
        }
        
        unsigned char md[SHA256_DIGEST_LENGTH]; // 32 bytes
        if(!simpleSHA256(md, concatenated_string, sizeof(concatenated_string) * sizeof(char)))
        {
            // error
        }
        mpz_import(output, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, md);
        
        
        int ret = mpz_probab_prime_p(output, reps);
        if (ret) {
            return ret;
        }
    }
    
    return 0;
}
