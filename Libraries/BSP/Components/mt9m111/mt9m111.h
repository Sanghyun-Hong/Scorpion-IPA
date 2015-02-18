

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MT9M111_H
#define __MT9M111_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "../Common/camera.h"
   
/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup mt9m111
  * @{
  */

/** @defgroup MT9M111_Exported_Types
  * @{
  */
     
/**
  * @}
  */ 

/** @defgroup MT9M111_Exported_Constants
  * @{
  */
/** 
  * @brief  MT9M111 ID  
  */  
#define MT9M111_ID                       0x26
/** 
  * @brief  MT9M111 Registers  
  */
/* MT9M111 Registers definition when DSP bank selected (0xFF = 0x00) */
#define MT9M111_DSP_R_BYPASS             0x05
#define MT9M111_DSP_Qs                   0x44
#define MT9M111_DSP_CTRL                 0x50
#define MT9M111_DSP_HSIZE1               0x51
#define MT9M111_DSP_VSIZE1               0x52
#define MT9M111_DSP_XOFFL                0x53
#define MT9M111_DSP_YOFFL                0x54
#define MT9M111_DSP_VHYX                 0x55
#define MT9M111_DSP_DPRP                 0x56
#define MT9M111_DSP_TEST                 0x57
#define MT9M111_DSP_ZMOW                 0x5A
#define MT9M111_DSP_ZMOH                 0x5B
#define MT9M111_DSP_ZMHH                 0x5C
#define MT9M111_DSP_BPADDR               0x7C
#define MT9M111_DSP_BPDATA               0x7D
#define MT9M111_DSP_CTRL2                0x86
#define MT9M111_DSP_CTRL3                0x87
#define MT9M111_DSP_SIZEL                0x8C
#define MT9M111_DSP_HSIZE2               0xC0
#define MT9M111_DSP_VSIZE2               0xC1
#define MT9M111_DSP_CTRL0                0xC2
#define MT9M111_DSP_CTRL1                0xC3
#define MT9M111_DSP_R_DVP_SP             0xD3
#define MT9M111_DSP_IMAGE_MODE           0xDA
#define MT9M111_DSP_RESET                0xE0
#define MT9M111_DSP_MS_SP                0xF0
#define MT9M111_DSP_SS_ID                0x7F
#define MT9M111_DSP_SS_CTRL              0xF8
#define MT9M111_DSP_MC_BIST              0xF9
#define MT9M111_DSP_MC_AL                0xFA
#define MT9M111_DSP_MC_AH                0xFB
#define MT9M111_DSP_MC_D                 0xFC
#define MT9M111_DSP_P_STATUS             0xFE
#define MT9M111_DSP_RA_DLMT              0xFF

/* MT9M111 Registers definition when sensor bank selected (0xFF = 0x01) */
#define MT9M111_SENSOR_GAIN              0x00
#define MT9M111_SENSOR_COM1              0x03
#define MT9M111_SENSOR_REG04             0x04
#define MT9M111_SENSOR_REG08             0x08
#define MT9M111_SENSOR_COM2              0x09
#define MT9M111_SENSOR_PIDH              0x0A
#define MT9M111_SENSOR_PIDL              0x0B
#define MT9M111_SENSOR_COM3              0x0C
#define MT9M111_SENSOR_COM4              0x0D
#define MT9M111_SENSOR_AEC               0x10
#define MT9M111_SENSOR_CLKRC             0x11
#define MT9M111_SENSOR_COM7              0x12
#define MT9M111_SENSOR_COM8              0x13
#define MT9M111_SENSOR_COM9              0x14
#define MT9M111_SENSOR_COM10             0x15
#define MT9M111_SENSOR_HREFST            0x17
#define MT9M111_SENSOR_HREFEND           0x18
#define MT9M111_SENSOR_VSTART            0x19
#define MT9M111_SENSOR_VEND              0x1A
#define MT9M111_SENSOR_MIDH              0x1C
#define MT9M111_SENSOR_MIDL              0x1D
#define MT9M111_SENSOR_AEW               0x24
#define MT9M111_SENSOR_AEB               0x25
#define MT9M111_SENSOR_W                 0x26
#define MT9M111_SENSOR_REG2A             0x2A
#define MT9M111_SENSOR_FRARL             0x2B
#define MT9M111_SENSOR_ADDVSL            0x2D
#define MT9M111_SENSOR_ADDVHS            0x2E
#define MT9M111_SENSOR_YAVG              0x2F
#define MT9M111_SENSOR_REG32             0x32
#define MT9M111_SENSOR_ARCOM2            0x34
#define MT9M111_SENSOR_REG45             0x45
#define MT9M111_SENSOR_FLL               0x46
#define MT9M111_SENSOR_FLH               0x47
#define MT9M111_SENSOR_COM19             0x48
#define MT9M111_SENSOR_ZOOMS             0x49
#define MT9M111_SENSOR_COM22             0x4B
#define MT9M111_SENSOR_COM25             0x4E
#define MT9M111_SENSOR_BD50              0x4F
#define MT9M111_SENSOR_BD60              0x50
#define MT9M111_SENSOR_REG5D             0x5D
#define MT9M111_SENSOR_REG5E             0x5E
#define MT9M111_SENSOR_REG5F             0x5F
#define MT9M111_SENSOR_REG60             0x60
#define MT9M111_SENSOR_HISTO_LOW         0x61
#define MT9M111_SENSOR_HISTO_HIGH        0x62 

/** 
 * @brief  MT9M111 Features Parameters  
 */
#define MT9M111_BRIGHTNESS_LEVEL0        0x40   /* Brightness level -2         */
#define MT9M111_BRIGHTNESS_LEVEL1        0x30   /* Brightness level -1         */
#define MT9M111_BRIGHTNESS_LEVEL2        0x20   /* Brightness level 0          */
#define MT9M111_BRIGHTNESS_LEVEL3        0x10   /* Brightness level +1         */
#define MT9M111_BRIGHTNESS_LEVEL4        0x00   /* Brightness level +2         */

#define MT9M111_BLACK_WHITE_BW           0x18   /* Black and white effect      */
#define MT9M111_BLACK_WHITE_NEGATIVE     0x40   /* Negative effect             */
#define MT9M111_BLACK_WHITE_BW_NEGATIVE  0x58   /* BW and Negative effect      */
#define MT9M111_BLACK_WHITE_NORMAL       0x00   /* Normal effect               */

#define MT9M111_CONTRAST_LEVEL0          0x3418 /* Contrast level -2           */
#define MT9M111_CONTRAST_LEVEL1          0x2A1C /* Contrast level -2           */
#define MT9M111_CONTRAST_LEVEL2          0x2020 /* Contrast level -2           */
#define MT9M111_CONTRAST_LEVEL3          0x1624 /* Contrast level -2           */
#define MT9M111_CONTRAST_LEVEL4          0x0C28 /* Contrast level -2           */

#define MT9M111_COLOR_EFFECT_ANTIQUE     0xA640 /* Antique effect              */
#define MT9M111_COLOR_EFFECT_BLUE        0x40A0 /* Blue effect                 */
#define MT9M111_COLOR_EFFECT_GREEN       0x4040 /* Green effect                */
#define MT9M111_COLOR_EFFECT_RED         0xC040 /* Red effect                  */   
/**
  * @}
  */
  
/** @defgroup MT9M111_Exported_Functions
  * @{
  */ 
void     mt9m111_Init(uint16_t DeviceAddr, uint32_t resolution);
void     mt9m111_Config(uint16_t DeviceAddr, uint32_t feature, uint32_t value, uint32_t BR_value);
uint16_t mt9m111_ReadID(uint16_t DeviceAddr);

void     CAMERA_IO_Init(void);
void     CAMERA_IO_Write(uint8_t addr, uint8_t reg, uint8_t value);
uint8_t  CAMERA_IO_Read(uint8_t addr, uint8_t reg);
void     CAMERA_Delay(uint32_t delay);

/* CAMERA driver structure */
extern CAMERA_DrvTypeDef   mt9m111_drv;
/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __MT9M111_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
