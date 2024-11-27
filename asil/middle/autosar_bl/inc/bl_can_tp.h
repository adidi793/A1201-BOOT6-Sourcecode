/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the CAN Transport Protocol module.
 *
 *  \file       bl_can_tp.h
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
#ifndef _BL_CAN_TP_H_
#define _BL_CAN_TP_H_
#include "bl_typedefs.h"
#include "bl_can_tp_funcfg.h"

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
#define CANTP_TYPE_STANDARD         (0u)
#define CANTP_TYPE_EXTENDED         (1u)
#define CANTP_TYPE_MIXED            (2u)

#define CANTP_TATYPE_PHYSICAL       (0u)
#define CANTP_TATYPE_FUNCTIONAL     (1u)

#define CANTP_MAX_FRAME_SIZE        (8u)//(64u)    /* CAN frame max size*/   //这个配置，和那个CANFD的长度有关

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias of the struct _tag_CanTpChannelCfg.*/
typedef struct _tag_CanTpChannelCfg bl_CanTpChannelCfg_t;
/** \brief The can tp handle.*/
typedef bl_u16_t bl_CanTpHandle_t;
/** \brief The Result of the Rx or Tx.*/
typedef bl_Return_t bl_CanTpResult_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/** \brief The channel configuration informations for the CAN TP.*/
struct _tag_CanTpChannelCfg {
    bl_u8_t type;   /**< The type of a tp channel.*/
    bl_u8_t taType; /**< The TA type of a tp channel.*/
    bl_ComIfHandle_t rxId;  /**< RX ID from CanIf.*/
    bl_ComIfHandle_t txId;  /**< TX ID to CanIf.*/
    bl_u16_t timerA;        /**< The timer A is used to send a can frame.*/
    bl_u16_t timerB;        /**< The timer B is used to wait for the FC frame.*/
    bl_u16_t timerC;        /**< The timer C is used to wait for the CF frame.*/
    bl_u8_t ta;     /**< The TA of a tp channel.*/
    bl_u8_t st;     /**< The STmin of a tp channel.*/
    bl_u8_t bs;     /**< The block size of a tp channel.*/
    bl_u8_t wft;    /**< The max wft of a tp channel.*/
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the can tp module.*/
extern void Cantp_Init(void);
/** \brief the period function of the can tp module*/
extern void Cantp_PeriodFunction(void);
/** \brief Transmit a data.*/
extern bl_Return_t Cantp_Transmit(bl_CanTpHandle_t handle,
    bl_BufferSize_t size);
/** \brief Indicate a frame to be received.*/
extern void Cantp_RxIndication(bl_ComIfHandle_t handle,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);
/** \brief Confire a frame to be transmitted.*/
extern void Cantp_TxConfirmation(bl_ComIfHandle_t handle);

#endif

