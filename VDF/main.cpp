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

void generate_rsa_prime(mpz_class &p,
                        mpz_class &q,
                        unsigned short int modulus);

void hash_prime();


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

void hash(mpz_class const& pk,
          mpz_class const& x)
{
    char padding_string[] = "residue";
    char* x_string = mpz_get_str(NULL, 10, x.get_mpz_t());
    size_t buffer_len = strlen(padding_string) + strlen(x_string);
    char buffer[buffer_len];
    strcpy(buffer, padding_string);
    strcat(buffer, x_string);
    unsigned char md[SHA256_DIGEST_LENGTH]; // 32 bytes
    if(!simpleSHA256(buffer, buffer_len, md))
    {
        printf("error");
    }
    printf("%s", md);
}
          

bool verify(mpz_class const& pk,
            mpz_class const& x,
            mpz_class const& proof,
            unsigned short iterations);

static unsigned short int k = 128;

int main(int argc, const char * argv[]) {
    mpz_class a,b;
    a= 2;
    b=3;
    hash(a, b);
    std::cout << "Hello, World!\n";
    return 0;
}
