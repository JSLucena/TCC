/**
  ******************************************************************************
  * @file    UART/UART_WakeUpFromStopUsingFIFO/CM7/Src/main.c
  * @author  MCD Application Team
  * @brief   This example shows how to use UART HAL API to wake up the MCU from
  *          STOP mode using FIFO filling level.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#define DEBUG
//#define REPEAT
//#define REP_TIMES 15



#include "main.h"
#include <../../wolfssl_lib/Header/user_settings.h>

#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/random.h> /* for CUSTOM_RAND_TYPE */
#include <wolfssl/wolfcrypt/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <wolfssl/wolfcrypt/des3.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/arc4.h>
#include <wolfssl/wolfcrypt/camellia.h>
#include <wolfssl/wolfcrypt/chacha20_poly1305.h>
#include <wolfssl/wolfcrypt/wc_encrypt.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup UART_WakeUpFromStopUsingFIFO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define HAL_TIMEOUT_VALUE 0xFFFFFFFF
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;

/* Buffer used for transmission */
uint8_t aTxStartMessage[] = "\n\r ****UART-Hyperterminal TXRX communication ****\n\r";
uint8_t ubTxStartSizeToSend = sizeof(aTxStartMessage);
uint8_t aTxMessage[] = "\n\r Enter 10 characters using keyboard ...\n\r";
uint8_t ubSizeToSend = sizeof(aTxMessage);

/* Buffer used for reception */
uint8_t RxBuffer[16];
uint8_t *pRxBuffer = RxBuffer;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
#define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
/*!< DWT Control register */
#define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
/*!< DWT Cycle Counter register */
#define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define KIN1_TRCENA_BIT              (1UL<<24)
/*!< Trace enable bit in DEMCR register */

#define KIN1_InitCycleCounter() \
KIN1_DEMCR |= KIN1_TRCENA_BIT
/*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */

#define KIN1_ResetCycleCounter() \
KIN1_DWT_CYCCNT = 0
/*!< Reset cycle counter */

#define KIN1_EnableCycleCounter() \
KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
/*!< Enable cycle counter */

#define KIN1_DisableCycleCounter() \
KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
/*!< Disable cycle counter */

#define KIN1_GetCycleCounter() \
KIN1_DWT_CYCCNT
/*!< Read cycle counter register */

uint32_t cycles; /* number of cycles */

char print_string[200];

