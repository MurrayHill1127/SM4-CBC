#ifndef SM4_H
#define SM4_H

#include <stdint.h>


/*
 *  uint32_su type:
 *  undigned int 32 bits _ splited union
 *
 */
typedef union {
    struct {
        uint8_t h0, h1, h2, h3;
    };
    uint32_t _;
} uint32_su;


/*
 *  uint32_st type:
 *  unsigned int 32 bits _ splited struct
 *
 */
typedef struct {
    uint32_su h0, h1, h2, h3;
} uint128_st;


/*
 *  sm4_blk_t type:
 *  SM4 block type
 *
 *  sm4_key_t type:
 *  SM4 key type
 *
 */
typedef uint128_st sm4_blk_t;
typedef uint128_st sm4_key_t;


/*
 *  rk_t type: 
 *  round key type
 *
 */
typedef union {
    struct {
        sm4_key_t mk;
        uint32_su inner_k[32];
    };
    uint32_su k[36]; 
} rk_t;


/*
 *  sm4_keygen func:
 *  SM4 Key Generator
 *  
 *  Input:  sm4_key_t   mk, "main key",
 *          rk_t      *rkv, "round key vector"
 *  Return: void
 *
 */
void sm4_keygen(sm4_key_t mk, rk_t *rkv);


/*
 *  sm4_algo func:
 *  SM4 Algorithm
 *  Input:  sm4_blk_t   msg, "message",
 *          rk_t       *rkv, "round key vector" 
 *          int        iter, "iterator"
 *
 *  Return: sm4_blk_t      , "en/decrypted msg"
 *
 */
sm4_blk_t sm4_algo(sm4_blk_t msg, rk_t *rkv, int iter);
#undef
