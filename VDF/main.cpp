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
#include <cmath>

using namespace std;

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

int proof(int const& id, int const& l, int const& t, int const& kappa, int const& gamma, vector<unsigned long long int> const& base)
{
    int kappa1 = floor((double)kappa/(double)2);
    int kappa0 = kappa - kappa1;
    int x = id;
    
    for (int j = gamma -1; j >= 0; j = j-1)
    {
        x = pow(x,(1 << kappa));
        size_t length_of_vector = (1<<kappa);
        vector<unsigned long long int> y(length_of_vector);
        
        for(int b = 0; b < (1<<kappa); b++)
            y[b] = id;
        
        int upper_bound = ceil((double)t/(kappa * gamma));
        for (int i = 0; i < upper_bound; i++)
        {
            unsigned long long int help = (1 << (t - kappa * (i * gamma + j + 1)));
            unsigned long long int a, c;
            a=(1<<kappa);
            c=(help % l);
            unsigned long long int b = (double)(a * c) / (double)l;
            y[b] = y[b] * base[i];
            //cout << y[0] << endl;
            //cout << y[1] << endl;
        }
        unsigned long long int z;
        for(int b1 = 0; b1 < (1<<kappa1); b1++)
        {
            z = id;
            for(int b0 = 0; b0 < (1<<kappa0); b0++)
            {
                int idx = b1 * (1<<kappa0) + b0;
                z = z * y[idx];
            }
            
            x = x * pow(z,b1 * (1<<kappa0));
        }
                      
        for(int b0 = 0; b0 < (1<<kappa0); b0++)
        {
            z = id;
            for(int b1 = 0; b1 < (1<<kappa1); b1++)
            {
                z = z * y[b1 * (1<<kappa0) + b0];
            }
            x = x * pow(z,b0);
        }
    }
    return x;
}

//static unsigned short int k = 128;

int main(int argc, const char * argv[]) {
    int g = 2;
    int l = 9;
    int t = 5;
    int kappa = 1;
    int gamma = 1;
    // We fail... often
    
    //VDF function
    unsigned long long int y = g;
    for (int i = 0; i < t; i++)
        y = y * y;
    
    //Computation of the basis elements
    vector<unsigned long long int> test;
    for (int i = 0; i < 5; i++)
        test.push_back(pow(g,pow(2,i*kappa*gamma)));

    //Computation of the proof via our function
    unsigned long long int pi = proof(1,l,t,kappa,gamma,test);
    
    //Computation of the proof directly
    unsigned long long int pi_original = pow(g,floor(pow(2,t)/l));
    
    //Output of the results
    cout << "The VDF outputs " << y << endl;
    cout << "Computed proof: " << pi << endl;
    cout << "Original proof: " << pi_original << endl;
    if (pi == pi_original)
        cout << "SUCCESS" << endl;
    else
        cout << "FAILURE" << endl;
    
   //Output of our base elements
    for ( size_t i = 0; i < test.size(); i++ )
        cout << "Element " << i << " of our base array is: " << test[i] << endl;
    
    return 0;
}
