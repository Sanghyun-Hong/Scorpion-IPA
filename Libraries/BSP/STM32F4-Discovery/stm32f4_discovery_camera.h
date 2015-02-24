

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4_DISCOVERY_CAMERA_H
#define __STM32F4_DISCOVERY_CAMERA_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_io.h"

/* Include Camera component Driver */
#include "../Components/mt9m111/mt9m111.h"

/* Include uart debug function */
extern HAL_StatusTypeDef UB_UART_Debug(const char * format, ...);
   
/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F4_DISCOVERY
  * @{
  */
    
/** @addtogroup STM32F4_DISCOVERY_CAMERA
  * @{
  */ 
   
/** @defgroup STM32F4_DISCOVERY_CAMERA_Exported_Types
  * @{
  */
  
/** 
  * @brief  Camera State structures definition  
  */  
typedef enum 
{
  CAMERA_OK       = 0x00,
  CAMERA_ERROR    = 0x01,
  CAMERA_TIMEOUT  = 0x02 
}Camera_StatusTypeDef;

#define RESOLUTION_R160x120      CAMERA_R160x120      /* QQVGA Resolution     */
#define RESOLUTION_R320x240      CAMERA_R320x240      /* QVGA Resolution      */
#define RESOLUTION_R480x272      CAMERA_R480x272      /* 480x272 Resolution   */
#define RESOLUTION_R640x480      CAMERA_R640x480      /* VGA Resolution       */  
/**
  * @}
  */ 
 
/** @defgroup STM32F4_DISCOVERY_CAMERA_Exported_Constants
  * @{
  */
/* Slave address when SADDR is high  */
#define MT9M111_I2C_ADDRESS     0xBA
#define MT9M111_I2c_ADDRESS_RD  0xBB

/**
  * @}
  */

/** @defgroup STM32F4_DISCOVERY_CAMERA_Exported_Functions
  * @{
  */    
uint8_t BSP_CAMERA_Init(uint32_t Resolution);  
void    BSP_CAMERA_ContinuousStart(uint8_t *buff);
void    BSP_CAMERA_SnapshotStart(uint8_t *buff);
void    BSP_CAMERA_Suspend(void);
void    BSP_CAMERA_Resume(void);
uint8_t BSP_CAMERA_Stop(void); 
void    BSP_CAMERA_LineEventCallback(void);
void    BSP_CAMERA_VsyncEventCallback(void);
void    BSP_CAMERA_FrameEventCallback(void);
void    BSP_CAMERA_ErrorCallback(void);

/* Camera features functions prototype */
void    BSP_CAMERA_ContrastBrightnessConfig(uint32_t contrast_level, uint32_t brightness_level);
void    BSP_CAMERA_BlackWhiteConfig(uint32_t Mode);
void    BSP_CAMERA_ColorEffectConfig(uint32_t Effect);

/* To be called in DCMI_IRQHandler function */
void    BSP_CAMERA_IRQHandler(void);
/* To be called in DMA2_Stream1_IRQHandler function */
void    BSP_CAMERA_DMA_IRQHandler(void);
   
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

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4_DISCOVERY_CAMERA_H */