#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/random.h> /* for CUSTOM_RAND_TYPE */
#include <wolfssl/wolfcrypt/types.h>
#include <user_settings.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <wolfssl/wolfcrypt/des3.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/arc4.h>
#include <wolfssl/wolfcrypt/camellia.h>
//#include <wolfssl/wolfcrypt/rabbit.h> //investigar
#include <wolfssl/wolfcrypt/chacha20_poly1305.h>
//#include <wolfssl/wolfcrypt/idea.h>
//#include <wolfssl/wolfcrypt/hc_128.h>
#include <wolfssl/wolfcrypt/wc_encrypt.h>

// NOTA, Rabbit, IDEA e HC_128 foram removidos por serem considerados obsoletos?. Investigar

//#define DEBUG
#define REPEAT
#define REP_TIMES 15

void test_DES(byte* key, byte* iv, word32* data, word32 size)
{
  Des enc,dec;
  int ret,byte_value;
  short i;
  struct timespec tick,tock;
  double spent;
  byte out[512];
  byte decrypt[512];

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_Des_SetKey(&enc,key,iv,DES_ENCRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_Des_CbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    printf("Error encrypting DES");

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encription time: %.0lf nanoseconds\n", spent); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_Des_SetKey(&dec,key,iv,DES_DECRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_Des_CbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    printf("Error encrypting DES");

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decription time: %.0lf nanoseconds\n", spent);  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif
}

void test_3DES(byte* key, byte* iv, word32* data, word32 size)
{
  Des3 enc,dec;
  int ret,byte_value;
  short i;
  struct timespec tick,tock;
  double spent;
  byte out[512];
  byte decrypt[512];

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_Des3_SetKey(&enc,key,iv,DES_ENCRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_Des3_CbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    printf("Error encrypting DES3");

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encription time: %.0lf nanoseconds\n", spent); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_Des3_SetKey(&dec,key,iv,DES_DECRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_Des3_CbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    printf("Error encrypting DES3");

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decription time: %.0lf nanoseconds\n", spent);  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif
}

void test_AES(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Aes enc,dec;
  int ret,byte_value;
  short i;
  struct timespec tick,tock;
  double spent;
  byte out[512];
  byte decrypt[512];

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_AesSetKey(&enc,key,key_len,iv,AES_ENCRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_AesCbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    printf("Error encrypting AES");
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encryption time: %.0lf nanoseconds\n", spent); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_AesSetKey(&dec,key,key_len,iv,AES_DECRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_AesCbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    printf("Error encrypting AES");
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decryption time: %.0lf nanoseconds\n", spent);  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif


}

void test_ARC4(byte* key, word32 key_len, word32* data, word32 size)
{
  Arc4 enc;
  int i, byte_value;
  struct timespec tick,tock;
  double spent;
  byte cipher[size];
  byte decrypted[size];
  // encrypt the plain into cipher
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_Arc4SetKey(&enc, key, key_len);
  wc_Arc4Process(&enc, cipher, (byte*)data, size);

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encryption time: %.0lf nanoseconds\n", spent); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif
  // decrypt the cipher
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_Arc4SetKey(&enc, key, key_len);
  wc_Arc4Process(&enc, decrypted, cipher, sizeof(cipher));

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decryption time: %.0lf nanoseconds\n", spent);  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif
}

void test_CAMELLIA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Camellia enc,dec;
  int ret,byte_value;
  short i;
  struct timespec tick,tock;
  double spent;
  byte out[512];
  byte decrypt[512];

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_CamelliaSetKey(&enc,key,key_len,iv);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_CamelliaCbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    printf("Error encrypting Camellia");
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encryption time: %.0lf nanoseconds\n", spent); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_CamelliaSetKey(&dec,key,key_len,iv);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_CamelliaCbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    printf("Error decrypting CAmellia");
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decryption time: %.0lf nanoseconds\n", spent);  
   #ifdef DEBUG
   for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif


}

void test_CHACHA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  ChaCha enc;
  int i, byte_value;
  struct timespec tick,tock;
  double spent;
  byte cipher[size];
  byte decrypted[size];

  // encrypt the plain into cipher
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_Chacha_SetKey(&enc, key, key_len);
  wc_Chacha_SetIV(&enc,iv,0);
  wc_Chacha_Process(&enc, cipher, (byte*)data, size);

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encryption time: %.0lf nanoseconds\n", spent); 
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_Chacha_SetKey(&enc, key, key_len);
  wc_Chacha_SetIV(&enc,iv,0);
  wc_Chacha_Process(&enc, decrypted, cipher, size);

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decryption time: %.0lf nanoseconds\n", spent);  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif
}

void test_CHACHA20_POLY1305(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  int i, byte_value;
  struct timespec tick,tock;
  double spent;
  byte cipher[size];
  byte decrypted[size];
  byte auth_tag[16];
  byte aad[] = "";
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_ChaCha20Poly1305_Encrypt(key,iv, aad,0,(byte*)data,size,cipher,auth_tag);
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encryption time: %.0lf nanoseconds\n", spent); 
  #ifdef DEBUG
   for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_ChaCha20Poly1305_Decrypt(key,iv, aad,0,cipher,size,decrypted,auth_tag);
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decryption time: %.0lf nanoseconds\n", spent);  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      printf("%x",byte_value);
    }
    printf("\n");
  #endif

}
/*
void test_RABBIT(byte* key, byte* iv, word32* data, word32 size)
{
  Rabbit enc;
  int i, byte_value;
  struct timespec tick,tock;
  double spent;
  byte cipher[size];
  byte decrypted[size];
  // encrypt the plain into cipher
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_RabbitSetKey(&enc, key,iv);
  wc_RabbitProcess(&enc, cipher, (byte*)data, size);

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encryption time: %.0lf nanoseconds\n", spent); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = cipher[i];
      Serial.print(byte_value,HEX);
    }
    printf();
  #endif
  // decrypt the cipher
  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);
  wc_RabbitSetKey(&enc, key,iv);
  wc_RabbitProcess(&enc, decrypted, cipher, size);

  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decryption time: %.0lf nanoseconds\n", spent);  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      Serial.print(byte_value,HEX);
    }
    printf();
  #endif
}*/
/*
void test_IDEA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Idea enc,dec;
  int ret,byte_value;
  short i;
  struct timespec tick,tock;
  double spent;
  byte out[512];
  byte decrypt[512];

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_IdeaSetKey(&enc,key,key_len,iv,IDEA_ENCRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_IdeaCbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    printf("Error encrypting IDEA");
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Encryption time: %.0lf nanoseconds\n", spent); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      Serial.print(byte_value,HEX);
    }
    printf();
  #endif

  clock_gettime(CLOCK_MONOTONIC_RAW,&tick);

  ret = wc_IdeaSetKey(&dec,key,key_len,iv,IDEA_DECRYPTION);
  if(ret)
    printf("Error setting Key/IV");
  ret = wc_IdeaCbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    printf("Error encrypting IDEA");
  clock_gettime(CLOCK_MONOTONIC_RAW,&tock);
  spent = (double)(tock.tv_nsec - tick.tv_nsec);// / (double)CLOCKS_PER_SEC;
  printf("Decryption time: %.0lf nanoseconds\n", spent);  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      Serial.print(byte_value,HEX);
    }
    printf();
  #endif
}
* */

