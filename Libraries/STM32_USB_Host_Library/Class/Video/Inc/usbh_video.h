/**
  ******************************************************************************
  * @file    usbh_video.h
  * @author  Hong, Sanghyun
  * @version V0.1.0
  * @date    17-March-2015
  * @brief   This file contains all the prototypes for the usbh_video.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_VIDEO_H
#define __USBH_VIDEO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"


/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_VIDEO_CLASS
* @{
*/

/** @defgroup USBH_VIDEO_CLASS
* @brief This file is the Header file for usbh_template.c
* @{
*/ 


/**
  * @}
  */ 

/** @defgroup USBH_VIDEO_CLASS_Exported_Types
* @{
*/

/* 3.6. Interface Association Descriptor */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bFirstInterface;
  uint8_t bInterfaceCount;
  uint8_t bFunctionClass;
  uint8_t bFunctionSubClass;
  uint8_t bFunctionProtocol;
  uint8_t iFunction;
} VIDEO_IADDescTypeDef;

/* 3.7. VC Interface Descriptor - 1. Standard */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternativeString;
  uint8_t bNumEndPoints;
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t iInterface;
} VIDEO_VCStandardDescTypeDef;

/* 3.7. VC Interface Descriptor - 2. Class-Specific */
#define VC_INTERFACE_MAX_NR     5
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bcdUVC[2];
  uint8_t wTotalLength[2];
  uint8_t dwClockFrequency[4];
  uint8_t bInCollection;
  uint8_t baInterfaceNr[VC_INTERFACE_MAX_NR];
} VIDEO_VCClassSpecificDescTypeDef;

/* 3.7. VC Interface Descriptor - 2.1. Input Terminal */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescritproSubType;
  uint8_t bTerminalID;
  uint8_t wTerminalType[2];
  uint8_t bAssocTerminal;
  uint8_t iTerminal;
  // Depends on the implementation, additional fields can be stored in here
} VIDEO_VCInputTerminalDescTypeDef;

/* 3.7. VC Interface Descriptor - 2.2. Output Terminal */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bTerminalID;
  uint8_t wTerminalType[2];
  uint8_t bAssocTerminal;
  uint8_t bSourceID;
  uint8_t iTerminal;
  // Depends on the implementation, additional fields can be stored in here
} VIDEO_VCOutputTerminalDescTypeDef;

/* 3.7. VC Interface Descriptor - 2.3. Camera Terminal */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bTerminalID;
  uint8_t wTerminalType[2];
  uint8_t bAssocTerminal;
  uint8_t iTerminal;
  uint8_t wObjectiveFocalLengthMin[2];
  uint8_t wObjectiveFocalLengthMax[2];
  uint8_t wObjectiveFocalLength;
  uint8_t bControlSize;
  uint8_t bmControls[3];
} VIDEO_VCCameraTerminalDescTypeDef;

/* 3.7. VC Interface Descriptor - 2.4. Selector Unit */
#define VC_INTERFACE_SELECTOR_MAX_NR    5
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bUnitID;
  uint8_t bNrInPins;
  uint8_t baSourceID[VC_INTERFACE_SELECTOR_MAX_NR];
  uint8_t iSelector;
} VIDEO_VCSelectorUnitDescTypeDef;

/* 3.7. VC Interface Descriptor - 2.5. Processing Unit */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bUnitID;
  uint8_t bSourceID;
  uint8_t wMaxMultiplier[2];
  uint8_t bControlSize;
  uint8_t bmControls[3];
  uint8_t iProcessing;
  uint8_t bmVideoStandards;
} VIDEO_VCProcessingUnitDescTypeDef;

/* 3.7. VC Interface Descriptor - 2.6. Encoding Unit */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bUnitID;
  uint8_t bSourceID;
  uint8_t iEncoding;
  uint8_t bControlSize;
  uint8_t bmControls[3];
  uint8_t bmControlsRuntime[3];
} VIDEO_VCEncodingUnitDescTypeDef;

