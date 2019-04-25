//
//  main.cpp
//  VDF
//
//  Created by Adrian Hamelink on 20.03.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <gmp.h>
#include <openssl/sha.h>
#include <vector>
#include <math.h>
#include <string>

//void generate_rsa_prime(mpz_class &p,
//                        mpz_class &q,
//                        unsigned short int modulus);

void hash_prime();

//https://stackoverflow.com/a/2262447
bool simpleSHA256(void *input,
                  unsigned long length,
                  unsigned char* md)
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

bool hash(mpz_t output,
          const mpz_t x,
          const mpz_t pk)
{
    char residue_string[] = "residue";
    int base = 10;
    size_t residue_len = strlen(residue_string);
    size_t x_len = mpz_sizeinbase(x, base);
    char concatenated_string[residue_len + x_len];
    
    unsigned char md[SHA256_DIGEST_LENGTH]; // 32 bytes
    if(!simpleSHA256(concatenated_string,
                     sizeof(concatenated_string) * sizeof(char), md))
    {
        printf("error in sha");
        return false;
    } else {
        mpz_import(output, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, md);
        mpz_mod(output, output, pk);
        return true;
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
        if(!simpleSHA256(concatenated_string, sizeof(concatenated_string) * sizeof(char), md))
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

// Function to calculate 2^t using bit setting, should be faster than usual
void mpz_pow_2(mpz_t rop, unsigned long int exp) {
    mpz_set_ui(rop, 0);
    mpz_setbit(rop, exp);
    
}

// Prints a mpz_t
void printz(mpz_t const out, std::string s) {
    std::cout << s << ": " << mpz_get_str (NULL, 10, out) << std::endl;
}

// returns
unsigned long int get_block(unsigned long i,
                            unsigned long t,
                            unsigned long k,
                            mpz_t const l)
{
    mpz_t block, exp, two;
    mpz_init(block);
    mpz_init_set_ui(two, 2);
    mpz_init_set_ui(exp, t - k * (i + 1));
    
    // b = b (mod l)
    mpz_powm(block, two, exp, l);
    
    // b = 2^k * b
    mpz_mul_2exp(block, block, (mp_bitcnt_t)k);
    
    // b = floor(b/l)
    mpz_fdiv_q(block, block, l);
    
    // b should be the right size again ( < 2^k )
    unsigned long int res = mpz_get_ui(block);
    
    mpz_clear(block);
    mpz_clear(two);
    mpz_clear(exp);
    return res;
    
}

//verify that sum_i(get_block(i)*2^(ki)) = floor(2^t/l)
void test_get_block(unsigned long t,
                    unsigned long k,
                    mpz_t const l)
{
    mpz_t exp, test, pow_two;
    mpz_inits(exp, pow_two, test, NULL);
    
    // exp = floor(2^t/l)
    mpz_pow_2(exp, t);
    mpz_fdiv_q(exp, exp, l);
    
    unsigned long int i = 0;
    while (t > k * (i + 1)) {
        mpz_pow_2(pow_two, k * i);
        mpz_mul_ui(pow_two, pow_two, get_block(i, t, k, l));
        mpz_add(test, test, pow_two);
        ++i;
    }
    
    int res = mpz_cmp(test, exp);
    
    mpz_clears(exp, pow_two, test, NULL);
    assert(res == 0);
}

// Do t squarings of rop
void squaring(mpz_t rop, unsigned long int t, const mpz_t mod) {
    for (unsigned long int i = 0; i < t; ++i) {
        mpz_mul(rop, rop, rop);
        mpz_mod(rop, rop, mod);
    }
}

void squaring_precomp(mpz_t rop,
                      mpz_t precomp[],
                      mpz_t g,
                      unsigned long int k,
                      unsigned long int gamma,
                      unsigned long int t,
                      const mpz_t mod)
{
    mpz_set(rop, g);
    for (unsigned long int i = 0; i < t; ++i) {
        if (i % (k * gamma) == 0) {
            mpz_set(precomp[i / (k * gamma)], rop);
        }
        mpz_mul(rop, rop, rop);
        mpz_mod(rop, rop, mod);
    }
}

// We set t to be an unsigned long int. This gives a maximum size of
// 4'294'967'295.kappa, kappa0, kappa1 and gamma are all smaller than this, and
// this can also be unsigned long int
void generate_proof(mpz_t output,
                    mpz_t const pk,
                    mpz_t const id,
                    mpz_t const g,
                    mpz_t const l,
                    mpz_t* precomp,
                    unsigned long bound,
                    unsigned long k,
                    unsigned long gamma,
                    unsigned long t)
{
    mpz_t one, two, block, z, k_exp, k0_exp, k1_exp;
    unsigned long k0, k1, b, b0, b1;
    
    // output = id
    mpz_init_set(output, id);
    
    mpz_init_set_ui(one,1);
    mpz_init_set_ui(two,2);
    
    mpz_inits(block, z, k_exp, k0_exp, k1_exp, NULL);
    
    // else we overflow
    //    assert(k<32);
    
    // floor(k/2)
    k1 = k / 2;
    
    // k0 = k - k1
    k0 = k - k1;
    
    // allocate 2^k ints representing the base
    mpz_t y[(1 << k)];
    for (b = 0; b < (1 << k); ++b) {
        mpz_init_set(y[b], id);
    }
    
    unsigned long int j = gamma;
    // j = gamma - 1 -> 0
    while (j > 0) {
        j--;
        //    for (unsigned long int j = gamma - 1; j >= 0; --j) {
        //        std::cout << "j " << j << std::endl;
        // x = x ^ (2^k)
        squaring(output, k, pk);
        
        // y[b] = 1_g
        for (b = 0; b < (1 << k); ++b) {
            mpz_set(y[b], id);
        }
        
        //        for (unsigned long int i = 0; i < bound ; ++i) {
        for (unsigned long int i = 0; t >= k * (i * gamma + j + 1) ; ++i) {
            b = get_block(i * gamma + j, t, k, l);
            
            // yb = yb * Ci
            mpz_mul(y[b], y[b], precomp[i]);
        }
        
        for (b1 = 0; b1 < (1<<k1); ++b1) {
            mpz_set(z, id);
            for (b0 = 0; b0 < (1<<k0); ++b0) {
                mpz_mul(z, z, y[b1 * (1<<k0) + b0]);
                mpz_mod(z, z, pk);
            }
            mpz_powm_ui(z, z, b1 * (1 << k0), pk);
            mpz_mul(output, output, z);
            mpz_mod(output, output, pk);
        }
        
        for (b0 = 0; b0 < (1<<k0); ++b0) {
            mpz_set(z, id);
            for (b1 = 0; b1 < (1<<k1); ++b1) {
                mpz_mul(z, z, y[b1 * (1<<k0) + b0]);
                mpz_mod(z, z, pk);
            }
            mpz_powm_ui(z, z, b0, pk);
            mpz_mul(output, output, z);
            mpz_mod(output, output, pk);
        }
    }
    
    mpz_clears(one, two, block, z, k_exp, k0_exp, k1_exp, NULL);
    
    for (b = 0; b < (1 << k); ++b) {
        mpz_clear(y[b]);
    }
}

bool test_generate_proof(mpz_t x,
                         mpz_t id,
                         unsigned long int t,
                         unsigned long int k,
                         unsigned long int gamma,
                         mpz_t pk)
{
    mpz_t y, l, exp, proof;
    mpz_inits(y, l, exp, proof, NULL);
    
    unsigned long int bound = t / (k * gamma) + (t % (k * gamma) != 0);
    
    mpz_t precomp[bound + 1];
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_init(precomp[i]);
    }
    
    squaring_precomp(y, precomp, x, k, gamma, t, pk);
    
    // l = Hprime(x||y)
    hash_prime(l, x, y, 50);
    
    // exp = floor(2^t/l)
    mpz_pow_2(exp, t);
    mpz_fdiv_q(exp, exp, l);
    
    // y = g^floor(2^t/l)
    mpz_powm(y, x, exp, pk);
    
    generate_proof(proof, pk, id, x, l, precomp, bound, k, gamma, t);
    
    bool res = (mpz_cmp(proof, y) == 0);
    mpz_clears(y, l, exp, proof, NULL);
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_clear(precomp[i]);
    }
    return res;
}