int ret;
char data[] = "The quick brown fox jumps over the lazy dog";
word32 in[4] ={ 0xDEADBEEF, 0xA5A5A5A5, 0xDEADBEEF, 0xA5A5A5A5};
byte result[32];
byte key[] = "01234567765432101010101010101010100123456776543210101010101010101010012345677654321010101010101010101001234567765432101010101010101010100123456776543210101010101010101010012345677654321010101010101010101001234567765432101010101010101010100123456776543210101010101010101010";
byte iv[] =  "0123456776543210101010101010101010";

int main(){
  
  printf("#######################\n");
  printf("Starting DES CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_DES(key,iv,in,16);
  printf("Finished DES CBC\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting DES3 CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_3DES(key,iv,in,16);
  printf("Finished DES3 CBC\n");
  printf("#######################\n");

  printf("#######################\n");
  printf("Starting AES-128 CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_AES(key,16,iv,in,16);
  printf("Finished AES-128 CBC\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting AES-192 CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_AES(key,24,iv,in,16);
  printf("Finished AES-192 CBC\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting AES-256 CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_AES(key,32,iv,in,16);
  printf("Finished AES-256 CBC\n");
  printf("#######################\n");

  printf("#######################\n");
  printf("Starting ARC4 40-bit key\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_ARC4(key,5,in,16);
  printf("Finished ARC4 40-bit key\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting ARC4 256-bit key\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_ARC4(key,32,in,16);
  printf("Finished ARC4 256-bit key\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting ARC4 2048-bit key\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_ARC4(key,256,in,16);
  printf("Finished ARC4 2048-bit key\n");
  printf("#######################\n");

  printf("#######################\n");
  printf("Starting Camellia-128 CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CAMELLIA(key,16,iv,in,16);
  printf("Finished Camellia-128 CBC\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting Camellia-192 CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif  
  test_CAMELLIA(key,24,iv,in,16);
  printf("Finished Camellia-192 CBC\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting Camellia-256 CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CAMELLIA(key,32,iv,in,16);
  printf("Finished Camellia-256 CBC\n");
  printf("#######################\n");

  printf("#######################\n");
  printf("Starting CHACHA 128-bit key\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CHACHA(key,16,iv,in,16);
  printf("Finished CHACHA 128-bit key\n");
  printf("#######################\n");
  printf("#######################\n");
  printf("Starting CHACHA 256-bit key\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CHACHA(key,32,iv,in,16);
  printf("Finished CHACHA 256-bit key\n");
  printf("#######################\n"); 


 //Ta retornando uns valor mto estranho
 /*
  printf("#######################");
  printf("Starting CHACHA-POLY1305 256-bit key");
  test_CHACHA20_POLY1305(key,32,iv,in,16);
  printf("Finished CHACHA-POLY1305 256-bit key");
  printf("#######################");  
 */ 
  /*
  printf("Starting RABBIT\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_RABBIT(key,iv,in,16);
  printf("Finished RABBIT\n");
  printf("#######################\n"); 
  printf("#######################\n");
  **/
  /*
  printf("Starting IDEA CBC\n");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_IDEA(key,16,iv,in,16);
  printf("Finished IDEA CBC\n");
  printf("#######################\n"); 
*/
  return 0;
}
