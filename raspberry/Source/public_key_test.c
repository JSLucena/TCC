#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/random.h> /* for CUSTOM_RAND_TYPE */
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/error-crypt.h>
#include <../Header/user_settings.h>
//#include <user_settings.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>



//#include <wolfssl/wolfcrypt/curve25519.h>
//#include <wolfssl/wolfcrypt/curve448.h>
//#include <wolfssl/wolfcrypt/ed448.h>
//#include <wolfssl/wolfcrypt/ed25519.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <wolfssl/wolfcrypt/dh.h>
#include <wolfssl/wolfcrypt/dsa.h>
#include <wolfssl/wolfcrypt/rsa.h>


#define DEBUG
//#define REPEAT
#define REP_TIMES 15

//#define RSAK
//#define DSAK
//#define DHK
#define ECCK


void test_RSA(word32 key_len,word32* data, word32 size) //Teste para chaves que nao sao de 2048bits ta com problema
{

  RsaKey  key;
  WC_RNG  rng;
  struct timespec tick,tock;
  double spent;

  byte out[256];
  byte decrypt[256];
  word32 sizeout;

  int ret;  

  memset(out,0,sizeof(out));
  memset(decrypt,0,sizeof(decrypt));

  BAD_FUNC_ARG;
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

  clock_gettime(CLOCK_MONOTONIC,&tick);
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

  clock_gettime(CLOCK_MONOTONIC,&tock);
  
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Key Generation time: %lf seconds\n", spent);
  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_RsaSSL_Sign((byte*)data,size,out,sizeof(out),&key,&rng);
  if(ret < 0)
    printf("Fodeu1 %d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data sign time: %lf seconds\n", spent);

  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret=  wc_RsaSSL_Verify(out,ret,decrypt,sizeof(decrypt),&key);
  if(ret < 0)
    printf("Fodeu2 %d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data verify time: %lf seconds\n", spent);


  memset(out,0,sizeof(out));
  memset(decrypt,0,sizeof(decrypt));


  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_RsaPublicEncrypt((byte*)data,size,out,sizeof(out),&key,&rng);
  if(ret < 0)
    printf("Fodeu3 %d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Public encryption time: %lf seconds\n", spent);

  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_RsaPrivateDecrypt(out,ret,decrypt,sizeof(decrypt),&key);
  if(ret  < 0)
    printf("Fodeu4 %d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Private decryption time: %lf seconds\n", spent);


  wc_FreeRsaKey(&key);
}

void test_DSA(word32 key_len,word32* data,word32 size)
{
  DsaKey key;
  WC_RNG  rng;
  struct timespec tick,tock;
  double spent;
  word32 ret;

  int answer;
  byte sig[256];
  byte decrypt[256];

  ret = wc_InitRng(&rng);
 // printf("%d",ret);
  ret = wc_InitDsaKey(&key);
 // printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_MakeDsaParameters(&rng,key_len,&key);
 // printf("%d",ret);
  ret = wc_MakeDsaKey(&rng,&key);
 // printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Key Generation time: %lf seconds\n", spent);

  clock_gettime(CLOCK_MONOTONIC,&tick);
 ret = wc_DsaSign((byte*)data,sig,&key,&rng);
 // printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data sign time: %lf seconds\n", spent);

  clock_gettime(CLOCK_MONOTONIC,&tick);
  wc_DsaVerify((byte*)data,sig,&key,&answer);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data verify time: %lf seconds, %d\n", spent,answer);

  wc_FreeDsaKey(&key);
}

