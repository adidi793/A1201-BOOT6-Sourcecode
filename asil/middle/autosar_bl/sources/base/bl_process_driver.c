/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the process driver module.
 *
 *  \file       bl_process_driver.c
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
#include "bl_process_driver.h"
#include "bl_process_driver_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 506,1532,3406 EOF*/
/*
 * 506:Dereferencing pointer value that is possibly NULL.
 * The g_CurProcessDriver is not a null pointer when the proc_init is called.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * MISRA-C:2004 8.5(3406):Object with external linkage has been defined in a
 * header file.
 * The g_CurProcessDriver is not defined in a header file.
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
/** \brief Point to current process driver.*/
/* -----------------------------------------------------------------------A12
bl_u16_t type;
bl_u16_t index;
const struct _tag_Adapter *adapter;	//指向 g_UdsPlatformAdapter
void (*Schedule)(const struct _tag_Adapter *adapter);   //指向 Proc_PassiveSchedule
----------------------------------------------------------------------- */
const bl_ProcessDriver_t *g_CurProcessDriver;

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Initialize the process driver module. This function initializes
 *           the current process driver pointer.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
void Proc_Init(void) {
    g_CurProcessDriver = &(g_ProcessDriverList.procList[PROC_DEFAULT_DRIVER]);
    
    if (g_CurProcessDriver->adapter != BL_NULL_PTR) { // 就是 g_UdsPlatformAdapter
        (void)g_CurProcessDriver->adapter->Init();  // 调用 Adpt_UdsInit
    }
}

#if (PROC_NUMBER_OF_DRIVER > 1u)   //不成立
/**************************************************************************//**
 *
 *  \details Activate the current process driver.
 *
 *  \param[in]  id - Indicates which process driver will be activated.
 *
 *  \return If activation is successful return BL_ERR_OK,
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - Activate successfully.
 *  \retval BL_ERR_NOT_OK   - Activate unsuccessfully.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
bl_Return_t Proc_ActivateDriver(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (id < g_ProcessDriverList.number) {
        g_CurProcessDriver = &(g_ProcessDriverList.procList[id]);  //就是 gs_ProcessDriver
        
        (void)g_CurProcessDriver->adapter->Init();  //激活 Adpt_UdsInit
        
        ret = BL_ERR_OK;
    }
    
    return ret;
}
#endif

/**************************************************************************//**
 *
 *  \details timeout callback.
 *
 *  \since  V5.0.0
 *  是靠 Rte_ReportEvent(RTE_EVENT_TYPE_MEMOPS_TIMEOUT);  //要發送 78負響應 調用 的
 *****************************************************************************/
void Proc_Timeout(void) {
    if ((g_CurProcessDriver->adapter != BL_NULL_PTR)) { // 就是 g_UdsPlatformAdapter
        g_CurProcessDriver->adapter->Timeout(); //調用 Adpt_UdsTimeOutCallBack
    }
}

/**************************************************************************//**
 *
 *  \details system sleep callback.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
void Proc_Sleep(void) {
    if ((g_CurProcessDriver->adapter != BL_NULL_PTR)) { // 就是 g_UdsPlatformAdapter
        g_CurProcessDriver->adapter->Sleep();       //調用 Canif_Sleep
    }
}
