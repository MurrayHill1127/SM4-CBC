#include <stdio.h>
#include "sm4_p.h"
#include "sm4.h"

void sm4_keygen(sm4_key_t mk, rk_t *rkv)
{
    rkv->mfk = (sm4_key_t) {
        .h0._   =   mk.h0._ ^ FK[0],
        .h1._   =   mk.h1._ ^ FK[1],
        .h2._   =   mk.h2._ ^ FK[2],
        .h3._   =   mk.h3._ ^ FK[3]
    };

    for(int i = 0; i < 32; i++) {
        rkv->rk[i]  =   rkv->rk[i-4]
                    ^   GET_T_PRIME_RES(
                            rkv->rk[i-3]
                        ^   rkv->rk[i-2]
                        ^   rkv->rk[i-1]
                        ^   CK[i] 
                                        ); 
        printf("rk[%2d] == %#8x\n", i, rkv->rk[i]);}
    return ;                        
}


sm4_blk_t sm4_algo(sm4_blk_t msg, rk_t *rkv, int flag)
{
    int start = (flag == 1) ? 0 : 31;
    int step = flag;

    sm4_blk_t b1, b2;
    b1 = msg;

    for(int i = start; i >= 0 && i < 32; i += step) {
        b2.h0._ =   b1.h1._ ;
        b2.h1._ =   b1.h2._ ;
        b2.h2._ =   b1.h3._ ;
        
        b2.h3._ =   b1.h0._
                ^   GET_T_RES(
                        b1.h1._
                    ^   b1.h2._
                    ^   b1.h3._
                    ^   rkv->rk[i]
                                    );
        b1 = b2; 
    }
    b2.h0._ = b1.h3._ ;
    b2.h1._ = b1.h2._ ;
    b2.h2._ = b1.h1._ ;
    b2.h3._ = b1.h0._ ;

   
    return b2;
}
