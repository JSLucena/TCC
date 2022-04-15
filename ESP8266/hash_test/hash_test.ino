#include <wolfssl.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/blake2.h>
#include <user_settings.h>

#define DEBUG


void hash_data(enum wc_HashType type,char* data){
  
byte result[512];
long int t1,t2,elapsed;
word32 hash_len =wc_HashGetDigestSize(type);
int i, ret;
int byte_value;

  t1 = micros();
 if(wc_Hash(type,(byte*)data,strlen(data),result,hash_len))
     Serial.println("ERROR");    
  t2 = micros();
 // Serial.print(data);
 // Serial.println();
 // Serial.print(strlen((char*)data));
 // Serial.println();
 // Serial.print(hash_len);
 // Serial.println();
  elapsed = t2-t1;
  Serial.print("Time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");
 #ifdef DEBUG
    for(i = 0;i < hash_len;i++)
    {
      byte_value = result[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}
void hash_blake2b(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2b b2b;
  byte hash[64];

  // initialize Blake2b structure with 64 byte digest
  long int t1,t2,elapsed;
  t1 = micros();
  wc_InitBlake2b(&b2b, digestsize);
  wc_Blake2bUpdate(&b2b,(byte*)data,strlen(data));
  ret = wc_Blake2bFinal(&b2b, hash, digestsize);
  if( ret != 0) {
      Serial.println("ERROR"); 
  }
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

void hash_blake2s(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2s b2s;
  byte hash[64];

  // initialize Blake2b structure with 64 byte digest
  long int t1,t2,elapsed;
  t1 = micros();
  wc_InitBlake2s(&b2s, digestsize);
  wc_Blake2sUpdate(&b2s,(byte*)data,strlen(data));
  ret = wc_Blake2sFinal(&b2s, hash, digestsize);
  if( ret != 0) {
      Serial.println("ERROR"); 
  }
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

int ret;
uint8_t byt;
int byte_value;
short i;
char data[] = "The quick brown fox jumps over the lazy dog";
byte result[32];

void setup() {
  Serial.begin(9600);
  Serial.flush();
  while (!Serial);
  // put your setup code here, to run once:
  
  

/*
Serial.println();
Serial.print(sizeof(result));
Serial.println();

for(i = 0;i < sizeof(result);i++)
{
  byte_value = result[i];
  Serial.print(byte_value,HEX);
}
  Serial.println();
  */
}
void loop(){
  Serial.println("Starting MD5");
  hash_data(WC_HASH_TYPE_MD5,data);
  Serial.println("Finished MD5");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting MD5");
  hash_data(WC_HASH_TYPE_MD5,data);
  Serial.println("Finished MD5");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();
  
  Serial.println("Starting SHA1");
  hash_data(WC_HASH_TYPE_SHA,data);
  Serial.println("Finished SHA1");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting SHA2-224");
  hash_data(WC_HASH_TYPE_SHA224,data);
  Serial.println("Finished SHA2-224");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting SHA2-256");
  hash_data(WC_HASH_TYPE_SHA256,data);
  Serial.println("Finished SHA2-256");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting SHA2-384");
  hash_data(WC_HASH_TYPE_SHA384,data);
  Serial.println("Finished SHA2-384");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting SHA2-512");
  hash_data(WC_HASH_TYPE_SHA512,data);
  Serial.println("Finished SHA2-512");
  Serial.println("################################");
  wolfCrypt_Cleanup();

  Serial.println("Starting SHA3-224");
  hash_data(WC_HASH_TYPE_SHA3_224,data);
  Serial.println("Finished SHA3-224");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting SHA3-256");
  hash_data(WC_HASH_TYPE_SHA3_256,data);
  Serial.println("Finished SHA3-256");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting SHA3-384");
  hash_data(WC_HASH_TYPE_SHA3_384,data);
  Serial.println("Finished SHA3-384");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting SHA3-512");
  hash_data(WC_HASH_TYPE_SHA3_512,data);
  Serial.println("Finished SHA3-512");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2B-224");
  hash_blake2b(data,28);
  Serial.println("Finished BLAKE2B-224");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2B-256");
  hash_blake2b(data,32);
  Serial.println("Finished BLAKE2B-256");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2B-384");
  hash_blake2b(data,48);
  Serial.println("Finished BLAKE2B-384");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2B-512");
  hash_blake2b(data,64);
  Serial.println("Finished BLAKE2B-512");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2S-128");
  hash_blake2s(data,16);
  Serial.println("Finished BLAKE2S-128");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2S-192");
  hash_blake2s(data,24);
  Serial.println("Finished BLAKE2S-192");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2S-224");
  hash_blake2s(data,28);
  Serial.println("Finished BLAKE2S-224");
  Serial.println("################################");
  wolfCrypt_Cleanup();
  wolfCrypt_Init();

  Serial.println("Starting BLAKE2S-256");
  hash_blake2s(data,32);
  Serial.println("Finished BLAKE2S-256");
  Serial.println("################################");

  wolfCrypt_Cleanup();


  while(1)
    delay(1000);
}