void test_DH(word32 key_len)//Algo ta dando problema sei la.
{
  DhKey key;
  WC_RNG  rng;
  DhParams par;
  int ret;
  
  byte priv[512];
  byte pub[512];
  byte pub_2[512];
  byte agree[512];
  word32 privSz, pubSz,agreeSz;
  struct timespec tick,tock;
  double spent;

  for(int i =0;i< sizeof(pub_2);i++)
    pub_2[i] = 0xFF;

  
  ret = wc_InitRng(&rng);
  ret = wc_InitDhKey(&key);
  printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tick);
  switch(key_len)
  {
    case 2048:
      par.p = wc_Dh_ffdhe2048_Get()->p;
      par.p_len = wc_Dh_ffdhe2048_Get()->p_len;
      par.g = wc_Dh_ffdhe2048_Get()->g;
      par.g_len = wc_Dh_ffdhe2048_Get()->g_len;
      break;
    case 4096:
      par.p = wc_Dh_ffdhe4096_Get()->p;
      par.p_len = wc_Dh_ffdhe4096_Get()->p_len;
      par.g = wc_Dh_ffdhe4096_Get()->g;
      par.g_len = wc_Dh_ffdhe4096_Get()->g_len;
      break;

  }
  //printf("%d#%d",par.p_len,par.g_len);
  ret = wc_DhSetKey(&key,par.p,par.p_len,par.g,par.g_len);
  printf("%d",ret);
  
  ret = wc_DhGenerateKeyPair(&key, &rng, priv, &privSz, pub, &pubSz);
  printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Key Generation time: %lf seconds\n", spent);


  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_DhAgree(&key, agree, agreeSz, priv, sizeof(priv), pub_2, sizeof(pub_2));
  printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Secret agreement time: %lf seconds\n", spent);

  wc_FreeDhKey(&key);
}

void test_ECC(word32 key_len, word32*data, word32 size) //Crasha quando tento liberar a memoria
{
  ecc_key key, pub;
  WC_RNG  rng;
  struct timespec tick,tock;
  double spent;

  byte sig[512];
  byte out[512];
  word32 sig_len, out_len;
  int stat,ret;
  mp_int r, s;
  wc_InitRng(&rng);
  wc_ecc_init(&key);
  
  sig_len = sizeof(sig);
  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_ecc_make_key(&rng,key_len,&key);
  printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Key Generation time: %lf seconds\n", spent);
  
  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_ecc_sign_hash((byte*)data,size,sig,&sig_len,&rng,&key);
  printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data sign time: %lf seconds\n", spent);



  
  clock_gettime(CLOCK_MONOTONIC,&tick);
  ret = wc_ecc_verify_hash(sig,sig_len,(byte*)data,size,&stat,&key);
  printf("%d",ret);
  clock_gettime(CLOCK_MONOTONIC,&tock);
  spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
  spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
  printf("Data verify time: %lf seconds\n", spent);

 // sig_len = sizeof(sig);
 // clock_gettime(CLOCK_MONOTONIC,&tick);
 // wc_ecc_encrypt(&key,&key,data,size,sig,&sig_len,NULL);
 // printf("%d",ret);
 // clock_gettime(CLOCK_MONOTONIC,&tock);
 // spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
 // spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
 // printf("Data verify time: %lf seconds\n", spent);
//
 // clock_gettime(CLOCK_MONOTONIC,&tick);
 // wc_ecc_decrypt(&key,&key,sig,sig_len,out,&out_len,NULL);
 // printf("%d",ret);
 // clock_gettime(CLOCK_MONOTONIC,&tock);
 // spent = (double)(tock.tv_sec - tick.tv_sec)*1e9;
 // spent = (spent + (tock.tv_nsec - tick.tv_nsec)) * 1e-9;
 // printf("Data verify time: %lf seconds\n", spent);

  wc_ecc_key_free(&key);
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
 // printf("#######################\n");
 // printf("Starting RSA 1024bit key\n");
 //   #ifdef REPEAT
 // for(int i = 0;i < REP_TIMES;i++)
 //   #endif
 // test_RSA(1024,in,16);
 // printf("Finished RSA 1024bit key\n");
 // printf("#######################\n");
    printf("Starting RSA 2048bit key\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_RSA(2048,in,16);
    printf("Finished RSA 2048bit key\n");
//  printf("#######################\n");
//  printf("Starting RSA 4096bit key\n");
//    #ifdef REPEAT
//  for(int i = 0;i < REP_TIMES;i++)
//    #endif
//  test_RSA(4096,in,16);
//  printf("Finished RSA 4096bit key\n");
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
  return 0;
}

