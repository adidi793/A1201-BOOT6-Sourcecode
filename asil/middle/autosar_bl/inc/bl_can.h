/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the header file of the can module.
 *
 *  \file       bl_can.h
 *  \ingroup    driver_module
 *  \author
 *
 *  \version    1.1.0
 *  \date       14/05/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | -------------
 *      01.00.00 | 14/05/2014 | ning.chen        | N/A | BootDrv010001
 *      01.01.00 | 21/08/2014 | ning.chen        | N/A | BootDrv010002
 *
 *****************************************************************************/
#ifndef _BL_CAN_H_
#define _BL_CAN_H_


#include "common_types.h"
#include "can.h"
#include "bl_typedefs.h"
#include "bl_can_if.h"


/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 783,3205,3210,3447 EOF*/
/*
 * 783:A subsequent declaration of the identifier means that it is being used
 * both as a structure/union member and also as a label, ordinary identifier.
 * The member of the structure is used through the structure object.
 *
 * 3205:The identifier is not used and could be removed.
 * The identifiers are used in the bl_can.c file.
 *
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_can_if.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/

#define MSK_EXT_ID_MSB                  ((u32)(0x10000000))      /*為什麼人為的定義最高位來判斷PDU 是不是擴展幀*/


#define CAN_STATUS_NOINIT               (0u)
#define CAN_STATUS_IDLE                 (1u)
#define CAN_STATUS_TRANSMITTING         (2u)
#define CAN_STATUS_TRANSMIT_FINISH      (3u)
#define CAN_STATUS_BUSOFF               (4u)

#define CAN_MAX_SIZE_OF_DATA            (CAN_FRAME_MAX_DLC)   //要查清楚，该宏的调用 --原来是8
#define CAN_MAX_NUMBER_OF_CONTROLLER    (1u)  //目前只支持一個CAN   --現在先是兼容兩個，再用宏來屏蔽某路

#define CAN_USED_CONTROLLER_NUMBER      (1u)  //理論上，和 CAN_MAX_NUMBER_OF_CONTROLLER 相同
#define CAN_USED_CONTROLLER_ID          (0u)
#define CAN_USED_CONTROLLER_ID_1          (1u)
#define CAN_USED_CONTROLLER_ISR_ID      (36)


#if 0
/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias of struct _tag_CanControllerRegs.*/
typedef struct _tag_CanControllerRegs bl_CanControllerRegs_t;
/** \brief A alias of struct _tag_CanController.*/
typedef struct _tag_CanController bl_CanController_t;
/** \brief A alias of struct _tag_CanControllerBaseRegs.*/
typedef struct _tag_CanControllerBaseRegs bl_CanControllerBaseRegs_t;
/** \brief A alias of struct _tag_CanControllerCfg.*/
typedef struct _tag_CanControllerCfg bl_CanControllerCfg_t;
/** \brief A alias of struct _tag_CanFrameBuffer.*/
typedef struct _tag_CanFrameBuffer bl_CanFrameBuffer_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
struct _tag_CanControllerBaseRegs {
    bl_u8_t ctl0;   /**< Control register0*/
    bl_u8_t ctl1;   /**< Control register1*/
    bl_u8_t btr0;   /**< Baudrate register0*/
    bl_u8_t btr1;   /**< Baudrate register1*/
    bl_u8_t rflg;   /**< Receive flag register*/
    bl_u8_t rier;   /**< Receive interrupt enable register*/
    bl_u8_t tflg;   /**< Transmit flag register*/
    bl_u8_t tier;   /**< Transmit interrupt enable register*/
    bl_u8_t tarq;   /**< Transmit abort request register*/
    bl_u8_t taak;   /**< Transmit abort ack register*/
    bl_u8_t tbsel;  /**< Transmit box select*/
    bl_u8_t idac;   /**< Identifier acceptance control register*/
    bl_u8_t res0;   /**< Reserve*/
    bl_u8_t misc;   /**< Miscellaneous*/
    bl_u8_t exerr;  /**< Rx error counter*/
    bl_u8_t txerr;  /**< Tx error counter*/
    bl_u8_t idar0;  /**< Identifier acceptance 0*/
    bl_u8_t idar1;  /**< Identifier acceptance 1*/
    bl_u8_t idar2;  /**< Identifier acceptance 2*/
    bl_u8_t idar3;  /**< Identifier acceptance 3*/
    bl_u8_t idmr0;  /**< Identifier filter mask 0*/
    bl_u8_t idmr1;  /**< Identifier filter mask 1*/
    bl_u8_t idmr2;  /**< Identifier filter mask 2*/
    bl_u8_t idmr3;  /**< Identifier filter mask 3*/
    bl_u8_t idar4;  /**< Identifier acceptance 4*/
    bl_u8_t idar5;  /**< Identifier acceptance 5*/
    bl_u8_t idar6;  /**< Identifier acceptance 6*/
    bl_u8_t idar7;  /**< Identifier acceptance 7*/
    bl_u8_t idmr4;  /**< Identifier filter mask 4*/
    bl_u8_t idmr5;  /**< Identifier filter mask 5*/
    bl_u8_t idmr6;  /**< Identifier filter mask 6*/
    bl_u8_t idmr7;  /**< Identifier filter mask 7*/
};

