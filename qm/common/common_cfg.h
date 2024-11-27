/** ##########################################################################
**     Filename  : common_memory.h
**     Project   : general
**     Module    : BSP
**     Processor : general
**     Compiler  : general
**     Date/Time : 2020/9/14
**     Abstract  :
**
**     Note	     :
**
**
**     (c) Copyright dmdz Co.,Ltd
**  --------------------------------------------------------------------------
**               R E V I S I O N   H I S T O R Y
**  --------------------------------------------------------------------------
**  Date       Ver   Author             Description

**  ---------  ----  -----------------  --------------------------------------
** #########################################################################*/
#ifndef __COMMON_CFG_H__
#define __COMMON_CFG_H__

/*****************************************************************************
* I N C L U D E   F I L E S
*****************************************************************************/
#include "common_types.h"

#define FUNCTION_OPEN                   1U
#define FUNCTION_CLOSE                  0U

/*实际产品需要开启的功能--FUNCTION_OPEN*/
//#define WATCHDOG_FUNCTION_ON            FUNCTION_OPEN  //没用的宏
#define MCU_GOTO_SLEEP_FUNCTION_ON  FUNCTION_CLOSE

//#define RTC_WAKEUP_ENABLE  1   //间隙唤醒，

#ifdef __ICCARM__

#if 0
#define MEMORY_MAP_ROM_FOR_ASIL  #pragma location = ".ROM_ASIL"  // #pragma location = ".RAM_ASIL"
#define MEMORY_MAP_RAM_FOR_ASIL  #pragma location = ".RAM_ASIL"
#else

#define MEMORY_MAP_ROM_FOR_ASIL  __attribute__((section(".ROM_ASIL")))  // #pragma location = ".RAM_ASIL"
#define MEMORY_MAP_ROM_DATA_FOR_ASIL  __attribute__((section(".ROM_DATA_ASIL")))  // #pragma location = ".ROM_DATA_ASIL"
#define MEMORY_MAP_RAM_FOR_ASIL  __attribute__((section(".RAM_ASIL")))  // #pragma location = ".RAM_ASIL"
#define MEMORY_MAP_ROM_DATA_FOR_UDS  __attribute__((section(".ROM_DATA_UDS")))  // #pragma location = ".ROM_DATA_UDS"
#define MEMORY_MAP_ROM_APP_RES  __attribute__((section(".ROM_APP_RES")))  // #pragma location = ".ROM_APP_VALID"
#define MEMORY_MAP_ROM_APP_VALID  __attribute__((section(".ROM_APP_VALID")))  // #pragma location = ".ROM_APP_VALID1"

#define ENABLE_STATIC_FOR_ASIL static
#define MEMORY_MAP_RAM_FLAG_NOINIT  __attribute__((section(".RAM_FLAG_NOLINT")))  // #pragma location = ".RAM_ASIL"
#define MEMORY_MAP_RAM_FLAG_PROG  __attribute__((section(".RAM_FLAG_PROG")))  // #pragma location = ".RAM_FLAG_PROG"
#define MEMORY_MAP_RAM_EEPROM  __attribute__((section(".RAM_EEPROM")))  // #pragma location = ".RAM_EEPROM"
#define MEMORY_MAP_RAM_WDOG  __attribute__((section(".RAM_WDOG")))  // #pragma location = ".RAM_WDOG"

#define MEMORY_MAP_FLAG_NO_OPTIME __root  //不被優化

#endif

//#define MEMORY_MAP_CONTINUS 1   /**/
#define LIN_MEMORY_MAP_ENABLE 1  /**/

#else

#define MEMORY_MAP_ROM_FOR_ASIL  __attribute__((section(".ROM_ASIL")))
#define MEMORY_MAP_ROM_DATA_FOR_ASIL  __attribute__((section(".ROM_DATA_ASIL")))  // #pragma location = ".RAM_ASIL"
#define MEMORY_MAP_RAM_FOR_ASIL  __attribute__((section(".RAM_ASIL")))
#define MEMORY_MAP_ROM_DATA_FOR_UDS  __attribute__((section(".ROM_DATA_UDS")))  // #pragma location = ".ROM_DATA_UDS"
#define MEMORY_MAP_ROM_APP_RES  __attribute__((section(".ROM_APP_RES")))  // #pragma location = ".ROM_APP_VALID"
#define MEMORY_MAP_ROM_APP_VALID  __attribute__((section(".ROM_APP_VALID")))  // #pragma location = ".ROM_APP_VALID1"

#define ENABLE_STATIC_FOR_ASIL //注意GCC不能有 static
#define MEMORY_MAP_RAM_FLAG_NOINIT  __attribute__((section(".RAM_FLAG_NOLINT")))  // #pragma location = ".RAM_ASIL"
#define MEMORY_MAP_RAM_FLAG_PROG  __attribute__((section(".RAM_FLAG_PROG")))  // #pragma location = ".RAM_FLAG_PROG"
#define MEMORY_MAP_RAM_EEPROM  __attribute__((section(".RAM_EEPROM")))  // #pragma location = ".RAM_EEPROM"
#define MEMORY_MAP_RAM_WDOG  __attribute__((section(".RAM_WDOG")))  // #pragma location = ".RAM_WDOG"

#define MEMORY_MAP_FLAG_NO_OPTIME __attribute__((used))  //不被優化


#endif




#define SystemSoftwareReset  NVIC_SystemReset



//主任务的时间
#define APP_TASK_1MS   1
#define APP_TASK_2MS   2
#define APP_TASK_5MS   5
#define APP_TASK_10MS   10
#define APP_TASK_100MS   100

#define APP_TASK_TIME_SLICE   //时间片

//#define KEY_DIGITAL_INIT_INVALID  1  //数字按键，上电无效

//项目相关的宏


//#define LIN_ENABLE 

//#define KEY_STICK_ENABLE  1  //卡滞功能开始
//#define KEY_DIGITAL_INIT_INVALID  1  //数字按键，上电无效
//#define RAM_OPTIMIZE_EN  1      // optimize  ram開啟優化-- 目的是為了把程序，放在zvl32上跑；

//#define ASIL_EN  1  //功能安全相關
//#define BOOTLOADER_EN 1  //是否开启了 bootloader
#ifdef ASIL_EN
//#define ASIL_MOTE  //更多的功能安全
#endif

//TO MODIFY  要最後修改
//
//
//
#endif/*__COMMON_CFG_H__*/

/* [] END OF FILE */