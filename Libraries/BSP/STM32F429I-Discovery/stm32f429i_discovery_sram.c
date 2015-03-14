/**
  ******************************************************************************
  * @file    stm32f429i_discovery_sram.c
  * @author  Hong, Sanghyun
  * @version V0.1.0
  * @date    15-March-2015
  * @brief   This file provides a set of functions needed to drive the
  *          IS61WV102416ALL SRAM memory connected to STM32F429I-Discovery Kit.    
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 Hong, Sanghyun</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f429i_discovery_sram.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */
  
/** @defgroup STM32F429I_DISCOVERY_SRAM
  * @{
*/ 

/** @defgroup STM32F429I_DISCOVERY_SRAM_Private_Types_Definitions
  * @{
  */
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_SRAM_Private_Defines
  * @{
  */
/**
  * @}
  */  

/** @defgroup STM32F429I_DISCOVERY_SRAM_Private_Macros
  * @{
  */ 
/**
  * @}
  */  

/** @defgroup STM32F429I_DISCOVERY_SRAM_Private_Variables
  * @{
  */
static SRAM_HandleTypeDef SramHandle;
static FMC_NORSRAM_TimingTypeDef Timing;
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_SRAM_Private_Function_Prototypes
  * @{
  */ 
static void MspInit(void);
/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_SRAM_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the SDRAM device.
  * @param  None
  * @retval None
  */
uint8_t BSP_SRAM_Init(void)
{
  uint8_t retval = 0x00;
  
  /* SRAM device configuration */
  SramHandle.Instance  = FMC_NORSRAM_DEVICE;
  SramHandle.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;
  
  /* SRAM structure for timing */
  Timing.AddressSetupTime               = SRAM_FMC_ADDR_SETUP_TIME;
  Timing.AddressHoldTime                = SRAM_FMC_ADDR_HOLD_TIME;
  Timing.DataSetupTime                  = SRAM_FMC_DATA_SETUP_TIME;
  Timing.BusTurnAroundDuration          = 0;
  Timing.CLKDivision                    = 0;
  Timing.DataLatency                    = 0;
  Timing.AccessMode                     = FMC_ACCESS_MODE_A;
  
  /* SRARM structure for Bank-2/PSRAM-1 */
  SramHandle.Init.NSBank                = FMC_NORSRAM_BANK2;
  SramHandle.Init.DataAddressMux        = FMC_DATA_ADDRESS_MUX_DISABLE;
  SramHandle.Init.MemoryType            = FMC_MEMORY_TYPE_SRAM;
  SramHandle.Init.MemoryDataWidth       = SRAM_MEMORY_WIDTH;
  SramHandle.Init.BurstAccessMode       = FMC_BURST_ACCESS_MODE_DISABLE;
  SramHandle.Init.WaitSignalPolarity    = FMC_WAIT_SIGNAL_POLARITY_LOW;
  SramHandle.Init.WrapMode              = FMC_WRAP_MODE_DISABLE;
  SramHandle.Init.WaitSignalActive      = FMC_WAIT_TIMING_BEFORE_WS;
  SramHandle.Init.WriteOperation        = FMC_WRITE_OPERATION_ENABLE;
  SramHandle.Init.WaitSignal            = FMC_WAIT_SIGNAL_DISABLE;
  SramHandle.Init.ExtendedMode          = FMC_EXTENDED_MODE_DISABLE;
  SramHandle.Init.AsynchronousWait      = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  SramHandle.Init.WriteBurst            = FMC_WRITE_BURST_DISABLE;
  SramHandle.Init.ContinuousClock       = SRAM_CONTINUOUS_CLOCK;
  
  /* Initialize the SRAM controller */
  retval = HAL_SRAM_Init(&SramHandle, &Timing, &Timing);
  
  return retval;
  
}

/**
  * @brief  Write data to SRAM memory
  * @param  addr Address to write data
  * @param  data Data to write
  * @retval None
  */
void BSP_SRAM_Write(uint32_t addr, uint16_t data)
{
  *(__IO uint16_t*) (SRAM_DEVICE_ADDR + (addr << 1)) = data;
}

/**
  * @brief  Read data from SRAM memory
  * @param  addr Address to read data
  * @retval read data
  */
uint16_t BSP_SRAM_Read(uint32_t addr)
{
  return *(__IO uint16_t*) (SRAM_DEVICE_ADDR + (addr << 1));
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

/************************ (C) COPYRIGHT Hong, Sanghyun *****END OF FILE****/