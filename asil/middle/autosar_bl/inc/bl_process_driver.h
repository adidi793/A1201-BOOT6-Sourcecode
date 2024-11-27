/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file of the process driver module.
 *
 *  \file       bl_process_driver.h
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
#ifndef _BL_PROCESS_DRIVER_H_
#define _BL_PROCESS_DRIVER_H_
#include "bl_typedefs.h"
#include "bl_adapter.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 783,3205,3210,3447,3448 EOF*/
/*
 * 783:A subsequent declaration of the identifier means that it is being used
 * both as a structure/union member and also as a label, ordinary identifier.
 * The member of the structure is used through the structure object.
 *
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
#define PROC_DRIVER_TYPE_ACTIVE         (0x0001u)
#define PROC_DRIVER_TYPE_PASSIVE        (0x0002u)

// 就是  Proc_PassiveSchedule(&g_UdsPlatformAdapter)
#define PROC_SCHEDULE() g_CurProcessDriver->Schedule \
    (g_CurProcessDriver->adapter)
/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
typedef struct _tag_ProcessDriver bl_ProcessDriver_t;

typedef struct _tag_ProcessDriverList bl_ProcessDriverList_t;
/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/** \brief This is process driver info.*/
struct _tag_ProcessDriver {
    bl_u16_t type;
    bl_u16_t index;
    const struct _tag_Adapter *adapter;	//指向 g_UdsPlatformAdapter
    void (*Schedule)(const struct _tag_Adapter *adapter);   //指向 Proc_PassiveSchedule
};

/** \brief This is process driver list info.*/
struct _tag_ProcessDriverList {
    bl_u16_t number;
    const struct _tag_ProcessDriver *procList;
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
/** \brief Point to current process driver.*/
extern const bl_ProcessDriver_t *g_CurProcessDriver;

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the process driver module.*/
extern void Proc_Init(void);
/** \brief Activate the current process driver.*/
extern bl_Return_t Proc_ActivateDriver(bl_u16_t id);
/** \brief timeout callback.*/
extern void Proc_Timeout(void);
/** \brief system sleep callback.*/
extern void Proc_Sleep(void);

#endif
