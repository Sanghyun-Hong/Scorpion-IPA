/**
  ******************************************************************************
  * @file    stm32f429i_discovery_sram.h
  * @author  Hong, Sanghyun
  * @version V0.1.0
  * @date    14-March-2015
  * @brief   This file contains all the functions prototypes for the 
  *          stm32f429i_discovery_sram.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 Hong, Sanghyun</center></h2>
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
#ifndef __STM32F429I_DISCOVERY_SRAM_H
#define __STM32F429I_DISCOVERY_SRAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f429i_discovery.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */
 
/** @defgroup STM32F429I_DISCOVERY_SRAM
  * @{
  */
  
/** @defgroup STM32F429I_DISCOVERY_SRAM_Exported_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_SRAM_Exported_Constants
  * @{
  */
/* FMC SRAM Bank address */
#define SRAM_DEVICE_ADDR        ((uint32_t)0x64000000)  // Use Bank-2
//#define SRAM_DEVICE_SIZE        ((uint32_t)0x800000)    // SRAM device size in MBytes

/* FMC SRAM memory width */
/* #define SRAM_MEMORY_WIDTH            FMC_NORSRAM_MEM_BUS_WIDTH_8  */
#define SRAM_MEMORY_WIDTH               FMC_NORSRAM_MEM_BUS_WIDTH_16
/* #define SRAM_MEMORY_WIDTH            FMC_NORSRAM_MEM_BUS_WIDTH_32 */

/* FMC SRAM Asyncronous clock */
/* #define SRAM_CONTINUOUS_CLOCK    FMC_CONTINUOUS_CLOCK_SYNC_ONLY */
#define SRAM_CONTINUOUS_CLOCK    FMC_CONTINUOUS_CLOCK_SYNC_ASYNC
    
/* FMC Memory Access Timing */
#define SRAM_FMC_ADDR_SETUP_TIME        3
#define SRAM_FMC_DATA_SETUP_TIME        0
#define SRAM_FMC_ADDR_HOLD_TIME         3

/**
  * @}
  */
  
/** @defgroup STM32F429I_DISCOVERY_SRAM_Exported_Macro
  * @{
  */
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_SRAM_Exported_Functions
  * @{
  */
uint8_t         BSP_SRAM_Init(void);
void            BSP_SRAM_Write(uint32_t addr, uint16_t data);
uint16_t        BSP_SRAM_Read(uint32_t addr);
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

#endif /* __STM32F429I_DISCOVERY_SRAM_H */

/************************ (C) COPYRIGHT Hong, Sanghyun *****END OF FILE****/