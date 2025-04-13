#ifndef CBC_H
#define CBC_H

#include <stdint.h>

#define BLOCK_SZ 16

static void cbc_pkcs7_padding(uint8_t *data, size_t *len);
static void cbc_pkcs7_unpadding(uint8_t *data, size_t *len);



#endif
