/**
  ******************************************************************************
  * @file
  * @author
  * @version
  * @date
  * @brief
  * @function List:
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2021 </center></h2>
  ******************************************************************************
  * @History:
  * @Author:
  * @Data:
  * @Version:
*/


#ifndef __BOOT_APP_CFG_H__
#define __BOOT_APP_CFG_H__


#if 0
#include "common_types.h"
#include "gpio.h"
#include "flash_cfg.h"
#include "nvic.h"
#include "device_registers.h"
#else
#include "common_types.h"
#include "common_cfg.h"
#include "gpio.h"
//#include "wdt.h"

#endif


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/
//暫時放這裡  手冊上可能不是這地址
#define FLASH_APP_BASE            ((uint32_t)0x0000C000) /*!< FLASH(up to 1 MB) base address in the alias region                         */
#define FLASH_ASW_MAX_ADDR 0x0001C000   //0x0001C000
// app 的偏移地址
// app 的偏移地址
#define BOOT_APP_START  FLASH_APP_BASE
#define BOOT_APP_VECTOR_OFFSET  0x00000100
//app的起始地址
//app的起始地址
#define  APP_ADRRESS_START  BOOT_APP_START //(FLASH_APP_BASE  + BOOT_APP_START)
#define  APP_ADRRESS_VECTOR  (FLASH_APP_BASE  + BOOT_APP_VECTOR_OFFSET)

#define APP_VALID_FLAG_1_ADDR (APP_ADRRESS_START)    //首地址，也做为标志

#define APP_VALID_ADDR_OFFSET 0x10 //0X100  //倒推回來的偏移地址
//app有效标志地址 app_valid_flag
#define APP_VALID_FLAG1_ADDR (APP_ADRRESS_START)    //0x8000
#define APP_VALID_FLAG_ADDR 0x0001BFF8 //
#define APP_VALID_FLAG_IN_ADDR  0x0001BFCCu //没用到
#define BOOT_CALIBRATION_START_ADDR 0x0000780u
#define APP_CALIBRATION_START_ADDR 0x0001BBCCu //

#define P_APP_VALID_FLAG_ADDR_U32        ((u32 *)APP_VALID_FLAG_ADDR)
#define P_APP_VALID_FLAG_ADDR_U8        ((u8 *)APP_VALID_FLAG_ADDR)
#define FLAG_APP_VALID_FLAG_DATA   0xAAAA04E5ul    //app valid flag
#define FLAG_APP_VALID_FLAG_DATA0   0xaa    //app valid flag
#define FLAG_APP_VALID_FLAG_DATA1   0xaa    //app valid flag
#define FLAG_APP_VALID_FLAG_DATA2   0x04    //app valid flag
#define FLAG_APP_VALID_FLAG_DATA3   0xe5    //app valid flag



//以下從 fe-5ha移植
#define FLAG_REPROGRAM_ID   0x01U
#define FLAG_RESET_ID     0x02U
#define FLAG_FINGERPRINT_ID   0x03U

//這是RAM 相關的，重擦除標志
#define FLAG_BASE_SIZE      0x04U


//CAL的起始地址
#define  CAL_ADRRESS_START  (FLASH_ASW_MAX_ADDR)
#define  CAL_ADRRESS_LEN  (0X800)


//#define APP_USER_CONST_ADDR_OFFSET 0X800  //倒推回來的偏移地址--用戶一些用戶數據
//#define APP_USER_CONST_ADDR (FLASH_ASW_MAX_ADDR-APP_USER_CONST_ADDR_OFFSET)    //144 最大512k = 0x0008 0000

#define FLAG_BASE_ADDR      0x20007FC0u  //


#define FLAG_REPROGRAM_ADDR   (FLAG_BASE_ADDR + 0u)   //重編程請求的地址

#define FLAG_REPROGRAM_LEN 4u
#define FLAG_REPROGRAM_DATA   0xA5A5E534U    //重編程請求的值

#define FLAG_REPROGRAM_u8_DATA0   0xA5U    //重編程請求的值
#define FLAG_REPROGRAM_u8_DATA1   0xA5U    //重編程請求的值
#define FLAG_REPROGRAM_u8_DATA2   0xE5U    //重編程請求的值
#define FLAG_REPROGRAM_u8_DATA3   0x34U    //重編程請求的值


#define FLAG_JUMP_TO_BOOT_RESPONSE_ADDR   (u32)(FLAG_BASE_ADDR + 0x04u)   //標志，用於跳轉到boot時，要應答的數據；
#define FLAG_JUMP_TO_BOOT_RESPONSE_LEN   (4u)   //標志，用於跳轉到boot時，要應答的數據；
#define FLAG_JUMP_TO_BOOT_RESPONSE_INIT   ((u32)0xaa769021)   //無效值
#define FLAG_JUMP_TO_BOOT_RESPONSE_11_01   ((u32)0x554b1101)   //復位的應答
#define FLAG_JUMP_TO_BOOT_RESPONSE_11_03   ((u32)0x554b1103)   //復位的應答
#define FLAG_JUMP_TO_BOOT_RESPONSE_10_02   ((u32)0x554b1002)   //編程的應答
#define FLAG_JUMP_TO_BOOT_RESPONSE_10_01   ((u32)0x554b1001)   //默認的應答
#define FLAG_JUMP_TO_APP_RESPONSE_11_01_PRE   ((u32)0x55491101)   //復位的應答----因为跳转前，先又进入boot，所以加以区分
#define FLAG_JUMP_TO_APP_RESPONSE_11_03_PRE   ((u32)0x55491103)   //復位的應答
#define FLAG_JUMP_TO_APP_RESPONSE_10_02_PRE   ((u32)0x55491002)   //編程的應答
#define FLAG_JUMP_TO_APP_RESPONSE_10_01_PRE   ((u32)0x55491001)   //默認的應答
#define FLAG_JUMP_TO_APP_RESPONSE_11_01   ((u32)0x554a1101)   //復位的應答----因为跳转前，先又进入boot，所以加以区分
#define FLAG_JUMP_TO_APP_RESPONSE_11_03   ((u32)0x554a1103)   //復位的應答
#define FLAG_JUMP_TO_APP_RESPONSE_10_02   ((u32)0x554a1002)   //編程的應答
#define FLAG_JUMP_TO_APP_RESPONSE_10_01   ((u32)0x554a1001)   //默認的應答

#if 1
#define FLAG_RAM_WDOG_ADDR   0x20007800   //看門狗在ram中運行
#define FLAG_RAM_DRIVER_ADDR   0x20007000  //驅動在driver中運行
#define FLAG_RAM_RE_START_ADDR FLAG_RAM_DRIVER_ADDR
#endif
#define RAM_TOTAL_SIZES  0x00008000UL  //32k

//#define FlagReadHandle(addr,size,buf)     Boot_MemCpy((u8 *)buf,(const u8 *)addr,size)
//#define FlagWriteHandle(addr,size,buf)      Boot_MemCpy((u8 *)addr,(const u8 *)buf,size)
#define ResetMCUHandle()                SystemSoftwareReset()
//#define Boot_ComControlHandle()           0
//#define Boot_CheckProgramming()                 0



//#define BOOT_APP_VALID_DEBUG_EN  1  //VALID FLAG 標志，后一個，開啟調試


#endif   /*  __BOOT_APP_CFG_H__  */







