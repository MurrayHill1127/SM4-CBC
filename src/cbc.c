#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "cbc.h"
#include "sm4.h"

/*  
 *  UINT32_SU_END_SWAP macro:
 *  uint32_su type endian swap
 *  
 *  Input:  uint32_su x
 *
 */
#define UINT32_SU_END_SWAP(x) (x) = (uint32_su) { \
                            .h0 = (x).h3 ,      \
                            .h1 = (x).h2 ,      \
                            .h2 = (x).h1 ,      \
                            .h3 = (x).h0        } 


/*
 *  SM4_BLK_END_SWAP macro:
 *  sm4_blk_t type endian swap
 *  Input:  sm4_blk_t x
 *
 */
#define SM4_BLK_END_SWAP(x) do {        \
            UINT32_SU_END_SWAP((x).h0); \
            UINT32_SU_END_SWAP((x).h1); \
            UINT32_SU_END_SWAP((x).h2); \
            UINT32_SU_END_SWAP((x).h3); \
                            } while(0)  \


/*
 *  SM4_BLK_EXOR macro:
 *  SM4 Block Exclusive Or
 *  Input:  sm4_blk_t *bef, sm4_blk_t *now
 *  
 */
#define SM4_BLK_EXOR(bef, now) *(now) = (sm4_blk_t) {   \
                .h3._   =   (bef)->h3._ ^ (now)->h3._ , \
                .h2._   =   (bef)->h2._ ^ (now)->h2._ , \
                .h1._   =   (bef)->h1._ ^ (now)->h1._ , \
                .h0._   =   (bef)->h0._ ^ (now)->h0._       }
               


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

void cbc_entry(uint8_t *buf_in, uint8_t *buf_out, size_t *len, rk_t *rkv, int mode)
{

    if(mode == 1) {
        SM4_BLK_END_SWAP(*((sm4_blk_t *)buf_out - 1));

        cbc_pkcs7_padding(buf_in, len);

        size_t rd = *len / BLOCK_SZ;
        sm4_blk_t *now_in, *now_out, *bef_out;

        for(int i = 0; i < rd; i++) {
            now_in  = (sm4_blk_t *)(buf_in  + (i  ) * BLOCK_SZ);
            now_out = (sm4_blk_t *)(buf_out + (i  ) * BLOCK_SZ);
            bef_out = (sm4_blk_t *)(buf_out + (i-1) * BLOCK_SZ);
 
            SM4_BLK_EXOR(bef_out, now_in);

            SM4_BLK_END_SWAP(*now_in);
            *now_out = sm4_algo(*now_in, rkv, mode);
            SM4_BLK_END_SWAP(*now_out);
        }
    }



    else if (mode == -1) {
        SM4_BLK_END_SWAP(*((sm4_blk_t *)buf_in - 1));

        size_t rd = *len / BLOCK_SZ;
        sm4_blk_t *now_in, *now_out, *bef_in;

        for(int i = rd-1; i >= 0; i--) {
            bef_in  = (sm4_blk_t *)(buf_in  + (i-1) * BLOCK_SZ);
            now_in  = (sm4_blk_t *)(buf_in  + (i  ) * BLOCK_SZ);
            now_out = (sm4_blk_t *)(buf_out + (i  ) * BLOCK_SZ);
             
            SM4_BLK_END_SWAP(*now_in); 
            *now_out = sm4_algo(*now_in, rkv, mode);
            SM4_BLK_END_SWAP(*now_out);

            SM4_BLK_EXOR(bef_in, now_out);
        }
        cbc_pkcs7_unpadding(buf_out, len);

    }
}

