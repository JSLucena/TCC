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
#include <wolfssl/wolfcrypt/hash.h>
#include <wolfssl/wolfcrypt/md5.h>
#include <wolfssl/wolfcrypt/sha.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/sha512.h>
#include <wolfssl/wolfcrypt/sha3.h>
#include <wolfssl/wolfcrypt/blake2.h>

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

void hash_data(enum wc_HashType type,char* data){

byte result[512];
uint32_t tick,tock;
uint32_t spent;
word32 hash_len = wc_HashGetDigestSize(type);
int i, ret;
int byte_value;

 tick = KIN1_GetCycleCounter();
 ret = wc_Hash(type,(byte*)data,strlen(data),result,hash_len);
 
  sprintf(print_string,"%d\n\r",ret);

  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
     
 tock = KIN1_GetCycleCounter();
 spent = (tock - tick);
  sprintf(print_string,"Elapsed time: %ld clock cycles\n\r", spent);
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
 #ifdef DEBUG
    for(i = 0;i < hash_len;i++)
    {
      byte_value = result[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(500);
    }
   // sprintf("\n\r");
  #endif
}
void hash_blake2b(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2b b2b;
  byte hash[64];

  
  // initialize Blake2b structure with 64 byte digest
  uint32_t tick,tock;
  uint32_t spent;
  tick = KIN1_GetCycleCounter();
  ret = wc_InitBlake2b(&b2b, digestsize);

  ret = wc_Blake2bUpdate(&b2b,(byte*)data,strlen(data));

  ret = wc_Blake2bFinal(&b2b, hash, digestsize);
  
 
  tock = KIN1_GetCycleCounter();
  spent = (tock - tick);
  sprintf(print_string,"Elapsed time: %u clock cycles\n\r", spent);
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(100);
    }
   // sprintf("\n\r");
  #endif
}

void hash_blake2s(char* data, int digestsize)
{
  int i, ret;
  int byte_value;
  Blake2s b2s;
  byte hash[64];
  byte result[512];
  // initialize Blake2b structure with 64 byte digest
  uint32_t tick,tock;
  uint32_t spent;
  tick = KIN1_GetCycleCounter();
  wc_InitBlake2s(&b2s, digestsize);
  wc_Blake2sUpdate(&b2s,(byte*)data,strlen(data));
  ret = wc_Blake2sFinal(&b2s, hash, digestsize);
  if( ret != 0) {
	  sprintf(print_string,"ERROR\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  }
  tock = KIN1_GetCycleCounter();
  spent = (tock - tick);
  sprintf(print_string,"Elapsed time: %ld clock cycles\n\r", spent);
  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
  HAL_Delay(500);
  #ifdef DEBUG
    for(i = 0;i < digestsize;i++)
    {
      byte_value = hash[i];
      sprintf(print_string,"%x",byte_value);
      HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
      HAL_Delay(500);
    }
  //  HAL_UART_transmit("\n\r");
  #endif
}

int ret;
u_int8_t byt;
int byte_value;
short i;
char data[] = "The quick brown fox jumps over the lazy dog";
byte result[32];
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
    sprintf(print_string,"Starting MD5\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i<REP_TIMES;i++)
    #endif
      hash_data(WC_HASH_TYPE_MD5,data);
    sprintf(print_string,"Finished MD5\n\r");
    HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
  // HAL_UART_Transmit("################################\n\r");
    wolfCrypt_Cleanup();
    wolfCrypt_Init();


     sprintf(print_string,"Starting SHA1\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i<REP_TIMES;i++)
    #endif
	    hash_data(WC_HASH_TYPE_SHA,data);
	  sprintf(print_string,"Finished SHA1\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	//  HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

    sprintf(print_string,"Starting SHA2-224\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i<REP_TIMES;i++)
    #endif
      hash_data(WC_HASH_TYPE_SHA224,data);
	  sprintf(print_string,"Finished SHA2-224\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	 // HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

    sprintf(print_string,"Starting SHA2-256\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i<REP_TIMES;i++)
    #endif
	    hash_data(WC_HASH_TYPE_SHA256,data);
	  sprintf(print_string,"Finished SHA2-256\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	 // HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

    sprintf(print_string,"Starting SHA2-384\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i<REP_TIMES;i++)
    #endif
	  hash_data(WC_HASH_TYPE_SHA384,data);
	  sprintf(print_string,"Finished SHA2-384\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	//  HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA2-512\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
    #ifdef REPEAT
      for(int i = 0;i<REP_TIMES;i++)
    #endif
	  hash_data(WC_HASH_TYPE_SHA512,data);
	  sprintf(print_string,"Finished SHA2-512\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	 // HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();


    sprintf(print_string,"Starting SHA3-224\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_224,data);
	  sprintf(print_string,"Finished SHA3-224\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	 // HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA3-256\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_256,data);
	  sprintf(print_string,"Finished SHA3-256\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	 // HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA3-384\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_384,data);
	  sprintf(print_string,"Finished SHA3-384\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	 // HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting SHA3-512\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_data(WC_HASH_TYPE_SHA3_512,data);
	  sprintf(print_string,"Finished SHA3-512\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
    HAL_Delay(500);
	//  HAL_UART_Transmit("################################\n\r");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();
*/
    sprintf(print_string,"Starting BLAKE2B-224\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,28);
	  sprintf(print_string,"Finished BLAKE2B-224\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	  //HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2B-256\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,32);
	  sprintf(print_string,"Finished BLAKE2B-256\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	 // HAL_UART_Transmit("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2B-384\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,48);
	  sprintf(print_string,"Finished BLAKE2B-384\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	  //printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();
	  sprintf(print_string,"Starting BLAKE2B-512\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2b(data,64);
	  sprintf(print_string,"Finished BLAKE2B-512\n\r");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	//  printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-128\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,16);
	  sprintf(print_string,"Finished BLAKE2S-128\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	  //printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-192\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,24);
	  sprintf(print_string,"Finished BLAKE2S-192\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	 // printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-224\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,28);
	  sprintf(print_string,"Finished BLAKE2S-224\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	  //printf("################################\n");
	  wolfCrypt_Cleanup();
	  wolfCrypt_Init();

	  sprintf(print_string,"Starting BLAKE2S-256\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	#ifdef REPEAT
	  for(int i = 0;i<REP_TIMES;i++)
	#endif
	  hash_blake2s(data,32);

	  sprintf(print_string,"Finished BLAKE2S-256\n");
	  HAL_UART_Transmit(&UartHandle, (uint8_t*)print_string, strlen(print_string), 1000);
	 // printf("################################\n");

	  wolfCrypt_Cleanup();

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