struct _tag_CanFrameBuffer {
    bl_u32_t id;
    bl_Buffer_t data[CAN_MAX_SIZE_OF_DATA];
    bl_u8_t dlc;
    bl_u8_t priority;
    bl_u16_t timeStamp;
};

struct _tag_CanControllerRegs {
    struct _tag_CanControllerBaseRegs base;
    struct _tag_CanFrameBuffer rxBuf;
    struct _tag_CanFrameBuffer txBuf;
};

struct _tag_CanControllerCfg {
    bl_u8_t phyId;
    const struct _tag_CanControllerBaseRegs *initReg;
};

struct _tag_CanController {
    bl_u8_t status;
    volatile struct _tag_CanControllerRegs *regs;
};

#else

/** \brief A alias of struct _tag_CanFrameBuffer.*/
typedef struct _tag_CanFrameBuffer bl_CanFrameBuffer_t;

//typedef CAN_Type  CAN_TypeDef;
typedef CAN_TypeDef  bl_CanControllerBaseRegs_t ;

typedef struct {
    bl_CanControllerBaseRegs_t base;
    //struct _tag_CanFrameBuffer rxBuf;
    //struct _tag_CanFrameBuffer txBuf;
} bl_CanControllerRegs_t;

typedef struct {
    bl_u8_t phyId;
    const   bl_CanControllerBaseRegs_t *initReg;
} bl_CanControllerCfg_t;

typedef struct _tag_CanController {
    bl_u8_t status;
    bl_CanControllerRegs_t *regs; //指向CAN寄存器结构体
} bl_CanController_t;


struct _tag_CanFrameBuffer {
    bl_u32_t id;
    bl_Buffer_t data[CAN_MAX_SIZE_OF_DATA];
    bl_u8_t dlc;
    bl_u8_t priority;
    bl_u16_t timeStamp;
};

#endif


/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the can module.*/
extern bl_Return_t Can_Init(void);
/** \brief Deinitialize the can module.*/
extern void Can_Deinit(void);
/** \brief Transmit the data to can bus.*/
extern bl_Return_t Can_Write(const bl_CanTxPdu_t *pdu);
/** \brief Receive the data from can bus.*/
extern bl_Return_t Can_Read(bl_CanRxPdu_t *pdu);
/** \brief Check whether can controller is Tx successfully.*/
extern bl_Return_t Can_CheckTxStatus(bl_ComIfHandle_t handle);
/** \brief Check can controller busoff.*/
extern void Can_BusOff_Check(void);
/** \brief The can controller into sleep status.*/
extern void Can_Sleep(void);


void Can_Deinit(void);
void can_phy1_busoff_op_task(void);

void can_id_phy_rx_irq_task(u8 phy_id, const can_queue_elem_s *p_frame_info);
void can_id_phy_wakeup_irq_task(u8 phy_id);

void can_test_task(void);
//void can_logic0_phy_busoff_irq_task(void);
void  can1_busoff_op_task(void);



void Can_SetTxStatus_finish(bl_ComIfHandle_t handle);

void bl_can_tx_task(void);
void bl_can_rx_task(void);

u8 can_drive_get_logic_id(u8 phy_id);
u8 can_drive_get_phy_id(u8 logic_id);
bl_Return_t  Can_Controller_enable(u8 Controller, u8 state);

#endif
