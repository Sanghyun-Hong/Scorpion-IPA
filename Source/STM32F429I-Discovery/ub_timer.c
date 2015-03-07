/**
  ******************************************************************************
  * @file    ub_timer.c
  * @author  Sanghyun Hong
  * @version V0.1.0
  * @date    18-Febuary-2105
  * @brief   This file includes TIM(Timer)-related functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ub_timer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef       TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef      sConfig;
    
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize HAL TIM (PWM mode)
  * @param  instance    : TIM instance
  * @param  prescaler   : TIM Prescaler value
  * @param  period      : TIM Period
  * @param  clkdivision : TIM Clock divider
  * @param  countermode : TIM Counter mode
  * @retval HAL_OK      : Successfully initialized
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_TIM_PWM_Init(TIM_TypeDef *instance,
                                  uint32_t prescaler,
                                  uint32_t period,
                                  uint32_t clkdivision,
                                  uint32_t countermode)
{
  /* Configure the TIM peripheral */
  TimHandle.Instance = instance;
  
  TimHandle.Init.Prescaler = prescaler;
  TimHandle.Init.Period = period;
  TimHandle.Init.ClockDivision = clkdivision;
  TimHandle.Init.CounterMode = countermode;
  
  /* Initialize HAL TIM, and returns corresponding values */
  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    return HAL_ERROR;
  }
  else 
  {
    return HAL_OK;
  }
  
}

/**
  * @brief  De-initialize HAL TIM (PWM mode)
  * @retval HAL_OK      : Successfully de-initialized
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_TIM_PWM_DeInit(void)
{
  /* De-initialize HAL TIM, and returns corresponding values */
  if(HAL_TIM_PWM_DeInit(&TimHandle) != HAL_OK) 
  {
    return HAL_ERROR;
  }
  else 
  {
    return HAL_OK;
  }
  
}

/**
  * @brief  Configure the PWM channel with the given values
  * @param  polarity    : PWM output polarity
  * @param  fastmode    : PWM output fastmode
  * @param  pulse       : PWM pulse type
  * @param  channel     : PWM channel select
  * @retval HAL_OK      : Successfully configured
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_TIM_PWM_ConfigChannel(uint32_t polarity,
                                           uint32_t fastmode,
                                           uint32_t pulse,
                                           uint32_t channel)
{
  /* Initialize basic configurations */
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.OCPolarity = polarity;
  sConfig.OCFastMode = fastmode;
  sConfig.Pulse = pulse;
  
  /* Configure HAL TIM with the configuration above, and returns corresponding value */
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, channel) != HAL_OK)
  {
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
  
}

/**
  * @brief  Generate the PWM signal with the given channel
  * @param  channel     : PWM channel select
  * @retval HAL_OK      : Successfully started
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_TIM_PWM_Start(uint32_t channel)
{
  
  /* PWM Signal generation start */
  if(HAL_TIM_PWM_Start(&TimHandle, channel) != HAL_OK)
  { 
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
  
}

/**
  * @brief  Stop the PWM signal generation
  * @param  channel     : PWM channel select
  * @retval HAL_OK      : Successfully stopped
            HAL_ERROR   : An error occured
  */
HAL_StatusTypeDef UB_TIM_PWM_Stop(uint32_t channel)
{
  
  /* PWM Signal generation start */
  if(HAL_TIM_PWM_Stop(&TimHandle, channel) != HAL_OK)
  { 
    return HAL_ERROR;
  }
  else
  {
    return HAL_OK;
  }
  
}

