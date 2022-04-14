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
    clock_t tick,tock;
    double spent;

    

    if((mdctx = EVP_MD_CTX_new()) == NULL)
    printf("Error creating structure\n");

    tick = clock();

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

    tock = clock();
    spent = (double)(tock - tick) / (double)CLOCKS_PER_SEC;
    printf("Elapsed time: %lf seconds\n", spent);
    
    EVP_MD_CTX_free(mdctx);

    #ifdef DEBUG
        for (size_t i = 0; i < strlen(digest); ++i) printf("%x", digest[i]);
        printf("\n");
    #endif

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

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext, const EVP_CIPHER* type)
{
    EVP_CIPHER_CTX *ctx;
    clock_t tick,tock;
    double spent;
    int len;
    int starting_len = plaintext_len;
    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        printf("failed to create evp\n");

    /*
     * Initialise the encryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */

    tick = clock();    

    if(1 != EVP_EncryptInit_ex(ctx, type, NULL, key, iv))
        printf("failed to initialize encrypt\n");

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        printf("failed to encrypt\n");
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        printf("failed to finalize\n");
    ciphertext_len += len;

    tock = clock();
    spent = (double)(tock - tick) / (double)CLOCKS_PER_SEC;
    printf("Encription time: %lf seconds\n", spent);
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    #ifdef DEBUG
        for (size_t i = 0; i < strlen(ciphertext); ++i) printf("%x", ciphertext[i]);
        printf("\n");
    #endif

    #ifdef EXEC_BURN 
        clock_t begin = clock();
        clock_t end = clock();
        double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
       
        printf("Power consumption burn test\n");
    
        while(time_spent < BURNTIME)
        {
             ciphertext_len = starting_len;
            if(1 != EVP_EncryptInit_ex(ctx, type, NULL, key, iv))
                printf("failed to initialize encrypt\n");

            /*
            * Provide the message to be encrypted, and obtain the encrypted output.
            * EVP_EncryptUpdate can be called multiple times if necessary
            */
            if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
                printf("failed to encrypt\n");
            ciphertext_len = len;

            /*
            * Finalise the encryption. Further ciphertext bytes may be written at
            * this stage.
            */
            if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
                printf("failed to finalize\n");
            ciphertext_len += len;
            /* Clean up */
           
            
            clock_t end = clock();
            time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
        }
        EVP_CIPHER_CTX_free(ctx);
    #endif




    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext, const EVP_CIPHER* type)
{
    EVP_CIPHER_CTX *ctx;
    clock_t tick,tock;
    double spent;
    int len;
    int starting_len = ciphertext_len;
    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        printf("failed to create evp\n");

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    tick = clock();
    if(1 != EVP_DecryptInit_ex(ctx, type, NULL, key, iv))
        printf("failed to initialize decrypt\n");

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        printf("failed to encrypt\n");
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        printf("failed to end\n");
    plaintext_len += len;

    tock = clock();
    spent = (double)(tock - tick) / (double)CLOCKS_PER_SEC;
    printf("Decription time: %lf seconds\n", spent);
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    #ifdef DEBUG
        for (size_t i = 0; i < plaintext_len; ++i) printf("%c", plaintext[i]);
        printf("\n");
    #endif

    #ifdef EXEC_BURN 
        clock_t begin = clock();
        clock_t end = clock();
        double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
        
        printf("Power consumption burn test\n");
    
        while(time_spent < BURNTIME)
        {
            plaintext_len = starting_len;
            if(1 != EVP_DecryptInit_ex(ctx, type, NULL, key, iv))
                printf("failed to initialize decrypt\n");

            /*
            * Provide the message to be decrypted, and obtain the plaintext output.
            * EVP_DecryptUpdate can be called multiple times if necessary.
            */
            if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
                printf("failed to encrypt\n");
            plaintext_len = len;

            /*
            * Finalise the decryption. Further plaintext bytes may be written at
            * this stage.
            */
            if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
                printf("failed to end\n");
            plaintext_len += len;
            /* Clean up */
            

            clock_t end = clock();
            time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
        }
        EVP_CIPHER_CTX_free(ctx);
    #endif



    return plaintext_len;
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

        #ifdef EXEC_CIPHER //Aqui ta uns bug de double free quando executo em loop, sei la
            printf("--##Symmetric cypher test##--\n");

            /* A 256 bit key */
            unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
            unsigned char *key192 = (unsigned char *)"012345678901234567890123";
            /* A 128 bit IV */
            unsigned char *iv = (unsigned char *)"0123456789012345";
            unsigned char *iv54 = (unsigned char *)"0123456";
            unsigned char ciphertext[128];
            unsigned char decryptedtext[128];

            #ifdef AES 
                //Alem disso o AES-192 ta bugado, tive q fazer uma magica com ele

                //printf("Starting AES128CBC Test\n");
               // encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_aes_128_cbc());
               // decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_aes_128_cbc());
               // printf("Finished AES128CBC Test\n\n");
                printf("Starting AES128CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_aes_128_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_aes_128_cbc());
                printf("Finished AES128CBC Test\n\n");
                printf("Starting AES192CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key192,iv54,ciphertext,EVP_aes_192_cbc());
                decrypt(ciphertext,strlen(ciphertext),key192,iv54,decryptedtext,EVP_aes_192_cbc());
                printf("Finished AES192CBC Test\n\n");
                printf("Starting AES256CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_aes_256_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_aes_256_cbc());
                printf("Finished AES256CBC Test\n\n");
            #endif

            #ifdef DES
                printf("Starting DES-CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_des_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_des_cbc());
                printf("Finished DES-CBC Test\n\n");
                
            #endif

            #ifdef TDES
                printf("Starting 3DES-CBC 2 keys Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_des_ede_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_des_ede_cbc());
                printf("Finished 3DES-CBC 2 keys Test\n\n");
                printf("Starting 3DES-CBC 3 keys Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_des_ede3_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_des_ede3_cbc());
                printf("Finished 3DES-CBC 3 keys Test\n\n");
            #endif

            #ifdef RC4
                printf("Starting RC4 Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_rc4());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_rc4());
                printf("Finished RC4 Test\n\n");
            #endif

            #ifdef BF //modo cbc tava com erro no decrypt, por causa da frase
                printf("Starting BF Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_bf_cfb());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_bf_cfb());
                printf("Finished BF Test\n\n");
            #endif

            #ifdef CAST5
                
                printf("Starting CAST5 Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_cast5_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_cast5_cbc());
                printf("Finished CAST5 Test\n\n");
            #endif

            #ifdef ARIA
                printf("Starting ARIA128CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_aria_128_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_aria_128_cbc());
                printf("Finished ARIA128CBC Test\n\n");
                printf("Starting ARIA192CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_aria_192_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_aria_192_cbc());
                printf("Finished ARIA192CBC Test\n\n");
                printf("Starting ARIA256CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_aria_256_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_aria_256_cbc());
                printf("Finished ARIA256CBC Test\n\n");
            #endif

            #ifdef CAMELLIA
                printf("Starting CAMELLIA-128CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_camellia_128_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_camellia_128_cbc());
                printf("Finished CAMELLIA-128CBC Test\n\n");
                printf("Starting CAMELLIA-196CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_camellia_192_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_camellia_192_cbc());
                printf("Finished CAMELLIA-196CBC Test\n\n");
                printf("Starting CAMELLIA-256CBC Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_camellia_256_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_camellia_256_cbc());
                printf("Finished CAMELLIA-256CBC Test\n\n");
            #endif

            #ifdef CHACHA
                printf("Starting CHACHA20 Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_chacha20());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_chacha20());
                printf("Finished CHACHA20 Test\n\n");
                printf("Starting CHACHA20-POLY1305 Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_chacha20_poly1305());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_chacha20_poly1305());
                printf("Finished CHACHA20-POLY1305 Test\n\n");
            #endif

            #ifdef SEED
                printf("Starting SEED Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_seed_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_seed_cbc());
                printf("Finished SEED Test\n\n");
            #endif

            #ifdef SM4
                printf("Starting SM4 Test\n");
                encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_sm4_cbc());
                decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_sm4_cbc());
                printf("Finished SM4 Test\n\n");
            #endif
        #endif

         #ifdef EXEC_PK
            printf("--##Hashing Function Test##--\n");
        #endif
    return 0;
}