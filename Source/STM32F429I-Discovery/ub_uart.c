/**
  ******************************************************************************
  * @file    ub_uart.c
  * @author  Sanghyun Hong
  * @version V0.1.0
  * @date    12-Febuary-2105
  * @brief   This file contains the pre-declared stuffs regarding UART peripherals
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ub_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
    
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize HAL UART
  * @param  instance    : UART instance
  * @param  baudrate    : UART Baudrate
  * @param  stopbits    : UART Stopbits
  * @param  parity      : UART Paritybit
  * @param  hwflowctl   : UART Flow control (CTS or RTS)
  * @param  mode        : UART Operating mode
  * @param  oversampling: UART Oversampling mode
  * @retval HAL_OK      : Successfully initialized
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_UART_Init(USART_TypeDef *instance, 
                               uint32_t baudrate, 
                               uint32_t wordlength, 
                               uint32_t stopbits, 
                               uint32_t parity, 
                               uint32_t hwflowctl, 
                               uint32_t mode, 
                               uint32_t oversampling)
{
  /* Configure the UART peripheral
     Put the USART peripheral in the Asynchronous mode (UART Mode) */
  UartHandle.Instance          = instance;
  
  UartHandle.Init.BaudRate     = baudrate;
  UartHandle.Init.WordLength   = wordlength;
  UartHandle.Init.StopBits     = stopbits;
  UartHandle.Init.Parity       = parity;
  UartHandle.Init.HwFlowCtl    = hwflowctl;
  UartHandle.Init.Mode         = mode;
  UartHandle.Init.OverSampling = oversampling;
    
  /* Initialize HAL UART/USART, and returns corresponding values */
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    return HAL_ERROR;
  }
  else 
  {
    return HAL_OK;
  }
  
}

/**
  * @brief  De-initialize HAL UART
  * @retval HAL_OK      : Successfully de-initialized
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_UART_DeInit(void)
{
  /* De-initialize HAL UART/USART, and returns corresponding values */
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK) 
  {
    return HAL_ERROR;
  }
  else 
  {
    return HAL_OK;
  }
  
}
                              
/**
  * @brief  Transmit the given buffer through uart interface
  * @param  pData       : Data buffer to transfer
  * @param  Size        : Size of data buffer to transfer
  * @param  timeout     : Timeout
  * @retval HAL_OK      : Successfully transmitted
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_UART_Transmit(uint8_t *pData, uint16_t Size)
{
  
  /** Starts the transmission process
    *  - While the UART in reception process, user can transmit data through 
    *    "aTxBuffer" buffer 
    */
  if(HAL_UART_Transmit(&UartHandle, pData, Size, USARTx_TIMEOUT)!= HAL_OK)
  {
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
  
}

/**
  * @brief  Receive the data from UART interface, and store it to a buffer
  * @param  pData       : Buffer to store the data
  * @param  Size        : Size of data read from UART
  * @param  timeout     : Timeout
  * @retval HAL_OK      : Successfully received
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_UART_Receive(uint8_t *pData, uint16_t Size)
{
  /** Put UART peripheral in reception process
    *  - 
    */  
  if(HAL_UART_Receive(&UartHandle, pData, Size, USARTx_TIMEOUT) != HAL_OK)
  {
    return HAL_ERROR;
  }
  else 
  {
    return HAL_OK;
  }
  
}

/**
  * @brief  Print the debug message through UART peripheral
  * @param  str         : Debug message string
  * @param  len         : Debug message length
  * @retval HAL_OK      : Successfully received
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_UART_Debug(uint8_t * str, uint16_t len)
{
  HAL_StatusTypeDef ret;
  
  /* Sends debug message through UART interface */ 
  if(HAL_UART_Transmit(&UartHandle, (uint8_t *)str, len, USARTx_TIMEOUT) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  else 
  {
    ret = HAL_OK;
  }
  
  return ret;
}
