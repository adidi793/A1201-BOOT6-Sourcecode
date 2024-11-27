/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the passive process driver.
 *
 *  \file       bl_proc_passive_schedule.h
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
#include "bl_rte.h"
#include "bl_proc_passive_schedule.h"
#include "bl_watchdog.h"
#include "bl_flash_if.h"
#include "bl_flash.h"
/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 506,602,1532,2103,3112,3227 EOF*/
/*
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
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

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Passive process driver schedule function.
 *
 *  \note The corresponding adapter can't be null, otherwise system will always
 *        reset.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/

void Proc_PassiveSchedule(const bl_Adapter_t *adapter)
{
    bl_FlashDrvParam_t l_flash_para;
    BL_DEBUG_ASSERT_NO_RET(BL_NULL_PTR != adapter);
    bl_FlashDrvFun_t flash_jump_fun;
    bl_FeedWgdCallback_t wdg_jump_fun;

    //for (;;)
    {
        #if 0
        Wdg_Feed();
        g_WdgHeader.feed();
        //g_RteInfo.wdgOps->Feed(); //喂狗
        #if 1
        flash_jump_fun =  FLASH_DRIVER_INIT_POINT(FLASHIF_DRIVER_STARTADDRESS);  //  FLASHIF_WRITE_POINT()
        flash_jump_fun(&l_flash_para);
        #else
        FLASHIF_WRITE(&l_flash_para);
        #endif
        l_flash_para.FeedDog = g_WdgHeader.feed;
        g_FlashHeader.Write(&l_flash_para);
        #endif

        adapter->Callback();	// Adpt_UdsCallback
        						/*
    								Canif_TxMainFunction();
									Canif_RxMainFunction();
									Canif_BusoffMainFunction();
        						 */
        Rte_PeriodicSchedule(adapter->PeriodCallback);  // Rte_PeriodicSchedule(&Adpt_UdsPeriodicCallback);
        													/**
        													 	Adpt_DelayTimeCallback(&gs_UdsPrivateData.delay);
																Dcm_TimerFunction();
																Cantp_PeriodFunction();
																Dcm_MainFunction();
																//超時 超時 復位 的邏輯 ；
        													 */
    }
}


