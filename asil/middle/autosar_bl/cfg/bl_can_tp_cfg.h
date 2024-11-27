/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a configuration head file of the CAN Transport Protocol
 *              module.
 *
 *  \file       bl_can_tp_cfg.h
 *  \ingroup    cantp_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 17/03/2010 | rui.guo          | N/A | Boot010001
 *      02.00.00 | 21/02/2011 | rui.guo          | N/A | Boot020001
 *      05.00.00 | 19/06/2013 | rui.guo          | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_CAN_TP_CFG_H_
#define _BL_CAN_TP_CFG_H_
#include "bl_can_tp.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_security_cfg.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief The schedule period of the cantp module.*/
#define CANTP_SCHEDULE_PERIOD           (1u)   //原來配置  1ms

/** \brief The number of rx channels of the cantp module.*/
#define CANTP_NUMBER_OF_RX_CHANNEL      (2u)
/** \brief The number of tx channels of the cantp module.*/
#define CANTP_NUMBER_OF_TX_CHANNEL      (1u)

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
extern const bl_CanTpChannelCfg_t g_CanTpRxChnsCfg[CANTP_NUMBER_OF_RX_CHANNEL];

extern const bl_CanTpChannelCfg_t g_CanTpTxChnsCfg[CANTP_NUMBER_OF_TX_CHANNEL];

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

