/**
  ******************************************************************************
  * @file    ub_timer.h
  * @author  Sanghyun Hong
  * @version V0.1.0
  * @date    18-Febuary-2105
  * @brief   This file includes pre-defined stuffs regarding TIM(Timer)
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UB_TIMER_H
#define __UB_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


#ifdef __cplusplus
 extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
#define  PERIOD_VALUE       (666 - 1)  /* Period Value  */
#define  PULSE1_VALUE       333        /* Capture Compare 1 Value  */
   
/* Exported constants --------------------------------------------------------*/
/*******************************************************************************
  * User can use this section to tailor TIMx instance
  * used and associated resources
  *****************************************************************************/
/* Definition for TIMx clock resources */
#define TIMx                           TIM3
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE

/* Definition for USARTx Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define GPIO_PIN_CHANNEL1              GPIO_PIN_6
#define GPIO_PIN_CHANNEL2              GPIO_PIN_7
#define GPIO_PIN_CHANNEL3              GPIO_PIN_8
#define GPIO_PIN_CHANNEL4              GPIO_PIN_9
/******************************************************************************/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef UB_TIM_PWM_Init(TIM_TypeDef *instance,
                                  uint32_t prescaler,
                                  uint32_t period,
                                  uint32_t clkdivision,
                                  uint32_t countermode);
HAL_StatusTypeDef UB_TIM_PWM_DeInit(void);
HAL_StatusTypeDef UB_TIM_PWM_ConfigChannel(uint32_t polarity,
                                           uint32_t fastmode,
                                           uint32_t pulse,
                                           uint32_t channel);
HAL_StatusTypeDef UB_TIM_PWM_Start(uint32_t channel);
HAL_StatusTypeDef UB_TIM_PWM_Stop(uint32_t channel);


#ifdef __cplusplus
}
#endif

#endif  /* __UB_TIMER_H */