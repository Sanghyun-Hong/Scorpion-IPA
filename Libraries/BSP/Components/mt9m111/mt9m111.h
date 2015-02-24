

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
#define MT9M111_VERSION                 0x143A
/** 
  * @brief  MT9M111 Registers  
  */
/* MT9M111 Sensor core register addresses (0x000..0x0ff) - Address page 0 */
#define MT9M111_CHIP_VERSION            0x000
#define MT9M111_ROW_START               0x001
#define MT9M111_COLUMN_START            0x002
#define MT9M111_WINDOW_HEIGHT           0x003
#define MT9M111_WINDOW_WIDTH            0x004
#define MT9M111_HORIZONTAL_BLANKING_B   0x005
#define MT9M111_VERTICAL_BLANKING_B     0x006
#define MT9M111_HORIZONTAL_BLANKING_A   0x007
#define MT9M111_VERTICAL_BLANKING_A     0x008
#define MT9M111_SHUTTER_WIDTH           0x009
#define MT9M111_ROW_SPEED               0x00A
#define MT9M111_EXTRA_DELAY             0x00B
#define MT9M111_SHUTTER_DELAY           0x00C
#define MT9M111_RESET                   0x00D
#define MT9M111_READ_MODE_B             0x020
#define MT9M111_READ_MODE_A             0x021
#define MT9M111_FLASH_CONTROL           0x023
#define MT9M111_GREEN1_GAIN             0x02B
#define MT9M111_BLUE_GAIN               0x02C
#define MT9M111_RED_GAIN                0x02D
#define MT9M111_GREEN2_GAIN             0x02E
#define MT9M111_GLOBAL_GAIN             0x02F
#define MT9M111_CONTEXT_CONTROL         0x0C8
#define MT9M111_PAGE_MAP                0x0F0
#define MT9M111_BYTE_WISE_ADDR          0x0F1

#define MT9M111_RESET_SYNC_CHANGES      (1 << 15)
#define MT9M111_RESET_RESTART_BAD_FRAME (1 << 9)
#define MT9M111_RESET_SHOW_BAD_FRAMES   (1 << 8)
#define MT9M111_RESET_RESET_SOC         (1 << 5)
#define MT9M111_RESET_OUTPUT_DISABLE    (1 << 4)
#define MT9M111_RESET_CHIP_ENABLE       (1 << 3)
#define MT9M111_RESET_ANALOG_STANDBY    (1 << 2)
#define MT9M111_RESET_RESTART_FRAME     (1 << 1)
#define MT9M111_RESET_RESET_MODE        (1 << 0)

#define MT9M111_RM_FULL_POWER_RD        (0 << 10)
#define MT9M111_RM_LOW_POWER_RD         (1 << 10)
#define MT9M111_RM_COL_SKIP_4X          (1 << 5)
#define MT9M111_RM_ROW_SKIP_4X          (1 << 4)
#define MT9M111_RM_COL_SKIP_2X          (1 << 3)
#define MT9M111_RM_ROW_SKIP_2X          (1 << 2)
#define MT9M111_RMB_MIRROR_COLS         (1 << 1)
#define MT9M111_RMB_MIRROR_ROWS         (1 << 0)
#define MT9M111_CTXT_CTRL_RESTART       (1 << 15)
#define MT9M111_CTXT_CTRL_DEFECTCOR_B   (1 << 12)
#define MT9M111_CTXT_CTRL_RESIZE_B      (1 << 10)
#define MT9M111_CTXT_CTRL_CTRL2_B       (1 << 9)
#define MT9M111_CTXT_CTRL_GAMMA_B       (1 << 8)
#define MT9M111_CTXT_CTRL_XENON_EN      (1 << 7)
#define MT9M111_CTXT_CTRL_READ_MODE_B   (1 << 3)
#define MT9M111_CTXT_CTRL_LED_FLASH_EN  (1 << 2)
#define MT9M111_CTXT_CTRL_VBLANK_SEL_B  (1 << 1)
#define MT9M111_CTXT_CTRL_HBLANK_SEL_B  (1 << 0)

/* MT9M111 Colorpipe register addresses (0x100..0x1ff) - Address page 1 */
#define MT9M111_OPER_MODE_CTRL          0x106
#define MT9M111_OUTPUT_FORMAT_CTRL      0x108
#define MT9M111_REDUCER_XZOOM_B         0x1A0
#define MT9M111_REDUCER_XSIZE_B         0x1A1
#define MT9M111_REDUCER_YZOOM_B         0x1A3
#define MT9M111_REDUCER_YSIZE_B         0x1A4
#define MT9M111_REDUCER_XZOOM_A         0x1A6
#define MT9M111_REDUCER_XSIZE_A         0x1A7
#define MT9M111_REDUCER_YZOOM_A         0x1A9
#define MT9M111_REDUCER_YSIZE_A         0x1AA

#define MT9M111_OUTPUT_FORMAT_CTRL2_A   0x13A
#define MT9M111_OUTPUT_FORMAT_CTRL2_B   0x19B

#define MT9M111_OPMODE_AUTOEXPO_EN      (1 << 14)
#define MT9M111_OPMODE_AUTOWHITEBAL_EN  (1 << 1)
#define MT9M111_OUTFMT_FLIP_BAYER_COL   (1 << 9)
#define MT9M111_OUTFMT_FLIP_BAYER_ROW   (1 << 8)
#define MT9M111_OUTFMT_PROCESSED_BAYER  (1 << 14)
#define MT9M111_OUTFMT_BYPASS_IFP       (1 << 10)
#define MT9M111_OUTFMT_INV_PIX_CLOCK    (1 << 9)
#define MT9M111_OUTFMT_RGB              (1 << 8)
#define MT9M111_OUTFMT_RGB565           (0 << 6)
#define MT9M111_OUTFMT_RGB555           (1 << 6)
#define MT9M111_OUTFMT_RGB444x          (2 << 6)
#define MT9M111_OUTFMT_RGBx444          (3 << 6)
#define MT9M111_OUTFMT_TST_RAMP_OFF     (0 << 4)
#define MT9M111_OUTFMT_TST_RAMP_COL     (1 << 4)
#define MT9M111_OUTFMT_TST_RAMP_ROW     (2 << 4)
#define MT9M111_OUTFMT_TST_RAMP_FRAME   (3 << 4)
#define MT9M111_OUTFMT_SHIFT_3_UP       (1 << 3)
#define MT9M111_OUTFMT_AVG_CHROMA       (1 << 2)
#define MT9M111_OUTFMT_SWAP_YCbCr_C_Y_RGB_EVEN  (1 << 1)
#define MT9M111_OUTFMT_SWAP_YCbCr_Cb_Cr_RGB_R_B (1 << 0)
/**
  * @}
  */
  
/** @defgroup MT9M111_Exported_Functions
  * @{
  */ 
void     MT9M111_Init(uint16_t DeviceAddr, uint32_t resolution);
uint16_t MT9M111_ReadVersion(uint16_t DeviceAddr);

void     CAMERA_IO_Init(void);
void     CAMERA_IO_WriteByte(uint8_t Addr, uint16_t Reg, uint8_t Value);
uint8_t  CAMERA_IO_ReadByte(uint8_t Addr, uint16_t Reg);
void     CAMERA_IO_WriteWord(uint8_t Addr, uint16_t Reg, uint16_t Value);
uint16_t CAMERA_IO_ReadWord(uint8_t Addr, uint16_t Reg);
void     CAMERA_Delay(uint32_t Delay);

/* CAMERA driver structure */
extern   CAMERA_DrvTypeDef   mt9m111_drv;
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
