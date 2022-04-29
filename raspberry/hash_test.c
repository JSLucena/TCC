#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/random.h> /* for CUSTOM_RAND_TYPE */
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/md5.h>
#include <wolfssl/wolfcrypt/sha.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/sha512.h>
#include <wolfssl/wolfcrypt/sha3.h>
#include <wolfssl/wolfcrypt/blake2.h>
#include <user_settings.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#define DEBUG
#define REPEAT
#define REP_TIMES 15


void hash_data(enum wc_HashType type,char* data){
  
byte result[512];
struct timespec tick,tock;
double spent;
word32 hash_len = wc_HashGetDigestSize(type);
int i, ret;
int byte_value;

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
 if(wc_Hash(type,(byte*)data,strlen(data),result,hash_len))
     printf("ERROR\n");    
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Elapsed time: %.0lf nanoseconds\n", spent);
 #ifdef DEBUG
    for(i = 0;i < hash_len;i++)
    {
      byte_value = result[i];
      printf("%x",byte_value);
    }
    printf("\n"); 
  #endif
}
void hash_blake2b(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2b b2b;
  byte hash[64];

  // initialize Blake2b structure with 64 byte digest
  struct timespec tick,tock;
  double spent;
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_InitBlake2b(&b2b, digestsize);
  wc_Blake2bUpdate(&b2b,(byte*)data,strlen(data));
  ret = wc_Blake2bFinal(&b2b, hash, digestsize);
  if( ret != 0) {
      printf("ERROR\n"); 
  }
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Elapsed time: %.0lf nanoseconds\n", spent);
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif
}

void hash_blake2s(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2s b2s;
  byte hash[64];

  // initialize Blake2b structure with 64 byte digest
  struct timespec tick,tock;
  double spent;
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_InitBlake2s(&b2s, digestsize);
  wc_Blake2sUpdate(&b2s,(byte*)data,strlen(data));
  ret = wc_Blake2sFinal(&b2s, hash, digestsize);
  if( ret != 0) {
       printf("ERROR\n");
  }
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Elapsed time: %.0lf nanoseconds\n", spent);
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif
}

int ret;
u_int8_t byt;
int byte_value;
short i;
char data[] = "The quick brown fox jumps over the lazy dog";
byte result[32];


int main(){
  printf("Starting MD5\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
    hash_data(WC_HASH_TYPE_MD5,data);
  printf("Finished MD5\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();
  
  printf("Starting SHA1\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
    hash_data(WC_HASH_TYPE_SHA,data);
  printf("Finished SHA1\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting SHA2-224\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif  
    hash_data(WC_HASH_TYPE_SHA224,data);
  printf("Finished SHA2-224\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting SHA2-256\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif  
    hash_data(WC_HASH_TYPE_SHA256,data);
  printf("Finished SHA2-256\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting SHA2-384\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif  
  hash_data(WC_HASH_TYPE_SHA384,data);
  printf("Finished SHA2-384\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting SHA2-512\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_data(WC_HASH_TYPE_SHA512,data);
  printf("Finished SHA2-512\n");
  printf("################################\n");
  wolfCrypt_Cleanup();

  printf("Starting SHA3-224\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_data(WC_HASH_TYPE_SHA3_224,data);
  printf("Finished SHA3-224\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting SHA3-256\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_data(WC_HASH_TYPE_SHA3_256,data);
  printf("Finished SHA3-256\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting SHA3-384\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_data(WC_HASH_TYPE_SHA3_384,data);
  printf("Finished SHA3-384\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting SHA3-512\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_data(WC_HASH_TYPE_SHA3_512,data);
  printf("Finished SHA3-512\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2B-224\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2b(data,28);
  printf("Finished BLAKE2B-224\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2B-256\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2b(data,32);
  printf("Finished BLAKE2B-256\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2B-384\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2b(data,48);
  printf("Finished BLAKE2B-384\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2B-512\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2b(data,64);
  printf("Finished BLAKE2B-512\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2S-128\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2s(data,16);
  printf("Finished BLAKE2S-128\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2S-192\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2s(data,24);
  printf("Finished BLAKE2S-192\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2S-224\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2s(data,28);
  printf("Finished BLAKE2S-224\n");
  printf("################################\n");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  printf("Starting BLAKE2S-256\n");
#ifdef REPEAT  
  for(int i = 0;i<REP_TIMES;i++)  
#endif
  hash_blake2s(data,32);
  printf("Finished BLAKE2S-256\n");
  printf("################################\n");

  wolfCrypt_Cleanup();

  return 0;

}
