/**
  ******************************************************************************
  * @file    stm32f429i_discovery_camera.c
  * @author  MCD Application Team
  * @version V2.0.3
  * @date    10-December-2014
  * @brief   This file includes the driver for Camera modules mounted on
  *          STM324x9I-EVAL evaluation board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* File Info: ------------------------------------------------------------------
                                   User NOTES
1. How to use this driver:
--------------------------
   - This driver is used to drive the camera.
   - The OV2640 component driver MUST be included with this driver.          

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the camera using the BSP_CAMERA_Init() function.
     o Start the camera capture/snapshot using the CAMERA_Start() function.
     o Suspend, resume or stop the camera capture using the following functions:
      - BSP_CAMERA_Suspend()
      - BSP_CAMERA_Resume()
      - BSP_CAMERA_Stop()
      
  + Options
     o Increase or decrease on the fly the brightness and/or contrast
       using the following function:
       - BSP_CAMERA_ContrastBrightnessConfig 
     o Add a special effect on the fly using the following functions:
       - BSP_CAMERA_BlackWhiteConfig()
       - BSP_CAMERA_ColorEffectConfig()  
      
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f429i_discovery_camera.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */
    
/** @addtogroup STM32F429I_DISCOVERY_CAMERA
  * @{
  */ 

/** @defgroup STM32F429I_DISCOVERY_CAMERA_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_CAMERA_Private_Defines
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup STM32F429I_DISCOVERY_CAMERA_Private_Macros
  * @{
  */
/**
  * @}
  */  

/** @defgroup STM32F429I_DISCOVERY_CAMERA_Private_Variables
  * @{
  */ 
static DCMI_HandleTypeDef  hdcmi_disco;
       CAMERA_DrvTypeDef   *camera_drv;
uint32_t current_resolution;
/**
  * @}
  */ 
  
/** @defgroup STM32F429I_DISCOVERY_CAMERA_Private_FunctionPrototypes
  * @{
  */
/* New Libraries for a Camera Module */
/**
  * If the source code enables the camera power control function,
  * then, these functions will be enabled by the feature.
  */
#ifdef  CAMERA_POWER_CONTROL
  static void BSP_CAMERA_Init_Power(void);
#endif
static void BSP_CAMERA_Init_IO(void);
static void BSP_CAMERA_Init_DCMI(void);
static void BSP_SENSOR_Init_Sensor(void);
    
/* MSP Libraries */
static void DCMI_MspInit(void);
static void DCMI_MspDeInit(void);
static uint32_t GetSize(uint32_t resolution);
/**
  * @}
  */ 
  
/** @defgroup STM32F429I_DISCOVERY_CAMERA_Private_Functions
  * @{
  */
void BSP_CAMERA_Init(void)
{
#ifdef  CAMERA_POWER_CONTROL
  /* Initialize the power */
  BSP_CAMERA_Init_Power();
#endif
  
  /* Initialize the I/O lines */
  BSP_CAMERA_Init_IO();
  
  /* Initialize the DCMI interface */
  BSP_CAMERA_Init_DCMI();
  
  /* Initialize the sensor module */
  BSP_SENSOR_Init_Sensor();
}

/**
  * If the source code enables the camera power control function,
  * then, these functions will be enabled by the feature.
  */
#ifdef  CAMERA_POWER_CONTROL
void BSP_CAMERA_PowerOn(void)
{
  /* Disable the output of the camera module */
  BSP_CAMERA_DisableOutput();
  
  /* Setting the pins related to power on */
  HAL_GPIO_WritePin(POWER_GPIO_PORT,    POWER_PIN,      GPIO_PIN_SET);
  HAL_GPIO_WritePin(STANDBY_GPIO_PORT,  STANDBY_PIN,    GPIO_PIN_SET);
  HAL_GPIO_WritePin(CS_GPIO_PORT,       CS_PIN,         GPIO_PIN_SET);
  HAL_Delay(25);
  
  /* Make the camera be ready */
  BSP_CAMERA_Standby();
}

void BSP_CAMERA_PowerOff(void)
{
  /* Make the camera standby */
  BSP_CAMERA_Standby();
  
  /* Disable the output of the camera module */
  BSP_CAMERA_DisableOutput();
  
  /* Disable the power */
  HAL_GPIO_WritePin(POWER_GPIO_PORT,    POWER_PIN,      GPIO_PIN_RESET);
}

