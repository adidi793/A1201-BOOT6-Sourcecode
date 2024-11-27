/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the runtime environment module.
 *
 *  \file       bl_rte.c
 *  \ingroup    rte_module
 *  \author
 *
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
#include "bl_logical_block.h"
#include "bl_memory.h"
#include "bl_system.h"
#include "bl_booting.h"
#include "bl_app.h"
#include "bl_process_driver.h"
#include "bl_security.h"
#include "bl_rte_cfg.h"

#include "bl_timer.h"
#include "svd.h"
#include "svd_interrupt.h"
//#include "system_ac780x.h"



#ifdef FALSH_DRIVER_INIT_EN
#include "common_types.h"
#include "bl_data.h"
#endif

//#define DCM_DEBUG_EN 1

#ifdef DCM_DEBUG_EN
typedef struct {
    u8 id;
    u32 value;
} bl_dcm_debug_s;
STATIC bl_dcm_debug_s g_bl_dcm_debug;
#endif

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 506,602,781,1252,1257,1503,1532,2100,2103,3112,3212,3227,3406 EOF*/
/*PRQA S 3453 EOF*/
/*
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * MISRA-C:2004 5.6(781):The identifier is used as a structure/union member.
 * The member of the structure is used through the structure object.
 *
 * 1252:suffixed integer constant implicitly converted to different integer.
 *
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * MISRA-C:2004 14.1(1503):The function is defined but is not used within this
 * project.
 * these functions will be used When correlative macro is enable.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 2100:Integral promotion : unsigned char promoted to signed int.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3212:This explicit cast is redundant and could be removed.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * MISRA-C:2004 8.5(3406):Object with external linkage has been defined in a
 * header file.
 * The g_DownContext is not defined in a header file.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * These macros have no side-effect.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Function Macro
 *****************************************************************************/
#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_OFF)
#if (RTE_FUN_STAY_IN_BOOT_RESET == BL_FUN_ON)
#error "The stay in boot function is closed!"
#endif
#endif

#if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_OFF)
#if (RTE_FUN_ADDITIONAL_SLEEP_CONDITION == BL_FUN_ON)
#error "The system sleep function is closed!"
#endif
#endif

#if (RTE_FUN_FILE_HEADER_PROCESS == BL_FUN_OFF)
#if ((RTE_FUN_INTEGRALITY_VALUE_IN_HEADER == BL_FUN_ON) \
|| (RTE_FUN_COMPATIBILITY_VALUE_IN_HEADER == BL_FUN_ON))
#error "The file header process function is closed!"
#endif
#endif

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
#define RTE_RESET_SYSTEM_SLEEP_COUNT()  (gs_SystemSleepCount =\
        (bl_u32_t)(RTE_SYSTEM_SLEEP_TIME\
            / RTE_SYSTEM_SCHEDULE_PERIOD))  //80s

#define RTE_SUB_SYSTEM_SLEEP_COUNT()    (gs_SystemSleepCount -= 1UL)

#define RTE_ADD_SYSTEM_SLEEP_COUNT()    (gs_SystemSleepCount += 1UL)

#define RTE_IS_SYSTEM_SLEEP()           (gs_SystemSleepCount == 0UL)

#define RTE_RESET_SIB_COUNT()           (gs_StayInBootResetCount =\
        (bl_u16_t)(RTE_STAY_IN_BOOT_RESET_TIME\
            / RTE_SYSTEM_SCHEDULE_PERIOD))  //20s

#define RTE_SUB_SIB_COUNT()             (gs_StayInBootResetCount -= 1u)

#define RTE_IS_SIB_RESET()              (gs_StayInBootResetCount == 0u)

#define RTE_DISABLE_SIB_RESET()         (gs_StayInBootResetFlag = 0u)

#define RTE_ENABLE_SIB_RESET()          (gs_StayInBootResetFlag = 1u)

