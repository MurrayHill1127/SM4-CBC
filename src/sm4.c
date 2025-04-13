#include <stdio.h>
#include "sm4_p.h"
#include "sm4.h"

void keygen(sm4_key_t mk, rk_t *rkv)
{
    rkv->mk = mk; 
    for(int i = 0; i < 32; i++)
        rkv->rk[i]  =   rkv->[i-4]
                    ^   GET_T_PRIME(
                            rkv->rk[i-3]
                        ^   rkv->rk[i-2]
                        ^   rkv->rk[i-1]
                        ^   CK[i] 
                                        ); 

    return ;                        
}


