/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file to configurate the data of the runtime
 *              environment module.
 *
 *  \file       bl_rte_cfg.h
 *  \ingroup    rte_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 17/03/2010 | hewei.zhang      | N/A | Boot010001
 *      01.01.00 | 20/04/2010 | hewei.zhang      | N/A | Boot010002
 *      01.03.00 | 28/05/2010 | hewei.zhang      | N/A | Boot010004
 *      01.04.00 | 26/06/2010 | hewei.zhang      | N/A | Boot010005
 *      01.08.00 | 24/08/2010 | hewei.zhang      | N/A | Boot010009
 *      01.11.00 | 07/12/2010 | hewei.zhang      | N/A | Boot010012
 *      02.00.00 | 21/02/2011 | mingqing.tang    | N/A | Boot020001
 *      02.02.00 | 15/03/2011 | mingqing.tang    | N/A | Boot020003
 *      02.07.00 | 14/06/2011 | mingqing.tang    | N/A | Boot020008
 *      03.00.00 | 12/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.03.00 | 17/10/2011 | mingqing.tang    | N/A | Boot030004
 *      03.04.00 | 24/10/2011 | mingqing.tang    | N/A | Boot030005
 *      03.10.00 | 03/01/2012 | mingqing.tang    | N/A | Boot030011
 *      03.12.00 | 18/01/2012 | mingqing.tang    | N/A | Boot030013
 *      03.18.00 | 29/05/2012 | mingqing.tang    | N/A | Boot030019
 *      05.00.00 | 19/06/2013 | mingqing.tang    | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_RTE_CFG_H_
#define _BL_RTE_CFG_H_
#include "bl_rte.h"
#include "can_cfg.h"

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
/** */
#define RTE_NUMBER_OF_DOWN_SEGMENT      (30u)    // boot時的 seg 總數，不能超過
/** */
#define RTE_NUMBER_OF_BOOTING_PROCESSOR (2u)   // boot 過程，，0是 prog 1 是 app valid

/** If the system sleep is enabled, This macro indicates the sleep time of
 *  the system sleep. The unit is ms.*/
#define RTE_SYSTEM_SLEEP_TIME           (80000UL)  // 休眠時間；

#if defined CAN_ID_0_FD_EN  || defined CAN_ID_1_FD_EN
#define RTE_STAY_IN_BOOT_MSG_SIZE       (6u)       //收到5個；至少；本項目中的值是 (0x04u),(0x31u),(0x01u),(0xF5u),(0x18u)
#define RTE_STAY_IN_BOOT_TIME           (80)	//在boot保持時，讀取的次數；
#define RTE_STAY_IN_BOOT_HANDLE         (0)		//目前没用到
#define RTE_STAY_IN_BOOT_RESET_TIME     (20000UL)   //20s 復位
#define RTE_STAY_IN_BOOT_VALUE          (0x00u),(0x04u),(0x31u),(0x01u),(0xF5u),(0x18u)   // 給 g_StayInBootValue
#else
#define RTE_STAY_IN_BOOT_MSG_SIZE       (5u)       //收到5個；至少；本項目中的值是 (0x04u),(0x31u),(0x01u),(0xF5u),(0x18u)
#define RTE_STAY_IN_BOOT_TIME           (80)	//在boot保持時，读取接收数据的次数，若不相等的次数到达设置值就转入APP
#define RTE_STAY_IN_BOOT_HANDLE         (0)		//目前没用到
#define RTE_STAY_IN_BOOT_RESET_TIME     (20000UL)   //20s 復位
#define RTE_STAY_IN_BOOT_VALUE          (0x04u),(0x31u),(0x01u),(0xF5u),(0x18u)   // 給 g_StayInBootValue
#endif

#define RTE_WATCHDOG_OPS_INIT           (&Wdg_Init)
#define RTE_WATCHDOG_OPS_FEED           (&Wdg_Feed)
#define RTE_WATCHDOG_OPS_RESET          (&Wdg_FastReset)
#define RTE_WATCHDOG_OPS_DEINIT         (&Wdg_Deinit)
#define RTE_SYSTEM_OPS_ISSLEEP          BL_NULL_PTR
#define RTE_SYSTEM_OPS_SLEEPCALLBACK    (&Rte_Dummy)
#define RTE_SYSTEM_OPS_SYSTEMSLEEP      (&Sys_Sleep)
#define RTE_SYSTEM_OPS_WAKEUPCALLBACK   (&Wdg_FastReset)/*Reset system*/
#define RTE_TIMER_OPS_INIT              (&Tim_Init)
#define RTE_TIMER_OPS_DEINIT            (&Tim_Deinit)
#define RTE_TIMER_OPS_TIMEOUT           (&Tim_IsTimeout)
#define RTE_COMIF_OPS_INIT              (&Canif_Init)
#define RTE_COMIF_OPS_DEINIT            (&Canif_DeInit)
#define RTE_COMIF_OPS_READ              (&Canif_TryToRead)
#define RTE_COMIF_OPS_WRITE             (&Canif_Write)
#define RTE_COMIF_OPS_ISSUCCWRITE       (&Canif_IsWritten)
#define RTE_REPROGRAM_PROCESS           (&Adpt_ReprogramValidProcess)
#define RTE_RESET_PROCESS               (&Adpt_SystemResetProcess)
#define RTE_STAYINBOOT_PROCESS          (&Adpt_AppValidProcess)
/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
extern const bl_ResetProcessList_t g_ResetProcessList;
#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
extern const bl_Buffer_t g_StayInBootValue[RTE_STAY_IN_BOOT_MSG_SIZE];
#endif
extern bl_DownSegment_t g_DownSegment[RTE_NUMBER_OF_DOWN_SEGMENT];

extern const bl_EventCallback_t g_EventProcessList[RTE_NUMBER_OF_EVENT];

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

