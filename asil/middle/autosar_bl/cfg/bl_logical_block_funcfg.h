/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file to configurate the functions of the logical
 *              block manager module.
 *
 *  \file       bl_logical_block_funcfg.h
 *  \ingroup    lb_manager_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      03.00.00 | 12/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.11.00 | 15/02/2012 | mingqing.tang    | N/A | Boot030012
 *      05.00.00 | 19/06/2013 | mingqing.tang    | N/A | Boot030001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_LOGICAL_BLOCK_FUNCFG_H_
#define _BL_LOGICAL_BLOCK_FUNCFG_H_
#include "bl_typedefs.h"
#include "bl_rte_funcfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3205 EOF*/
/*
 * 3205:The identifier is not used and could be removed.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
#define LBM_FUN_ATTEMPT_COUNTER         BL_FUN_ON ///BL_FUN_ON
/** enable the counter of success download.*/
#define LBM_FUN_SUCCESS_COUNTER         BL_FUN_ON ///BL_FUN_ON
/** save the fingerprint to the logical block.*/
#define LBM_FUN_SAVE_FINGERPRINT        BL_FUN_ON
#define LBM_FUN_SAVE_PROGRAMMING_DATE   BL_FUN_ON

/**
 *  when a logical block has been download successfully, its external driver
 *  is deinitialized.
 */
#define LBM_FUN_AUTO_DRIVER_DEINIT      BL_FUN_OFF		//不能開啟，因為FLASH DRIVER 是通過 傳輸載入的
/**
 *  If a logical block is unreadable(its memory is unreadable), its hash is
 *  calculated during the data transfer.
 */
#define LBM_FUN_AUTO_CALCULATE_HASH     BL_FUN_OFF	//不自動計算，而是在36傳輸完再計算

#define LBM_ATTEMPT_COUNTER_SIZE        (2u)
#define LBM_SUCCESS_COUNTER_SIZE        (2u)
/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
#if (LBM_ATTEMPT_COUNTER_SIZE == 2u)    //進入該選項
typedef bl_u16_t bl_AttemptCounter_t;   /**< a counter for attempt download.*/
typedef bl_u16_t bl_SuccessCounter_t;   /**< a counter for success download.*/
#else 
typedef bl_u8_t bl_AttemptCounter_t;   /**< a counter for attempt download.*/
typedef bl_u8_t bl_SuccessCounter_t;   /**< a counter for success download.*/
#endif
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

