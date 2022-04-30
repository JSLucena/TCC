#include <wolfssl.h>
#include <user_settings.h>

//#include <wolfssl/wolfcrypt/curve25519.h>
//#include <wolfssl/wolfcrypt/curve448.h>
//#include <wolfssl/wolfcrypt/ed448.h>
//#include <wolfssl/wolfcrypt/ed25519.h>
//#include <wolfssl/wolfcrypt/ecc.h>
//#include <wolfssl/wolfcrypt/dh.h>
//#include <wolfssl/wolfcrypt/dsa.h>
#include <wolfssl/wolfcrypt/rsa.h>

// tive que mexer o sp_int.h mudar o sp_wordsize e o random.c para mudar o generate_seed()
#define DEBUG
//#define REPEAT
#define REP_TIMES 15


void test_RSA(word32 key_len,word32* data, word32 size)
{

  RsaKey  key;
  WC_RNG  rng;
  long int t1,t2,elapsed;

  int ret;  

  BAD_FUNC_ARG;
  ret = wc_InitRng(&rng);
   switch(ret)
  {
    case MEMORY_E: Serial.println("MEMORY_E");break;
    case WINCRYPT_E: Serial.println("WINCRYPT_E");break;
    case CRYPTGEN_E: Serial.println("CRYPTGEN_E");break;
    case BAD_FUNC_ARG: Serial.println("BAD_FUNC_ARG");break;
    case DRBG_CONT_FIPS_E: Serial.println("DRBG_CONT_FIPS_E");break;
    case RNG_FAILURE_E: Serial.println("RNG_FAILURE_E");break;
  }
  
  
    //Serial.println("Erro no RNG");
    
  ret = wc_InitRsaKey(&key,NULL);
   switch(ret)
  {
    case BAD_FUNC_ARG: Serial.println("BAD FUNC ARG");break;
    case MEMORY_E: Serial.println("MEMORY_E");break;
  }

 // if(wc_RsaSetRNG(&key,&rng))
  //  Serial.println("erro no set RNG");
   // Serial.println("Erro no init RSA");
  //if(wc_RsaSetRNG(key,&rng))
  //  Serial.println("erro no set RNG");

  t1 = micros();
  //ret = wc_MakeRsaKey(&key, key_len, 65537, &rng);
  switch(ret)
  {
    case BAD_FUNC_ARG: Serial.println("BAD FUNC ARG");break;
    case RNG_FAILURE_E: Serial.println("RNG_FAILURE_E");break;
    case MP_INIT_E: Serial.println("MP_INIT_E");break;
    case MP_READ_E: Serial.println("MP_READ_E");break;
    case MP_CMP_E: Serial.println("MP_CMP_E");break;
    case MP_INVMOD_E: Serial.println("MP_INVMOD_E");break;
    case MP_EXPTMOD_E: Serial.println("MP_EXPTMOD_E");break;
  }
  //  Serial.println("erro gerando a chave");

  t2 = micros();
  elapsed = t2 - t1;
  Serial.print("Encryption time elapsed: "); Serial.print(elapsed); Serial.println(" microseconds"); 
  
//wc_FreeRsaKey(&key);
}

int ret;
char data[] = "The quick brown fox jumps over the lazy dog";
word32 in[4] ={ 0xDEADBEEF, 0xA5A5A5A5, 0xDEADBEEF, 0xA5A5A5A5};
byte result[32];

//int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  Serial.flush();
  while (!Serial);
  // put your setup code here, to run once:
  
  //pinMode()
  Serial.println("#######################");
  Serial.println("Starting RSA");
  int done = 0;
    #ifdef REPEAT
  for(int i = 0;i < REP_TIMES;i++)
    #endif
  test_RSA(2048,in,16);
  Serial.println("Finished RSA");
}
void loop(){
  
  

  while(1)
    delay(1000);
}
