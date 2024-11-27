/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file of the adapter module.
 *
 *  \file       bl_adapter.h
 *  \ingroup    communication_adapter_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2018
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 25/03/2018 | xucong      | N/A | Boot010001

 *
 *****************************************************************************/
#ifndef _BL_ADAPTER_H_
#define _BL_ADAPTER_H_
#include "bl_typedefs.h"
#include "bl_rte.h"
#include "bl_adapter_funcfg.h"

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
#define ADPT_SYSTEM_STATUS_MASK         (RTE_SYSFLAG_ERASE_BLOCK \
    | RTE_SYSFLAG_WRITE_DATA)

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias of struct _tag_SecurityDelay.*/
typedef struct _tag_SecurityDelay bl_SecurityDelay_t;
/** \brief A alias of struct _tag_Adapter.*/
typedef struct _tag_Adapter bl_Adapter_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/**
 *  \brief This is delay information of security access. it includes delaytime,
 *         attemptCount and so on.
 */
struct _tag_SecurityDelay {
    bl_u32_t delayTime;         /**< remain delay time*/
    bl_u16_t attemptCount;      /**< remain attempt count*/
    #if (ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
    bl_u16_t delayFlagId;       /**< position of delay flag*/
    #endif
    #if (ADPT_FUN_ADVANCED_ATTEMPT_DELAY == BL_FUN_ON)
    bl_u16_t failedCntFlagId;   /**< position of failed count flag*/
    #endif
};

/** \brief This is a available adapter.*/
struct _tag_Adapter {
    bl_u16_t type;                  /**< the type of adapter*/
    bl_u16_t index;                 /**< the index*/
    bl_Return_t (*Init)(void);      /**< the initialization interface.*/
    void (*Callback)(void);         /**< the callback interface.*/
    void (*PeriodCallback)(void);   /**< the period callback interface.*/
    void (*Timeout)(void);          /**< the timeout callback interface.*/
    void (*Sleep)(void);            /**< the sleep callback interface.*/
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the security access delay info.*/
extern void Adpt_InitDelayInfo(bl_SecurityDelay_t *delay);
/** \brief Activate download.*/
extern bl_Return_t Adpt_ActivateDownload(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size);
/** \brief Beginning to download data.*/
extern bl_Return_t Adpt_DownloadData(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer);
/** \brief Erase a logical block.*/
extern bl_Return_t Adpt_EraseLB(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size);
/** \brief Verify checksum value of a logical block.*/
extern bl_Return_t Adpt_VerifyData(bl_DownContext_t *dct,
    bl_Size_t size,
    bl_Buffer_t *value);
/** \brief Check compatibility of all downloaded logical block.*/
extern bl_Return_t Adpt_CheckCompatibility(void);
/** \brief Check whether security access is available.*/
extern bl_Return_t Adpt_IsAccessible(const bl_SecurityDelay_t *delay);
/** \brief Decrease the attemptcount of security access.*/
extern bl_u16_t Adpt_DecreaseAttemptCount(bl_SecurityDelay_t *delay);
/** \brief Decrease the delaytime of security access.*/
extern void Adpt_DelayTimeCallback(bl_SecurityDelay_t *delay);

#endif

