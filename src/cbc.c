#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "cbc.h"

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


