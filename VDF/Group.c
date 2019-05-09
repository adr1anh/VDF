//
//  Group.c
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#include <stdlib.h>
#include <gmp.h>
#include <assert.h>

#include "Group.h"


struct GroupElementStruct {
    mpz_t el;
    mpz_t* mod;
};

GroupElement group_init(mpz_t* context) {
    GroupElement rop = malloc(sizeof(struct GroupElementStruct));
    mpz_init(rop->el);
    rop->mod = context;
    return rop;
}

void group_set(GroupElement rop, const GroupElement g) {
    mpz_set(rop->el, g->el);
    rop->mod = g->mod;
}

void group_set_ui(GroupElement rop, unsigned long int ui) {
    mpz_set_ui(rop->el, ui);
    mpz_mod(rop->el, rop->el, *(rop->mod));
}

GroupElement group_init_set(const GroupElement g) {
    GroupElement rop = malloc(sizeof(struct GroupElementStruct));
    mpz_init_set(rop->el, g->el);
    rop->mod = g->mod;
    return rop;
}

GroupElement group_init_set_ui(mpz_t* context, unsigned long int ui) {
    GroupElement rop = malloc(sizeof(struct GroupElementStruct));
    mpz_init_set_ui(rop->el, ui);
    rop->mod = context;
    return rop;
}

void group_clear(GroupElement rop) {
    mpz_clear(rop->el);
    free(rop);
}

int group_cmp(const GroupElement op1, const GroupElement op2) {
    return mpz_cmp(op1->el, op2->el);
}

mpz_t* group_get_context(const GroupElement g) {
    return g->mod;
}

char* group_get_string(const GroupElement g) {
    return mpz_get_str(NULL, 10, g->el);
}

GroupElement group_get_identity(mpz_t* context) {
    GroupElement rop = group_init(context);
    mpz_set_ui(rop->el, 1);
    return rop;
}

void group_hash(GroupElement rop, const GroupElement g) {
    group_set(rop, g);
}

void group_mul(GroupElement rop, const GroupElement op1, const GroupElement op2) {
    assert(op1->mod == op2->mod);
    rop->mod = op1->mod;
    
    mpz_mul(rop->el, op1->el, op2->el);
    mpz_mod(rop->el, rop->el, *(rop->mod));
}

void group_square(GroupElement rop, const GroupElement g) {
    mpz_mul(rop->el, g->el, g->el);
    mpz_mod(rop->el, rop->el, *(rop->mod));
}

void group_seq_square(GroupElement rop, const GroupElement g, uint64_t t) {
    mpz_set(rop->el, g->el);
    for (uint64_t i = 0; i < t; ++i) {
        group_square(rop, rop);
    }
}

void group_pow(GroupElement rop, const GroupElement g, const mpz_t exp) {
    mpz_set(rop->el, g->el);
    mpz_powm(rop->el, g->el, exp, *(g->mod));
}

void group_pow_ui(GroupElement rop, const GroupElement g, unsigned long exp) {
    mpz_set(rop->el, g->el);
    mpz_powm_ui(rop->el, g->el, exp, *(g->mod));
}
