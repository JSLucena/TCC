#include <wolfssl.h>
#include <user_settings.h>

#include <wolfssl/wolfcrypt/des3.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/arc4.h>
#include <wolfssl/wolfcrypt/camellia.h>
#include <wolfssl/wolfcrypt/rabbit.h>
#include <wolfssl/wolfcrypt/chacha20_poly1305.h>
#include <wolfssl/wolfcrypt/idea.h>
//#include <wolfssl/wolfcrypt/hc_128.h>
#include <wolfssl/wolfcrypt/wc_encrypt.h>

//#define DEBUG
#define REPEAT
#define REP_TIMES 15

void test_DES(byte* key, byte* iv, word32* data, word32 size)
{
  Des enc,dec;
  int ret,byte_value;
  short i;
  long int t1,t2,elapsed;
  byte out[512];
  byte decrypt[512];

  t1 = micros();

  ret = wc_Des_SetKey(&enc,key,iv,DES_ENCRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_Des_CbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    Serial.println("Error encrypting DES");

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

  t1 = micros();

  ret = wc_Des_SetKey(&dec,key,iv,DES_DECRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_Des_CbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    Serial.println("Error encrypting DES");

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

void test_3DES(byte* key, byte* iv, word32* data, word32 size)
{
  Des3 enc,dec;
  int ret,byte_value;
  short i;
  long int t1,t2,elapsed;
  byte out[512];
  byte decrypt[512];

  t1 = micros();

  ret = wc_Des3_SetKey(&enc,key,iv,DES_ENCRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_Des3_CbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    Serial.println("Error encrypting DES3");

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

  t1 = micros();

  ret = wc_Des3_SetKey(&dec,key,iv,DES_DECRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_Des3_CbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    Serial.println("Error encrypting DES3");

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

void test_AES(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Aes enc,dec;
  int ret,byte_value;
  short i;
  long int t1,t2,elapsed;
  byte out[512];
  byte decrypt[512];

  t1 = micros();

  ret = wc_AesSetKey(&enc,key,key_len,iv,AES_ENCRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_AesCbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    Serial.println("Error encrypting AES");
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

  t1 = micros();

  ret = wc_AesSetKey(&dec,key,key_len,iv,AES_DECRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_AesCbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    Serial.println("Error encrypting AES");
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif


}

void test_ARC4(byte* key, word32 key_len, word32* data, word32 size)
{
  Arc4 enc;
  int i, byte_value;
  long int t1,t2,elapsed;
  byte cipher[size];
  byte decrypted[size];
  // encrypt the plain into cipher
  t1 = micros();
  wc_Arc4SetKey(&enc, key, key_len);
  wc_Arc4Process(&enc, cipher, (byte*)data, size);

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = cipher[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
  // decrypt the cipher
  t1 = micros();
  wc_Arc4SetKey(&enc, key, key_len);
  wc_Arc4Process(&enc, decrypted, cipher, sizeof(cipher));

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

void test_CAMELLIA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Camellia enc,dec;
  int ret,byte_value;
  short i;
  long int t1,t2,elapsed;
  byte out[512];
  byte decrypt[512];

  t1 = micros();

  ret = wc_CamelliaSetKey(&enc,key,key_len,iv);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_CamelliaCbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    Serial.println("Error encrypting Camellia");
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

  t1 = micros();

  ret = wc_CamelliaSetKey(&dec,key,key_len,iv);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_CamelliaCbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    Serial.println("Error decrypting CAmellia");
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif


}

void test_CHACHA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  ChaCha enc;
  int i, byte_value;
  long int t1,t2,elapsed;
  byte cipher[size];
  byte decrypted[size];

  // encrypt the plain into cipher
  t1 = micros();
  wc_Chacha_SetKey(&enc, key, key_len);
  wc_Chacha_SetIV(&enc,iv,0);
  wc_Chacha_Process(&enc, cipher, (byte*)data, size);

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = cipher[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

  t1 = micros();
  wc_Chacha_SetKey(&enc, key, key_len);
  wc_Chacha_SetIV(&enc,iv,0);
  wc_Chacha_Process(&enc, decrypted, cipher, size);

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

void test_CHACHA20_POLY1305(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  int i, byte_value;
  long int t1,t2,elapsed;
  byte cipher[size];
  byte decrypted[size];
  byte auth_tag[16];
  byte aad[] = "";
  t1 = micros();
  wc_ChaCha20Poly1305_Encrypt(key,iv, aad,0,(byte*)data,size,cipher,auth_tag);
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = cipher[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

  t1 = micros();
  wc_ChaCha20Poly1305_Decrypt(key,iv, aad,0,cipher,size,decrypted,auth_tag);
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

}

void test_RABBIT(byte* key, byte* iv, word32* data, word32 size)
{
  Rabbit enc;
  int i, byte_value;
  long int t1,t2,elapsed;
  byte cipher[size];
  byte decrypted[size];
  // encrypt the plain into cipher
  t1 = micros();
  wc_RabbitSetKey(&enc, key,iv);
  wc_RabbitProcess(&enc, cipher, (byte*)data, size);

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = cipher[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
  // decrypt the cipher
  t1 = micros();
  wc_RabbitSetKey(&enc, key,iv);
  wc_RabbitProcess(&enc, decrypted, cipher, size);

  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

void test_IDEA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Idea enc,dec;
  int ret,byte_value;
  short i;
  long int t1,t2,elapsed;
  byte out[512];
  byte decrypt[512];

  t1 = micros();

  ret = wc_IdeaSetKey(&enc,key,key_len,iv,IDEA_ENCRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_IdeaCbcEncrypt(&enc,out,(byte*)data,size);
  if(ret)
    Serial.println("Error encrypting IDEA");
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif

  t1 = micros();

  ret = wc_IdeaSetKey(&dec,key,key_len,iv,IDEA_DECRYPTION);
  if(ret)
    Serial.println("Error setting Key/IV");
  ret = wc_IdeaCbcDecrypt(&dec,decrypt,out,size);
  if(ret)
    Serial.println("Error encrypting IDEA");
  t2 = micros();
  elapsed = t2-t1;
  Serial.print("Decryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds");  
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      Serial.print(byte_value,HEX);
    }
    Serial.println();
  #endif
}

int ret;
char data[] = "The quick brown fox jumps over the lazy dog";
word32 in[4] ={ 0xDEADBEEF, 0xA5A5A5A5, 0xDEADBEEF, 0xA5A5A5A5};
byte result[32];
byte key[] = "01234567765432101010101010101010100123456776543210101010101010101010012345677654321010101010101010101001234567765432101010101010101010100123456776543210101010101010101010012345677654321010101010101010101001234567765432101010101010101010100123456776543210101010101010101010";
byte iv[] =  "0123456776543210101010101010101010";

void setup() {
  Serial.begin(9600);
  Serial.flush();
  while (!Serial);
  // put your setup code here, to run once:
  
  
}
void loop(){
  
  Serial.println("#######################");
  Serial.println("Starting DES CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_DES(key,iv,in,16);
  Serial.println("Finished DES CBC");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting DES3 CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_3DES(key,iv,in,16);
  Serial.println("Finished DES3 CBC");
  Serial.println("#######################");

  Serial.println("#######################");
  Serial.println("Starting AES-128 CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_AES(key,16,iv,in,16);
  Serial.println("Finished AES-128 CBC");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting AES-192 CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_AES(key,24,iv,in,16);
  Serial.println("Finished AES-192 CBC");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting AES-256 CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_AES(key,32,iv,in,16);
  Serial.println("Finished AES-256 CBC");
  Serial.println("#######################");

  Serial.println("#######################");
  Serial.println("Starting ARC4 40-bit key");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_ARC4(key,5,in,16);
  Serial.println("Finished ARC4 40-bit key");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting ARC4 256-bit key");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_ARC4(key,32,in,16);
  Serial.println("Finished ARC4 256-bit key");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting ARC4 2048-bit key");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_ARC4(key,256,in,16);
  Serial.println("Finished ARC4 2048-bit key");
  Serial.println("#######################");

  Serial.println("#######################");
  Serial.println("Starting Camellia-128 CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CAMELLIA(key,16,iv,in,16);
  Serial.println("Finished Camellia-128 CBC");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting Camellia-192 CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif  
  test_CAMELLIA(key,24,iv,in,16);
  Serial.println("Finished Camellia-192 CBC");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting Camellia-256 CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CAMELLIA(key,32,iv,in,16);
  Serial.println("Finished Camellia-256 CBC");
  Serial.println("#######################");

  Serial.println("#######################");
  Serial.println("Starting CHACHA 128-bit key");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CHACHA(key,16,iv,in,16);
  Serial.println("Finished CHACHA 128-bit key");
  Serial.println("#######################");
  Serial.println("#######################");
  Serial.println("Starting CHACHA 256-bit key");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_CHACHA(key,32,iv,in,16);
  Serial.println("Finished CHACHA 256-bit key");
  Serial.println("#######################"); 


/* Ta retornando uns valor mto estranho
  Serial.println("#######################");
  Serial.println("Starting CHACHA-POLY1305 256-bit key");
  test_CHACHA20_POLY1305(key,32,iv,in,16);
  Serial.println("Finished CHACHA-POLY1305 256-bit key");
  Serial.println("#######################");  
  */
  Serial.println("Starting RABBIT");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_RABBIT(key,iv,in,16);
  Serial.println("Finished RABBIT");
  Serial.println("#######################"); 
  Serial.println("#######################");
  Serial.println("Starting IDEA CBC");
#ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
#endif
  test_IDEA(key,16,iv,in,16);
  Serial.println("Finished IDEA CBC");
  Serial.println("#######################"); 

  while(1)
    delay(1000);
}
