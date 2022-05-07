

//#include <../Header/user_settings.h>
#include <user_settings.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <wolfssl/wolfcrypt/error-crypt.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/random.h> /* for CUSTOM_RAND_TYPE */
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/fe_operations.h>
#include <wolfssl/wolfcrypt/ge_operations.h>
#include <wolfssl/wolfcrypt/kdf.h>
#include <wolfssl/wolfcrypt/hmac.h>

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
//#define DSAK //ta com bug em alguma coisa.
//#define DHK
//#define ECCK
//#define ED25519K
//#define ED448K Nao cabe ?????


void test_RSA(word32 key_len,word32* data, word32 size) //Teste para chaves que nao sao de 2048bits ta com problema
{

  RsaKey  key;
  WC_RNG  rng;
  long int tick,tock,elapsed;
  long int spent;

  byte out[512];
  byte decrypt[512];
  word32 sizeout;

  int ret;  

  memset(out,0,sizeof(out));
  memset(decrypt,0,sizeof(decrypt));

  BAD_FUNC_ARG;
  ret = wc_InitRng(&rng);
   switch(ret)
  {
    case MEMORY_E: Serial.println("MEMORY_E\n");break;
    case WINCRYPT_E: Serial.println("WINCRYPT_E\n");break;
    case CRYPTGEN_E: Serial.println("CRYPTGEN_E\n");break;
    case BAD_FUNC_ARG: Serial.println("BAD_FUNC_ARG\n");break;
    case DRBG_CONT_FIPS_E: Serial.println("DRBG_CONT_FIPS_E\n");break;
    case RNG_FAILURE_E: Serial.println("RNG_FAILURE_E\n");break;
  }
  
  
    //Serial.println("Erro no RNG");
    
  ret = wc_InitRsaKey(&key,NULL);
   switch(ret)
  {
    case BAD_FUNC_ARG: Serial.println("BAD FUNC ARG\n");break;
    case MEMORY_E: Serial.println("MEMORY_E\n");break;
  }

 // if(wc_RsaSetRNG(&key,&rng))
  //  Serial.println("erro no set RNG");
   // Serial.println("Erro no init RSA");
  if(wc_RsaSetRNG(&key,&rng))
    Serial.println("erro no set RNG");

  tick = micros();
  ret = wc_MakeRsaKey(&key, key_len, 65537, &rng);
  switch(ret)
  {
    case BAD_FUNC_ARG: Serial.println("BAD FUNC ARG\n");break;
    case RNG_FAILURE_E: Serial.println("RNG_FAILURE_E\n");break;
    case MP_INIT_E: Serial.println("MP_INIT_E\n");break;
    case MP_READ_E: Serial.println("MP_READ_E\n");break;
    case MP_CMP_E: Serial.println("MP_CMP_E\n");break;
    case MP_INVMOD_E: Serial.println("MP_INVMOD_E\n");break;
    case MP_EXPTMOD_E: Serial.println("MP_EXPTMOD_E\n");break;
  }

  tock = micros();
  
  
  spent = tock - tick;
  Serial.print("Key Generation time "); Serial.print(spent); Serial.println(" microseconds");
  tick = micros();
  ret = wc_RsaSSL_Sign((byte*)data,size,out,sizeof(out),&key,&rng);
  if(ret < 0)
    Serial.println(ret);
  tock = micros();
  
  spent = tock - tick;
  Serial.print("Data sign time "); Serial.print(spent); Serial.println(" microseconds");

  tick = micros();
  ret=  wc_RsaSSL_Verify(out,ret,decrypt,sizeof(decrypt),&key);
  if(ret < 0)
    Serial.println(ret);
  tock = micros();
  
  spent = tock - tick;
  Serial.print("Data Verify time "); Serial.print(spent); Serial.println(" microseconds");


  memset(out,0,sizeof(out));
  memset(decrypt,0,sizeof(decrypt));


  tick = micros();
  ret = wc_RsaPublicEncrypt((byte*)data,size,out,sizeof(out),&key,&rng);
  if(ret < 0)
    Serial.println(ret);
  tock = micros();
  
  spent = tock - tick;
  Serial.print("Public encryption time "); Serial.print(spent); Serial.println(" microseconds");

  tick = micros();
  ret = wc_RsaPrivateDecrypt(out,ret,decrypt,sizeof(decrypt),&key);
  if(ret  < 0)
    Serial.println(ret);
  tock = micros();
  
  spent = tock - tick;
  Serial.print("Private Decryption time "); Serial.print(spent); Serial.println(" microseconds");


  wc_FreeRsaKey(&key);
}