void test_DES(byte* key, byte* iv, word32* data, word32 size)
{
  Des enc,dec;
  int ret[2],byte_value;
  int i;
  uint32_t tick,tock;
  uint32_t spent;
  byte out[512];
  byte decrypt[512];


  for(i = 0; i < 512; i++)
  {
    out[i] = 0;
    decrypt[i] = 0;
  }
  tick = KIN1_GetCycleCounter();

  ret[0] = wc_Des_SetKey(&enc,key,iv,DES_ENCRYPTION);
  ret[1] = wc_Des_CbcEncrypt(&enc,out,(byte*)data,size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Encription time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
    printf("\n");
  #endif

  tick = KIN1_GetCycleCounter();

  ret[0] = wc_Des_SetKey(&dec,key,iv,DES_DECRYPTION);
  ret[1] = wc_Des_CbcDecrypt(&dec,decrypt,out,size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Decryption time: %lu clock cycles\n\r",ret[0],ret[1], spent);
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
  #endif
}

void test_3DES(byte* key, byte* iv, word32* data, word32 size)
{
  Des3 enc,dec;
  int ret[2],byte_value;
  int i;
  uint32_t tick,tock;
  uint32_t spent;
  byte out[512];
  byte decrypt[512];


  for(i = 0; i < 512; i++)
  {
    out[i] = 0;
    decrypt[i] = 0;
  }

  tick = KIN1_GetCycleCounter();

  ret[0] = wc_Des3_SetKey(&enc,key,iv,DES_ENCRYPTION);
  ret[1] = wc_Des3_CbcEncrypt(&enc,out,(byte*)data,size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Encryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
  #endif

  tick = KIN1_GetCycleCounter();
  ret[0] = wc_Des3_SetKey(&dec,key,iv,DES_DECRYPTION);
  ret[1] = wc_Des3_CbcDecrypt(&dec,decrypt,out,size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Decryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500); 
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
  #endif
}

void test_AES(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Aes enc,dec;
  int ret[2],byte_value;
  int i;
  uint32_t tick,tock;
  uint32_t spent;
  byte out[512];
  byte decrypt[512];


  for(i = 0; i < 512; i++)
  {
    out[i] = 0;
    decrypt[i] = 0;
  }

  tick = KIN1_GetCycleCounter();


  ret[0] = wc_AesSetKey(&enc,key,key_len,iv,AES_ENCRYPTION);
  ret[1] = wc_AesCbcEncrypt(&enc,out,(byte*)data,size);
  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Encryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
  #endif

  tick = KIN1_GetCycleCounter();

  ret[0] = wc_AesSetKey(&dec,key,key_len,iv,AES_DECRYPTION);
  ret[1] = wc_AesCbcDecrypt(&dec,decrypt,out,size);
  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Decryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
  #endif


}


void test_ARC4(byte* key, word32 key_len, word32* data, word32 size)
{
  Arc4 enc;
  int i, byte_value;
  int ret[2];
  uint32_t tick,tock;
  uint32_t spent;
  byte cipher[size];
  byte decrypted[size];
  // encrypt the plain into cipher
  tick = KIN1_GetCycleCounter();
  ret[0]=wc_Arc4SetKey(&enc, key, key_len);
  ret[1]=wc_Arc4Process(&enc, cipher, (byte*)data, size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Encryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = cipher[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
  #endif
  // decrypt the cipher
  tick = KIN1_GetCycleCounter();
  ret[0]=wc_Arc4SetKey(&enc, key, key_len);
  ret[1]=wc_Arc4Process(&enc, decrypted, cipher, sizeof(cipher));

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Decryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }

  #endif
}

void test_CAMELLIA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  Camellia enc,dec;
  int ret[2],byte_value;
  short i;
  uint32_t tick,tock;
  uint32_t spent;
  byte out[512];
  byte decrypt[512];

  for(i = 0; i < 512; i++)
  {
    out[i] = 0;
    decrypt[i] = 0;
  }

  tick = KIN1_GetCycleCounter();

  ret[0] = wc_CamelliaSetKey(&enc,key,key_len,iv);
  ret[1] = wc_CamelliaCbcEncrypt(&enc,out,(byte*)data,size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Encryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
   #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = out[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }

  #endif

  tick = KIN1_GetCycleCounter();

  ret[0] = wc_CamelliaSetKey(&dec,key,key_len,iv);
  ret[1] = wc_CamelliaCbcDecrypt(&dec,decrypt,out,size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d Decryption time: %lu clock cycles\n\r",ret[0],ret[1], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);  
   #ifdef DEBUG
   for(i = 0;i < size;i++)
    {
      byte_value = decrypt[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }

  #endif


}

void test_CHACHA(byte* key, word32 key_len, byte* iv, word32* data, word32 size)
{
  ChaCha enc;
  int i, byte_value;
  uint32_t tick,tock;
  uint32_t spent;
  int ret[3];
  byte cipher[size];
  byte decrypted[size];

  // encrypt the plain into cipher
  tick = KIN1_GetCycleCounter();
  ret[0] = wc_Chacha_SetKey(&enc, key, key_len);
  ret[1] = wc_Chacha_SetIV(&enc,iv,0);
  ret[2] = wc_Chacha_Process(&enc, cipher, (byte*)data, size);


  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d %d Decryption time: %lu clock cycles\n\r",ret[0],ret[1], ret[2], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = cipher[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }

  #endif

  tick = KIN1_GetCycleCounter();
  ret[0] = wc_Chacha_SetKey(&enc, key, key_len);
  ret[1] = wc_Chacha_SetIV(&enc,iv,0);
  ret[2] = wc_Chacha_Process(&enc, decrypted, cipher, size);

  tock = KIN1_GetCycleCounter();
  spent = tock- tick; 
  sprintf(print_string,"%d %d %d Decryption time: %lu clock cycles\n\r",ret[0],ret[1], ret[2], spent); 
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);  
  #ifdef DEBUG
    for(i = 0;i < size;i++)
    {
      byte_value = decrypted[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }

  #endif
}


int ret;
char data[] = "The quick brown fox jumps over the lazy dog";
word32 in[4] ={ 0xDEADBEEF, 0xA5A5A5A5, 0xDEADBEEF, 0xA5A5A5A5};
byte result[32];
byte key[] = "01234567765432101010101010101010100123456776543210101010101010101010012345677654321010101010101010101001234567765432101010101010101010100123456776543210101010101010101010012345677654321010101010101010101001234567765432101010101010101010100123456776543210101010101010101010";
byte iv[] =  "0123456776543210101010101010101010";
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */



int main(void)
{
  int32_t timeout;
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
    Error_Handler();
  }
  /* STM32H7xx HAL library initialization:
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();



  wolfCrypt_Init();
  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* Configure leds */
  BSP_LED_Init(LED2);

  /*##########################################################################*/
  /*##-1- Configure the UART peripheral ######################################*/
  /*##########################################################################*/

  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits (7 data bit + 1 parity bit) :
                      BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
      - Stop Bit    = One Stop bit
      - Parity      = Odd
      - BaudRate    = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance             = USARTx;
  UartHandle.Init.BaudRate        = 115600;
  UartHandle.Init.WordLength      = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits        = UART_STOPBITS_1;
  UartHandle.Init.Parity          = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl       = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode            = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling    = UART_OVERSAMPLING_16;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Set the RXFIFO threshold */
  HAL_UARTEx_SetRxFifoThreshold(&UartHandle, UART_RXFIFO_THRESHOLD_1_4);

  /* Enable the FIFO mode */
  HAL_UARTEx_EnableFifoMode(&UartHandle);

  /*##-2- Configure UART peripheral for reception process ##########*/  

  /* Enable MCU wakeup by UART */
  HAL_UARTEx_EnableStopMode(&UartHandle);

  /* Enable the UART RX FIFO full interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXFF);

  /* Enable the UART wakeup from stop mode interrupt */
  __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_WUF);

  /*##-3- Start the transmission process #############*/

  /* Send start message */
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxStartMessage, ubTxStartSizeToSend, 1000)!= HAL_OK)
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }


  KIN1_InitCycleCounter(); /* enable DWT hardware */

	KIN1_ResetCycleCounter(); /* reset cycle counter */
  KIN1_EnableCycleCounter(); /* start counting */
  /* Infinite loop */
  while (1)
  {
    /* USART IRQ handler is not anymore routed to HAL_UART_IRQHandler() function 
       and is now based on LL API functions use. 
       Therefore, use of HAL IT based services is no more possible : use TX HAL polling services */
    if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxMessage, ubSizeToSend, 1000)!= HAL_OK)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }

    /* Put UART peripheral in reception process */
    HAL_UART_Receive_IT(&UartHandle, (uint8_t*)&RxBuffer, 10);
    while(HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY);

    /* USART IRQ handler is not anymore routed to HAL_UART_IRQHandler() function 
       and is now based on LL API functions use. 
       Therefore, use of HAL IT based services is no more possible : use TX HAL polling services */
    if(HAL_UART_Transmit(&UartHandle, (uint8_t*)pRxBuffer, 10, 1000)!= HAL_OK)
    {
      /* Transfer error in transmission process */
      Error_Handler();
    }

    /* Toggle LED2 */
    BSP_LED_Toggle(LED2);

    /* Manage temporisation between TX buffer sendings */
    HAL_Delay(500);

    /*
    sprintf(print_string,"Starting DES CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i < REP_TIMES;i++)
    #endif
    test_DES(key,iv,in,16);
    sprintf(print_string,"Finished DES CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    
    sprintf(print_string,"Starting 3DES CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i < REP_TIMES;i++)
    #endif
    test_3DES(key,iv,in,16);
    sprintf(print_string,"Finished 3DES CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    
    sprintf(print_string,"Starting AES-128 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_AES(key,16,iv,in,16);
    sprintf(print_string,"Finished AES-128 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);

    sprintf(print_string,"Starting AES-192 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_AES(key,24,iv,in,16);
    sprintf(print_string,"Finished AES-192 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);

    sprintf(print_string,"Starting AES-256 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_AES(key,32,iv,in,16);
    sprintf(print_string,"Finished AES-256 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    
    sprintf(print_string,"Starting ARC4 40-bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_ARC4(key,5,in,16);
    sprintf(print_string,"Finished ARC4 40-bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);

    sprintf(print_string,"Starting ARC4 256-bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_ARC4(key,32,in,16);
    sprintf(print_string,"Finished ARC4 256-bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);

    sprintf(print_string,"Starting ARC4 2048-bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_ARC4(key,256,in,16);
    sprintf(print_string,"Finished ARC4 2048-bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    

    sprintf(print_string,"Starting Camellia-128 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_CAMELLIA(key,16,iv,in,16);
    sprintf(print_string,"Finished Camellia-128 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);

    sprintf(print_string,"Starting Camellia-192 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif  
    test_CAMELLIA(key,24,iv,in,16);
    sprintf(print_string,"Finished Camellia-192 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);

    sprintf(print_string,"Starting Camellia-256 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_CAMELLIA(key,32,iv,in,16);
    sprintf(print_string,"Finished Camellia-256 CBC\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    */

    sprintf(print_string,"Starting CHACHA-128 bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_CHACHA(key,16,iv,in,16);
    sprintf(print_string,"Finished CHACHA-128 bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);

    sprintf(print_string,"Starting CHACHA-256 bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  #ifdef REPEAT
    for(int i = 0;i < REP_TIMES;i++)
  #endif
    test_CHACHA(key,32,iv,in,16);
    sprintf(print_string,"Finished CHACHA-256 bit key\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE BYPASS)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);
  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

/* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 | RCC_CLOCKTYPE_PCLK1 | \
                                 RCC_CLOCKTYPE_PCLK2  | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    Error_Handler();
  }

}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1) { ; }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n\r", file, line) */

  /* Infinite loop */
  while(1) { ; }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/