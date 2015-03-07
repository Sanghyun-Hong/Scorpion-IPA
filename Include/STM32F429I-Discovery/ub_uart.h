/**
  ******************************************************************************
  * @file    ub_uart.h
  * @author  Sanghyun Hong
  * @version V0.1.0
  * @date    12-Febuary-2105
  * @brief   This file contains the pre-defined stuffs regarding UART peripherals
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UB_UART_H
#define __UB_UART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*******************************************************************************
  * User can use this section to tailor USARTx/UARTx instance
  * used and associated resources
  *****************************************************************************/
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA  
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA 
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler
   
/* Definition for USARTx's Timeout */
#define USARTx_TIMEOUT                   5000
/******************************************************************************/

/* Size of Transmission/Reception buffer */
#define BUFFERSIZE(x)           (COUNTOF(x) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)     (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef UB_UART_Init(USART_TypeDef *instance, 
                               uint32_t baudrate, 
                               uint32_t wordlength, 
                               uint32_t stopbits, 
                               uint32_t parity, 
                               uint32_t hwflowctl, 
                               uint32_t mode, 
                               uint32_t oversampling);
HAL_StatusTypeDef UB_UART_DeInit(void);
HAL_StatusTypeDef UB_UART_Transmit(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef UB_UART_Receive(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef UB_UART_Debug(const char * format, ...);

#ifdef __cplusplus
}
#endif

#endif  /* __UB_UART_H */