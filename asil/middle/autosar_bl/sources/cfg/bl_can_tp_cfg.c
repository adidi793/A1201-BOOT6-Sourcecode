/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a configuration source file of the CAN Transport
 *              Protocol module.
 *
 *  \file       bl_can_tp_cfg.c
 *  \ingroup    cantp_module
 *  \author     
 *              
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
#include "bl_rte_funcfg.h"
#include "bl_can_tp_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1257,1533,3120,3211,3453 EOF*/
/*
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1533:The object is referenced in only one translation unit.
 *
 * 3120:Hard-coded magic integer constant.
 * This is a only configuration.
 *
 * 3211:The global identifier is defined but not used.
 * The g_CanTpTxChnsCfg and g_CanTpRxChnsCfg are used in the bl_can_tp.c file.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Function Macro
 *****************************************************************************/
#if (RTE_SYSTEM_SCHEDULE_PERIOD != CANTP_SCHEDULE_PERIOD)
#error "The period of cantp is error."
#endif

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
#define CANTP_MAKE_TIMEOUT(ms)  ((bl_u16_t)((ms)/CANTP_SCHEDULE_PERIOD)) /* for 1000us period */
//#define CANTP_MAKE_TIMEOUT(ms)  ((bl_u16_t)((ms) * 4)) /* for 250us period */
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
/** \brief the configurations of the Rx channels.*/
#define STmin_Tester (0U)
#define STmin_ECU    (0U)
#define BS_Tester    (0U)

const bl_CanTpChannelCfg_t g_CanTpRxChnsCfg[CANTP_NUMBER_OF_RX_CHANNEL] =
{
    /*Channel 0:Phsical & Standars addressing*/
    {
        CANTP_TYPE_STANDARD,
        CANTP_TATYPE_PHYSICAL,
        0,  /* RX HANDLE */
        0,  /* TX HANDLE */
        CANTP_MAKE_TIMEOUT(70u),     /* TIME A */
        CANTP_MAKE_TIMEOUT(70u),     /* TIME B */
        CANTP_MAKE_TIMEOUT(150u),    /* TIME C */
        0,  /* TA */
        0,  /* STmin */ //最小分离时间A12:0;BYD:1
        0,  /* BS */ //数据块大小A12:0;BYD:8
        0,  /* WFT */
    },
    /*Channel 1:functional & Standars addressing*/
    {
        CANTP_TYPE_STANDARD,
        CANTP_TATYPE_FUNCTIONAL,
        1,  /* RX HANDLE */
        0,  /* TX HANDLE */
        CANTP_MAKE_TIMEOUT(70u),     /* TIME A */
        CANTP_MAKE_TIMEOUT(70u),     /* TIME B */
        CANTP_MAKE_TIMEOUT(150u),    /* TIME C */
        0,  /* TA */
        0,  /* STmin */ //最小分离时间A12:0;BYD:1
        0,  /* BS */ //数据块大小A12:0;BYD:8
        0,  /* WFT */
    },
};

/** \brief the configurations of the Tx channels.*/
const bl_CanTpChannelCfg_t g_CanTpTxChnsCfg[CANTP_NUMBER_OF_TX_CHANNEL] =
{
    /*Channel 0:Phsical & Standars addressing*/
    {
        CANTP_TYPE_STANDARD,
        CANTP_TATYPE_PHYSICAL,
        0,  /* RX HANDLE */
        0,  /* TX HANDLE */
        CANTP_MAKE_TIMEOUT(70u),     /* TIME A */
        CANTP_MAKE_TIMEOUT(150u),    /* TIME B */
        CANTP_MAKE_TIMEOUT(70u),     /* TIME C */
        0,  /* TA */
        0,  /* STmin */
        0,  /* BS */
        0,  /* WFT */
    },
};

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