#define RTE_IS_SIB_RESET_ENABLE()       (gs_StayInBootResetFlag == 1u)

#define RTE_DO_EVENT_PROCESSER(event)   g_EventProcessList[event]()

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
static void _Rte_Deinit(void);
#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
static void _Rte_InitStayInBoot(void);

static bl_Return_t _Rte_VerifyStayInBoot(bl_ComIfHandle_t handle,
       bl_BufferSize_t size,
       const bl_Buffer_t *buffer);
#endif

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
#if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
STATIC bl_u32_t gs_SystemSleepCount;
#endif

#if (RTE_FUN_STAY_IN_BOOT_RESET == BL_FUN_ON)
STATIC bl_u16_t gs_StayInBootResetCount;
STATIC bl_u8_t gs_StayInBootResetFlag;
#endif

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/
bl_DownContext_t g_DownContext;
/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details In this interface, the system clock driver is initialized first.
 *
 *  \return None.
 *
 *  \warning If the initialization is fails, this interface will wait until
 *           the power on again.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/

void Rte_PreInit(void) {



    ///#if ((RTE_FUN_WATCHDOG == BL_FUN_ON) && (RTE_FUN_PRE_INIT_WDG == BL_FUN_ON))  //RTE_FUN_PRE_INIT_WDG ==OFF
    ///g_RteInfo.wdgOps->Init();  //看門狗初始化  --這時不初始化  Wdg_Init
    
    ///#endif
    //////////////////////////////
    FL_IWDT_InitTypeDef IWDT_InitStruct;
    IWDT_InitStruct.iwdtWindows    = 0;
    IWDT_InitStruct.overflowPeriod = FL_IWDT_PERIOD_4000MS; //原FL_IWDT_PERIOD_500MS，
    FL_IWDT_Init(IWDT, &IWDT_InitStruct);
    FL_IWDT_EnableFreezeWhileSleep(IWDT);
    /////////////////////////////////////////watchdog init
    FL_Init();                       ///////////////systick init
    SVD_Init(SVD_MONTIOR_VDD, FL_SVD_WARNING_THRESHOLD_GROUP11, FL_SVD_REFERENCE_1P0V);
    while (false == SVD_Result_Confirmed(SVD_HIGHER_THRESHOLD, 2000U/*us*/));      //////svd fun
    FL_RMU_BOR_SetThreshold(RMU, FL_RMU_BOR_THRESHOLD_2P40V);
    FL_RMU_BOR_Enable(RMU);                                                         //////bor fun
    Sys_InitClock();
    
    
    #if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
    _Rte_InitStayInBoot();           // gs_StayInBootResetCount =20s  //gs_StayInBootResetFlag
    (void)g_RteInfo.comOps->Init();  //can 初始化  Canif_Init
    g_RteInfo.timerOps->Init();	 //定時器初始化  Tim_Init，用于主任务超时
    #endif
    
    (void)Lbm_Init();  // logic block 初始化 包含 memeory data等等
    __enable_irq();  //可以再移上面一點
    return ;
}

