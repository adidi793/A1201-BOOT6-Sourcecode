/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the header file of the watchdog module .
 *
 *  \file       bl_watchdog.h
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
#ifndef _BL_WATCHDOG_H_
#define _BL_WATCHDOG_H_
#include "bl_typedefs.h"
#include "bl_rte_funcfg.h"
#include "common_types.h"
#include "bl_system_cfg.h"

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

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
extern void Wdg_Init(void);
extern void Wdg_Deinit(void);
extern void Wdg_FastReset(void);


typedef void(*bl_WdgFun_t)(void);
typedef struct
{
    //bl_u16_t type;          /**< The type of the flash driver.*/
    //bl_u16_t index;         /**< The index of the flash driver.*/
    bl_WdgFun_t feed;  /**< Initialize the flash drvier.*/
    //u32 res;
}wdg_Header;


#if (RTE_FUN_COPY_WDG_TO_RAM == BL_FUN_ON)  //條件成立，
	extern __attribute__((section(".RAMCODE_WDG1"))) void Wdg_Feed(void);
#else

	extern void Wdg_Feed(void);

#endif

#if (RTE_FUN_COPY_WDG_TO_RAM == BL_FUN_ON)  //條件成立，
    extern const __IO wdg_Header g_WdgHeader  __attribute__((section(".RAMCODE_WDG")));
#else
    //#pragma location = "RAMCODE"
    extern  const __IO wdg_Header g_WdgHeader;
#endif

#endif