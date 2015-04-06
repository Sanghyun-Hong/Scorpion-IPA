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

// Basic functions
static USBH_StatusTypeDef USBH_VIDEO_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_SOFProcess(USBH_HandleTypeDef *phost);

// Internal functions
static USBH_StatusTypeDef USBH_VIDEO_FindVideoStreamingIN(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_FindVideoStreamingOUT(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_FindVideoControl(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_ParseCSDescriptors(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_BuildInputStreamPath(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_VIDEO_BuildOutputStreamPath(USBH_HandleTypeDef *phost);

// Helper functions
static int32_t FindLinkedInputUnit(USBH_HandleTypeDef *phost, uint8_t UnitID);
static int32_t FindLinkedOutputUnit(USBH_HandleTypeDef *phost, uint8_t UnitID);
static USBH_StatusTypeDef ParseCSDescriptors(VIDEO_ClassSpecificDescTypedef *ClassSpecificDesc, 
                                             uint8_t VCSubclass, 
                                             uint8_t *pdesc);

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
static USBH_StatusTypeDef USBH_VIDEO_InterfaceInit(USBH_HandleTypeDef *phost)
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
    in_status = USBH_VIDEO_FindVideoStreamingIN(phost);
    out_status = USBH_VIDEO_FindVideoStreamingOUT(phost);
    
    if( !(out_status == USBH_FAIL 
          && in_status == USBH_FAIL) ) 
    {
      /* 2nd Step: Select Video Streaming Interfaces with largest endponint size, default */
      for(index = 0; index < VIDEO_MAX_VIDEO_STD_INTERFACE; index++) {
        // Searching the largest input
        if(VIDEO_Handle->VSInputHandle[index].Valid == 1) {
          if(ep_size_in < VIDEO_Handle->VSInputHandle[index].EndpointSize) {
            ep_size_in = VIDEO_Handle->VSInputHandle[index].EndpointSize;
            VIDEO_Handle->InputStream.Endpoint          = VIDEO_Handle->VSInputHandle[index].Endpoint;
            VIDEO_Handle->InputStream.EndpointSize      = VIDEO_Handle->VSInputHandle[index].EndpointSize;
            VIDEO_Handle->InputStream.Interval          = VIDEO_Handle->VSInputHandle[index].Interval;
            VIDEO_Handle->InputStream.Interface         = VIDEO_Handle->VSInputHandle[index].Interface;
            VIDEO_Handle->InputStream.AltSettings       = VIDEO_Handle->VSInputHandle[index].AltSettings;
            VIDEO_Handle->InputStream.Supported = 1;
          }
        }
        
        // Searching the largest output
        if(VIDEO_Handle->VSOutputHandle[index].Valid == 1) {
          if(ep_size_out < VIDEO_Handle->VSOutputHandle[index].EndpointSize) {
            ep_size_out = VIDEO_Handle->VSOutputHandle[index].EndpointSize;
            VIDEO_Handle->OutputStream.Endpoint         = VIDEO_Handle->VSOutputHandle[index].Endpoint;
            VIDEO_Handle->OutputStream.EndpointSize     = VIDEO_Handle->VSOutputHandle[index].EndpointSize;
            VIDEO_Handle->OutputStream.Interval         = VIDEO_Handle->VSOutputHandle[index].Interval;
            VIDEO_Handle->OutputStream.Interface        = VIDEO_Handle->VSOutputHandle[index].Interface;
            VIDEO_Handle->OutputStream.AltSettings      = VIDEO_Handle->VSOutputHandle[index].AltSettings;
            VIDEO_Handle->OutputStream.Supported = 1;
          }
        }
      }
    }
    
    /* 3rd Step: Find control interface */
    if(USBH_VIDEO_FindVideoControl(phost) == USBH_OK) {
      VIDEO_Handle->Control.Supported = 1;
    }
    
    /* 4rd Step: Find and parse Video interfaces */
    USBH_VIDEO_ParseCSDescriptors(phost);
    
    /* 5th Step: Open the Video Streaming Pipes */
    if(VIDEO_Handle->InputStream.Supported == 1) {
      // Opens the input stream, which uses isochronous transfer
      USBH_VIDEO_BuildInputStreamPath(phost);
      
      VIDEO_Handle->InputStream.Pipe = USBH_AllocPipe(phost, VIDEO_Handle->InputStream.Endpoint);
      
      // Open the pipe
      USBH_OpenPipe(phost,
                    VIDEO_Handle->InputStream.Pipe,
                    VIDEO_Handle->InputStream.Endpoint,
                    phost->device.address,
                    phost->device.speed,
                    USB_EP_TYPE_ISOC,
                    VIDEO_Handle->InputStream.EndpointSize);
      
      USBH_LL_SetToggle(phost, VIDEO_Handle->InputStream.Pipe, 0);
    }
    
    if(VIDEO_Handle->OutputStream.Supported == 1) {
      // Opens the output stream, which uses isochronous transfer
      USBH_VIDEO_BuildOutputStreamPath(phost);
      
      VIDEO_Handle->OutputStream.Pipe = USBH_AllocPipe(phost, VIDEO_Handle->OutputStream.Endpoint);
      
      // Open the pipe
      USBH_OpenPipe(phost,
                    VIDEO_Handle->OutputStream.Pipe,
                    VIDEO_Handle->OutputStream.Endpoint,
                    phost->device.address,
                    phost->device.speed,
                    USB_EP_TYPE_ISOC,
                    VIDEO_Handle->OutputStream.EndpointSize);
      
      USBH_LL_SetToggle(phost, VIDEO_Handle->OutputStream.Pipe, 0);
    }
    
    if(VIDEO_Handle->Control.Supported == 1) {
      // Opens the control pipe, which uses interrupt
      VIDEO_Handle->Control.Pipe = USBH_AllocPipe(phost, VIDEO_Handle->Control.Endpoint);
      
      // Open the pipe
      USBH_OpenPipe(phost,
                    VIDEO_Handle->Control.Pipe,
                    VIDEO_Handle->Control.Endpoint,
                    phost->device.address,
                    phost->device.speed,
                    USB_EP_TYPE_ISOC,
                    VIDEO_Handle->Control.EndpointSize);
      
      USBH_LL_SetToggle(phost, VIDEO_Handle->Control.Pipe, 0);
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

/**
  * @brief  Find IN Video Streaming interfaces
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_FindVideoStreamingIN(USBH_HandleTypeDef *phost)
{
  uint8_t interface, alt_settings;
  USBH_StatusTypeDef status = USBH_FAIL;
  VIDEO_HandleTypeDef *VIDEO_Handle;
  
  VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
  
  // Look for Video Streaming In interfaces
  alt_settings = 0;
  for (interface = 0; interface < USBH_MAX_NUM_INTERFACES; interface++) {
    if((phost->device.CfgDesc.Itf_Desc[interface].bInterfaceClass == USB_VIDEO_CLASS) &&
       (phost->device.CfgDesc.Itf_Desc[interface].bInterfaceSubClass == USB_SUBCLASS_VIDEO_STREAMING))
    {
      if((phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x80) &&
         (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize > 0))
      {
        VIDEO_Handle->VSInputHandle[alt_settings].Endpoint      = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress;
        VIDEO_Handle->VSInputHandle[alt_settings].EndpointSize  = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
        VIDEO_Handle->VSInputHandle[alt_settings].Interface     = phost->device.CfgDesc.Itf_Desc[interface].bInterfaceNumber;
        VIDEO_Handle->VSInputHandle[alt_settings].AltSettings   = phost->device.CfgDesc.Itf_Desc[interface].bAlternateSetting;
        VIDEO_Handle->VSInputHandle[alt_settings].Interval      = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bInterval;
        VIDEO_Handle->VSInputHandle[alt_settings].Valid         = 1;
        alt_settings++;
      } // Endpoint and packet size
    }   // Interface class and subclass
  }
  
  if(alt_settings > 0) {
    status = USBH_OK;
  }
  
  return status;
}

/**
  * @brief  Find Out Video Streaming interfaces
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_FindVideoStreamingOUT(USBH_HandleTypeDef *phost)
{
  uint8_t interface, alt_settings;
  USBH_StatusTypeDef status = USBH_FAIL ;
  VIDEO_HandleTypeDef *VIDEO_Handle;

  VIDEO_Handle =  (VIDEO_HandleTypeDef *) phost->pActiveClass->pData; 

  /* Look For Video Streaming Out interfaces */
  alt_settings = 0;
  for(interface = 0;  interface < USBH_MAX_NUM_INTERFACES; interface ++) {
    if((phost->device.CfgDesc.Itf_Desc[interface].bInterfaceClass == USB_VIDEO_CLASS) &&
       (phost->device.CfgDesc.Itf_Desc[interface].bInterfaceSubClass == USB_SUBCLASS_VIDEO_STREAMING))
    {
      if(((phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x80) == 0x00) &&
         (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize > 0))        
      {
        VIDEO_Handle->VSOutputHandle[alt_settings].Endpoint      = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress;
        VIDEO_Handle->VSOutputHandle[alt_settings].EndpointSize = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
        VIDEO_Handle->VSOutputHandle[alt_settings].Interface     = phost->device.CfgDesc.Itf_Desc[interface].bInterfaceNumber;
        VIDEO_Handle->VSOutputHandle[alt_settings].AltSettings   = phost->device.CfgDesc.Itf_Desc[interface].bAlternateSetting;
        VIDEO_Handle->VSOutputHandle[alt_settings].Interval      = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bInterval;
        VIDEO_Handle->VSOutputHandle[alt_settings].Valid         = 1;
        alt_settings++;
      } // Endpoint and packet size
    }   // Interface class and subclass
  }
  
  if(alt_settings > 0)
  {
     status = USBH_OK;
  }
  
  return status;
}

/**
  * @brief  Find HID Control interfaces
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_FindVideoControl(USBH_HandleTypeDef *phost)
{
  uint8_t interface;
  USBH_StatusTypeDef status = USBH_FAIL ;
  VIDEO_HandleTypeDef *VIDEO_Handle;
  
  VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
  
  /* Look for Video Control interfaces */
  interface = USBH_FindInterface(phost, USB_VIDEO_CLASS, USB_SUBCLASS_VIDEO_CONTROL, 0xFF);
  if(interface != 0xFF) {
    for(interface = 0; interface < USBH_MAX_NUM_INTERFACES; interface++) {
      if((phost->device.CfgDesc.Itf_Desc[interface].bInterfaceClass == USB_VIDEO_CLASS) &&
         (phost->device.CfgDesc.Itf_Desc[interface].bInterfaceSubClass == USB_SUBCLASS_VIDEO_CONTROL))
      {
        if(((phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x80) == 0x00) &&
            (phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize > 0))        
        {
          VIDEO_Handle->Control.Endpoint        = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress;
          VIDEO_Handle->Control.EndpointSize    = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
          VIDEO_Handle->Control.Interval        = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bInterval;
          VIDEO_Handle->Control.Interface       = phost->device.CfgDesc.Itf_Desc[interface].bInterfaceNumber;
          VIDEO_Handle->Control.Supported       = 1;
          status = USBH_OK;
          break;
        } // Endpoint and packet size
      }   // Interface class and subclass      
    }   
  }
  
  return status;
}

/**
  * @brief  Parse VC and Interfaces Descriptors
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_ParseCSDescriptors(USBH_HandleTypeDef *phost)
{
  USBH_DescHeader_t             *pdesc ;
  uint16_t                      ptr;
  int8_t                        itf_index = 0;
  int8_t                        itf_number = 0; 
  int8_t                        alt_setting;   
  VIDEO_HandleTypeDef           *VIDEO_Handle;
  
  VIDEO_Handle  = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
  pdesc         = (USBH_DescHeader_t *) phost->device.CfgDesc_Raw;
  ptr           = USB_LEN_CFG_DESC;
  
  VIDEO_Handle->ClassSpecificDescs.VSNum = 0;
  VIDEO_Handle->ClassSpecificDescs.InputTerminalNum = 0;
  VIDEO_Handle->ClassSpecificDescs.OutputTerminalNum = 0;
  VIDEO_Handle->ClassSpecificDescs.SelectorUnitNum = 0;
  VIDEO_Handle->ClassSpecificDescs.ProcessingUnitNum = 0;
  VIDEO_Handle->ClassSpecificDescs.EncodingUnitNum = 0;
  VIDEO_Handle->ClassSpecificDescs.ExtensionUnitNum = 0;
  
  while(ptr < phost->device.CfgDesc.wTotalLength) {
    pdesc = USBH_GetNextDesc((uint8_t *)pdesc, &ptr);
    
    switch(pdesc->bDescriptorType) 
    {
    case USB_DESC_TYPE_INTERFACE:
      itf_number        = *((uint8_t *)pdesc + 2);
      alt_setting       = *((uint8_t *)pdesc + 3);
      itf_index         = USBH_FindInterfaceIndex(phost, itf_number, alt_setting);
      break;
      
    case USB_DESC_TYPE_CS_INTERFACE:
      if(itf_number <= phost->device.CfgDesc.bNumInterfaces) {
        ParseCSDescriptors(&VIDEO_Handle->ClassSpecificDescs,
                           phost->device.CfgDesc.Itf_Desc[itf_index].bInterfaceSubClass,
                           (uint8_t *)pdesc);
      }
      break;
      
    default:
      break;
    }
  }
  
  return USBH_OK;
}

/**
  * @brief  Link a Input Unit to next associated one
  * @param  phost: Host handle
  * @param  UnitID: Unit identifer
  * @retval UnitID, Index and Type of the associated Unit 
  */
static int32_t FindLinkedInputUnit(USBH_HandleTypeDef *phost, uint8_t UnitID)
{
  uint8_t index;  
  VIDEO_HandleTypeDef *VIDEO_Handle;
  
  VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;    
  
  // Find Output Terminal Unit
  for(index = 0; index < VIDEO_Handle->ClassSpecificDescs.OutputTerminalNum; index++) {
    if(VIDEO_Handle->ClassSpecificDescs.VCDescriptor.OutputTerminalDesc[index]->bSourceID == UnitID) {
      UnitID = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.OutputTerminalDesc[index]->bTerminalID;
      return ((UnitID << 16) | (USB_VC_OUTPUT_TERMINAL << 8) | index);
    }
  }
  
  // Find Selector Unit
  for(index = 0; index < VIDEO_Handle->ClassSpecificDescs.SelectorUnitNum; index++) {
    if(VIDEO_Handle->ClassSpecificDescs.VCDescriptor.SelectorUnitDesc[index]->bUnitID == UnitID) {
      UnitID = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.SelectorUnitDesc[index]->bUnitID;
      return ((UnitID << 16) | (USB_VC_SELECTOR_UNIT << 8) | index);
    }
  }
  
  // Find Processing Unit
  for(index = 0; index < VIDEO_Handle->ClassSpecificDescs.ProcessingUnitNum; index++) {
    if(VIDEO_Handle->ClassSpecificDescs.VCDescriptor.ProcessingUnitDesc[index]->bSourceID == UnitID) {
      UnitID = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.ProcessingUnitDesc[index]->bUnitID;
      return ((UnitID << 16) | (USB_VC_PROCESSING_UNIT << 8) | index);
    }
  }
  
  // Find Extension Unit
  for(index = 0; index < VIDEO_Handle->ClassSpecificDescs.ExtensionUnitNum; index++) {
    if(VIDEO_Handle->ClassSpecificDescs.VCDescriptor.ExtensionUnitDesc[index]->bUnitID == UnitID) {
      UnitID = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.ExtensionUnitDesc[index]->bUnitID;
      return ((UnitID << 16) | (USB_VC_EXTENSION_UNIT << 8) | index);
    }
  }
  
  // Find Encoding Unit
  for(index = 0; index < VIDEO_Handle->ClassSpecificDescs.EncodingUnitNum; index++) {
    if(VIDEO_Handle->ClassSpecificDescs.VCDescriptor.EncodingUnitDesc[index]->bUnitID == UnitID) {
      UnitID = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.EncodingUnitDesc[index]->bUnitID;
      return ((UnitID << 16) | (USB_VC_ENCODING_UNIT << 8) | index);
    }
  }
  
  // No linked unit found
  return -1;
}

/**
  * @brief  Link a Output Unit to next associated one
  * @param  phost: Host handle
  * @param  UnitID: Unit identifer
  * @retval UnitID, Index and Type of the associated Unit 
  */
static int32_t FindLinkedOutputUnit(USBH_HandleTypeDef *phost, uint8_t UnitID)
{
  uint8_t index;  
  VIDEO_HandleTypeDef *VIDEO_Handle;
  
  VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData; 
  
  // FIXME - In here
  
  // No linked unit found
  return -1;
}

/**
  * @brief  Parse VC interfaces
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef ParseCSDescriptors(VIDEO_ClassSpecificDescTypedef *ClassSpecificDesc, 
                                             uint8_t VCSubclass, 
                                             uint8_t *pdesc)
{
  // If the subclass is Video Control
  if(VCSubclass == USB_SUBCLASS_VIDEO_CONTROL) {
    switch(pdesc[2]) 
    {
    case USB_VC_HEADER:
      ClassSpecificDesc->VCDescriptor.HeaderDesc = (VIDEO_VCClassSpecificDescTypeDef *)pdesc;
      break;
    case USB_VC_INPUT_TERMINAL:
      ClassSpecificDesc->VCDescriptor.InputTerminalDesc[ClassSpecificDesc->InputTerminalNum++] = (VIDEO_VCInputTerminalDescTypeDef *)pdesc;
      break;
    case USB_VC_OUTPUT_TERMINAL:
      ClassSpecificDesc->VCDescriptor.OutputTerminalDesc[ClassSpecificDesc->OutputTerminalNum++] = (VIDEO_VCOutputTerminalDescTypeDef *)pdesc;
      break;
    case USB_VC_SELECTOR_UNIT:
      ClassSpecificDesc->VCDescriptor.SelectorUnitDesc[ClassSpecificDesc->SelectorUnitNum++] = (VIDEO_VCSelectorUnitDescTypeDef *)pdesc;
      break;
    case USB_VC_PROCESSING_UNIT:
      ClassSpecificDesc->VCDescriptor.ProcessingUnitDesc[ClassSpecificDesc->ProcessingUnitNum++] = (VIDEO_VCProcessingUnitDescTypeDef *)pdesc;
      break;
    case USB_VC_EXTENSION_UNIT:
      ClassSpecificDesc->VCDescriptor.ExtensionUnitDesc[ClassSpecificDesc->ExtensionUnitNum++] = (VIDEO_VCExtensionUnitDescTypeDef *)pdesc;
      break;
    case USB_VC_ENCODING_UNIT:
      ClassSpecificDesc->VCDescriptor.EncodingUnitDesc[ClassSpecificDesc->EncodingUnitNum++] = (VIDEO_VCEncodingUnitDescTypeDef *)pdesc;
      break;
      
    case USB_VC_DESC_UNDEFINED: // Error or Unknown header
    default:
      break;
    }   // End switch(...
  }
  // If the subclass is Video Streaming
  else if(VCSubclass == USB_SUBCLASS_VIDEO_STREAMING) {
    switch(pdesc[2])
    {
    case USB_VS_INPUT_HEADER:
      ClassSpecificDesc->VSDescriptor[ClassSpecificDesc->VSNum++].CSInputTypeDesc = (VIDEO_VSInterfaceCSInputDescTypeDef *)pdesc;
      break;
    case USB_VS_OUTPUT_HEADER:
      ClassSpecificDesc->VSDescriptor[ClassSpecificDesc->VSNum++].CSOutputTypeDesc = (VIDEO_VSInterfaceCSOutputDescTypeDef *)pdesc;
      break;
    // This module does not support Payload and Video Frame Descriptors  
    case USB_VS_STILL_IMAGE_FRAME:
      ClassSpecificDesc->VSDescriptor[ClassSpecificDesc->VSNum++].StillImageFrameDesc = (VIDEO_VSInterfaceStillImageFrameDescTypeDef *)pdesc;
      break;
    case USB_VS_COLORFORMAT:
      ClassSpecificDesc->VSDescriptor[ClassSpecificDesc->VSNum++].ColorMatchingDesc = (VIDEO_VSInterfaceColorMatchingDescTypeDef *)pdesc;
      break;
      
    default:                    // Error or Unknown header
      break;
    }   // End switch(...
  }
  
  return USBH_OK;
}

/**
  * @brief  Build full path for InputStream device
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_BuildInputStreamPath(USBH_HandleTypeDef *phost)
{
  uint8_t UnitID = 0, Type, Index;
  uint32_t value;
  uint8_t terminalIndex;  
  VIDEO_HandleTypeDef *VIDEO_Handle;
  
  VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
  
  // Find InputStream interface
  for(terminalIndex = 0; terminalIndex < VIDEO_Handle->ClassSpecificDescs.InputTerminalNum; terminalIndex++) {
    if(LE16(VIDEO_Handle->ClassSpecificDescs.VCDescriptor.InputTerminalDesc[terminalIndex]->wTerminalType) == 0x201) {    
      UnitID = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.InputTerminalDesc[terminalIndex]->bTerminalID;
      //VIDEO_Handle->InputStream.AssociatedChannels = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.InputTerminalDesc[terminalIndex]->bNrChannels;
      break;
    }
  }
  
  do {
    value       = FindLinkedInputUnit(phost, UnitID);
    Index       = value & 0xFF;
    Type        = (value >> 8) & 0xFF;
    UnitID      = (value >> 16) & 0xFF;
    
    switch (Type)
    {
    case USB_VC_OUTPUT_TERMINAL:
      VIDEO_Handle->InputStream.AssociatedTerminal = Index;
      break;
    case USB_VC_SELECTOR_UNIT:
      VIDEO_Handle->InputStream.AssociatedSelector = Index;
      break;
    case USB_VC_PROCESSING_UNIT:
      VIDEO_Handle->InputStream.AssociatedProcessing = Index;
      break;
    case USB_VC_EXTENSION_UNIT:
      VIDEO_Handle->InputStream.AssociatedExtension = Index;
      break;
    case USB_VC_ENCODING_UNIT:
      VIDEO_Handle->InputStream.AssociatedEncoding = Index;
      break;
    }
  }
  while ((Type != USB_VC_OUTPUT_TERMINAL) && (value > 0));
  
  return USBH_OK;
}

/**
  * @brief  Build full path for OutputStream device
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_BuildOutputStreamPath(USBH_HandleTypeDef *phost)
{
  uint8_t UnitID = 0, Type, Index;
  uint32_t value;
  uint8_t terminalIndex;  
  VIDEO_HandleTypeDef *VIDEO_Handle;
  
  VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
  
  // Find the Output Stream interface
  for(terminalIndex = 0; terminalIndex < VIDEO_Handle->ClassSpecificDescs.OutputTerminalNum; terminalIndex++) {
    if(LE16(VIDEO_Handle->ClassSpecificDescs.VCDescriptor.OutputTerminalDesc[terminalIndex]->wTerminalType) == 0x0101) {    
      UnitID = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.OutputTerminalDesc[terminalIndex]->bTerminalID;
      //VIDEO_Handle->OutputStream.AssociatedChannels = VIDEO_Handle->ClassSpecificDescs.VCDescriptor.OutputTerminalDesc[terminalIndex]->bNrChannels;
      break;
    }
  }
  
  // FIXME - in below
  do {
    value       = FindLinkedOutputUnit(phost, UnitID);
    Index       = value & 0xFF;
    Type        = (value >> 8) & 0xFF;
    UnitID      = (value >> 16) & 0xFF;
    
    switch (Type)
    {
    case USB_VS_INPUT_HEADER:
      // FIXME
      break;
    case USB_VS_OUTPUT_HEADER:
      // FIXME
      break;
    // This module does not support Payload and Video Frame Descriptors  
    case USB_VS_STILL_IMAGE_FRAME:
      // FIXME
      break;
    case USB_VS_COLORFORMAT:
      // FIXME
      break;
    }
  }
  while ((Type != USB_VC_OUTPUT_TERMINAL) && (value > 0));
  
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
#include "stm32f429i_discovery.h" // Just a Test Code ///////////////
static USBH_StatusTypeDef USBH_VIDEO_Process (USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status = USBH_BUSY;
  VIDEO_HandleTypeDef *VIDEO_Handle = (VIDEO_HandleTypeDef *) phost->pActiveClass->pData;
  
  static uint8_t toggle = 0;    // Just a Test Code ///////////////
  
  if(VIDEO_Handle->InputStream.Supported == 1) {
    if(toggle == 1) {
      BSP_LED_Off(LED4);        // Just a Test Code ///////////////
    }
    else {
      BSP_LED_On(LED4);         // Just a Test Code ///////////////
    }
  }
  
  if(VIDEO_Handle->OutputStream.Supported == 1) {
    if(toggle == 1) {
      BSP_LED_Off(LED4);        // Just a Test Code ///////////////
      toggle = 0;
    }
    else {
      BSP_LED_On(LED4);         // Just a Test Code ///////////////
      toggle = 1;
    }
  }
  
  return USBH_OK;
}

/**
  * @brief  USBH_VIDEO_SOFProcess 
  *         The function is for SOF state
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_VIDEO_SOFProcess(USBH_HandleTypeDef *phost)
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
