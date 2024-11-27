/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the configuration source file of the runtime
 *              environment module.
 *
 *  \file       bl_rte_cfg.c
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
#include "bl_rte.h"
#include "bl_rte_cfg.h"
#include "bl_app.h"
#include "bl_booting.h"
#include "bl_watchdog.h"
#include "bl_system.h"
#include "bl_timer.h"
#include "bl_can_if.h"
#include "bl_process_driver.h"
#include "bl_adpt_uds_platform.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 305,1257,1531,1533,3211,3406 EOF*/
/*
 * MISRA-C:2004 11.3(305):Cast a pointer to an integral type.
 * The feed interface of the watchdog module must be run in the RAM.
 *
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1531:The object is referenced in only one translation unit.
 * g_SecurityDelayInfo is a configurations.
 *
 * 1533:The object is only referenced by one function.
 * The objects are a configurations.
 *
 * 3211:The global identifier is defined but not used.
 * The identifier are used in the bl_rte.c file.
 *
 * MISRA-C:2004 8.5(3406):Object with external linkage has been defined in a
 * header file.
 * The g_DownSegment is not defined in a header file.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
STATIC const bl_ResetProcess_t
gs_ResetProcess[RTE_NUMBER_OF_BOOTING_PROCESSOR] = {
    //flag  = 01                    // *Process = &Adpt_ReprogramValidProcess
    {
        BOOTING_FLAG_REPROGRAM_VALID, RTE_REPROGRAM_PROCESS
    },
    #if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
    {
        //flag  = 02                   // *Process = &Adpt_AppValidProcess
        BOOTING_FLAG_APPLICATION_VALID, RTE_STAYINBOOT_PROCESS
    },
    #endif
};

STATIC const bl_WatchdogOps_t gs_WatchdogOps = {
    RTE_WATCHDOG_OPS_INIT,  //*Init  Wdg_Init
    #if ((RTE_FUN_WATCHDOG == BL_FUN_ON) && (RTE_FUN_COPY_WDG_TO_RAM == BL_FUN_ON))  //條件成立
    (bl_FeedWgdCallback_t)RTE_WATCHDOG_FEED_INTERFACE_ADDR,  // 1160；在 Wdg_Init時把 Wdg_Feed 載入 /*PRQA S 0305*/
    #else
    RTE_WATCHDOG_OPS_FEED,
    #endif
    RTE_WATCHDOG_OPS_RESET,  //*FastReset  Wdg_FastReset
    RTE_WATCHDOG_OPS_DEINIT  //*Deinit   Wdg_Deinit
};

#if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
STATIC const bl_SystemSleepOps_t gs_SystemSleepOps = {
    RTE_SYSTEM_OPS_ISSLEEP,  // *IsSleep BL_NULL_PTR
    RTE_SYSTEM_OPS_SLEEPCALLBACK,  // *SleepCallback  &Rte_Dummy
    RTE_SYSTEM_OPS_SYSTEMSLEEP,   // *SystemSleep &Sys_Sleep
    RTE_SYSTEM_OPS_WAKEUPCALLBACK  //  *WakeupCallback &Wdg_FastReset
};
#endif

STATIC const bl_TimerOps_t gs_TimerOps = {
    RTE_TIMER_OPS_INIT,   // *Init Tim_Init
    RTE_TIMER_OPS_DEINIT,  // *Deinit  Tim_Deinit
    RTE_TIMER_OPS_TIMEOUT  // *IsTimeout  Tim_IsTimeout
};

STATIC const bl_ComIfOps_t gs_ComIfOps = {
    RTE_COMIF_OPS_INIT,         // *Init      Canif_Init
    RTE_COMIF_OPS_DEINIT,	    // *Deinit    Canif_DeInit
    RTE_COMIF_OPS_READ,		    // *Read      Canif_TryToRead
    RTE_COMIF_OPS_WRITE,	    // *Write     Canif_Write
    RTE_COMIF_OPS_ISSUCCWRITE   // *IsWritten Canif_IsWritten
};


/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/
// RTE_NUMBER_OF_DOWN_SEGMENT =30
bl_DownSegment_t g_DownSegment[RTE_NUMBER_OF_DOWN_SEGMENT];  // 下載時的 seg 情況， 相當於每個seg 的地址和長度，列出來

const bl_ResetProcessList_t g_ResetProcessList = {
    RTE_NUMBER_OF_BOOTING_PROCESSOR,  // 2
    gs_ResetProcess
};

#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)  //條件成立
const bl_Buffer_t g_StayInBootValue[RTE_STAY_IN_BOOT_MSG_SIZE] = {
    RTE_STAY_IN_BOOT_VALUE   //(0x04u),(0x31u),(0x01u),(0xF5u),(0x18u)
};
#endif

/*
#define RTE_NUMBER_OF_EVENT             (4)
#define RTE_EVENT_TYPE_RECV_REQUEST     (0x0000u)
#define RTE_EVENT_TYPE_MEMOPS_TIMEOUT   (0x0001u)
#define RTE_EVENT_TYPE_S3TIME_TIMEOUT   (0x0002u)
#define RTE_EVENT_TYPE_SYSTEM_SLEEP     (0x0003u)
*/
const bl_EventCallback_t g_EventProcessList[RTE_NUMBER_OF_EVENT] = { // 被重命名 成 RTE_DO_EVENT_PROCESSER
    &Rte_ResetTimer,  // 重置  gs_StayInBootResetCount=初始
    &Proc_Timeout,    // Adpt_UdsTimeOutCallBack  //負響應 78
    RTE_WATCHDOG_OPS_RESET,  //Wdg_FastReset
    &Proc_Sleep,
};

const bl_RteInfo_t g_RteInfo = {
    RTE_SYSTEM_VERSION,	   // version = 0x00050100UL
    &gs_WatchdogOps,       // *wdgOps 指向看門狗 實體
    #if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
    &gs_SystemSleepOps,    // *sleepOps  指向休眠相關實體
    #else
    BL_NULL_PTR, /*RTE_FUN_SYSTEM_SLEEP is OFF*/
    #endif
    &gs_TimerOps,          // *timerOps 指向定時器 實體
    &gs_ComIfOps,          // *comOps 指向 can 實體
    RTE_WATCHDOG_OPS_RESET // *SystemReset == Wdg_FastReset
};
/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