void BSP_CAMERA_Standby(void)
{
  /* Set the standby pin to active low */
  HAL_GPIO_WritePin(STANDBY_GPIO_PORT,  STANDBY_PIN,    GPIO_PIN_RESET);
}

void BSP_CAMERA_WakeUp(void)
{
  /* Set the standby pin to active high */
  HAL_GPIO_WritePin(STANDBY_GPIO_PORT,  STANDBY_PIN,    GPIO_PIN_SET);
  HAL_Delay(5);
}

uint8_t BSP_CAMERA_IsOn(void)
{
  return HAL_GPIO_ReadPin(POWER_GPIO_PORT, POWER_PIN);
}

uint8_t BSP_CAMERA_IsAwake(void)
{
  return HAL_GPIO_ReadPin(STANDBY_GPIO_PORT, STANDBY_PIN);
}

void BSP_CAMERA_EnableOutput(void)
{
  /* Set the cs pin to active low */
  HAL_GPIO_WritePin(CS_GPIO_PORT,       CS_PIN,         GPIO_PIN_SET);
}

void BSP_CAMERA_DisableOutput(void)
{
  /* Set the cs pin to active high */
  HAL_GPIO_WritePin(CS_GPIO_PORT,       CS_PIN,         GPIO_PIN_RESET);
}
#endif

uint16_t BSP_CAMERA_GetVersion(void)
{
  /* Read the version of the camera module */
  return MT9M111_ReadVersion();
}

/**
  * @brief  Starts the camera capture in continuous mode.
  * @param  buff: pointer to the camera output buffer
  * @retval None
  */
void BSP_CAMERA_ContinuousStart(uint8_t *buff)
{ 
  /* Start the camera capture */
  HAL_DCMI_Start_DMA(&hdcmi_disco, DCMI_MODE_CONTINUOUS, (uint32_t)buff, GetSize(current_resolution));  
}

/**
  * @brief  Starts the camera capture in snapshot mode.
  * @param  buff: pointer to the camera output buffer
  * @retval None
  */
void BSP_CAMERA_SnapshotStart(uint8_t *buff)
{ 
  /* Start the camera capture */
  HAL_DCMI_Start_DMA(&hdcmi_disco, DCMI_MODE_SNAPSHOT, (uint32_t)buff, GetSize(current_resolution));  
}

/**
  * @brief Suspend the CAMERA capture 
  * @param  None
  * @retval None
  */
void BSP_CAMERA_Suspend(void) 
{
  /* Disable the DMA */
  __HAL_DMA_DISABLE(hdcmi_disco.DMA_Handle);
  /* Disable the DCMI */
  __HAL_DCMI_DISABLE(&hdcmi_disco);
}

/**
  * @brief Resume the CAMERA capture 
  * @param  None
  * @retval None
  */
void BSP_CAMERA_Resume(void) 
{
  /* Enable the DCMI */
  __HAL_DCMI_ENABLE(&hdcmi_disco);
  /* Enable the DMA */
  __HAL_DMA_ENABLE(hdcmi_disco.DMA_Handle);
}

/**
  * @brief  Stop the CAMERA capture 
  * @param  None
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Stop(void) 
{
  DCMI_HandleTypeDef *phdcmi;
  
  uint8_t ret = CAMERA_ERROR;
  
  /* Get the DCMI handle structure */
  phdcmi = &hdcmi_disco;
  
  if(HAL_DCMI_Stop(phdcmi) == HAL_OK)
  {
    ret = CAMERA_OK;
  }
  
  return ret;
}

/**
  * @brief  Handles DCMI interrupt request.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_IRQHandler(void) 
{
  HAL_DCMI_IRQHandler(&hdcmi_disco);
}

/**
  * @brief  Handles DMA interrupt request.
  * @param  None
  * @retval None
  */
void BSP_CAMERA_DMA_IRQHandler(void) 
{
  HAL_DMA_IRQHandler(hdcmi_disco.DMA_Handle);
}

/**
  * If the source code enables the camera power control function,
  * then, these functions will be enabled by the feature.
  */
