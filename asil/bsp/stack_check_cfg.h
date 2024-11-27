/**
  ******************************************************************************
  * @file    stack_check_cfg.h
  * @author  Mingyea Asamiya
  * @version V1.0.0
  * @date    2017-01-19
  * @brief
  * @function List:
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  * @History:
  * @Author:
  * @Data:
  * @Version:
  */





#ifndef __STACK_CHECK_CFG_H__
#define __STACK_CHECK_CFG_H__


#include "common_types.h"
#include "Cpu.h"
#include "common_cfg.h"



/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

#define STACK_CHECK_DEBUG_EN  1

#define STACK_CHECK_TIME_UNIT  100  //放在100ms 周期裡面
#define STACK_CHECK_TASK_TIME  (200/STACK_CHECK_TIME_UNIT)

//预留16字节，检测堆栈是否溢出
//#define  RAM_STACK_CHECK_ADDR  (0x20001ec0 -16u) //(m_data_end-__size_cstack__+1u - 16u)

#define RAM_STACK_CHECK_MAX_LEN  16u
#define RAM_STACK_CHECK_VALUE_BUF0  0x55u
#define RAM_STACK_CHECK_VALUE_BUF1  0x5eu
#define RAM_STACK_CHECK_VALUE_BUF2  0x5cu
#define RAM_STACK_CHECK_VALUE_BUF3  0x4eu

//extern const MEMORY_MAP_ROM_DATA_FOR_ASIL u8 g_stack_check_fix_value[RAM_STACK_CHECK_MAX_LEN] ;


#endif   /*  __BOOT_APP_CFG_H__  */







