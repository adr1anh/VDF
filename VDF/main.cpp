//
//  main.cpp
//  VDF
//
//  Created by Adrian Hamelink on 20.03.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <gmpxx.h>
#include <gmp.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <vector>

void generate_rsa_prime(mpz_class &p,
                        mpz_class &q,
                        unsigned short int modulus);

void hash_prime();

//gf
//https://stackoverflow.com/a/2262447
bool simpleSHA256(void* input, unsigned long length, unsigned char* md)
{
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;
    
    if(!SHA256_Update(&context, (unsigned char*)input, length))
        return false;
    
    if(!SHA256_Final(md, &context))
        return false;
    
    return true;
}

bool hash(mpz_t & output,
          mpz_t const& pk,
          mpz_t const& x)
{
    char residue_string[] = "residue";
    int base = 10;
    size_t residue_len = strlen(residue_string);
    size_t x_len = mpz_sizeinbase(x, base);
    char concatenated_string[residue_len + x_len];
    
    unsigned char md[SHA256_DIGEST_LENGTH]; // 32 bytes
    if(!simpleSHA256(concatenated_string, sizeof(concatenated_string) * sizeof(char), md))
    {
        printf("error in sha");
        return false;
    } else {
        mpz_import(output, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, md);
        
        return true;
    }
}

int hash_prime(mpz_t & output,
               mpz_t const& x,
               mpz_t const& y,
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
    std::cout << concatenated_string << std::endl;
    
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
        if(!simpleSHA256(concatenated_string, sizeof(concatenated_string) * sizeof(char), md))
        {
            // error
        }
        mpz_import(output, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, md);
        int ret = mpz_probab_prime_p(output, reps);
        if (ret) {
            std::cout << i << std::endl << mpz_get_str(NULL, 10, output);
            return ret;
        }
    }
    
    return 0;
}

bool verify(mpz_t const& pk,
            mpz_t const& x,
            mpz_t const& proof,
            unsigned short iterations)
{
    mpz_t g;
    if (!hash(g, pk, x)) {
        
    }
    return true;
}

//static unsigned short int k = 128;

int main(int argc, const char * argv[]) {
    mpz_t x, y;
    mpz_init(x); mpz_init(y);
    mpz_set_ui(x, 1955);
    mpz_set_ui(y, 1000000009);
    hash_prime(x, x, y, 50);
    std::cout << sizeof(char);
    return 0;
}