/* 3.7. VC Interface Descriptor - 2.7. Extension Unit */
#define VC_INTERFACE_EXTENSION_MAX_NR   5
#define VC_INTERFACE_EXTENSION_MAX_NUM  5
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bUnitID;
  uint8_t guidExtensionCode;
  uint8_t bNumControls;
  uint8_t bNrInPins;
  uint8_t baSourceID[VC_INTERFACE_EXTENSION_MAX_NR];
  uint8_t bControlSize;
  uint8_t bmControls[VC_INTERFACE_EXTENSION_MAX_NUM];
  uint8_t iExtension;
} VIDEO_VCExtensionUnitDescTypeDef;

/* 3.8. VC Control Endpoint Descriptor - 1.1. Standard (NONE) */
/* 3.8. VC Control Endpoint Descriptor - 1.2. Class Specific (NONE) */
/* 3.8. VC Interrupt Endpoint Descriptor - 2.1. Standard */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bEndpointAddress;
  uint8_t bmAttributes;
  uint8_t wMaxPacketSize[2];
  uint8_t bInterval;
} VIDEO_VCInterrpptEndpointSTDTypeDef;

/* 3.8. VC Interrupt Endpoint Descriptor - 2.2. Class-specific */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t wMaxTransferSize[2];
} VIDEO_VCInterruptEndpointCSDescTypeDef;

/* 3.9. Video Streaming Interface Descriptor - 1.1. Standard */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bAlternateSetting;
  uint8_t bNumEndpoints;
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t iInterface;
} VIDEO_VSInterfaceSTDDescTypeDef;


/* 3.9. Video Streaming Interface Descritor - 2.1. Class-Specific Input Header */
#define VS_INTERFACE_CS_INPUT_MAX_CTRL_NUM      5
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bNumFormats;
  uint8_t wTotalLength[2];
  uint8_t bEndpointAddress;
  uint8_t bmInfo;
  uint8_t bTerminalLink;
  uint8_t bStillCaptureMethod;
  uint8_t bTriggerSupport;
  uint8_t bTriggerUsage;
  uint8_t bControlSize;
  uint8_t bmaControls[VS_INTERFACE_CS_INPUT_MAX_CTRL_NUM];
} VIDEO_VSInterfaceCSInputDescTypeDef;

/* 3.9. Video Streaming Interface Descriptor - 2.2. Class-Specific Output Header */
#define VS_INTERFACE_CS_OUTPUT_MAX_CTRL_NUM     5
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bNumFormats;
  uint8_t wTotalLength[2];
  uint8_t bEndpointAddress;
  uint8_t bTerminalLink;
  uint8_t bControlSize;
  uint8_t bmaControls[VS_INTERFACE_CS_OUTPUT_MAX_CTRL_NUM];
} VIDEO_VSInterfaceCSOutputDescTypeDef;

/* 3.9. Video Streaming Interface Descriptor - 2.3. Payload Descritpr (Differ from companies to companies */
/* 3.9. Video Streaming Interface Descriptor - 2.4. Video Frame Descriptor (Differ from companies to companies */
/* 3.9. Video Streaming Interface Descriptor - 2.5. Still Image Frame Descritpor */
#define VS_INTERFACE_STILL_IMAGE_MAX_WH_NUM     10
#define VS_INTERFACE_STILL_IMAGE_MAX_COMP_NUM   10
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bEndpointAddress;
  uint8_t bNumImageSizePatterns;
  uint8_t wWidthHeight[VS_INTERFACE_STILL_IMAGE_MAX_WH_NUM][2][2];
  uint8_t bNumCompressionPatterns;
  uint8_t bCompression[VS_INTERFACE_STILL_IMAGE_MAX_COMP_NUM];
} VIDEO_VSInterfaceStillImageFrameDescTypeDef;

/* 3.9. Video Streaming Interface Descriptor - 2.6. Color Matching Descriptor */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bColorPrimaries;
  uint8_t bTransferCharacteristics;
  uint8_t bMatrixCoefficients;
} VIDEO_VSInterfaceColorMatchingDescTypeDef;

