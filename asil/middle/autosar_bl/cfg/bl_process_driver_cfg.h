/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file to configurate the process driver module.
 *
 *  \file       bl_process_driver_cfg.h
 *  \ingroup    process_driver_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      04.00.00 | 12/03/2012 | mingqing.tang    | N/A | Boot040001
 *      04.03.00 | 22/03/2012 | mingqing.tang    | N/A | Boot040004
 *      04.07.00 | 18/04/2012 | mingqing.tang    | N/A | Boot040008
 *      05.00.00 | 19/06/2013 | heli.zhang       | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_PROCESS_DRIVER_CFG_H_
#define _BL_PROCESS_DRIVER_CFG_H_
#include "bl_process_driver.h"

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
#define PROC_NUMBER_OF_DRIVER           (0x01u)
#define PROC_DEFAULT_DRIVER             (0x00u)

#define PROC_DRIVER_0_TYPE              PROC_DRIVER_TYPE_PASSIVE  //好像目前没有意義
#define PROC_DRIVER_0_INDEX             (0x00u)
#define PROC_DRIVER_0_ADAPTER           (&g_UdsPlatformAdapter)
#define PROC_DRIVER_0_SCHEDULER         (&Proc_PassiveSchedule)

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
/** \brief The process driver list, including all process driver info.*/
extern const bl_ProcessDriverList_t g_ProcessDriverList;

#endif