#ifdef  CAMERA_POWER_CONTROL
/**
  * @brief  Initialize the power of the module.
  * @param  None.
  * @retval None.
  */
static void BSP_CAMERA_Init_Power(void)
{

  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Configure the GPIO port */
  POWER_GPIO_CLK_ENABLE();
  STANDBY_GPIO_CLK_ENABLE();
  CS_GPIO_CLK_ENABLE();

  /* Configure the GPIO pins */
  GPIO_InitStruct.Pin = POWER_PIN | STANDBY_PIN | CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /* Camera Power on */
  BSP_CAMERA_PowerOn();
}
#endif

/**
  * @brief  Initialize the I/O lines for the module.
  * @param  None.
  * @retval None.
  */
static void BSP_CAMERA_Init_IO(void)
{
  CAMERA_IO_Init();
}

/**
  * @brief  Initilize the DCMI interface for the module.
  * @param  None.
  * @retval None.
  */
static void BSP_CAMERA_Init_DCMI(void)
{
  DCMI_HandleTypeDef *phdcmi;
    
  /* Get the DCMI handle structure */
  phdcmi = &hdcmi_disco;
  
  /* DCMI configuration */
  phdcmi->Init.CaptureRate      = DCMI_CR_ALL_FRAME;  
  phdcmi->Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
  phdcmi->Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
  phdcmi->Init.VSPolarity       = DCMI_VSPOLARITY_LOW;
  phdcmi->Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  phdcmi->Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;
  phdcmi->Instance              = DCMI;  
  HAL_DCMI_Init(phdcmi);

  /* DCMI Initialization */
  DCMI_MspInit();
}

/**
  * @brief  Initialize the sensor module.
  * @param  None.
  * @retval None.
  */
static void BSP_SENSOR_Init_Sensor(void)
{
  // TODO
}

/**
  * @brief  Get the capture size.
  * @param  current_resolution: the current resolution.
  * @retval capture size.
  */
static uint32_t GetSize(uint32_t resolution)
{ 
  uint32_t size = 0;
  
  /* Get capture size */
  switch (resolution)
  {
  case CAMERA_R160x120:
    {
      size =  0x2580;
    }
    break;    
  case CAMERA_R320x240:
    {
      size =  0x9600;
    }
    break;
  case CAMERA_R480x272:
    {
      size =  0xFF00;
    }
    break;
  case CAMERA_R640x480:
    {
      size =  0x25800;
    }    
    break;
  default:
    {
      break;
    }
  }
  
  return size;
}

/**
  * @brief  Initializes the DCMI MSP.
  * @param  None
  * @retval None
  */
static void DCMI_MspInit(void)
{
  static DMA_HandleTypeDef hdma_disco;
  GPIO_InitTypeDef GPIO_Init_Structure;
  DCMI_HandleTypeDef *hdcmi = &hdcmi_disco;
  
  /* Enable DCMI clock */
  __DCMI_CLK_ENABLE();

  /* Enable DMA2 clock */
  __DMA2_CLK_ENABLE(); 
  
  /* Enable GPIO clocks */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();
  __GPIOG_CLK_ENABLE();

  /**   
    *   DCMI GPIO Configuration (alternative function)
    *
    *   PC6   ----> DCMI_D0
    *   PC7   ----> DCMI_D1
    *   PG10  ----> DCMI_D2
    *   PG11  ----> DCMI_D3
    *   PE4   ----> DCMI_D4
    *   PD3   ----> DCMI_D5
    *   PE5   ----> DCMI_D6
    *   PE6   ----> DCMI_D7
    *
    *   PA6   ----> DCMI_PCLK
    *   PA4   ----> DCMI_HREF (HSYNC)
    *   PB7   ----> DCMI_VSYNC
    */
  GPIO_Init_Structure.Pin       = GPIO_PIN_4 | GPIO_PIN_6; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOA, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin       = GPIO_PIN_7; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;   
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;   
  HAL_GPIO_Init(GPIOC, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin       = GPIO_PIN_3; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;   
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin       = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;   
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure); 
  
  GPIO_Init_Structure.Pin       = GPIO_PIN_10 | GPIO_PIN_11; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;   
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);
  
  /* Configure the DMA : Set the parameters to be configured */
  hdma_disco.Init.Channel             = DMA_CHANNEL_1;
  hdma_disco.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_disco.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_disco.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_disco.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_disco.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;    // HALFWORD(?)
  hdma_disco.Init.Mode                = DMA_CIRCULAR;
  hdma_disco.Init.Priority            = DMA_PRIORITY_HIGH;      // VERY_HIGH(?)
  hdma_disco.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
  hdma_disco.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_disco.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_disco.Init.PeriphBurst         = DMA_PBURST_SINGLE; 

  hdma_disco.Instance = DMA2_Stream1;

  /* Associate the initialized DMA handle to the DCMI handle */
  __HAL_LINKDMA(hdcmi, DMA_Handle, hdma_disco);
  
  /* NVIC configuration for DCMI transfer complete interrupt */
  HAL_NVIC_SetPriority(DCMI_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DCMI_IRQn);  
  
  /* NVIC configuration for DMA2D transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  
  /* Configure the DMA stream */
  HAL_DMA_Init(hdcmi->DMA_Handle);  
}