/* 3.10. Video Streaming Endpoint Descriptor - 1.1. Isochronous Video Data */
/* 3.10. Video Streaming Endpoint Descriptor - 1.2. Bulk Video Data */
/* 3.10. Video Streaming Endpoint Descriptor - 2.1. Bulk Still Image Video Data */
typedef struct {
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bDescriptorSubType;
  uint8_t bEndpointAddress;
  uint8_t bmAttributes;
  uint8_t wMaxPacketSize[2];
  uint8_t bInterval;
} VIDEO_VSEndpointDescTypeDef;  // 1.1, 1.2, and 2.1 shares the same format

/* 3.11. String Descriptor (The same as the Standard VC Control Descriptor) */

/* CUSTOM: Number of Class-Specific VS Standard Interface */
#define VIDEO_MAX_VIDEO_STD_INTERFACE           0x05    // The number is chosed by heuristics

/* CUSTOM: Numver of Class-Specific VS(Video Streaming) Interface Descriptor */
#define VIDEO_MAX_STREAMING_INTERFACE           0x05    // The number is chosed by heuristics

/* CUSTOM: Number of Class-Specific VC(Video Control) Interface Descriptor */
#define VIDEO_MAX_NUM_IN_TERMINAL               0x04    // The numbers were chosen by heuristics
#define VIDEO_MAX_NUM_OUT_TERMINAL              0x04
#define VIDEO_MAX_NUM_CAMERA_UNIT               0x04
#define VIDEO_MAX_NUM_SELECTOR_UNIT             0x04
#define VIDEO_MAX_NUM_PROCESSING_UNIT           0x04
#define VIDEO_MAX_NUM_ENCODING_UNIT             0x04
#define VIDEO_MAX_NUM_EXTENSION_UNIT            0x04

/* CUSTOM: Class-Specific VC(Video Control) Interface Descriptor */
typedef struct {
  VIDEO_VCClassSpecificDescTypeDef      *HeaderDesc;
  VIDEO_VCInputTerminalDescTypeDef      *InputTerminalDesc [VIDEO_MAX_NUM_IN_TERMINAL];
  VIDEO_VCOutputTerminalDescTypeDef     *OutputTerminalDesc[VIDEO_MAX_NUM_OUT_TERMINAL];
  VIDEO_VCSelectorUnitDescTypeDef       *SelectorUnitDesc  [VIDEO_MAX_NUM_SELECTOR_UNIT];
  VIDEO_VCProcessingUnitDescTypeDef     *ProcessingUnitDesc[VIDEO_MAX_NUM_PROCESSING_UNIT];
  VIDEO_VCExtensionUnitDescTypeDef      *ExtensionUnitDesc [VIDEO_MAX_NUM_EXTENSION_UNIT];
  VIDEO_VCEncodingUnitDescTypeDef       *EncodingUnitDesc  [VIDEO_MAX_NUM_ENCODING_UNIT];
} VIDEO_VCDescTypeDef;

/* CUSTOM: Class-Specific VS(Video Streaming) Interface Descriptor */
typedef struct {
  VIDEO_VSInterfaceCSInputDescTypeDef           *CSInputTypeDesc;
  VIDEO_VSInterfaceCSOutputDescTypeDef          *CSOutputTypeDesc;
  // This module does not support Payload and Video Frame Descriptors
  VIDEO_VSInterfaceStillImageFrameDescTypeDef   *StillImageFrameDesc;
  VIDEO_VSInterfaceColorMatchingDescTypeDef     *ColorMatchingDesc;
} VIDEO_VSDescTypeDef;

/* CUSTOM: Class-Specific VC : Global descriptor*/
typedef struct {
  
  VIDEO_VCDescTypeDef   VCDescriptor;
  VIDEO_VSDescTypeDef   VSDescriptor[VIDEO_MAX_STREAMING_INTERFACE];
  // The number of each Video Control eescriptors
  uint16_t              VSNum; 
  uint16_t              InputTerminalNum;
  uint16_t              OutputTerminalNum;
  uint16_t              SelectorUnitNum;
  uint16_t              ProcessingUnitNum;
  uint16_t              EncodingUnitNum;
  uint16_t              ExtensionUnitNum;
} VIDEO_ClassSpecificDescTypedef;

