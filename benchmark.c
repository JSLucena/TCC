#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include "benchmark.h"
#include <sys/time.h>

void digest_message(const unsigned char *message, size_t message_len, unsigned char *digest, unsigned int *digest_len,const EVP_MD *type)
{
	EVP_MD_CTX *mdctx;
   // struct timeval st,et;

    
    //gettimeofday(&st,NULL);
    clock_t tick,tock;
    float spent;

    tick = clock();

    if((mdctx = EVP_MD_CTX_new()) == NULL)
    printf("Error creating structure\n");

    if(1 != EVP_DigestInit_ex(mdctx, type, NULL))
        printf("Error initializing evp\n");

    if(1 != EVP_DigestUpdate(mdctx, message, message_len))
        printf("Error updating evp\n");

    int size = EVP_MD_size(type);
    digest = malloc(size);
    if(digest == NULL)
        printf("Error allocating digest memory\n");

    if(1 != EVP_DigestFinal_ex(mdctx, digest, digest_len))
        printf("Error finalizing evp\n");

    
   // gettimeofday(&et,NULL);
    tock = clock();
    //int elapsed = ((et.tv_sec - st.tv_sec) * 1000000) + (et.tv_usec - st.tv_usec);
   // printf("Digest time: %d micro seconds\n",elapsed);
    spent = (double)(tock - tick) / (double)CLOCKS_PER_SEC;
    printf("Elapsed time: %lf seconds\n", spent);
    
    #ifdef DEBUG
        for (size_t i = 0; i < strlen(digest); ++i) printf("%x", digest[i]);
        printf("\n");
    #endif

    EVP_MD_CTX_free(mdctx);

    #ifdef EXEC_BURN 
        clock_t begin = clock();
        clock_t end = clock();
        double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;

        printf("Power consumption burn test\n");
    
        while(time_spent < BURNTIME)
        {
            if((mdctx = EVP_MD_CTX_new()) == NULL)
                printf("Error creating structure\n");

            if(1 != EVP_DigestInit_ex(mdctx, type, NULL))
                printf("Error initializing evp\n");

            if(1 != EVP_DigestUpdate(mdctx, message, message_len))
                printf("Error updating evp\n");

            int size = EVP_MD_size(type);
            digest = malloc(size);
            if(digest == NULL)
                printf("Error allocating digest memory\n");

            if(1 != EVP_DigestFinal_ex(mdctx, digest, digest_len))
                printf("Error finalizing evp\n");
            EVP_MD_CTX_free(mdctx);

            clock_t end = clock();
            time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
        }
    #endif
   
	
}


int main(void)
{

		//uint8_t answer[MD5_DIGEST_LENGTH];
/*
		unsigned char *plaintext[5] = {

            (unsigned char*)"The quick brown fox jumps over the lazy dog",
            (unsigned char*)"The quick brown fox jumps over the lazy dog",
            (unsigned char*)"The quick brown fox jumps over the lazy dog",
            (unsigned char*)"The quick brown fox jumps over the lazy dog",
            (unsigned char*)"The quick brown fox jumps over the lazy dog",

        };
        */

        /* Load the human readable error strings for libcrypto */
        ERR_load_crypto_strings();

        /* Load all digest and cipher algorithms */
        OpenSSL_add_all_algorithms();

        /* Load config file, and other important initialisation */
        OPENSSL_config(NULL);


        unsigned char *plaintext= (unsigned char*)"The quick brown fox jumps over the lazy dog";

        unsigned char* digest;
        unsigned int* digest_len;
       

        #ifdef EXEC_HASH
            printf("--##Hashing Function Test##--\n");

            #ifdef MD5
                printf("Starting MD5 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_md5());
                printf("Finished MD5 Test\n\n");
            #endif

            #ifdef SHA1
                printf("Starting SHA1 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha1());
                printf("Finished SHA1 Test\n\n");
            #endif

            #ifdef SHA2
                printf("Starting SHA2-224 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha224());
                printf("Finished SHA2-224 Test\n\n");
                printf("Starting SHA2-256 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha256());
                printf("Finished SHA2-256 Test\n\n");
                printf("Starting SHA2-384 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha384());
                printf("Finished SHA2-384 Test\n\n");
                printf("Starting SHA2-512 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha512());
                printf("Finished SHA2-512 Test\n\n");
                printf("Starting SHA2-512_224 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha512_224());
                printf("Finished SHA2-512_224 Test\n\n");
                printf("Starting SHA2-512_256 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha512_256());
                printf("Finished SHA2-512_256 Test\n\n");
            #endif

            #ifdef SHA3
                printf("Starting SHA3-224 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha3_224());
                printf("Finished SHA3-224 Test\n\n");
                printf("Starting SHA3-256 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha3_256());
                printf("Finished SHA3-256 Test\n\n");
                printf("Starting SHA3-384 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha3_384());
                printf("Finished SHA3-384 Test\n\n");
                printf("Starting SHA3-512 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sha3_512());
                printf("Finished SHA3-512 Test\n\n");
            #endif

            #ifdef BLAKE2
                printf("Starting BLAKE2b512 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_blake2b512());
                printf("Finished BLAKE2b512 Test\n\n");
                printf("Starting BLAKE2s256 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_blake2s256());
                printf("Finished BLAKE2s256 Test\n\n");
            #endif

            #ifdef SHAKE
                printf("Starting SHAKE128 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_shake128());
                printf("Finished SHAKE128 Test\n\n");
                printf("Starting SHAKE256 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_shake256());
                printf("Finished SHAKE256 Test\n\n");
            #endif 

            #ifdef RIPEMD
                printf("Starting RMD160 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_ripemd160());
                printf("Finished RMD160 Test\n\n");
            #endif

            #ifdef WHIRLPOOL
                printf("Starting WHIRLPOOL Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_whirlpool());
                printf("Finished WHIRLPOOL Test\n\n");
            #endif

            #ifdef SM3
                printf("Starting SM3 Test\n");
                digest_message(plaintext,strlen(plaintext),digest,digest_len,EVP_sm3());
                printf("Finished SM3 Test\n\n");
            #endif
        #endif 

        
    return 0;
}