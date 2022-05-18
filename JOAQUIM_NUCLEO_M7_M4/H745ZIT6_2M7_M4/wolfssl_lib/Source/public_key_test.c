#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/random.h> /* for CUSTOM_RAND_TYPE */
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/fe_operations.h>
#include <wolfssl/wolfcrypt/ge_operations.h>
#include <wolfssl/wolfcrypt/kdf.h>
#include <wolfssl/wolfcrypt/hmac.h>

#include <../Header/user_settings.h>
//#include <user_settings.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>



#include <wolfssl/wolfcrypt/curve25519.h>
#include <wolfssl/wolfcrypt/curve448.h>
#include <wolfssl/wolfcrypt/ed448.h>
#include <wolfssl/wolfcrypt/ed25519.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <wolfssl/wolfcrypt/dh.h>
#include <wolfssl/wolfcrypt/dsa.h>
#include <wolfssl/wolfcrypt/rsa.h>


#define DEBUG
//#define REPEAT
#define REP_TIMES 15

#define RSAK
//#define DSAK
//#define DHK
//#define ECCK
//#define ED25519K
//#define ED448K


void test_RSA(word32 key_len,word32* data, word32 size) //Teste para chaves que nao sao de 2048bits ta com problema
{

  RsaKey  key;
  WC_RNG  rng;
  struct timespec tick,tock;
  double spent;

  byte out[512];
  byte decrypt[512];
  word32 sizeout;

  int ret;  

  memset(out,0,sizeof(out));
  memset(decrypt,0,sizeof(decrypt));

  //BAD_FUNC_ARG;
  ret = wc_InitRng(&rng);
   switch(ret)
  {
    case MEMORY_E: printf("MEMORY_E\n");break;
    case WINCRYPT_E: printf("WINCRYPT_E\n");break;
    case CRYPTGEN_E: printf("CRYPTGEN_E\n");break;
    case BAD_FUNC_ARG: printf("BAD_FUNC_ARG\n");break;
    case DRBG_CONT_FIPS_E: printf("DRBG_CONT_FIPS_E\n");break;
    case RNG_FAILURE_E: printf("RNG_FAILURE_E\n");break;
  }
  
  
    //printf("Erro no RNG");
    
  ret = wc_InitRsaKey(&key,NULL);
   switch(ret)
  {
    case BAD_FUNC_ARG: printf("BAD FUNC ARG\n");break;
    case MEMORY_E: printf("MEMORY_E\n");break;
  }

 // if(wc_RsaSetRNG(&key,&rng))
  //  printf("erro no set RNG");
   // printf("Erro no init RSA");
  if(wc_RsaSetRNG(&key,&rng))
    printf("erro no set RNG");

  //clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_MakeRsaKey(&key, key_len, 65537, &rng);
  switch(ret)
  {
    case BAD_FUNC_ARG: printf("BAD FUNC ARG\n");break;
    case RNG_FAILURE_E: printf("RNG_FAILURE_E\n");break;
    case MP_INIT_E: printf("MP_INIT_E\n");break;
    case MP_READ_E: printf("MP_READ_E\n");break;
    case MP_CMP_E: printf("MP_CMP_E\n");break;
    case MP_INVMOD_E: printf("MP_INVMOD_E\n");break;
    case MP_EXPTMOD_E: printf("MP_EXPTMOD_E\n");break;
  }

  //clock_gettime(CLOCK_MONOTONIC,&tock);
  
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Key Generation time: %lf seconds\n", spent);
  //clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_RsaSSL_Sign((byte*)data,size,out,sizeof(out),&key,&rng);
  if(ret < 0)
    printf("Fodeu1 %d",ret);
  //clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data sign time: %lf seconds\n", spent);

  //clock_gettime(CLOCK_MONOTONIC,&tick);
  ret=  wc_RsaSSL_Verify(out,ret,decrypt,sizeof(decrypt),&key);
  if(ret < 0)
    printf("Fodeu2 %d",ret);
  //clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data verify time: %lf seconds\n", spent);


  memset(out,0,sizeof(out));
  memset(decrypt,0,sizeof(decrypt));


  //clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_RsaPublicEncrypt((byte*)data,size,out,sizeof(out),&key,&rng);
  if(ret < 0)
    printf("Fodeu3 %d",ret);
  //clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Public encryption time: %lf seconds\n", spent);

  //clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_RsaPrivateDecrypt(out,ret,decrypt,sizeof(decrypt),&key);
  if(ret  < 0)
    printf("Fodeu4 %d",ret);
  //clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Private decryption time: %lf seconds\n", spent);


  wc_FreeRsaKey(&key);
}

int ret;
char data[] = "The quick brown fox jumps over the lazy dog";
word32 in[4] ={ 0xDEADBEEF, 0xA5A5A5A5, 0xDEADBEEF, 0xA5A5A5A5};
byte result[32];

//int sensorPin = A0;

int main() {
  #ifdef RSAK
  //printf("#######################\n");
  //printf("Starting RSA 512bit key\n");
  //  #ifdef REPEAT
  //for(int i = 0;i < REP_TIMES;i++)
  //  #endif
  //test_RSA(512,in,16);
  //printf("Finished RSA 512bit key\n");
    printf("#######################\n");
    printf("Starting RSA 3072bit key\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_RSA(3072,in,16);
    printf("Finished RSA 3072bit key\n");
    printf("#######################\n");
    printf("Starting RSA 2048bit key\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_RSA(2048,in,16);
    printf("Finished RSA 2048bit key\n");
    
//    printf("#######################\n");
//    printf("Starting RSA 4096bit key\n");
//      #ifdef REPEAT
//    for(int i = 0;i < REP_TIMES;i++)
//      #endif
//    test_RSA(4096,in,16);
//    printf("Finished RSA 4096bit key\n");
  #endif
  #ifdef DSAK
    printf("Starting DSA 2048\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DSA(2048,in,16);
    printf("Finished DSA 2048\n");
    printf("#######################\n");
    printf("Starting DSA 3072\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DSA(3072,in,16);
    printf("Finished DSA 3072\n");
  #endif
  #ifdef DHK
    printf("Starting DH 2048\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DH(2048);
    printf("Finished DH 2048\n");
    printf("#######################\n");
    printf("Starting DH 4096\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DH(4096);
    printf("Finished DH 4096\n");
  #endif
  #ifdef ECCK
    printf("#######################\n");
    printf("Starting ECC256\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_ECC(32,in,16);
    printf("Finished ECC256\n");
//    printf("#######################\n");
//    printf("Starting ECC192\n");
//      #ifdef REPEAT
//    for(int i = 0;i < REP_TIMES;i++)
//      #endif
//    test_ECC(24,in,16);
//    printf("Finished ECC192\n");
  #endif
  #ifdef ED25519K
    printf("#######################\n");
    printf("Starting ED25519\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_ED25519(in,16);
    printf("Finished ED25519\n");
  #endif
  #ifdef ED448K
    printf("#######################\n");
    printf("Starting ED448\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_ED448(in,16);
    printf("Finished ED448\n");
  #endif
  return 0;
}

