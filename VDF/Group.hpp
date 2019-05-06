//
//  Group.hpp
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>
#include <gmp.h>

//class Group {
//public:
//    virtual void multiply(mpz_t rop, const mpz_t op1 mpz_t rop, const mpz_t op2) = 0;
//
//    int hash(mpz_t rop, const mpz_t x);
//    virtual int hash_prime(mpz_t rop, const mpz_t x, const mpz_t y, int reps) = 0;
//    virtual void sequential_square(mpz_t rop, unsigned long int t) = 0;
//    virtual void squaring_precomp(mpz_t rop, mpz_t precomp[], mpz_t g,
//                                  unsigned long int k, unsigned long int gamma, unsigned long int t) = 0;
//};
//
//class RSAGroup : public Group {
//    virtual int hash_prime(mpz_t rop, const mpz_t x, const mpz_t y, int reps) override;
//    virtual void sequential_square(mpz_t rop, unsigned long int t) override;
//    virtual void squaring_precomp(mpz_t rop, mpz_t precomp[], mpz_t g,
//                                  unsigned long int k, unsigned long int gamma, unsigned long int t) override;
//private:
//    mpz_t modulus;
//    mpz_t prime_p;
//    mpz_t prime_q;
//    mpz_t order;
//};

#endif /* Group_hpp */