// FIXME - Under three lines
#define HEADPHONE_SUPPORTED                     0x01
#define MICROPHONE_SUPPORTED                    0x02
#define HEADSET_SUPPORTED                       0x03
   
/* CUSTOM: Input/Output Streaming Handler */
typedef struct {
  uint8_t               Endpoint;
  uint16_t              EndpointSize;
  uint8_t               AltSettings;
  uint8_t               Interface;
  uint8_t               Valid;
  uint8_t               Interval;
} VIDEO_VSInOutHandleTypeDef;

/* CUSTOM: Input/Output Streaming Properties */
typedef struct {
  // Endpoint information
  uint8_t               Endpoint;
  uint16_t              EndpointSize; 
  uint8_t               Interface; 
  uint8_t               AltSettings;
  uint8_t               Interval;
  uint8_t               Pipe;
  // Supported flag
  uint8_t               Supported;
  // Linked features: Input/Output
  uint8_t               AssociatedChannels;
  uint8_t               AssociatedTerminal;
  uint8_t               AssociatedSelector;
  uint8_t               AssociatedProcessing;
  uint8_t               AssociatedExtension;
  uint8_t               AssociatedEncoding;
  // Linked features: Output
  uint8_t               AssociatedVideoStream;
  
  /*
  uint32_t             timer ; 
  
  uint8_t              asociated_as; 
  uint8_t              asociated_mixer; 
  uint8_t              asociated_selector; 
  uint8_t              asociated_feature; 
  uint8_t              asociated_terminal;
  uint8_t              asociated_channels;
  
  uint32_t             frequency; 
  uint8_t              *buf;
  uint8_t              *cbuf; 
  uint32_t             partial_ptr; 

  uint32_t             global_ptr;  
  uint16_t             frame_length;  
  uint32_t             total_length; 
  
  AUDIO_ControlAttributeTypeDef attribute;  
  */
} VIDEO_InterfaceStreamTypeDef;

/* CUSTOM: Control Properties */
typedef struct {
  uint8_t       Endpoint;
  uint8_t       EndpointSize;
  uint8_t       Interface;
  uint8_t       Interval; 
  uint8_t       Pipe;
  
  uint8_t       Supported;
} VIDEO_InterfaceControlTypeDef;

// FIXME - Under the struct
typedef struct _VIDEO_Process
{
  /*
  VIDEO_ReqStateTypeDef                 req_state;
  VIDEO_CSReqStateTypeDef               cs_req_state;  
  VIDEO_PlayStateTypeDef                play_state;  
  VIDEO_ControlStateTypeDef             control_state; 
  VIDEO_ProcessingTypeDef               processing_state;
  */
  
  // Video Stream Input/Output Handler
  VIDEO_VSInOutHandleTypeDef            VSInputHandle[VIDEO_MAX_VIDEO_STD_INTERFACE];
  VIDEO_VSInOutHandleTypeDef            VSOutputHandle[VIDEO_MAX_VIDEO_STD_INTERFACE];
  
  // Class Specific Descriptor
  VIDEO_ClassSpecificDescTypedef        ClassSpecificDescs;
  
  // Interface Properties
  VIDEO_InterfaceStreamTypeDef          InputStream;
  VIDEO_InterfaceStreamTypeDef          OutputStream; 
  VIDEO_InterfaceControlTypeDef         Control;
  /*
  uint16_t                              mem[8];
  uint8_t                               temp_feature;
  uint8_t                               temp_channels;
  */
} VIDEO_HandleTypeDef;

/**
* @}
*/ 

/** @defgroup USBH_VIDEO_CLASS_Exported_Defines
* @{
*/ 

/* A.1. Video Interface Subclass Code */
#define USB_VIDEO_CLASS                 0x0E
   
