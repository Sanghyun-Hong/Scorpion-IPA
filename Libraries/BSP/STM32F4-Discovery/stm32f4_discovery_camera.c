
/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery_camera.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */
    
/** @addtogroup STM32F4_DISCOVERY_CAMERA
  * @{
  */ 

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Defines
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Macros
  * @{
  */
/**
  * @}
  */  

/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Variables
  * @{
  */ 
static DCMI_HandleTypeDef  hdcmi_disco;
       CAMERA_DrvTypeDef   *camera_drv;
uint32_t current_resolution;
/**
  * @}
  */ 
  
/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_FunctionPrototypes
  * @{
  */
static void DCMI_MspInit(void);
static uint32_t GetSize(uint32_t resolution);
/**
  * @}
  */ 
  
/** @defgroup STM32F4_DISCOVERY_CAMERA_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the camera.
  * @param  Camera: Pointer to the camera configuration structure
  * @retval Camera status
  */
uint8_t BSP_CAMERA_Init(uint32_t Resolution)
{ 
  DCMI_HandleTypeDef *phdcmi;
  
  uint8_t ret = CAMERA_ERROR;
  
  /* Get the DCMI handle structure */
  phdcmi = &hdcmi_disco;
  
  /*** Configures the DCMI to interface with the camera module ***/
  /* DCMI configuration */
  phdcmi->Init.CaptureRate      = DCMI_CR_ALL_FRAME;  
  phdcmi->Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
  phdcmi->Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
  phdcmi->Init.VSPolarity       = DCMI_VSPOLARITY_LOW;
  phdcmi->Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  phdcmi->Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;
  phdcmi->Instance              = DCMI;  

  /* DCMI Initialization */
  DCMI_MspInit();  
  HAL_DCMI_Init(phdcmi);
  
  if(mt9m111_ReadID(CAMERA_I2C_ADDRESS) == MT9M111_ID)
  { 
    /* Initialize the camera driver structure */
    camera_drv = &mt9m111_drv;     
    
    /* Camera Init */   
    camera_drv->Init(CAMERA_I2C_ADDRESS, Resolution);
    
    /* Return CAMERA_OK status */
    ret = CAMERA_OK;
  } 
  
  current_resolution = Resolution;
  
  return ret;
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
  * @brief  Configures the camera contrast and brightness.
  * @param  contrast_level: Contrast level
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_CONTRAST_LEVEL4: for contrast +2
  *            @arg  CAMERA_CONTRAST_LEVEL3: for contrast +1
  *            @arg  CAMERA_CONTRAST_LEVEL2: for contrast  0
  *            @arg  CAMERA_CONTRAST_LEVEL1: for contrast -1
  *            @arg  CAMERA_CONTRAST_LEVEL0: for contrast -2
  * @param  brightness_level: Contrast level
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_BRIGHTNESS_LEVEL4: for brightness +2
  *            @arg  CAMERA_BRIGHTNESS_LEVEL3: for brightness +1
  *            @arg  CAMERA_BRIGHTNESS_LEVEL2: for brightness  0
  *            @arg  CAMERA_BRIGHTNESS_LEVEL1: for brightness -1
  *            @arg  CAMERA_BRIGHTNESS_LEVEL0: for brightness -2    
  * @retval None
  */
void BSP_CAMERA_ContrastBrightnessConfig(uint32_t contrast_level, uint32_t brightness_level)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_CONTRAST_BRIGHTNESS, contrast_level, brightness_level);
  }  
}

/**
  * @brief  Configures the camera white balance.
  * @param  Mode: black_white mode
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_BLACK_WHITE_BW
  *            @arg  CAMERA_BLACK_WHITE_NEGATIVE
  *            @arg  CAMERA_BLACK_WHITE_BW_NEGATIVE
  *            @arg  CAMERA_BLACK_WHITE_NORMAL       
  * @retval None
  */
void BSP_CAMERA_BlackWhiteConfig(uint32_t Mode)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_BLACK_WHITE, Mode, 0);
  }  
}

/**
  * @brief  Configures the camera color effect.
  * @param  Effect: Color effect
  *          This parameter can be one of the following values:
  *            @arg  CAMERA_COLOR_EFFECT_ANTIQUE               
  *            @arg  CAMERA_COLOR_EFFECT_BLUE        
  *            @arg  CAMERA_COLOR_EFFECT_GREEN    
  *            @arg  CAMERA_COLOR_EFFECT_RED        
  * @retval None
  */
void BSP_CAMERA_ColorEffectConfig(uint32_t Effect)
{
  if(camera_drv->Config != NULL)
  {
    camera_drv->Config(CAMERA_I2C_ADDRESS, CAMERA_COLOR_EFFECT, Effect, 0);
  }  
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
  
  /*** Enable peripherals and GPIO clocks ***/
  /* Enable DCMI clock */
  __DCMI_CLK_ENABLE();

  /* Enable DMA2 clock */
  __DMA2_CLK_ENABLE(); 
  
  /* Enable GPIO clocks */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();

  /*** Configure the GPIO ***/
  /* Configure DCMI GPIO as alternate function */
  GPIO_Init_Structure.Pin       = GPIO_PIN_4 | GPIO_PIN_6 |\
                                  GPIO_PIN_9 | GPIO_PIN_10; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;  
  HAL_GPIO_Init(GPIOA, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7 |\
                                  GPIO_PIN_8 | GPIO_PIN_9; 
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;   
  HAL_GPIO_Init(GPIOB, &GPIO_Init_Structure);

  GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 |\
                                  GPIO_PIN_4;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DCMI;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);
 
  /*** Configure the DMA ***/
  /* Set the parameters to be configured */
  hdma_disco.Init.Channel             = DMA_CHANNEL_1;
  hdma_disco.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_disco.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_disco.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_disco.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_disco.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hdma_disco.Init.Mode                = DMA_CIRCULAR;
  hdma_disco.Init.Priority            = DMA_PRIORITY_HIGH;
  hdma_disco.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;         
  hdma_disco.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
  hdma_disco.Init.MemBurst            = DMA_MBURST_SINGLE;
  hdma_disco.Init.PeriphBurst         = DMA_PBURST_SINGLE; 

  hdma_disco.Instance = DMA2_Stream1;

  /* Associate the initialized DMA handle to the DCMI handle */
  __HAL_LINKDMA(hdcmi, DMA_Handle, hdma_disco);
  
  /*** Configure the NVIC for DCMI and DMA ***/
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
