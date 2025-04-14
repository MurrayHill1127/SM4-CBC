#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "sm4.h"
#include "cbc.h"

size_t get_file_sz(FILE * fp);
sm4_blk_t ivkey_transfer(const char * iv);

int main(int argc, char *argv[])
{
//  argv[1]:    mode: encrypt|decrypt
//  argv[2]:    input file
//  argv[3]:    output file
//  argv[4]:    initial vector
//  argv[5]:    key

    FILE *fp_in, *fp_out; 

    if(argc != 6) {
        fprintf(stderr, "[ERROR] argc != 5\n");
        exit(1);
    } 
    else if (   strcmp(argv[1], "encrypt")
            &&  strcmp(argv[1], "decrypt")) {
        fprintf(stderr, "[ERROR] argv[1] mode unknown\n");
        exit(1);
    }
    else if (   (fp_in  = fopen(argv[2], "rb")) == NULL
            ||  (fp_out = fopen(argv[3], "wb")) == NULL) {
        fprintf(stderr, "[ERROR] argv[2/3] open file failed\n");
        exit(1);
    } 
    else if (strlen(argv[4]) != 32 ||
             strlen(argv[5]) != 32      ) {
        fprintf(stderr, "[ERROR] argv[4/5] iv/key length != 32");
        exit(1);
    }

    int mode = (!strcmp(argv[1], "encrypt")) ? 1 : -1;
    size_t len = get_file_sz(fp_in); 


    uint8_t *buf_in, *buf_out;
    buf_in = (uint8_t *)malloc(len + 2 * BLOCK_SZ);
    if(mode == 1)
        buf_out = buf_in;
    else
        buf_out = (uint8_t *)malloc(len + 2 * BLOCK_SZ);


    sm4_blk_t iv = ivkey_transfer(argv[4]);
    memcpy((void *)buf_in, (void *)&iv, BLOCK_SZ);
    sm4_blk_t key= ivkey_transfer(argv[5]);
    rk_t rk;
    sm4_keygen(key, &rk);
    
    if(fread(buf_in + BLOCK_SZ, 1, len, fp_in) != len) {
        fprintf(stderr, "[ERROR] fread num error\n");
        exit(1);
    } 
    cbc_entry(buf_in + BLOCK_SZ, buf_out + 1, &len, &rk, mode);
    if(fwrite(buf_out + BLOCK_SZ, 1, len, fp_out) != len) {
        fprintf(stderr, "[ERROR] fwrite num error\n");
        exit(1);
    }

}

sm4_blk_t ivkey_transfer(const char *iv)
{
    sm4_blk_t blk;
    sscanf(iv, "%8x%8x%8x%8x",  &blk.h0._ ,
                                &blk.h1._ ,
                                &blk.h2._ ,
                                &blk.h3._   ); 
    return blk;
}

size_t get_file_sz(FILE *fp)
{
    fseek(fp, 0, SEEK_END);           
    size_t len = ftell(fp);            
    fseek(fp, 0, SEEK_SET);           

    return len;
}