/* A.2. Video Interface Subclass Code */
#define USB_SUBCLASS_UNDEFINED          0x00
#define USB_SUBCLASS_VIDEO_CONTROL      0x01
#define USB_SUBCLASS_VIDEO_STREAMING    0x02
#define USB_SUBCLASS_VIDEO_IC           0x03

/* A.3. Video Interface Protocol Code */
#define USB_PC_PROTOCOL_UNDEFINED       0x00
#define USB_PC_PROTOCOL_15              0x01

/* A.4. Video Class-Specific Descriptor Types */
#define USB_CS_DESC_UNDEFINED           0x20
#define USB_CS_DESC_DEVICE              0x21
#define USB_CS_DESC_CONFIGURATION       0x22
#define USB_CS_DESC_STRING              0x23
#define USB_CS_DESC_INTERFACE           0x24
#define USB_CS_DESC_ENDPOINT            0x25

/* A.5. Video Class-Specific VC Interface Descriptor Subtypes */
#define USB_VC_DESC_UNDEFINED           0x00
#define USB_VC_HEADER                   0x01
#define USB_VC_INPUT_TERMINAL           0x02
#define USB_VC_OUTPUT_TERMINAL          0x03
#define USB_VC_SELECTOR_UNIT            0x04
#define USB_VC_PROCESSING_UNIT          0x05
#define USB_VC_EXTENSION_UNIT           0x06
#define USB_VC_ENCODING_UNIT            0x07

/* A.6. Video Class-Specific VS Interface Descriptor Subtypes */
#define USB_VS_UNDEFINED                0x00
#define USB_VS_INPUT_HEADER             0x01
#define USB_VS_OUTPUT_HEADER            0x02
#define USB_VS_STILL_IMAGE_FRAME        0x03
#define USB_VS_FORMAT_UNCOMPRESSED      0x04
#define USB_VS_FRAME_UNCOMPRESSED       0x05
#define USB_VS_FORMAT_MJPEG             0x06
#define USB_VS_FRAME_MJPEG              0x07
// 0x08, 0x09: reserved
#define USB_VS_FORMAT_MPEG2TS           0x0A
// 0x0B: reserved
#define USB_VS_FORMAT_DV                0x0C
#define USB_VS_COLORFORMAT              0x0D
// 0x0E, 0x0F: reserved
#define USB_VS_FORMAT_FRAME_BASED       0x10
#define USB_VS_FRAME_FRAME_BASED        0x11
#define USB_VS_FORMAT_STREAM_BASED      0x12
#define USB_VS_FORMAT_H264              0x13
#define USB_VS_FRAME_H264               0x14
#define USB_VS_FORMAT_H264_SIMULCAST    0x15
#define USB_VS_FORMAT_VP8               0x16
#define USB_VS_FRAME_VP8                0x17
#define USB_VS_FORMAT_VP8_SIMULCAST     0x18
   
/* A.7. Video Class-Specific Endpoint Descriptor Subtypes */
#define USB_EP_UNDEFINED                0x00
#define USB_EP_GENERAL                  0x01
#define USB_EP_ENDPOINT                 0x02
#define USB_EP_INTERRUPT                0x03

/* A.8. Video Class-Specific Request Codes */
#define USB_REQ_RC_UNDEFINED            0x00
#define USB_REQ_SET_CUR                 0x01
#define USB_REQ_SET_CUR_ALL             0x11
#define USB_REQ_GET_CUR                 0x81
#define USB_REQ_GET_MIN                 0x82
#define USB_REQ_GET_MAX                 0x83
#define USB_REQ_GET_RES                 0x84
#define USB_REQ_GET_LEN                 0x85
#define USB_REQ_GET_INFO                0x86
#define USB_REQ_GET_DEF                 0x87
#define USB_REQ_GET_CUR_ALL             0x91
#define USB_REQ_GET_MIN_ALL             0x92
#define USB_REQ_GET_MAX_ALL             0x93
#define USB_REQ_GET_RES_ALL             0x94
#define USB_REQ_GET_DEF_ALL             0x97

