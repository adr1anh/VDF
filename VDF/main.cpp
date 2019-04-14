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

mpz_class* proof(mpz_class const& id, mpz_class const& l, mpz_class const& t, mpz_class const& kappa, mpz_class const& gamma, vector<mpz_class> const& base)
{
    mpz_class kappa1 = floor(*kappa/2);
    mpz_class kappa0 = *kappa - kappa1;
    x = unique_ptr<mpz_class>(new mpz_class(*id));
    
    for (mpz_class j = gamma - 1, j >= 0, j=j-1)
    {
        *x = x^(2^*kappa);
        y = new array<mpz_class, 2^(*kappa)>;
        
        for(mpz_class b = 0, b < 2^(*kappa), b++)
        {
            *y[b] = id;
        }
        
        for (mpz_class i = 0, i < ceil(*t/(*kappa * *gamma))), i++)
        {
            b = floor((2^*kappa * (2^(*t - *kappa * (i * *gamma + j)) % *l)/ *l);
            *y[b] = *y[b] * *base[i];
        }
                      
        for(mpz_class b1 = 0, b1 < 2^kappa1, b1++)
        {
            z = id;
            for(mpz_class b0 = 0, b0 < 2^(*kappa), b0++)
            {
                z = z * *y[b1 * 2^kappa0 + b0];
            }
            x = x * z^(b1 * 2^kappa0);
        }
                      
        for(mpz_class b0 = 0, b0 < 2^kappa0, b0++)
        {
            z = id;
            for(mpz_class b1 = 0, b1 < 2^kappa1, b1++)
            {
                z = z * *y[b1 * 2^kappa0 + b0];
            }
            x = x * z^b0;
        }
    }
    delete y;
    return x;
}

static unsigned short int k = 128;

int main(int argc, const char * argv[]) {
    mpz_class a,b;
    a= 5;
    b=3;
    hash(a, b);
    std::cout << "Hello, Alessio!\n";
    return 0;
}
