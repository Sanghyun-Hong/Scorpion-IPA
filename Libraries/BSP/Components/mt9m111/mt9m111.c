

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

CAMERA_DrvTypeDef   mt9m111_drv = 
{
  MT9M111_Init,
};

/**
  * @}
  */
  
/** @defgroup MT9M111_Private_Functions
  * @{
  */ 
  
/**
  * @brief  Initializes the MT9M111 CAMERA component.
  * @param  DeviceAddr: Device address on communication Bus.
  * @param  resolution: Camera resolution
  * @retval None
  */
void MT9M111_Init(uint16_t DeviceAddr, uint32_t resolution)
{
  uint32_t index;
  
  /* Initialize I2C */
  CAMERA_IO_Init();
  
  /* FIXME - Initialize MT9M111 */

}

/**
  * @brief  Read the MT9M111 Camera version.
  * @param  DeviceAddr: Device address on communication Bus.
  * @retval the MT9M111 Version
  */
uint16_t MT9M111_ReadVersion(uint16_t DeviceAddr)
{
  /* Initialize I2C */
  CAMERA_IO_Init();
  
  /* Get the camera version */
  return (CAMERA_IO_ReadWord(DeviceAddr, MT9M111_CHIP_VERSION));
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