/* A.9. Control Selector Codes - 1. VideoControl Interface Control Selectors */
#define USB_VC_CONTROL_ UNDEFINED               0x00
#define USB_VC_VIDEO_POWER_MODE_CONTROL         0x01
#define USB_VC_REQUEST_ERROR_CODE_CONTROL       0x02
// 0x03: reserved

/* A.9. Control Selector Codes - 2. Terminal Control Selectors */
#define USB_TE_CONTROL_UNDEFINED                0x00

/* A.9. Control Selector Codes - 3. Selector Unit Control Selectors */
#define USB_SU_CONTROL_UNDEFINED                0x00
#define USB_SU_INPUT_SELECT_CONTROL             0x01

/* A.9. Control Selector Codes - 4. Camera Terminal Control Selectors */
#define USB_CT_CONTROL_UNDEFINED                0x00
#define USB_CT_SCANNING_MODE_CONTROL            0x01
#define USB_CT_AE_MODE_CONTROL                  0x02
#define USB_CT_AE_PRIORITY_CONTROL              0x03
#define USB_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL   0x04
#define USB_CT_EXPOSURE_TIME_RELATIVE_CONTROL   0x05
#define USB_CT_FOCUS_ABSOLUTE_CONTROL           0x06
#define USB_CT_FOCUS_RELATIVE_CONTROL           0x07
#define USB_CT_FOCUS_AUTO_CONTROL               0x08
#define USB_CT_IRIS_ABSOLUTE_CONTROL            0x09
#define USB_CT_IRIS_RELATIVE_CONTROL            0x0A
#define USB_CT_ZOOM_ABSOLUTE_CONTROL            0x0B
#define USB_CT_ZOOM_RELATIVE_CONTROL            0x0C
#define USB_CT_PANTILT_ABSOLUTE_CONTROL         0x0D
#define USB_CT_PANTILT_RELATIVE_CONTROL         0x0E
#define USB_CT_ROLL_ABSOLUTE_CONTROL            0x0F
#define USB_CT_ROLL_RELATIVE_CONTROL            0x10
#define USB_CT_PRIVACY_CONTROL                  0x11
#define USB_CT_FOCUS_SIMPLE_CONTROL             0x12
#define USB_CT_WINDOW_CONTROL                   0x13
#define USB_CT_REGION_OF_INTEREST_CONTROL       0x14

/* A.9. Control Selector Codes - 5. Processing Unit Control Selectors */
#define USB_PU_CONTROL_UNDEFINED                0x00
#define USB_PU_BACKLIGHT_COMPENSATION_CONTROL   0x01
#define USB_PU_BRIGHTNESS_CONTROL               0x02
#define USB_PU_CONTRAST_CONTROL                 0x03
#define USB_PU_GAIN_CONTROL                     0x04
#define USB_PU_POWER_LINE_FREQUENCY_CONTROL     0x05
#define USB_PU_HUE_CONTROL                      0x06
#define USB_PU_SATURATION_CONTROL               0x07
#define USB_PU_SHARPNESS_CONTROL                0x08
#define USB_PU_GAMMA_CONTROL                    0x09
#define USB_PU_WHITE_BALANCE_TEMPERATURE_CONTROL        0x0A
#define USB_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL   0x0B
#define USB_PU_WHITE_BALANCE_COMPONENT_CONTROL          0x0C
#define USB_PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL     0x0D
#define USB_PU_DIGITAL_MULTIPLIER_CONTROL               0x0E
#define USB_PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL         0x0F
#define USB_PU_HUE_AUTO_CONTROL                 0x10
#define USB_PU_ANALOG_VIDEO_STANDARD_CONTROL    0x11
#define USB_PU_ANALOG_LOCK_STATUS_CONTROL       0x12
#define USB_PU_CONTRAST_AUTO_CONTROL            0x13