/**************************************************************************//**
 *
 *  \details post initialize the Bootloader.
 *
 *  \return If this module is initialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - initialize successfully.
 *  \retval BL_ERR_NOT_OK   - initialize unsuccessfully.

 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Rte_PostInit(void) {
    bl_Return_t ret;
    
    /*Initialize the download context.*/
    g_DownContext.segIndex = 0;
    g_DownContext.lbId = BL_ERROR_ID_16BIT;
    g_DownContext.segment.number = RTE_NUMBER_OF_DOWN_SEGMENT;  // 最多30
    g_DownContext.segment.segList = g_DownSegment;  // 每個seg的起始地址 和長度
    
    #if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
    RTE_RESET_SYSTEM_SLEEP_COUNT();   // gs_SystemSleepCount=80s
    #endif
    
    #if ((RTE_FUN_WATCHDOG == BL_FUN_ON) && (RTE_FUN_PRE_INIT_WDG == BL_FUN_OFF))
    ////	g_RteInfo.wdgOps->Init();  //看門狗初始化，真正
    #endif
    
    #if (RTE_FUN_STAY_IN_BOOT == BL_FUN_OFF)
    
    g_RteInfo.timerOps->Init();  //定時器又初始化？？
    #endif
    
    Proc_Init();  // 調用了 Adpt_UdsInit
    
    ret = bl_Mem_Init();	//memory 初始化，但其實 Flsif_Init 并没有真正初始化; 要在後面 34 36服務，把flash drv都下載完才可以，也可以手動。 FALSH_DRIVER_INIT_EN 有測試
    if (BL_ERR_OK == ret) {
        Secm_Init();  // 安全算法 Acc_UdsDemoInit Bzip2_Init 等
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details find and call the reset process function.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *  在main初始化時
 *****************************************************************************/
void Rte_ProcessReset(bl_BootingFlag_t flag) {
    bl_u16_t i;
    bl_u16_t num;
    const bl_ResetProcess_t *list;
    
    num = g_ResetProcessList.number;		// RTE_NUMBER_OF_BOOTING_PROCESSOR
    list = g_ResetProcessList.resetList;	// gs_ResetProcess
    
    for (i = 0; i < num; i++) {
        if (flag ==
            list[i].flag)   // 先是 BOOTING_FLAG_REPROGRAM_VALID == flag 則調用 Adpt_ReprogramValidProcess  一般會是這種情況; 也有可能都不滿足
            // 如果是 BOOTING_FLAG_APPLICATION_VALID == flag 則調用 Adpt_AppValidProcess   基本上都跳到 APP了
        {
            list[i].Process();
            break;
        }
    }
    #if (RTE_FUN_STAY_IN_BOOT_RESET == BL_FUN_ON)
    //如果 app flag 失敗，且不是編程模式
    if (flag == 0u) {
        RTE_ENABLE_SIB_RESET();
    }
    #endif
    return ;
}

/**************************************************************************//**
 *
 *  \details periodic schedule the callback.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Rte_PeriodicSchedule(bl_PeriodicCallback_t callback) {
    bl_Return_t ret;
    
    ret = g_RteInfo.timerOps->IsTimeout(); /* ATIM中断定时1ms， 超时后返回BL_ERR_OK */
    if (BL_ERR_OK == ret) {
        BL_DEBUG_ASSERT_NO_RET(callback != BL_NULL_PTR);
        
        callback();    //  Adpt_UdsPeriodicCallback
        /*
        	Adpt_DelayTimeCallback(&gs_UdsPrivateData.delay);	//
        	Dcm_TimerFunction();
        	Cantp_PeriodFunction();
        	Dcm_MainFunction();
         */
        
        #if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
        
        RTE_SUB_SYSTEM_SLEEP_COUNT();
        
        if (RTE_IS_SYSTEM_SLEEP()) {
        
            #if (RTE_FUN_ADDITIONAL_SLEEP_CONDITION == BL_FUN_ON)
            if (g_RteInfo.sleepOps->IsSleep())
            #endif
            {
                RTE_RESET_SYSTEM_SLEEP_COUNT();
                
                g_RteInfo.sleepOps->SleepCallback();
                RTE_DO_EVENT_PROCESSER(RTE_EVENT_TYPE_SYSTEM_SLEEP);
                g_RteInfo.sleepOps->SystemSleep();
                g_RteInfo.sleepOps->WakeupCallback();
            }
            #if (RTE_FUN_ADDITIONAL_SLEEP_CONDITION == BL_FUN_ON)
            else {
                RTE_ADD_SYSTEM_SLEEP_COUNT();
            }
            #endif
            
        }
        
        #endif
        
        #if (RTE_FUN_STAY_IN_BOOT_RESET == BL_FUN_ON)
        if (RTE_IS_SIB_RESET_ENABLE()) {        //在Rte_ProcessReset函数中将gs_StayInBootResetFlag = 1
            RTE_SUB_SIB_COUNT();	            //gs_StayInBootResetCount-1
            if (RTE_IS_SIB_RESET()) {           //gs_StayInBootResetCount==0
                RTE_DISABLE_SIB_RESET();		//gs_StayInBootResetFlag=0
                g_RteInfo.SystemReset();	    //boot复位
            }
        }
        #endif
        
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details call the event process function.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Rte_ReportEvent(bl_u16_t type) {
    BL_DEBUG_ASSERT_NO_RET(type < RTE_NUMBER_OF_EVENT);
    
    RTE_DO_EVENT_PROCESSER(type);  // 寫入 g_EventProcessList
    /*
    #define RTE_NUMBER_OF_EVENT             (4)
    #define RTE_EVENT_TYPE_RECV_REQUEST     (0x0000u)  &Rte_ResetTimer,  // 重置  gs_StayInBootResetCount=初始
    #define RTE_EVENT_TYPE_MEMOPS_TIMEOUT   (0x0001u)  &Proc_Timeout,    // Adpt_UdsTimeOutCallBack  //負響應 78
    #define RTE_EVENT_TYPE_S3TIME_TIMEOUT   (0x0002u)  RTE_WATCHDOG_OPS_RESET,  //Wdg_FastReset
    #define RTE_EVENT_TYPE_SYSTEM_SLEEP     (0x0003u)   &Proc_Sleep,
    */
}

/**************************************************************************//**
 *
 *  \details Reset the count of the sleep timer and SIB reset timer.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Rte_ResetTimer(void) {
    #if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
    RTE_RESET_SYSTEM_SLEEP_COUNT();
    #endif
    
    #if (RTE_FUN_STAY_IN_BOOT_RESET == BL_FUN_ON)
    RTE_RESET_SIB_COUNT();
    #endif
    
    return ;
}
/**************************************************************************//**
 *
 *  \details Save the address and size into dct.
 *
 *  \return None.
 *
 *  \since V3.0.0
 *  34 服務時，請求下載 則把 seg的地址和長度 保存
 *****************************************************************************/
bl_Return_t Rte_SaveAddressInfo(bl_DownContext_t *dct,   //g_DownContext
    bl_Address_t address,
    bl_Size_t size) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t index;
    
    index = dct->segIndex;
    if (index < dct->segment.number) {
        dct->segment.segList[index].address = address;
        dct->segment.segList[index].size = size;
        
        dct->segIndex = index + 1u;
        
        ret = BL_ERR_OK;
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Clear the address in the dct.
 *
 *  \return None.
 *
 *  \since V3.0.0
 *
 *****************************************************************************/
void Rte_ClearAddressInfo(bl_DownContext_t *dct) {
    dct->segIndex = 0;
}

/**************************************************************************//**
 *
 *  \details get the download status in the dct.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_DownStatus_t Rte_GetDownStatus(const bl_DownContext_t *dct, bl_u16_t gId) {
    bl_DownStatus_t status = 0;
    
    if (gId < RTE_NUMBER_OF_DOWN_STATUS) {
        status = dct->status[gId];
    }
    
    return RTE_GET_DOWNSTATUS_VALUE(status);   // status & 0x3fffffff
}

/**************************************************************************//**
 *
 *  \details set the download status in the dct.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Rte_SetDownStatus(bl_DownContext_t *dct, bl_DownStatus_t mask) {
    bl_u16_t id;
    
    id = RTE_GET_DOWNSTATUS_ID(mask);    /*PRQA S 2985*/ //mask >> 30 再 與上 0xff
    
    dct->status[id] |= RTE_GET_DOWNSTATUS_VALUE(mask);    // (mask&0x3ffffffff)
    
    return ;
}

/**************************************************************************//**
 *
 *  \details set mutex download status in the dct.
 *
 *  \return None.
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
/* -----------------------------------------------------------------------A12
mask = 40000100
mutex 互斥
----------------------------------------------------------------------- */
void Rte_SetMutexDownStatus(bl_DownContext_t *dct,
    bl_DownStatus_t mask,
    bl_DownStatus_t mutex) {
    bl_DownStatus_t tmp;
    bl_u16_t id;
    
    tmp = RTE_GET_DOWNSTATUS_VALUE(mask);  //mask&3fffffff = 0x100
    if (((tmp & (tmp - 1UL)) == 0UL) && ((mutex & tmp) > 0UL)) {
        id = RTE_GET_DOWNSTATUS_ID(mask);   /*PRQA S 2985*/ //mask >> 30 再 與上 0xff
        dct->status[id] = (dct->status[id] & (~mutex)) | tmp;
    }
}

/**************************************************************************//**
 *
 *  \details clear the download status in the dct.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Rte_ClearDownStatus(bl_DownContext_t *dct, bl_DownStatus_t mask) {
    bl_u16_t id;
    
    id = RTE_GET_DOWNSTATUS_ID(mask);  /*PRQA S 2985*/ //mask >> 30 再 與上 0xff
    /* -----------------------------------------------------------------------A12
    RTE_GET_DOWNSTATUS_VALUE(~1C) = 3FFF FFE3
    RTE_GET_DOWNSTATUS_VALUE(~8700 0000) = 7FF FFFF
    ----------------------------------------------------------------------- */
    dct->status[id] &= RTE_GET_DOWNSTATUS_VALUE(~mask);  // ~mask & 0x3fff ffff
    
    return ;
}

/**************************************************************************//**
 *
 *  \details clear all download status in dct.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Rte_ClearAllDownStatus(bl_DownContext_t *dct) {
    bl_u16_t id;
    
    for (id = 0; id < RTE_NUMBER_OF_DOWN_STATUS; id++) {
        dct->status[id] = 0;
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details check whether download status is valid.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Rte_IsValidStatus(const bl_DownContext_t *dct,
    bl_DownStatus_t mask) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t id;
    bl_DownStatus_t value;
    
    id = RTE_GET_DOWNSTATUS_ID(mask);  /*PRQA S 2985*/ //mask >>30
    value = RTE_GET_DOWNSTATUS_VALUE(mask); //mask & 3fffffff
    #ifdef DCM_DEBUG_EN
    g_bl_dcm_debug.id = id;
    g_bl_dcm_debug.value = value;
    #endif
    if ((value & dct->status[id]) > 0UL) {
        ret = BL_ERR_OK;
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details check whether all download status are valid.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Rte_IsAllValidStatus(const bl_DownContext_t *dct,
    bl_DownStatus_t mask) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t id;
    bl_DownStatus_t value;
    
    id = RTE_GET_DOWNSTATUS_ID(mask); // id = (mask >> 30) & 0xff
    value = RTE_GET_DOWNSTATUS_VALUE(mask); //value = mask & 0x3FFFFFFF
    
    if ((value & dct->status[id]) == value) { //注意 status 一共才 4個  即 RTE_NUMBER_OF_DOWN_STATUS = 4
        ret = BL_ERR_OK;
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details check whether only one download status is valid.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Rte_IsOnlyValidStatus(const bl_DownContext_t *dct,
    bl_DownStatus_t mask) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t id;
    bl_DownStatus_t value;
    
    id = RTE_GET_DOWNSTATUS_ID(mask); /*PRQA S 2985*/
    value = RTE_GET_DOWNSTATUS_VALUE(mask) & dct->status[id];
    
    if ((value > 0UL) && ((value & (value - 1UL)) == 0UL)) {
        ret = BL_ERR_OK;
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Go to the application.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Rte_GotoApplication(void) {

    //Sys_RemapInterrupt();   // IVBR 重新指向 中斷向量，相當於NVIC
    
    Sys_ProtectBootloader();	//目前是空的
    
    _Rte_Deinit();	//看門狗 定時器 CAN sysclk 反初始化
    __disable_irq();
    Sys_RemapInterrupt();   // IVBR 重新指向 中斷向量，相當於NVIC
    Sys_GotoApplication();  //真正跳到app
}
/**************************************************************************//**
 *
 *  \details Do nothing.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Rte_Dummy(void) {
    return ;
}

/**************************************************************************//**
 *
 *  \details Deinitialize the RTE module.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
static void _Rte_Deinit(void) {
    #if (RTE_FUN_PRE_INIT_WDG == BL_FUN_ON)
    g_RteInfo.wdgOps->Deinit();  // Wdg_Deinit
    #endif
    
    #if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
    g_RteInfo.comOps->Deinit();   // Canif_DeInit FLEXCAN_DRV_Deinit
    
    g_RteInfo.timerOps->Deinit();  // Tim_Deinit
    #endif
    
    Sys_DeinitClock();
    
    return ;
}


#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details Check whether the stay in boot message is receviced.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Rte_StayInBoot(void) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_Return_t readRet = BL_ERR_NOT_OK;
    bl_Return_t isTimeout = BL_ERR_OK;
    bl_Buffer_t buf[RTE_STAY_IN_BOOT_MSG_SIZE];
    bl_ComIfData_t comData;
    bl_u32_t count = (bl_u32_t)(RTE_STAY_IN_BOOT_TIME
            / RTE_SYSTEM_SCHEDULE_PERIOD);
            
    comData.buffer = buf;
    comData.bufSize = RTE_STAY_IN_BOOT_MSG_SIZE;    //收到5個；至少；本項目中的值是 (0x04u),(0x31u),(0x01u),(0xF5u),(0x18u)
    do {
        readRet = g_RteInfo.comOps->Read(&comData);  // Canif_TryToRead
        if (BL_ERR_OK == readRet) {
            #if 1
            ret = _Rte_VerifyStayInBoot(comData.retHandle,  //handle buf id
                    comData.retSize,  //  實際長度
                    buf);  //內容
            #endif
        }
        
        //isTimeout = Tim_IsTimeout();  // Tim_IsTimeout
        isTimeout = g_RteInfo.timerOps->IsTimeout();  // Tim_IsTimeout
        if (BL_ERR_OK == isTimeout) {
            count -= 1UL;
        }
        //这里可以考虙，外部喂狗
    } while ((ret != BL_ERR_OK) && (count > 0UL));
    
    #if (RTE_FUN_STAY_IN_BOOT_RESET == BL_FUN_ON)
    if (BL_ERR_OK == ret) {
        RTE_ENABLE_SIB_RESET();
    }
    #endif
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details verify whether the received message is a valid message.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *   //(0x04u),(0x31u),(0x01u),(0xF5u),(0x18u) 是否是這五個數
 *****************************************************************************/
static bl_Return_t _Rte_VerifyStayInBoot(bl_ComIfHandle_t handle,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t i;
    
    (void)handle;
    
    if (size >= RTE_STAY_IN_BOOT_MSG_SIZE) {
        ret = BL_ERR_OK;
        
        for (i = 0; i < RTE_STAY_IN_BOOT_MSG_SIZE; i++) {
            if (buffer[i] != g_StayInBootValue[i]) { //(0x04u),(0x31u),(0x01u),(0xF5u),(0x18u)
                ret = BL_ERR_NOT_OK;
                break;
            }
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details    When the function of the stay in boot reset is enable, the
 *              relative variables is initialized.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
static void _Rte_InitStayInBoot(void) {

    #if (RTE_FUN_STAY_IN_BOOT_RESET == BL_FUN_ON)
    RTE_RESET_SIB_COUNT();   // gs_StayInBootResetCount =20s
    RTE_DISABLE_SIB_RESET();   //gs_StayInBootResetFlag=0
    #endif
    
}
#endif