void eval(mpz_t output,
          mpz_t proof,
          mpz_t input,
          unsigned long int t,
          mpz_t pk)
{
    mpz_t g, l, exp, id;
    mpz_inits(g, l, exp, id, NULL);
    mpz_set_ui(id, 1);
    
    unsigned long int k = log(t)/3;
    unsigned long int gamma = sqrt(t);
    unsigned long int bound = t / (k * gamma) + (t % (k * gamma) != 0);
    
    // g = HG(x)
    hash(g, input, pk);
    
    mpz_t precomp[bound + 1];
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_init(precomp[i]);
    }
    squaring_precomp(output, precomp, g, k, gamma, t, pk);
    
    // l = Hprime(g||y)
    hash_prime(l, g, output, 50);
    
    generate_proof(proof, pk, id, g, l, precomp, bound, k, gamma, t);
    
    // clear mpz
    mpz_clears(g, l, exp, id, NULL);
    for (unsigned long int i = 0; i < bound + 1; ++i) {
        mpz_clear(precomp[i]);
    }
}

bool verify(const mpz_t x,
            const mpz_t y,
            mpz_t const proof,
            unsigned long int t,
            const mpz_t pk)
{
    mpz_t g, l, r, tmp;
    mpz_inits(g, l, tmp, NULL);
    mpz_init_set_ui(r, 2);
    
    hash(g, x, pk);
    
    hash_prime(l, g, y, 50);
    
    // r = 2^t (mod l)
    mpz_powm_ui(r, r, t, l);

    // r = g^r
    mpz_powm(r, g, r, pk);
    
    // tmp = pi^l
    mpz_powm(tmp, proof, l, pk);
    
    // tmp = g^r * pi^l
    mpz_mul(tmp, tmp, r);
    mpz_mod(tmp, tmp, pk);
    
    printz(tmp, "tmp");
    printz(y, "y");
    
    // res = true if g^r * pi^l == y
    bool res = mpz_cmp(tmp, y) == 0;
    mpz_clears(g, l, r, tmp, NULL);
    return res;
}

//static unsigned short int k = 128;

int main(int argc, const char * argv[]) {
    clock_t start, end;
    double cpu_time_used;
    
    // t = 2^26: 46s
    // t = 2^27: 98s
    // t = 2^28: 207s
    
    unsigned long int t = 1<<28;
    unsigned long int k = log(t)/3;
    unsigned long int gamma = sqrt(t);
    unsigned long int bound = t / (k * gamma) + (t % (k * gamma) != 0);
    
    
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
    
    
    start = clock();
    eval(y, proof, x, t, pk);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    std::cout << "eval: "<< cpu_time_used << std::endl;
    
    
    start = clock();
    assert(verify(x, y, proof, t, pk));
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    std::cout << "verify: "<< cpu_time_used << std::endl;
//    assert(test_generate_proof(x, id, t, k, gamma, pk));
//    test_gxet_block(t, k, l);
    
//    std::cout << t - k * gamma * bound;
    mpz_clears(tmp1, tmp2, p1, p2, pk, x, y, proof, id, NULL);
    return 0;
}