void test_DSA(word32 key_len,word32* data,word32 size)//remover TFM_TIMING_RESISTANT
{
  DsaKey key;
  WC_RNG  rng;
  long int tick,tock,elapsed;
  long int spent;
  int ret;

  int answer;
  byte sig[256];
 
  ret = wc_InitRng(&rng);
  Serial.println(ret);
  ret = wc_InitDsaKey(&key);
  Serial.println(ret);
  tick = micros();
  ret = wc_MakeDsaParameters(&rng,2048,&key);
  Serial.println(ret);
  ret = wc_MakeDsaKey(&rng,&key);
  
  tock = micros();
  Serial.println(ret);
  
  spent = tock - tick;
  Serial.printf("Key Generation time: %d microseconds\n", spent);

  tick = micros();
 ret = wc_DsaSign((byte*)data,sig,&key,&rng);
 
  tock = micros();
  Serial.println(ret);
  spent = tock - tick;
  Serial.printf("Data sign time: %d microseconds\n", spent);

  tick = micros();
  ret = wc_DsaVerify((byte*)data,sig,&key,&answer);
  
  tock = micros();
  Serial.println(ret);
  spent = tock - tick;
  Serial.printf("Data verify time: %ld microseconds, %d\n", spent,answer);

  wc_FreeDsaKey(&key);
}
/*
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
  long int tick,tock,elapsed;
  long int spent;

  for(int i =0;i< sizeof(pub_2);i++)
    pub_2[i] = 0xFF;

  
  ret = wc_InitRng(&rng);
  ret = wc_InitDhKey(&key);
  Serial.println("%d",ret);
  tick = micros();
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
  //Serial.println("%d#%d",par.p_len,par.g_len);
  ret = wc_DhSetKey(&key,par.p,par.p_len,par.g,par.g_len);
  Serial.println("%d",ret);
  
  ret = wc_DhGenerateKeyPair(&key, &rng, priv, &privSz, pub, &pubSz);
  Serial.println("%d",ret);
  tock = micros();
  
  spent = tock - tick;
  Serial.println("Key Generation time: %d microseconds\n", spent);


  tick = micros();
  ret = wc_DhAgree(&key, agree, agreeSz, priv, sizeof(priv), pub_2, sizeof(pub_2));
  Serial.println("%d",ret);
  tock = micros();
  
  spent = tock - tick;
  Serial.println("Secret agreement time: %lf microseconds\n", spent);

  wc_FreeDhKey(&key);
}
*/


void test_ECC(word32 key_len, word32*data, word32 size) //Crasha quando tento liberar a memoria
{
  ecc_key key, pub;
  WC_RNG  rng;
  long int tick,tock,elapsed;
  long int spent;

  byte sig[512];
  byte out[512];
  word32 sig_len, out_len;
  int stat,ret;
  mp_int r, s;
  wc_InitRng(&rng);
  wc_ecc_init(&key);
  
  sig_len = sizeof(sig);
  out_len = sizeof(out);
  tick = micros();
  ret = wc_ecc_make_key(&rng,key_len,&key);
 
  tock = micros();
  Serial.println(ret);
  
  spent = tock - tick;
  Serial.printf("Key Generation time: %d microseconds\n", spent);
  
  tick = micros();
  ret = wc_ecc_sign_hash((byte*)data,size,sig,&sig_len,&rng,&key);
  
  tock = micros();
  Serial.println(ret);
  
  spent = tock - tick;
  Serial.printf("Data sign time: %d microseconds\n", spent);



  
  tick = micros();
  ret = wc_ecc_verify_hash(sig,sig_len,(byte*)data,size,&stat,&key);
  
  tock = micros();
  Serial.println(ret);
  
  spent = tock - tick;
  Serial.printf("Data verify time: %d microseconds\n", spent);

  sig_len = sizeof(sig);
  tick = micros();
  wc_ecc_encrypt(&key,&key,(byte*)data,size,sig,&sig_len,NULL);
  
  tock = micros();
  Serial.println(ret);
  spent = tock - tick;
  Serial.printf("Data encrypt time: %dmicroseconds\n", spent);

  tick = micros();
  wc_ecc_decrypt(&key,&key,sig,sig_len,out,&out_len,NULL);
 
  tock = micros();
   Serial.println(ret);
  
  spent = tock - tick;
  Serial.printf("Data decrypt time: %d microseconds\n", spent);

  //wc_ecc_key_free(&key);
}

