/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of bl_can_if.
 *
 *  \file       bl_can_if.h
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      02.00.00 | 20/08/2013 | haiping.wang     | N/A | Boot020001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_CAN_IF_H_
#define _BL_CAN_IF_H_
#include "bl_typedefs.h"
#include "bl_rte.h"
#include "bl_can_if_funcfg.h"
#include "can_cfg.h"

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
 *  Internal Macro Definitions
 *****************************************************************************/
#define CANIF_MAX_CAN_FRAME_SIZE       (CAN_FRAME_MAX_DLC)

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief Indication that a can frame is received.*/
typedef void (*bl_RxIndication_t)(bl_ComIfHandle_t handle,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);
/** \brief Indication that a can frame is transmitted.*/
typedef void (*bl_TxConfirmation_t)(bl_ComIfHandle_t handle);
/** \brief A alias of struct _tag_CanIfPduCfg.*/
typedef struct _tag_CanIfPduCfg bl_CanIfPduCfg_t;
/** \brief A alias of struct _tag_CanIfRxList.*/
typedef struct _tag_CanIfRxList bl_CanIfRxList_t;
/** \brief A alias of struct _tag_CanIfTxList.*/
typedef struct _tag_CanIfTxList bl_CanIfTxList_t;
/** \brief A alias of struct _tag_CanRxPdu.*/
typedef struct _tag_CanRxPdu bl_CanRxPdu_t;
/** \brief A alias of struct _tag_CanTxPdu.*/
typedef struct _tag_CanTxPdu bl_CanTxPdu_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
struct _tag_CanRxPdu {
    bl_ComIfHandle_t handle; //CAN通道的句柄 2字节
    bl_BufferSize_t dlc;     //2字节
    bl_u32_t canId;          //4字节
    bl_Buffer_t frame[CANIF_MAX_CAN_FRAME_SIZE]; //8字节
};

struct _tag_CanTxPdu {
    bl_ComIfHandle_t handle;
    bl_BufferSize_t dlc;
    bl_u32_t canId;
    bl_u8_t canfd;  //增加canfd的標志
    bl_u8_t brs;  //速度可变标志
    const bl_Buffer_t *frame;
};

struct _tag_CanIfPduCfg {
    bl_u8_t status;          //狀態  CANIF_TRANSMITE 或者  CANIF_RECEIVE
    bl_u8_t type;            //類型  CANIF_STANDARD 或者 擴展
    bl_ComIfHandle_t handle; //原來hander是 物理層的通道
    bl_u32_t id;             //CAN的id值
    bl_u8_t canfd;           //增加canfd的標志
    bl_u8_t brs;             //速度可变标志
};

struct _tag_CanIfRxList {
    bl_u16_t number;
    #if (CANIF_FUN_DYNAMIC_CANID == BL_FUN_ON)
    struct _tag_CanIfPduCfg *rxList;
    #else
    const struct _tag_CanIfPduCfg *rxList;
    #endif
    bl_RxIndication_t RxInd;   //接收確認函數
};

struct _tag_CanIfTxList {
    bl_u16_t number;
    #if (CANIF_FUN_DYNAMIC_CANID == BL_FUN_ON)
    struct _tag_CanIfPduCfg *txList;
    #else
    const struct _tag_CanIfPduCfg *txList;
    #endif
    bl_TxConfirmation_t TxConf;    //發送確認函數
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the can IF module*/
extern bl_Return_t Canif_Init(void);
/** \brief Deinitialize the can IF module*/
extern void Canif_DeInit(void);
/** \brief Transmit a can frame to can bus.*/
extern bl_Return_t Canif_Write(bl_ComIfHandle_t handle,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);
/** \brief Try to receive a can frame from can bus.*/
extern bl_Return_t Canif_TryToRead(bl_ComIfData_t *data);
/** \brief Check whether a can frame is transmitted.*/
extern bl_Return_t Canif_IsWritten(bl_ComIfHandle_t handle);
/** \brief Main Tx function.*/
extern void Canif_TxMainFunction(void);
/** \brief Main Rx function.*/
extern void Canif_RxMainFunction(void);
/** \brief Main bus-off function.*/
extern void Canif_BusoffMainFunction(void);
/** \brief can IF module into sleep status.*/
extern void Canif_Sleep(void);

#if (CANIF_FUN_DYNAMIC_CANID == BL_FUN_ON)
/** \brief Set the Tx can id.*/
extern void Canif_SetTxCanId(bl_ComIfHandle_t handle, bl_u32_t id);
/** \brief Set the Rx can id.*/
extern void Canif_SetRxCanId(bl_ComIfHandle_t handle, bl_u32_t id);
#endif

#endif

