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

/* Includes standard C headers */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBUG_MAIN      // Enable debugging

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* SDCARD Device Variables */
FATFS RAMDISKFatFs;     /* File system object for RAM disk logical drive */
FIL MyFile;             /* File object */
char RAMDISKPath[4];    /* RAM disk logical drive path */

/* USB Device Handler for Camera Module */
#ifdef  USE_CAMERA_USB  ////////////////////////////////////////////////////////
  USBH_HandleTypeDef hUSBHost;
  VIDEO_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
#endif  ////////////////////////////////////////////////////////////////////////

/* Private function prototypes -----------------------------------------------*/
#ifdef  USE_CAMERA_DCMI ////////////////////////////////////////////////////////
  static void SavePicture(void);
#elif   defined USE_CAMERA_USB  ////////////////////////////////////////////////
  static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
  static void SavePicture_Process(void);
  static void SavePicture(void);
#endif  ////////////////////////////////////////////////////////////////////////
static void SystemClock_Config(void);
static void Error_Handler(void);
static void UART_Debug(const char * format, ...);

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
  UART_Debug("Initialization Done: UART/USART.\n");
  
  /* Configure the memory for storing captured image:
      - If it uses internal SDRAM, then enable the memory
      - If is uses external SRAM, then enable the memory module */
#ifdef  USE_INTERNAL_SDRAM      // USE_INTERNAL_SDRAM //////////////////////////
  /* Initialize the internal SDRAM */
  BSP_SDRAM_Init();
  UART_Debug("Initialization Done: SDRAM(Internal).\n");
  
  /* For testing the initialized memory */
  *(__IO uint32_t*) (CAMERA_FRAME_BUFFER) = 0x1234ABCD;
  uint32_t read_value = *(__IO uint32_t*) (CAMERA_FRAME_BUFFER);
  UART_Debug(" SDRAM: Check the data = 0x%X.\n", read_value);
  
#elif defined USE_EXTERNAL_SRAM // USE_EXTERNAL_SRAM ///////////////////////////
  /* Initialize the external SRAM */
  BSP_SRAM_Init();
  UART_Debug("Initialization Done: SRAM(External).\n");
  
  /* For testing the initialized memory */
  BSP_SRAM_Write(0x00, 0xABCD);
  uint16_t read_value = BSP_SRAM_Read(0x00);
  UART_Debug(" SRAM: Check the data = 0x%X.\n", read_value);
  
#endif  ////////////////////////////////////////////////////////////////////////
  
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
  UART_Debug("Initialization Done: Camera MCLK - MCO1(PA8).\n");

  /* Initialize the Camera */
  BSP_CAMERA_Init();
  UART_Debug("Initialization Done: MT9M111 Camera Module.\n");

  /* For testing the initialized module */
  uint16_t version = BSP_CAMERA_GetVersion();
  UART_Debug(" MT9M111: Camera Module Version = 0x%X.\n", version);
  
  /* Start the Camera Capture */
  BSP_CAMERA_ContinuousStart((uint8_t *)CAMERA_FRAME_BUFFER);
  UART_Debug(" MT9M111: Capturing Started with Continuous Mode.\n");
  
#elif defined USE_CAMERA_USB    // USE USB interface ///////////////////////////
  
  /** This Section is for the USB camera interface
    *   - Initialize the USB host device driver.
    *   - Register the USB host device class.
    *   - Register the USB camera video class.
    *   - Start the USB camera device.
    */
  
  /* Init Device Library */
  USBH_Init(&hUSBHost, USBH_UserProcess, 0);
  UART_Debug("Initialization Done: UVC Camera as an USB device.\n");
  
  /* Add Supported Class */
  USBH_RegisterClass(&hUSBHost, USBH_VIDEO_CLASS);
  UART_Debug(" UVC Camera: Register Class.\n");
  
  /* Start Device Process */
  USBH_Start(&hUSBHost);
  UART_Debug(" UVC Camera: Start the process.\n");
  