/* A.9. Control Selector Codes - 6. Encoding Unit Control Selectors */
#define USB_EU_CONTROL_UNDEFINED                0x00
#define USB_EU_SELECT_LAYER_CONTROL             0x01
#define USB_EU_PROFILE_TOOLSET_CONTROL          0x02
#define USB_EU_VIDEO_RESOLUTION_CONTROL         0x03
#define USB_EU_MIN_FRAME_INTERVAL_CONTROL       0x04
#define USB_EU_SLICE_MODE_CONTROL               0x05
#define USB_EU_RATE_CONTROL_MODE_CONTROL        0x06
#define USB_EU_AVERAGE_BITRATE_CONTROL          0x07
#define USB_EU_CPB_SIZE_CONTROL                 0x08
#define USB_EU_PEAK_BIT_RATE_CONTROL            0x09
#define USB_EU_QUANTIZATION_PARAMS_CONTROL      0x0A
#define USB_EU_SYNC_REF_FRAME_CONTROL           0x0B
#define USB_EU_LTR_BUFFER_CONTROL               0x0C
#define USB_EU_LTR_PICTURE_CONTROL              0x0D
#define USB_EU_LTR_VALIDATION_CONTROL           0x0E
#define USB_EU_LEVEL_IDC_LIMIT_CONTROL          0x0F
#define USB_EU_SEI_PAYLOADTYPE_CONTROL          0x10
#define USB_EU_QP_RANGE_CONTROL                 0x11
#define USB_EU_PRIORITY_CONTROL                 0x12
#define USB_EU_START_OR_STOP_LAYER_CONTROL      0x13
#define USB_EU_ERROR_RESILIENCY_CONTROL         0x14

/* A.9. Control Selector Codes - 7. Extension Unit Control Selectors */
#define USB_XU_CONTROL_UNDEFINED                0x00

/* A.9. Control Selector Codes - 8. VideoStreaming Interface Control Selectors */
#define USB_VS_CONTROL_UNDEFINED                0x00
#define USB_VS_PROBE_CONTROL                    0x01
#define USB_VS_COMMIT_CONTROL                   0x02
#define USB_VS_STILL_PROBE_CONTROL              0x03
#define USB_VS_STILL_COMMIT_CONTROL             0x04
#define USB_VS_STILL_IMAGE_TRIGGER_CONTROL      0x05
#define USB_VS_STREAM_ERROR_CODE_CONTROL        0x06
#define USB_VS_GENERATE_KEY_FRAME_CONTROL       0x07
#define USB_VS_UPDATE_FRAME_SEGMENT_CONTROL     0x08
#define USB_VS_SYNCH_DELAY_CONTROL              0x09

/* B.1. USB Terminal Types */
#define USB_TT_VENDOR_SPECIFIC                  0x0100
#define USB_TT_STREAMING                        0x0101

/* B.2. Input Terminal Types */
#define USB_ITT_VENDOR_SPECIFIC                 0x0200
#define USB_ITT_CAMERA                          0x0201
#define USB_ITT_MEDIA_TRANSPORT_INPUT           0x0202

/* B.3. Output Terminal Types */
#define USB_OTT_ VENDOR_SPECIFIC                0x0300
#define USB_OTT_DISPLAY                         0x0301
#define USB_OTT_MEDIA_TRANSPORT_OUTPUT          0x0302

/* B.4. External Terminal Types */
#define USB_EXTERNAL_VENDOR_SPECIFIC            0x0400
#define USB_COMPOSITE_CONNECTOR                 0x0401
#define USB_SVIDEO_CONNECTOR                    0x0402
#define USB_COMPONENT_CONNECTOR                 0x0403

/**
* @}
*/ 

/** @defgroup USBH_VIDEO_CLASS_Exported_Macros
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USBH_VIDEO_CLASS_Exported_Variables
* @{
*/ 
extern USBH_ClassTypeDef  VIDEO_Class;
#define USBH_VIDEO_CLASS    &VIDEO_Class
/**
* @}
*/ 

/** @defgroup USBH_VIDEO_CLASS_Exported_FunctionsPrototype
* @{
*/ 
USBH_StatusTypeDef USBH_VIDEO_IOProcess (USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_VIDEO_Init (USBH_HandleTypeDef *phost);
/**
* @}
*/ 

#ifdef __cplusplus
}
#endif

#endif /* __USBH_VIDEO_H */

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

