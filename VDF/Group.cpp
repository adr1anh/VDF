//
//  Group.cpp
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include "Group.hpp"
#include "string.h"
#include <openssl/sha.h>
#include "Extra.h"

//int Group::hash(mpz_t rop, const mpz_t x)
//{
//    char residue_string[] = "residue";
//    int base = 10;
//    size_t residue_len = strlen(residue_string);
//    size_t x_len = mpz_sizeinbase(x, base);
//    char concatenated_string[residue_len + x_len];
//    
//    unsigned char md[SHA256_DIGEST_LENGTH]; // 32 bytes
//    if(!simpleSHA256(concatenated_string,
//                     sizeof(concatenated_string) * sizeof(char), md))
//    {
//        printf("error in sha");
//        return -1;
//    } else {
//        mpz_import(output, SHA256_DIGEST_LENGTH, 1, sizeof(unsigned char), 0, 0, md);
//        mpz_mod(output, output, pk);
//        return 0;
//    }
//}

