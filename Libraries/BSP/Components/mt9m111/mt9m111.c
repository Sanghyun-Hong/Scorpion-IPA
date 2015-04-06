

/* Includes ------------------------------------------------------------------*/
#include "mt9m111.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup MT9M111
  * @brief     This file provides a set of functions needed to drive the 
  *            MT9M111 Camera module.
  * @{
  */

/** @defgroup MT9M111_Private_TypesDefinitions
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup MT9M111_Private_Defines
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup MT9M111_Private_Macros
  * @{
  */
     
/**
  * @}
  */  
  
/** @defgroup MT9M111_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup MT9M111_Private_Variables
  * @{
  */ 
  
/** @defgroup MT9M111_Private_Functions
  * @{
  */ 
  
/**
  * @brief  Initializes the MT9M111 module.
  * @param  None.
  * @retval None.
  */
void MT9M111_Init(void)
{ 
  /* Initialize I2C */
  CAMERA_IO_Init();
}

/**
  * @brief  Read the MT9M111 Camera version.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval The MT9M111 module version.
  */
uint16_t MT9M111_ReadVersion(void)
{
  /* Get the camera version */
  return (CAMERA_IO_ReadWord(MT9M111_I2C_ADDRESS, MT9M111_CHIP_VERSION));
}

/******************************************************************************
                            Static Functions
*******************************************************************************/

         
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