#endif   ///////////////////////////////////////////////////////////////////////
  
  /**
    * Execute user-defined code, which are enlisted below
    */
  while (1)
  {
    /* Capture images by using the camera:
      - SavePicture(): Use the DCMI interface
      - USBH_Process(): Use the USB interface */
#ifdef  USE_CAMERA_DCMI         // USE DCMI interface //////////////////////////
    
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
    
    /* Capture the Camera image in here */
    SavePicture();
    
    /* Turn LED3 on: Success capture */
    BSP_LED_Off(LED3);
    
#elif defined   USE_CAMERA_USB  // USE USB interface ///////////////////////////
    
    /* USB Host Background task */
    USBH_Process(&hUSBHost);
    
    /* Saving picture with USB camera module */
    SavePicture_Process();
    
#endif                          ////////////////////////////////////////////////
    
    /* Delay for another capture */
    HAL_Delay(500);
    
    /* Debug message */
    UART_Debug("Capture the camera image and send the data through UART/USART.\n");
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

#elif defined   USE_CAMERA_USB  // USE USB interface ///////////////////////////

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
    Appli_state = APPLICATION_CONNECT;
    break;

  default:
    break; 
  }
}

/**
  * @brief  SavePicture Process
  * @param  None
  * @retval None
  */
static void SavePicture_Process(void) 
{
  switch(Appli_state)
  {
  case APPLICATION_IDLE:
  case APPLICATION_READY:
    // Intentionally go through
    break;
  case APPLICATION_CONNECT:
    BSP_LED_On(LED3);
    // Save the image from the camera
    SavePicture();
    break;
  case APPLICATION_DISCONNECT:
    BSP_LED_Off(LED3);
    break;
    
  default:
    break;
  }
}

/**
  * @brief  SavePicture Function
  * @param  None
  * @retval None
  */
static void SavePicture(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */                              
  uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];                                   /* File read buffer */
  
  /*##-1- Link the RAM disk I/O driver #######################################*/
  if(FATFS_LinkDriver(&SD_Driver, RAMDISKPath) == 0)
  {
    /*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&RAMDISKFatFs, (TCHAR const*)RAMDISKPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      Error_Handler();
    }
    else
    {
      /*##-3- Create a FAT file system (format) on the logical drive #########*/
      if(f_mkfs((TCHAR const*)RAMDISKPath, 0, 0) != FR_OK) 
      {
        /* FatFs Format Error */
        Error_Handler();
      }
      else
      {      
        /*##-4- Create and Open a new text file object with write access #####*/
      if(f_open(&MyFile, "SAVEPICTURE.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
      {
        /* 'STM32.TXT' file Open for write Error */
        Error_Handler();
      }
      else
      {
        /*##-5- Write data to the text file ################################*/
        res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
        
        if((byteswritten == 0) || (res != FR_OK))
        {
          /* 'STM32.TXT' file Write or EOF Error */
          Error_Handler();
        }
        else
        {
            /*##-6- Close the open text file #################################*/
          f_close(&MyFile);
          
            /*##-7- Open the text file object with read access ###############*/
          if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
          {
            /* 'STM32.TXT' file Open for read Error */
            Error_Handler();
          }
          else
          {
              /*##-8- Read data from the text file ###########################*/
            res = f_read(&MyFile, rtext, sizeof(rtext), (void *)&bytesread);
            
              if((bytesread == 0) || (res != FR_OK))
            {
              /* 'STM32.TXT' file Read or EOF Error */
              Error_Handler();
            }
            else
            {
                /*##-9- Close the open text file #############################*/
              f_close(&MyFile);
              
                /*##-10- Compare read data with the expected data ############*/
                if ((bytesread != byteswritten))
                {                
                  /* Read data is different from the expected data */
                  Error_Handler(); 
                }
                else
                {
                  /* Success of the demo: no error occurrence */
                  UART_Debug("  Successfully write to SDCARD with filename 'STM32.txt'.\n");
                }
              }
            }
          }
        }
      }
    }
  }
  
  /*##-11- Unlink the RAM disk I/O driver ####################################*/
  FATFS_UnLinkDriver(RAMDISKPath);
}

#endif  ////////////////////////////////////////////////////////////////////////

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
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
  RCC_OscInitStruct.PLL.PLLN = 336;
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
static void UART_Debug(const char * format, ...)
{
#ifdef DEBUG_MAIN       ////////////////////////////////////////////////////////
  va_list list;
  va_start(list, format);
  
  int len = vsnprintf(0, 0, format, list);
  char *str = (char *) malloc(len + 1);
  vsprintf(str, format, list);
  
  /* Sends debug message through UART interface */ 
  if(UB_UART_Debug((uint8_t *)str, len) != HAL_OK)
  {
    Error_Handler();
  }
  
  free(str);
  va_end(list);
#endif  // DEBUG_MAIN //////////////////////////////////////////////////////////
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
