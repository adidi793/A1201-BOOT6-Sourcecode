/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-09-14 11:59:35
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-11-12 16:34:56
 * @FilePath: \A1201 BOOT5\asil\middle\autosar_bl\inc\bl_timer.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the timer module.
 *
 *  \file       bl_timer.h
 *  \ingroup    driver_module
 *  \author     
 *
 *  \version    1.0.0
 *  \date       14/05/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | -------------
 *      01.00.00 | 14/05/2014 | ning.chen        | N/A | BootDrv010001
 *****************************************************************************/
#ifndef _BL_TIMER_H_
#define _BL_TIMER_H_
#include "bl_typedefs.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3205,3210,3447,3448 EOF*/
/*
 * 3205:The identifier is not used and could be removed.
 * The bl_Adapter_t is used by the specifically adapter such as UDS platform in
 * the bl_adpt_uds_platform.c
 *
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_security_cfg.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 * MISRA-C:2004 5.3(3448):Declaration of typedef is not in a header file.
 * The bl_SecurityDelay_t is declared in the header file.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
#define TIMER_SYSTEM_PERIOD     (1) /*ms*/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
extern void Tim_Init(void);
extern bl_Return_t Tim_IsTimeout(void);
extern void Tim_Deinit(void);
extern void Tim_tick_count(void);
extern bl_u32_t Tim_seedtick_Get(void);
#endif
