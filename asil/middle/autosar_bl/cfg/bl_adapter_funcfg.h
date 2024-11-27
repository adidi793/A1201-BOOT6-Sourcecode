/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file to configurate the adapter module.
 *
 *  \file       bl_adapter_funcfg.h
 *  \ingroup    communication_adapter_module
 *  \author    
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      05.00.00 | 19/06/2013 | mingqing.tang    | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_ADAPTER_FUNCFG_H_
#define _BL_ADAPTER_FUNCFG_H_

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief The function is used to save the delay flag to DM module.*/
#define ADPT_FUN_SAVE_DELAY_FLAG                BL_FUN_OFF
/** \brief The function is used to sub access count.*/
#define ADPT_FUN_DELAY_WITH_CONTINUOUS_SEED     BL_FUN_ON
/** \brief The function is used for the future.*/
#define ADPT_FUN_ADVANCED_ATTEMPT_DELAY         BL_FUN_OFF
/** \brief The function is used for the future.*/
#define ADPT_FUN_SUBTRACT_FAILED_COUNT          BL_FUN_OFF
/** \brief Process the data when the data is received first.*/
#define ADPT_FUN_FIRST_DATA_PREPROCESS          BL_FUN_OFF
/** \brief Get the integrality from the App.*/
#define ADPT_FUN_GET_INTEGRALITY_VALUE          BL_FUN_OFF
/** \brief Check the compatibility from the App.*/
#define ADPT_FUN_CHECK_COMPATIBILITY            BL_FUN_OFF

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

#endif

