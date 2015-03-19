/**
  ******************************************************************************
  * @file    usbh_video.c
  * @author  Hong, Sanghyun
  * @version V0.1.0
  * @date    17-March-2015
  * @brief   This file is the Video Layer Handlers for USB Host Video class.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 Hong, Sanghyun</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_video.h"

/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_VIDEO_CLASS
* @{
*/

/** @defgroup USBH_VIDEO_CORE 
* @brief    This file includes VIDEO Layer Handlers for USB Host VIDEO class.
* @{
*/ 

/** @defgroup USBH_VIDEO_CORE_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_VIDEO_CORE_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_VIDEO_CORE_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_VIDEO_CORE_Private_Variables
* @{
*/
/**
* @}
*/ 


/** @defgroup USBH_VIDEO_CORE_Private_FunctionPrototypes
* @{
*/ 

static USBH_StatusTypeDef USBH_VIDEO_InterfaceInit  (USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_InterfaceDeInit  (USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_ClassRequest (USBH_HandleTypeDef *phost);


USBH_ClassTypeDef  VIDEO_Class = 
{
  "VIDEO",
  USB_VIDEO_CLASS,
  USBH_VIDEO_InterfaceInit,
  USBH_VIDEO_InterfaceDeInit,
  USBH_VIDEO_ClassRequest,
  USBH_VIDEO_Process,
  NULL,                         // The SOF pin is not used
  NULL,                         // The data pointer is not used
};
/**
* @}
*/ 


/** @defgroup USBH_VIDEO_CORE_Private_Functions
* @{
*/ 

/**
  * @brief  USBH_VIDEO_InterfaceInit 
  *         The function init the VIDEO class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_InterfaceInit (USBH_HandleTypeDef *phost)
{	

  return USBH_OK;
}



/**
  * @brief  USBH_VIDEO_InterfaceDeInit 
  *         The function DeInit the Pipes used for the VIDEO class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VIDEO_InterfaceDeInit (USBH_HandleTypeDef *phost)
{

  return USBH_OK;
}

/**
  * @brief  USBH_VIDEO_ClassRequest 
  *         The function is responsible for handling Standard requests
  *         for VIDEO class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_ClassRequest (USBH_HandleTypeDef *phost)
{   

  return USBH_OK; 
}


/**
  * @brief  USBH_VIDEO_Process 
  *         The function is for managing state machine for VIDEO data transfers 
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_Process (USBH_HandleTypeDef *phost)
{
 
  return USBH_OK;
}


/**
  * @brief  USBH_VIDEO_Init 
  *         The function Initialize the VIDEO function
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VIDEO_Init (USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef Status = USBH_BUSY;
#if (USBH_USE_OS == 1)
  osEvent event;
  
  event = osMessageGet( phost->class_ready_event, osWaitForever );
  
  if( event.status == osEventMessage )      
  {
    if(event.value.v == USBH_CLASS_EVENT)
    {
#else 
      
  while ((Status == USBH_BUSY) || (Status == USBH_FAIL))
  {
    /* Host background process */
    USBH_Process(phost);
    if(phost->gState == HOST_CLASS)
    {
#endif        
      Status = USBH_OK;
    }
  }
  return Status;   
}

/**
  * @brief  USBH_VIDEO_IOProcess 
  *         VIDEO VIDEO process
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_VIDEO_IOProcess (USBH_HandleTypeDef *phost)
{
  if (phost->device.is_connected == 1)
  {
    if(phost->gState == HOST_CLASS)
    {
      USBH_VIDEO_Process(phost);
    }
  }
  
  return USBH_OK;
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


/**
* @}
*/

/************************ (C) COPYRIGHT Hong, Sanghyun *****END OF FILE****/