void test_ED25519(word32*data,word32 size)
{
  ed25519_key key;
  WC_RNG  rng;
  long int tick,tock,elapsed;
  long int spent;
  int ret, ans;
  word32 sig_len;
  byte sig[64];
  sig_len = sizeof(sig);
  wc_InitRng(&rng);
  ret = wc_ed25519_init(&key);
  Serial.printf("%d",ret);

  tick = micros();
  ret = wc_ed25519_make_key(&rng,32,&key);
  tock = micros();
  Serial.printf("%d",ret);
  
  spent = tock - tick;
  Serial.printf("Key Generation time: %d microseconds\n", spent);

  tick = micros();
  ret = wc_ed25519_sign_msg((byte*)data,size,sig,&sig_len,&key);
  tock = micros();
  Serial.printf("%d",ret);
  
  spent = tock - tick;
  Serial.printf("Data sign time: %d microseconds\n", spent);

  tick = micros();
  ret = wc_ed25519_verify_msg(sig,sig_len,(byte*)data,size,&ans ,&key);
  tock = micros();
  Serial.printf("%d %d",ret,ans);
  
  spent = tock - tick;
  Serial.printf("Data verify time: %d microseconds\n", spent);

  wc_ed25519_free(&key); 

}
/*
void test_ED448(word32*data,word32 size)
{
  ed448_key key;
  WC_RNG  rng;
  long int tick,tock,elapsed;
  long int spent;
  int ret, ans;
  word32 sig_len;
  byte sig[128];
  sig_len = sizeof(sig);
  wc_InitRng(&rng);
  ret = wc_ed448_init(&key);
  Serial.printf("%d",ret);

  tick = micros();
  ret = wc_ed448_make_key(&rng,57,&key);
  tock = micros();
  Serial.printf("%d",ret);
  
  spent = tock - tick;
  Serial.printf("Key Generation time: %d microseconds\n", spent);

  tick = micros();
  ret = wc_ed448_sign_msg((byte*)data,size,sig,&sig_len,&key,(byte*)data,size);
  tock = micros();
  Serial.printf("%d",ret);
  
  spent = tock - tick;
  Serial.printf("Data sign time: %d microseconds\n", spent);

  tick = micros();
  ret = wc_ed448_verify_msg(sig,sig_len,(byte*)data,size,&ans ,&key,(byte*)data,size);
  tock = micros();
  Serial.printf("%d %d",ret,ans);
  
  spent = tock - tick;
  Serial.printf("Data verify time: %d microseconds\n", spent);

  wc_ed448_free(&key); 

}
*/
int ret;
char data[] = "The quick brown fox jumps over the lazy dog";
word32 in[4] ={ 0xDEADBEEF, 0xA5A5A5A5, 0xDEADBEEF, 0xA5A5A5A5};
byte result[32];

//int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  Serial.flush();
  while (!Serial);  



  #ifdef RSAK
  //Serial.println("#######################\n");
  //Serial.println("Starting RSA 512bit key\n");
  //  #ifdef REPEAT
  //for(int i = 0;i < REP_TIMES;i++)
  //  #endif
  //test_RSA(512,in,16);
  //Serial.println("Finished RSA 512bit key\n");
    Serial.println("#######################\n");
    Serial.println("Starting RSA 3072bit key\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_RSA(3072,in,16);
    Serial.println("Finished RSA 3072bit key\n");
    Serial.println("#######################\n");
    Serial.println("Starting RSA 2048bit key\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_RSA(2048,in,16);
    Serial.println("Finished RSA 2048bit key\n");
    
//    Serial.println("#######################\n");
//    Serial.println("Starting RSA 4096bit key\n");
//      #ifdef REPEAT
//    for(int i = 0;i < REP_TIMES;i++)
//      #endif
//    test_RSA(4096,in,16);
//    Serial.println("Finished RSA 4096bit key\n");
  #endif
  #ifdef DSAK
    Serial.println("Starting DSA 2048\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DSA(2048,in,16);
    Serial.println("Finished DSA 2048\n");
    Serial.println("#######################\n");
    Serial.println("Starting DSA 3072\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DSA(3072,in,16);
    Serial.println("Finished DSA 3072\n");
  #endif
  #ifdef DHK
    Serial.println("Starting DH 2048\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DH(2048);
    Serial.println("Finished DH 2048\n");
    Serial.println("#######################\n");
    Serial.println("Starting DH 4096\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_DH(4096);
    Serial.println("Finished DH 4096\n");
  #endif
  #ifdef ECCK
    Serial.println("#######################\n");
    Serial.println("Starting ECC256\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_ECC(32,in,16);
    Serial.println("Finished ECC256\n");
//    Serial.println("#######################\n");
//    Serial.println("Starting ECC192\n");
//      #ifdef REPEAT
//    for(int i = 0;i < REP_TIMES;i++)
//      #endif
//    test_ECC(24,in,16);
//    Serial.println("Finished ECC192\n");
  #endif
  #ifdef ED25519K
    Serial.println("#######################\n");
    Serial.println("Starting ED25519\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_ED25519(in,16);
    Serial.println("Finished ED25519\n");
  #endif
  #ifdef ED448K
    Serial.println("#######################\n");
    Serial.println("Starting ED448\n");
      #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
      #endif
    test_ED448(in,16);
    Serial.println("Finished ED448\n");
  #endif

}

void loop()
{
  //while(1)
    delay(1000);
}