/**
  * @brief  De-Initializes the DCMI MSP.
  * @param  None
  * @retval None
  */
static void DCMI_MspDeInit(void)
{
  /* Disable DMA clock */
  __DMA2_CLK_DISABLE();
  
  /* Disable DCMI clock */
  __DCMI_CLK_DISABLE();
  
  /**   
    *   DCMI GPIO Configuration (alternative function)
    *
    *   PC6   ----> DCMI_D0
    *   PC7   ----> DCMI_D1
    *   PG10  ----> DCMI_D2
    *   PG11  ----> DCMI_D3
    *   PE4   ----> DCMI_D4
    *   PD3   ----> DCMI_D5
    *   PE5   ----> DCMI_D6
    *   PE6   ----> DCMI_D7
    *
    *   PA6   ----> DCMI_PCLK
    *   PA4   ----> DCMI_HREF (HSYNC)
    *   PB7   ----> DCMI_VSYNC
    */
  HAL_GPIO_DeInit( GPIOA, GPIO_PIN_4 | GPIO_PIN_6 );
  HAL_GPIO_DeInit( GPIOB, GPIO_PIN_7 );
  HAL_GPIO_DeInit( GPIOC, GPIO_PIN_6 | GPIO_PIN_7 );
  HAL_GPIO_DeInit( GPIOD, GPIO_PIN_3 );
  HAL_GPIO_DeInit( GPIOE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 );
  HAL_GPIO_DeInit( GPIOG, GPIO_PIN_10 | GPIO_PIN_11 ); 
}

/**
  * @brief  Line event callback
  * @param  hdcmi: pointer to the DCMI handle  
  * @retval None
  */
void HAL_DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_LineEventCallback();
}

/**
  * @brief  Line Event callback.
  * @param  None
  * @retval None
  */
__weak void BSP_CAMERA_LineEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_LineEventCallback could be implemented in the user file
   */
}

/**
  * @brief  VSYNC event callback
  * @param  hdcmi: pointer to the DCMI handle  
  * @retval None
  */
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_VsyncEventCallback();
}

/**
  * @brief  VSYNC Event callback.
  * @param  None
  * @retval None
  */
__weak void BSP_CAMERA_VsyncEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_VsyncEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Frame event callback
  * @param  hdcmi: pointer to the DCMI handle  
  * @retval None
  */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_FrameEventCallback();
}

/**
  * @brief  Frame Event callback.
  * @param  None
  * @retval None
  */
__weak void BSP_CAMERA_FrameEventCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_FrameEventCallback could be implemented in the user file
   */
}

/**
  * @brief  Error callback
  * @param  hdcmi: pointer to the DCMI handle  
  * @retval None
  */
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{        
  BSP_CAMERA_ErrorCallback();
}

/**
  * @brief  Error callback.
  * @param  None
  * @retval None
  */
__weak void BSP_CAMERA_ErrorCallback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_ErrorCallback could be implemented in the user file
   */
  UB_UART_Debug("ERROR: DCMI Error Occured");
}

/**
  * @}
  */  
  
/**
  * @}
  */
  
/**
  * @}
  */
  
/**
  * @}
  */      

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
