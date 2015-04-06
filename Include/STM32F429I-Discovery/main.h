/**
  ******************************************************************************
  * @file    Include/main.h 
  * @author  Hong, Sanghyun
  * @version V0.1.0
  * @date    17-March-2015
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_sd.h"

/* Includes User-defined headers */
#include "ub_uart.h"

/* Includes FatFs component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported types ------------------------------------------------------------*/

//////////////////////////// CONFIGURATION BLOCK ///////////////////////////////
/**
  * Memory Type
  */
#define USE_INTERNAL_SDRAM
//#define USE_EXTERNAL_SRAM
/**
  * Camera Type
  */
//#define USE_CAMERA_DCMI
#define USE_CAMERA_USB
////////////////////////////////////////////////////////////////////////////////

/* Exported constants --------------------------------------------------------*/

/**
  * Memory Type
  */
#ifdef  USE_INTERNAL_SDRAM /////////////////////////////////////////////////////
  // Include SDRAM Headers
  #include "stm32f429i_discovery_sdram.h"
  // Define Framebuffer Address
  #define CAMERA_FRAME_BUFFER     0xD0000000
#elif defined USE_EXTERNAL_SRAM ////////////////////////////////////////////////
  // Include SRAM Headers
  #include "stm32f429i_discovery_sram.h"
  // Define Framebuffer Address
  #define CAMERA_FRAME_BUFFER     0x64000000    // Use Bank-2
#endif  ////////////////////////////////////////////////////////////////////////

/**
  * Camera Type
  */
#ifdef  USE_CAMERA_DCMI ////////////////////////////////////////////////////////
  // Includes DCMI headers
  #include "stm32f429i_discovery_camera.h"

#elif defined   USE_CAMERA_USB  ////////////////////////////////////////////////
  // Includes USB headers
  #include "usbh_core.h"
  #include "usbh_video.h"

  // USB Host Application Status
  typedef enum {
    APPLICATION_IDLE = 0,  
    APPLICATION_READY,
    APPLICATION_CONNECT,
    APPLICATION_DISCONNECT,
  } VIDEO_ApplicationTypeDef;
    
  extern USBH_HandleTypeDef hUSBHost;
#endif  ////////////////////////////////////////////////////////////////////////

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
