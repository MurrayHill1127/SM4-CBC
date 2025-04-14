#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "cbc.h"
#include "sm4.h"

static void cbc_pkcs7_padding(uint8_t *data, size_t *len)
{
    size_t padding_len = BLOCK_SZ - (*len % BLOCK_SZ);
    for(size_t i = 0; i < padding_len; i++)
        data[*len + i] = (uint8_t)padding_len;

    *len += padding_len;
    return ;
}

static void cbc_pkcs7_unpadding(uint8_t *data, size_t *len)
{
    size_t padding_len = data[*len - 1];
    *len -= padding_len;

    return ;
}

static void exor_sm4_blk(sm4_blk_t *bef, sm4_blk_t *now)
{
    *now = (sm4_blk_t) {
        .h3._   =   bef->h3._ ^ now->h3._ ,
        .h2._   =   bef->h2._ ^ now->h2._ ,
        .h1._   =   bef->h1._ ^ now->h1._ ,
        .h0._   =   bef->h0._ ^ now->h0._ 
    };
}

void cbc_entry(uint8_t *buf_in, uint8_t *buf_out, size_t *len, rk_t *rkv, int mode)
{
    if(mode == 1) {
        cbc_pkcs7_padding(buf_in, len);
        size_t rd = *len / BLOCK_SZ;

        sm4_blk_t *bef, *now ;

        for(size_t i = 0; i < rd; i++) {
            bef = (sm4_blk_t *)(buf_in +(i-1) * BLOCK_SZ);
            now = (sm4_blk_t *)(buf_in + i    * BLOCK_SZ);
            exor_sm4_blk(bef, now);
            *now = sm4_algo(*now, rkv, mode);
        }
    }


    else if (mode == -1) {
        size_t rd = *len / BLOCK_SZ;

        sm4_blk_t *now, *bef, *out;

        for(int i = rd-1; i >= 0; i--) {
            now = (sm4_blk_t *)(buf_in + i * BLOCK_SZ);
            bef = (sm4_blk_t *)(buf_in + (i-1) * BLOCK_SZ);
            out = (sm4_blk_t *)(buf_out + i * BLOCK_SZ);

            *out = sm4_algo(*now, rkv, mode);
            exor_sm4_blk(bef, out);
        }
        cbc_pkcs7_unpadding(buf_out, len);

    }
}

