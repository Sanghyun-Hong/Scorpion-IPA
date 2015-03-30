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

static USBH_StatusTypeDef USBH_VIDEO_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_SOFProcess(USBH_HandleTypeDef *phost);

USBH_ClassTypeDef  VIDEO_Class = 
{
  "VIDEO",
  USB_VIDEO_CLASS,
  USBH_VIDEO_InterfaceInit,
  USBH_VIDEO_InterfaceDeInit,
  USBH_VIDEO_ClassRequest,
  USBH_VIDEO_Process,
  USBH_VIDEO_SOFProcess,
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
  USBH_StatusTypeDef status = USBH_FAIL ;
  USBH_StatusTypeDef out_status, in_status ;    
  VIDEO_HandleTypeDef *VIDEO_Handle;
  uint8_t  interface, index;
  uint16_t ep_size_out = 0;
  uint16_t ep_size_in = 0;  
  
  interface = USBH_FindInterface(phost, USB_VIDEO_CLASS, USB_SUBCLASS_VIDEO_CONTROL, 0x00);
  
  if(interface == 0xFF) // Invalid Interface
  {
    status = USBH_FAIL;
  }
  else 
  {   
    phost->pActiveClass->pData = (VIDEO_HandleTypeDef *)USBH_malloc (sizeof(VIDEO_HandleTypeDef));
    VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
    USBH_memset(VIDEO_Handle, 0, sizeof(VIDEO_HandleTypeDef));
    
    /* 1st Step: Find Video Interface */
    //out_status = USBH_Video_FindVideoStreamingOUT(phost); 
    //in_status = USBH_Video_FindVideoStreamingIN(phost);
    
    if( !(out_status == USBH_FAIL 
          && in_status == USBH_FAIL) ) 
    {
      
    }
    
    /* Initialization done */
    status = USBH_OK;
  }
  
  return status;
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

/*
static USBH_StatusTypeDef USBH_VIDEO_FindVideoStreamingIN(USBH_HandleTypeDef *phost)
{
  uint8_t interface, alt_settings;
  USBH_StatusTypeDef status = USBH_FAIL;
  VIDEO_HandleTypeDef *VIDEO_Handle;
  
  VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
  
  // Look for Video Streaming In interface
  alt_settings = 0;
  for (interface = 0; interface < USBH_MAX_NUM_INTERFACES; interface++) {
    if((phost->device.CfgDesc.Itf_Desc[interface].bInterfaceClass == USB_VIDEO_CLASS) &&
       (phost->device.CfgDesc.Itf_Desc[interface].bInterfaceSubClass == USB_SUBCLASS_VIDEO_STREAMING))
    {
      if((phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x80) &&
         (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize > 0))
      {
        
        alt_settings++;
      } // Endpoint and packet size
    }   // Interface class and subclass
  }
  
  if(alt_settings > 0) {
    status = USBH_OK;
  }
  
  return status;
}
*/

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
  * @brief  USBH_VIDEO_SOFProcess 
  *         The function is for SOF state
  * @param  phost: Host handle
  * @retval USBH Status
  */
// Just a Test Code ///////////////
#include "stm32f429i_discovery.h"
static USBH_StatusTypeDef USBH_VIDEO_SOFProcess(USBH_HandleTypeDef *phost)
{
  
  // Just a Test Code ///////////////
  static uint8_t toggle = 0;
  if(toggle)
  {
    BSP_LED_Off(LED4);
    toggle = 0;
  }
  else 
  {
    BSP_LED_On(LED4);
    toggle = 1;
  }
  // Just a Test Code ///////////////
  
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
