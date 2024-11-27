/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-08-07 15:45:14
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-08-27 09:25:26
 * @FilePath: \A1201 BOOT4\asil\middle\autosar_bl\cfg\bl_dcm_funcfg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file to configurate the DCM module.
 *
 *  \file       bl_dcm_funcfg.h
 *  \ingroup    communication_protocol_stack_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      05.00.00 | 19/06/2013 | heli.zhang       | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_DCM_FUNCFG_H_
#define _BL_DCM_FUNCFG_H_

#include "nvm.h"

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
#define DCM_FUN_SECURITY_ACCESS     BL_FUN_ON   //進入編程會話時，是否要用安全算法來解鎖
/*P2 and S3 timer definition.*/
#define DCM_P2STARTIMER_DIV         (10u)  /*ISO15765-3*/
#define DCM_CALL_CYCLE              RTE_SYSTEM_SCHEDULE_PERIOD
#define DCM_P2TIMEVALUE             (50u)
#define DCM_P2STARTIMEVALUE         (2000U) //(2000u/DCM_CALL_CYCLE) //(5000u/DCM_CALL_CYCLE)
#define DCM_S3TIMERVALUE            (5000U) ///(5000u/DCM_CALL_CYCLE)


#define DCM_BUFFER_SIZE             (FLASH_P_BLOCK_SECTOR_SIZES+0x12E)   //多幀時 的緩存總數--因為 頁大小有0x800 ;原來fe-5ha的值是0x410,s32k144 0x1000
#define DCM_TX_HANDLE               (0)

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

