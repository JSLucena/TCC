#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <string.h>
#include "benchmark.h"
#include <sys/time.h>

void digest_message(const unsigned char *message, size_t message_len,const EVP_MD *type)
{
	EVP_MD_CTX *mdctx;
    struct timespec tick,tock;
    double spent;

    unsigned char *digest;
    unsigned int digest_len;

    if((mdctx = EVP_MD_CTX_new()) == NULL)
    printf("Error creating structure\n");

    clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

    if(1 != EVP_DigestInit_ex(mdctx, type, NULL))
        printf("Error initializing evp\n");

    if(1 != EVP_DigestUpdate(mdctx, message, message_len))
        printf("Error updating evp\n");

    int size = EVP_MD_size(type);
    digest = malloc(size);
    digest_len = size;
    if(digest == NULL)
        printf("Error allocating digest memory\n");

    if(1 != EVP_DigestFinal_ex(mdctx, digest, &digest_len))
        printf("Error finalizing evp\n");

    clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
    spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
    printf("Elapsed time: %.0lf nanoseconds\n", spent);
    
    EVP_MD_CTX_free(mdctx);
    #ifdef DEBUG
        for (size_t i = 0; i < strlen(digest); ++i) printf("%x", digest[i]);
        printf("\n");
    #endif
    free(digest);
   // free(digest_len);
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

            free(digest);
         //   free(digest_len);
        }
    #endif
   
	
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext, const EVP_CIPHER* type)
{
    EVP_CIPHER_CTX *ctx;
    struct timespec tick,tock;
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

    clock_gettime(CLOCK_MONOTONIC_RAW,&tick); 

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

    clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
    spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
    printf("Encription time: %.0lf nanoseconds\n", spent);
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
    struct timespec tick,tock;
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
    clock_gettime(CLOCK_MONOTONIC_RAW,&tick); 
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

    clock_gettime(CLOCK_MONOTONIC_RAW,&tock); 
    spent = (double)(tock.tv_nsec - tick.tv_nsec) ;// / (double)CLOCKS_PER_SEC;
    printf("Decription time: %.0lf nanoseconds\n", spent);
    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    #ifdef DEBUG
        for (size_t i = 0; i < plaintext_len; ++i) printf("%x", plaintext[i]);
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

int RSASign( const unsigned char* Msg,
              size_t MsgLen,
              unsigned char** EncMsg,
              size_t* MsgLenEnc, EVP_PKEY* priKey) {
  EVP_MD_CTX* m_RSASignCtx = EVP_MD_CTX_create();
  if (EVP_DigestSignInit(m_RSASignCtx,NULL, EVP_sha256(), NULL,priKey)<=0) {
      return 0;
  }
  if (EVP_DigestSignUpdate(m_RSASignCtx, Msg, MsgLen) <= 0) {
      return 0;
  }
  if (EVP_DigestSignFinal(m_RSASignCtx, NULL, MsgLenEnc) <=0) {
      return 0;
  }
  *EncMsg = (unsigned char*)malloc(*MsgLenEnc);
  if (EVP_DigestSignFinal(m_RSASignCtx, *EncMsg, MsgLenEnc) <= 0) {
      return 0;
  }
  #ifdef DEBUG
  for (size_t i = 0; i < *MsgLenEnc; ++i) printf("%c", EncMsg[i]);
        printf("\n");
    #endif
  EVP_MD_CTX_free(m_RSASignCtx);
  return 1;
}
void sign(EVP_PKEY* pkey,unsigned char* msg,const EVP_MD* type)
{
    EVP_MD_CTX *mdctx;
    int ret = 0;
    
    unsigned char *sig = NULL;
    size_t *slen;
    //*slen = 2048;
    /* Create the Message Digest Context */
    if(!(mdctx = EVP_MD_CTX_new())) 
        printf("sign error 0\n");

    /* Initialise the DigestSign operation - SHA-256 has been selected as the message digest function in this example */
    if(1 != EVP_DigestSignInit(mdctx, NULL,type, EVP_PKEY_get0_engine(pkey), pkey))
        printf("sign error 1\n");
    
    /* Call update with the message */
    if(1 != EVP_DigestSignUpdate(mdctx, msg, strlen(msg)))
        printf("sign error 2\n");
    
    /* Finalise the DigestSign operation */
    /* First call EVP_DigestSignFinal with a NULL sig parameter to obtain the length of the
    * signature. Length is returned in slen */
    if(1 != EVP_DigestSignFinal(mdctx, NULL, slen))
        printf("sign error 3\n");
   
    /* Allocate memory for the signature based on size in slen */
    if(!(sig = OPENSSL_malloc(sizeof(unsigned char) * (*slen))))
        printf("sign error 4\n");
    /* Obtain the signature */
    if(1 != EVP_DigestSignFinal(mdctx, sig, slen))
        printf("sign error 5\n");
    printf("OI\n");
    /* Success */
    /* Clean up */
    if(*sig && !ret) OPENSSL_free(sig);
    if(mdctx) EVP_MD_CTX_destroy(mdctx);

    return;
}
void generate_RSA(int key_size,unsigned char * msg)
{
    clock_t tick,tock;
    double spent;

    EVP_PKEY_CTX *ctx;
    EVP_PKEY *pkey = NULL;
    



    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if(!ctx)
        printf("Erro 0\n");


    tick = clock();
    if(EVP_PKEY_keygen_init(ctx) <= 0)
        printf("Erro 1\n");
    if(EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, key_size) <= 0)
        printf("Erro 2\n");

    /* Generate key */
    if(EVP_PKEY_keygen(ctx, &pkey) <= 0)
       printf("Erro 3\n");



    tock = clock();
    spent = (double)(tock - tick) / (double)CLOCKS_PER_SEC;
    printf("time elapsed: %lf seconds\n", spent);
     #ifdef DEBUG
       // BIO *bio = BIO_new_fp(stdout, BIO_NOCLOSE);
       // EVP_PKEY_print_private(bio,pkey,3,NULL);
       // BIO_free(bio);
        printf("\n");
    #endif
    
    unsigned char *encmsg;
    size_t* msg_len;
    //sign(pkey,msg,EVP_sha256());
    RSASign(msg,strlen(msg),&encmsg,msg_len,pkey);
    EVP_PKEY_free(pkey);
    
    EVP_PKEY_CTX_free(ctx);
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

        int i;
       

        #ifdef EXEC_HASH
            printf("--##Hashing Function Test##--\n");

            #ifdef MD5

                printf("Starting MD5 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_md5());
                printf("Finished MD5 Test\n\n");
               
            #endif

            #ifdef SHA1
                printf("Starting SHA1 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha1());
                printf("Finished SHA1 Test\n\n");
            #endif

            #ifdef SHA2_224
                printf("Starting SHA2-224 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha224());
                printf("Finished SHA2-224 Test\n\n");
            #endif
            #ifdef SHA2_256
                printf("Starting SHA2-256 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha256());
                printf("Finished SHA2-256 Test\n\n");
            #endif
            #ifdef SHA2_384
                printf("Starting SHA2-384 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha384());
                printf("Finished SHA2-384 Test\n\n");
            #endif
            #ifdef SHA2_512
                printf("Starting SHA2-512 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha512());
                printf("Finished SHA2-512 Test\n\n");
            #endif
            #ifdef SHA2_512_224
                printf("Starting SHA2-512_224 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha512_224());
                printf("Finished SHA2-512_224 Test\n\n");
            #endif
            #ifdef SHA2_512_256
                printf("Starting SHA2-512_256 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha512_256());
                printf("Finished SHA2-512_256 Test\n\n");
            #endif

            #ifdef SHA3
                printf("Starting SHA3-224 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha3_224());
                printf("Finished SHA3-224 Test\n\n");
                printf("Starting SHA3-256 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha3_256());
                printf("Finished SHA3-256 Test\n\n");
                
                printf("Starting SHA3-384 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha3_384());
                printf("Finished SHA3-384 Test\n\n"); 
                printf("Starting SHA3-512 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sha3_512());
                printf("Finished SHA3-512 Test\n\n");
            #endif

            #ifdef BLAKE2
                printf("Starting BLAKE2b512 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_blake2b512());
                printf("Finished BLAKE2b512 Test\n\n");   
                printf("Starting BLAKE2s256 Test\n");
                 #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_blake2s256());
                printf("Finished BLAKE2s256 Test\n\n");
            #endif

            #ifdef SHAKE
                printf("Starting SHAKE128 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_shake128());
                printf("Finished SHAKE128 Test\n\n");
                printf("Starting SHAKE256 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_shake256());
                printf("Finished SHAKE256 Test\n\n");
            #endif 

            #ifdef RIPEMD
                printf("Starting RMD160 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_ripemd160());
                printf("Finished RMD160 Test\n\n");
            #endif

            #ifdef WHIRLPOOL
                printf("Starting WHIRLPOOL Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_whirlpool());
                printf("Finished WHIRLPOOL Test\n\n");
            #endif

            #ifdef SM3
                printf("Starting SM3 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                #endif
                    digest_message(plaintext,strlen(plaintext),EVP_sm3());
                printf("Finished SM3 Test\n\n");
            #endif
        #endif 

        #ifdef EXEC_CIPHER //Aqui ta uns bug de double free quando executo em loop, sei la
            printf("--##Symmetric cypher test##--\n");

            /* A 256 bit key */
            unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
            unsigned char *key192 = (unsigned char *)"012345678901234567890121";
            /* A 128 bit IV */
            unsigned char *iv = (unsigned char *)"0123456789012341";
            unsigned char *iv54 = (unsigned char *)"01234561";

            int in[4] = { 0xDEADBEEF, 0xA5A5A5A5, 0xDEADBEEF, 0xA5A5A5A5};
           // unsigned char *in = (unsigned char*)"DEADBEEFA5A5A5A5DEADBEEFA5A5A5A5";
            unsigned char ciphertext[128];
            unsigned char decryptedtext[128];
            int ret;

            #ifdef AES 
                //Alem disso o AES-192 ta bugado, tive q fazer uma magica com ele

                //printf("Starting AES128CBC Test\n");
               // encrypt(plaintext,strlen(plaintext),key,iv,ciphertext,EVP_aes_128_cbc());
               // decrypt(ciphertext,strlen(ciphertext),key,iv,decryptedtext,EVP_aes_128_cbc());
               // printf("Finished AES128CBC Test\n\n");
                printf("Starting AES128CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
                
                #endif
                  ret =  encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_aes_128_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_aes_128_cbc());
                printf("Finished AES128CBC Test\n\n");
                printf("Starting AES192CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                   ret = encrypt((unsigned char*)in,sizeof(in),key192,iv54,ciphertext,EVP_aes_192_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key192,iv54,decryptedtext,EVP_aes_192_cbc());
                printf("Finished AES192CBC Test\n\n");
                printf("Starting AES256CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                   ret= encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_aes_256_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_aes_256_cbc());
                printf("Finished AES256CBC Test\n\n");
            #endif

            #ifdef DES
                printf("Starting DES-CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret= encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_des_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_des_cbc());
                printf("Finished DES-CBC Test\n\n");
                
            #endif

            #ifdef TDES
                printf("Starting 3DES-CBC 2 keys Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_des_ede_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_des_ede_cbc());
                printf("Finished 3DES-CBC 2 keys Test\n\n");
                printf("Starting 3DES-CBC 3 keys Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_des_ede3_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_des_ede3_cbc());
                printf("Finished 3DES-CBC 3 keys Test\n\n");
            #endif

            #ifdef RC4
                printf("Starting RC4 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_rc4());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_rc4());
                printf("Finished RC4 Test\n\n");
            #endif

            #ifdef BF //modo cbc tava com erro no decrypt, por causa da frase
                printf("Starting BF Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_bf_cfb());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_bf_cfb());
                printf("Finished BF Test\n\n");
            #endif

            #ifdef CAST5
                
                printf("Starting CAST5 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_cast5_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_cast5_cbc());
                printf("Finished CAST5 Test\n\n");
            #endif

            #ifdef ARIA
                printf("Starting ARIA128CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_aria_128_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_aria_128_cbc());
                printf("Finished ARIA128CBC Test\n\n");
                printf("Starting ARIA192CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_aria_192_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_aria_192_cbc());
                printf("Finished ARIA192CBC Test\n\n");
                printf("Starting ARIA256CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_aria_256_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_aria_256_cbc());
                printf("Finished ARIA256CBC Test\n\n");
            #endif

            #ifdef CAMELLIA
                printf("Starting CAMELLIA-128CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_camellia_128_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_camellia_128_cbc());
                printf("Finished CAMELLIA-128CBC Test\n\n");
                printf("Starting CAMELLIA-196CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_camellia_192_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_camellia_192_cbc());
                printf("Finished CAMELLIA-196CBC Test\n\n");
                printf("Starting CAMELLIA-256CBC Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_camellia_256_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_camellia_256_cbc());
                printf("Finished CAMELLIA-256CBC Test\n\n");
            #endif

            #ifdef CHACHA
                printf("Starting CHACHA20 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_chacha20());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_chacha20());
                printf("Finished CHACHA20 Test\n\n");
                printf("Starting CHACHA20-POLY1305 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_chacha20_poly1305());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_chacha20_poly1305());
                printf("Finished CHACHA20-POLY1305 Test\n\n");
            #endif

            #ifdef SEED
                printf("Starting SEED Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_seed_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_seed_cbc());
                printf("Finished SEED Test\n\n");
            #endif

            #ifdef SM4
                printf("Starting SM4 Test\n");
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    ret = encrypt((unsigned char*)in,sizeof(in),key,iv,ciphertext,EVP_sm4_cbc());
                #ifdef REPEAT
                for(i = 0; i < 15;i++)
            
                #endif
                    decrypt(ciphertext,ret,key,iv,decryptedtext,EVP_sm4_cbc());
                printf("Finished SM4 Test\n\n");
            #endif
        #endif

        #ifdef EXEC_PK
            printf("--##Public Key Algorithm Test##--\n");

            #ifdef RSAPK
                printf("Generating RSA Keys\n");
                printf("1024 bits\n");
                generate_RSA(1024,plaintext);
                printf("2048 bits\n");
              //  generate_RSA(2048);
                printf("4096 bits\n");
              //  generate_RSA(4096);
            #endif
        #endif
    return 0;
}
