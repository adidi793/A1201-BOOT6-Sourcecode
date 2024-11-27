/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-17 09:14:33
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-07-11 14:30:03
 * @FilePath: \A12 BOOT1\asil\bsp\Ls_Nm_Cfg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
#ifndef _LS_NM_CFG_H_
#define _LS_NM_CFG_H_

#include "common_types.h"
//#include "os_types.h"
//#include "Std_Types.h"
#include "can_cfg.h"
#include "gpio.h"


#ifdef  CAN_AUTOSAR_EN
#include "Can_Type.h"
#include "ComStack_Types.h"
#endif

//临时
#ifdef  CAN_AUTOSAR_EN
#else
typedef u16  PduIdType;
typedef u16  Can_IdType;
typedef u16  PduInfoType;
#endif


#define  LS_NM_MAINFUNCTION_PERIOD 	(1u)

#define NM_MAIN_BUSOFF_FAST_H  (50u/LS_NM_MAINFUNCTION_PERIOD)
#define NM_MAIN_BUSOFF_FAST_L  (10u/LS_NM_MAINFUNCTION_PERIOD)
#define NM_MAIN_BUSOFF_FAST_COUNT  (5u)

#define NM_MAIN_BUSOFF_SLOW_H  (200/LS_NM_MAINFUNCTION_PERIOD)
#define NM_MAIN_BUSOFF_SLOW_L  (10u/LS_NM_MAINFUNCTION_PERIOD)


#if 0

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
#define  NM_MAINFUNCTION_PERIOD 	(5u)

#define  NM_MASTER_CAN_ID			(0x650u)
#define  NM_LOCAL_CAN_ID			(0x651u)

#define  NM_BUSOFF_ERROR_COUNT		(5u)

#define LS_NM_DEBUF_EN 1

#endif


#define LS_NM_TEST_IO_ON()  TEST1_ON()
#define LS_NM_TEST_IO_OFF()  TEST1_OFF()


#endif