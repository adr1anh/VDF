//
//  Group.h
//  VDF
//
//  Created by Adrian Hamelink on 06.05.19.
//  Copyright © 2019 LACAL. All rights reserved.
//

#ifndef Group_h
#define Group_h

#include <stdint.h>

struct GroupElementStruct;
typedef struct GroupElementStruct* GroupElement;

// group_get_context:
// Get the representation of the group of g under a common representation
//
// g:
// An element of the group
//
// return:
// A pointer to a mpz_t which should be the common representation of the group
// to which the element g belongs.
// In RSA this should be the group modulus.
mpz_t* group_get_context(const GroupElement g);

// group_get_string:
// Get a string representing the group element
//
// g:
// An element of the group
//
// return:
// A string representing g.
// In RSA, this is the integer in base 10
char* group_get_string(const GroupElement g);

// group_get_identity:
// Get the group identity for a certain context
//
// context:
// The context from which we can deduce the group
//
// return:
// The GroupElement identity in the group
GroupElement group_get_identity(mpz_t* context);

// group_init:
// Initializes a group element for a given context.
// You must set the element using group_set before using the element!
//
// context:
// The context from which we can deduce the group
//
// return:
// An initialized GroupElement
GroupElement group_init(mpz_t* context);

// group_set:
//
// rop:
// A GroupElement copy of g
//
// g:
// A GroupElement to copy into rop
void group_set(GroupElement rop, const GroupElement g);

// group_set_ui:
//
// rop:
// A GroupElement copy of g
//
// ui:
// An unsigned long int to represent rop
void group_set_ui(GroupElement rop, unsigned long int ui);

// group_init_set:
// Initializes a group element for a given context.
// You must set the element using group_set before using the element!
//
// return:
// An initialized GroupElement copy of g
//
// g:
// A GroupElement to copy into rop
GroupElement group_init_set(const GroupElement g);

// group_init_set_ui:
//
// return:
// An initialized GroupElement copy of g
//
// ui:
// An unsigned long int to represent rop
GroupElement group_init_set_ui(mpz_t* context, unsigned long int ui);

// group_clear:
// Frees the Group element
//
// rop:
// GroupElement to destroy
void group_clear(GroupElement rop);

// group_cmp:
// Frees the Group element
//
// op1, op2:
// Two group elements we wish to compare
//
// return:
// 0 if they are the same, any thing else if not
int group_cmp(const GroupElement op1, const GroupElement op2);

// group_hash:
// Hashes g to a random element rop
//
// g:
// Input to be hashed
//
// rop:
// Output of the hash
void group_hash(GroupElement rop, const GroupElement g);

// group_mul:
// Multiply op1 by op2 and set the result in rop
//
// op1, op2:
// Operands of multiplication
//
// rop:
// Result of multiplication
void group_mul(GroupElement rop, const GroupElement op1, const GroupElement op2);

// group_seq_square:
// Multiply op1 by op2 and set the result in rop
//
// g:
// Element we need to square
//
// t:
// Number of sequential squarings to produce
//
// rop:
// Result of t sequential squarings
void group_seq_square(GroupElement rop, const GroupElement g, uint64_t t);

// group_square:
// Square g and put the result in rop
//
// g:
// Element we need to square
//
// rop:
// Result of squaring
void group_square(GroupElement rop, const GroupElement g);

// group_pow:
// Exponentiation g^exp
//
// g:
// Base element
//
// exp:
// Exponent
//
// rop:
// Result of g^exp
void group_pow(GroupElement rop, const GroupElement g, const mpz_t exp);



#endif /* Group_h */
