#ifndef CBC_H
#define CBC_H

#include <stdint.h>
#include "sm4.h"

#define BLOCK_SZ 16

void cbc_entry(uint8_t *buf_in, uint8_t *buf_out, size_t  *len, rk_t *rkv, int mode);

#endif
