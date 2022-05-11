


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
#include <wolfSSL.I-CUBE-wolfSSL_conf.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <main.h>



/* UART definitions */
#ifndef HAL_CONSOLE_UART
#define HAL_CONSOLE_UART huart3
#endif
extern UART_HandleTypeDef HAL_CONSOLE_UART;

//#define DEBUG
#define REPEAT
#define REP_TIMES 15



char print_string[200];

void hash_data(enum wc_HashType type,char* data){

byte result[512];
clock_t tick,tock;
double spent;
word32 hash_len = wc_HashGetDigestSize(type);
int i, ret;
int byte_value;

 tick =  clock();
 if(wc_Hash(type,(byte*)data,strlen(data),result,hash_len))
     sprintf(print_string,"ERROR\n");
 tock = clock();
 spent = (float)(tock - tick) / CLOCKS_PER_SEC;
  sprintf(print_string,"Elapsed time: %.0lf nanoseconds\n", spent);
  HAL_UART_Transmit(&HAL_CONSOLE_UART,(uint8_t*)print_string, strlen(print_string), 60);
 #ifdef DEBUG
    for(i = 0;i < hash_len;i++)
    {
      byte_value = result[i];
      sprintf("%x",byte_value);
    }
   // sprintf("\n");
  #endif
}
void hash_blake2b(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2b b2b;
  byte hash[64];

  // initialize Blake2b structure with 64 byte digest
  clock_t tick,tock;
  double spent;
  tick =  clock();
  wc_InitBlake2b(&b2b, digestsize);
  wc_Blake2bUpdate(&b2b,(byte*)data,strlen(data));
  ret = wc_Blake2bFinal(&b2b, hash, digestsize);
  if( ret != 0) {
	  sprintf(print_string,"ERROR\n");
  }
  tock = clock();
  spent = (float)(tock - tick) / CLOCKS_PER_SEC;
  sprintf(print_string,"Elapsed time: %.0lf nanoseconds\n", spent);
  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      sprintf("%x",byte_value);
    }
   // sprintf("\n");
  #endif
}

void hash_blake2s(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2s b2s;
  byte hash[64];

  // initialize Blake2b structure with 64 byte digest
  clock_t tick,tock;
  double spent;
  tick =  clock();
  wc_InitBlake2s(&b2s, digestsize);
  wc_Blake2sUpdate(&b2s,(byte*)data,strlen(data));
  ret = wc_Blake2sFinal(&b2s, hash, digestsize);
  if( ret != 0) {
	  sprintf(print_string,"ERROR\n");
  }
  tock = clock();
  spent = (float)(tock - tick) / CLOCKS_PER_SEC;
  sprintf(print_string,"Elapsed time: %.0lf nanoseconds\n", spent);
  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      sprintf(print_string,"%x",byte_value);
    }
  //  HAL_UART_transmit("\n");
  #endif
}

int ret;
u_int8_t byt;
int byte_value;
short i;
char data[] = "The quick brown fox jumps over the lazy dog";
byte result[32];


void wolfCryptDemo(const void* argument)
{

	sprintf(print_string,"Starting MD5\n");
	HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	    hash_data(WC_HASH_TYPE_MD5,data);
	  sprintf(print_string,"Finished MD5\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA1\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	    hash_data(WC_HASH_TYPE_SHA,data);
	  sprintf(print_string,"Finished SHA1\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	//  HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA2-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	    hash_data(WC_HASH_TYPE_SHA224,data);
	  sprintf(print_string,"Finished SHA2-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA2-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	    hash_data(WC_HASH_TYPE_SHA256,data);
	  sprintf(print_string,"Finished SHA2-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA2-384\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA384,data);
	  sprintf(print_string,"Finished SHA2-384\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	//  HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf("Starting SHA2-512\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA512,data);
	  sprintf(print_string,"Finished SHA2-512\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA3-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_224,data);
	  sprintf(print_string,"Finished SHA3-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA3-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_256,data);
	  sprintf(print_string,"Finished SHA3-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA3-384\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_384,data);
	  sprintf(print_string,"Finished SHA3-384\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA3-512\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_512,data);
	  sprintf(print_string,"Finished SHA3-512\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	//  HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2B-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,28);
	  sprintf(print_string,"Finished BLAKE2B-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	  //HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2B-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,32);
	  sprintf(print_string,"Finished BLAKE2B-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2B-384\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,48);
	  sprintf(print_string,"Finished BLAKE2B-384\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	  //printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2B-512\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,64);
	  sprintf(print_string,"Finished BLAKE2B-512\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	//  printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-128\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,16);
	  sprintf(print_string,"Finished BLAKE2S-128\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	  //printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-192\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,24);
	  sprintf(print_string,"Finished BLAKE2S-192\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,28);
	  sprintf(print_string,"Finished BLAKE2S-224\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	  //printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,32);

	  sprintf(print_string,"Finished BLAKE2S-256\n");
	  HAL_UART_Transmit(&HAL_CONSOLE_UART,print_string, strlen(print_string), 60);
	 // printf("################################\n");

	  wolfCrypt_Cleanup();

	  while(1)
		  HAL_Delay(1000);
}
