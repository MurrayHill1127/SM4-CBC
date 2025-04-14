#ifndef CBC_H
#define CBC_H

#include <stdint.h>
#include "sm4.h"

#define BLOCK_SZ 16

static void cbc_pkcs7_padding(uint8_t *data, size_t *len);
static void cbc_pkcs7_unpadding(uint8_t *data, size_t *len);

void cbc_entry(uint8_t *buf_in, uint8_t *buf_out, size_t  *len, rk_t *rkv, int mode);

#endif
