/**
  ******************************************************************************
  * @file    Source/main.c 
  * @author  Hong, Sanghyun
  * @version V0.1.0
  * @date    17-March-2015
  * @brief   Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* A flag for enabling debugging messages in main */
#ifndef DEBUG_MAIN
#define DEBUG_MAIN
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;

/* USB Device Handler for Camera Module */
#ifdef  USE_CAMERA_USB
  USBH_HandleTypeDef hUSBHost;
  VIDEO_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
#endif

/* Private function prototypes -----------------------------------------------*/
#ifdef  USE_CAMERA_DCMI
  static void SavePicture(void);
#elif   defined USE_CAMERA_USB
  static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
#endif
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure LED3,4 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  
  /* Configure KEY Button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
  
  /* Configure the System clock to 180 MHz */
  SystemClock_Config();

  /* Configure UART2 as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  if(UB_UART_Init(USARTx,
                  9600,
                  UART_WORDLENGTH_8B,
                  UART_STOPBITS_1,
                  UART_PARITY_NONE,
                  UART_HWCONTROL_NONE,
                  UART_MODE_TX_RX,
                  UART_OVERSAMPLING_16) != HAL_OK) 
  {
    Error_Handler();
  }

#ifdef  DEBUG_MAIN
  if(UB_UART_Debug("Initialization Done: UART/USART.\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif
  
  /* Configure the memory for storing captured image:
      - If it uses internal SDRAM, then enable the memory
      - If is uses external SRAM, then enable the memory module */
#ifdef  USE_INTERNAL_SDRAM
  /* Initialize the internal SDRAM */
  BSP_SDRAM_Init();
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug("Initialization Done: SDRAM(Internal).\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif
  
  /* For testing the initialized memory */
  *(__IO uint32_t*) (CAMERA_FRAME_BUFFER) = 0x1234ABCD;
  uint32_t read_value = *(__IO uint32_t*) (CAMERA_FRAME_BUFFER);
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug(" SDRAM: Check the data = 0x%X.\n", read_value)!= HAL_OK)
  {
    Error_Handler();
  }
#endif
#elif defined USE_EXTERNAL_SRAM
  /* Initialize the external SRAM */
  BSP_SRAM_Init();
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug("Initialization Done: SRAM(External).\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif
  
  /* For testing the initialized memory */
  BSP_SRAM_Write(0x00, 0xABCD);
  uint16_t read_value = BSP_SRAM_Read(0x00);
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug(" SRAM: Check the data = 0x%X.\n", read_value)!= HAL_OK)
  {
    Error_Handler();
  }
#endif
#endif
  
  /* Configure the camera for capturing image:
      - If it uses DCMI interface, then enabling DCMI interface
      - If is uses USB interface, then enabling UVC interface */
#ifdef  USE_CAMERA_DCMI         // USE DCMI interface //////////////////////////
  
  /** This Section is for the DCMI camera interface
    *   - Initialize the master clock (mclk).
    *   - Initialize the camera module.
    *   - Start the module with continuous mode.
    */
  
  /* Output HSE divided by 4 on MCO1 pin(PA8) */ 
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_4);
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug("Initialization Done: Camera MCLK - MCO1(PA8).\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif

  /* Initialize the Camera */
  BSP_CAMERA_Init();
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug("Initialization Done: MT9M111 Camera Module.\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif  

  /* For testing the initialized module */
  uint16_t version = BSP_CAMERA_GetVersion();
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug(" MT9M111: Camera Module Version = 0x%X.\n", version)!= HAL_OK)
  {
    Error_Handler();
  }
#endif  
  
  /* Start the Camera Capture */
  BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug(" MT9M111: Capturing Started with Continuous Mode.\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#elif defined USE_CAMERA_USB    // USE USB interface ///////////////////////////
  
  /** This Section is for the USB camera interface
    *   - Initialize the USB host device driver.
    *   - Register the USB host device class.
    *   - Register the USB camera video class.
    *   - Start the USB camera device.
    */
  
  /* Init Device Library */
  USBH_Init(&hUSBHost, USBH_UserProcess, 0);
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug("Initialization Done: UVC Camera as an USB device.\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif 
  
  /* Add Supported Class */
  USBH_RegisterClass(&hUSBHost, USBH_VIDEO_CLASS);
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug(" UVC Camera: Register Class.\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif
  
  /* Start Device Process */
  USBH_Start(&hUSBHost);
  
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug(" UVC Camera: Start the process.\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif
  
#endif                          ////////////////////////////////////////////////
  
  /**
    * Execute user-defined code, which are enlisted below
    */
  while (1)
  {
    /* Turn LED3 off */
    BSP_LED_Off(LED3);
    
    /**
      * 1. Wait for USER Button press before starting the Communication 
      * 2. Wait for USER Button release before starting the Communication
      */
    while (BSP_PB_GetState(BUTTON_KEY) == RESET)
    {
      /* Toggle LED3 waiting for user to press button */
      BSP_LED_Toggle(LED3);
      HAL_Delay(40);		
    }
    while (BSP_PB_GetState(BUTTON_KEY) == SET);
    
    /* Capture images by using the camera:
      - SavePicture(): Use the DCMI interface
      - USBH_Process(): Use the USB interface */
#ifdef  USE_CAMERA_DCMI         // USE DCMI interface //////////////////////////
    
    /* Capture the Camera image in here */
    SavePicture();
    
#elif   defined USB_CAMERA_USB  // USE USB interface ///////////////////////////
    
    /* USB Host Background task */
    // FIXME - USBH_Process(&hUSBHost);
    
#endif                          ////////////////////////////////////////////////
    
    /* Turn LED3 on: Success capture */
    BSP_LED_Off(LED3);
    
    /* Delay for another capture */
    HAL_Delay(500);
    
#ifdef  DEBUG_MAIN
  if(UB_UART_Debug("Capture the camera image and send the data through UART/USART.\n")!= HAL_OK)
  {
    Error_Handler();
  }
#endif
  }
}

#ifdef  USE_CAMERA_DCMI         // USE DCMI interface //////////////////////////

/**
  * @brief  Frame Event callback.
  * @param  None
  * @retval None
*/
void BSP_CAMERA_FrameEventCallback(void)
{
  /* Write the frame data to the specific storage */
  // FIXME - BSP_LCD_DrawRGBImage(0, 0, 320, 240, (uint8_t *)CAMERA_FRAME_BUFFER);
}

/**
  * @brief  Main routine for saving the camera image
  * @param  None
  * @retval None
  */
static void SavePicture(void)
{
  /* Suspend the camera capture */
  BSP_CAMERA_Suspend();
  
  // TODO - Refer to 'PicturePrepare' routine and 'SavePicture' routine in the other project.
  
  /* Resume the camera capture */
  BSP_CAMERA_Resume();
}

#elif   defined USE_CAMERA_USB  // USE USB interface ///////////////////////////

/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch(id)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
    break;
    
  case HOST_USER_DISCONNECTION:
    Appli_state = APPLICATION_DISCONNECT;
    
    break;
    
  case HOST_USER_CLASS_ACTIVE:
    Appli_state = APPLICATION_READY;
    break;
    
  case HOST_USER_CONNECTION:
    Appli_state = APPLICATION_START;
    break;

  default:
    break; 
  }
}

#endif                          ////////////////////////////////////////////////

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Turn LED3 on: Transfer error in reception/transmission process */
  BSP_LED_On(LED4); 
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on - RED */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
