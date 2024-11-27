/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a source file of the CAN Transport Protocol module.
 *
 *  \file       bl_can_tp.c
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
#include "bl_common.h"
#include "bl_rte.h"
#include "bl_dcm.h"
#include "bl_can.h"
#include "bl_can_if.h"
#include "bl_can_tp_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 506,602,771,781,783,1251,1257,1532,2100,2103,2201,2465,3112,3120 EOF*/
/*PRQA S 3198,3204,3212,3218,3227,3435,3453,3456,3673 EOF*/
/*
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * MISRA-C:2004 14.6(771):More than one break statement is being used to
 * terminate an iteration statement.
 * it can simplify the code and have no side-effect.
 *
 * MISRA-C:2004 5.6(781):The identifier is used as a structure/union member.
 * The member of the structure is used through the structure object.
 *
 * 783:A subsequent declaration of the identifier means that it is being used
 * both as a structure/union member and also as a label, ordinary identifier.
 * The member of the structure is used through the structure object.
 *
 * 1251:Suffixed integer constant causes implicit conversion of other
 * (integral) operand.
 *
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 2100:Integral promotion : unsigned char promoted to signed int.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * 2201:This indentation is not consistent with previous indentation.
 *
 * 2465:This 'for' loop will only be executed once.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3120:Hard-coded magic integer constant.
 * This is a only configuration.
 *
 * 3198:This assignment is redundant. The value of 'ret' is never used before
 * being modified.
 *
 * 3204:The variable is only set once and so it could be declared with the
 * 'const' qualifier.
 *
 * 3212:This explicit cast is redundant and could be removed.
 *
 * MISRA-C:2004 8.7(3218):File scope static identifier is only accessed in one
 * function.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * MISRA-C:2004 19.4(3435):Parameter is used more than once in this macro.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 *
 * 3456:Parameter will be evaluated more than once when this macro is used.
 * When these macros is used, there is no side-effect.
 *
 * MISRA-C:2004 16.7(3673): the pointer could be of type 'pointer to const'.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Macro
 *****************************************************************************/

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
/** \brief The CTS used for the FS field in the Flow Control Frame.*/
#define CANTP_FC_FRAME_CTS              (0x00u)
/** \brief The WAIT used for the FS field in the Flow Control Frame.*/
#define CANTP_FC_FRAME_WAIT             (0x01u)
/** \brief The OVERFLOW used for the FS field in the Flow Control Frame.*/
#define CANTP_FC_FRAME_OVERFLOW         (0x02u)

/** \brief The number of the PCI used for different types of cantp channel.*/
#define CANTP_NUMBER_OF_PCI_INFO        (0x02u)
/** \brief This PCI used for standard cantp channel.*/
#define CANTP_STANDARD_PCI_INFO         (0x00u)
/** \brief This PCI used for extended or mixed cantp channel.*/
#define CANTP_EXTENDED_PCI_INFO         (0x01u)

/** \brief The number of status of the Rx channels.*/
#define CANTP_NUMBER_OF_RX_STATUS       (0x05u)
/** \brief The number of status of the Tx channels.*/
#define CANTP_NUMBER_OF_TX_STATUS       (0x05u)

/** \brief The number of frame types according to ISO 15765-2.*/
#define CANTP_NUMBER_OF_FRAME_TYPE      (0x04u)
/** \brief The Single Frame.*/
#define CANTP_FRAME_TYPE_SF             (0x00u)
/** \brief The First Frame.*/
#define CANTP_FRAME_TYPE_FF             (0x01u)
/** \brief The Consecutive Frame.*/
#define CANTP_FRAME_TYPE_CF             (0x02u)
/** \brief The Flow Control Frame.*/
#define CANTP_FRAME_TYPE_FC             (0x03u)

/** \brief The type value of the Single Frame.*/
#define CANTP_FRAME_SF_VALUE            (0x00u)
/** \brief The type value of the First Frame.*/
#define CANTP_FRAME_FF_VALUE            (0x10u)
/** \brief The type value of the Consecutive Frame.*/
#define CANTP_FRAME_CF_VALUE            (0x20u)
/** \brief The type value of the Flow Control Frame.*/
#define CANTP_FRAME_FC_VALUE            (0x30u)

/** \brief The 16bits type value of the First Frame.*/
#define CANTP_FRAME_FF_VAULE_16BITS     (0x1000u)

/** \brief The ta offset in a frame.*/
#define CANTP_TA_OFFSET                 (0x00u)
/** \brief The max STmin value.*/
#define CANTP_MAX_STMIN_VALUE           (0x7Fu)
#define CANTP_MIN_STMIN_VALUE_US        (0xF0u)
#define CANTP_MAX_STMIN_VALUE_US        (0xFAu)

/** \brief The Idle status of a channel.*/
#define CANTP_STATUS_IDLE               (0x00u)
/** \brief The recvSF status of a Rx channel.*/
#define CANTP_STATUS_RECEIVING_SF       (0x01u)
/** \brief The recvFF status of a Rx channel.*/
#define CANTP_STATUS_RECEIVING_FF       (0x02u)
/** \brief The recvCF status of a Rx channel.*/
#define CANTP_STATUS_RECEIVING_CF       (0x03u)
/** \brief The tranFC status of a Rx channel.*/
#define CANTP_STATUS_TRANSMITTING_FC    (0x04u)

/** \brief The tranSF status of a Tx channel.*/
#define CANTP_STATUS_TRANSMITTING_SF    (0x01u)
/** \brief The tranFF status of a Tx channel.*/
#define CANTP_STATUS_TRANSMITTING_FF    (0x02u)
/** \brief The tranCF status of a Tx channel.*/
#define CANTP_STATUS_TRANSMITTING_CF    (0x03u)
/** \brief The recvFC status of a Tx channel.*/
#define CANTP_STATUS_RECEIVING_FC       (0x04u)

/** \brief The sub Idle status of a channel.*/
#define CANTP_SUB_STATUS_IDLE           (0x00u)
/** \brief The sub transmitting status of a channel.*/
#define CANTP_SUB_STATUS_TRANSMITTING   (0x01u)
/** \brief The sub receiving status of a channel.*/
#define CANTP_SUB_STATUS_RECEIVING      (0x02u)

/** \brief The mask of data size in the First Frame.*/
#define CANTP_FRAME_FF_DATASIZE_MASK    (0x0FFFu)
/** \brief the mask of data size in the Single Frame.*/
#define CANTP_FRAME_SF_DATASIZE_MASK    (0x0Fu)

/** \brief Get the data size in the Single Frame.*/
#define CANTP_GET_SF_DATASIZE(data)     BL_GET_LOW_HALF(data) //BL_GET_LOW_HALF(data)    //注意CANFD支持超过8，15字节等。
/** \brief Get the data size in the First Frame.*/
#define CANTP_GET_FF_DATASIZE(p)        (BL_BE16_TO_MCU(p) \
    & CANTP_FRAME_FF_DATASIZE_MASK)

/** \brief Get the SN in the Consecutive Frame.*/
#define CANTP_GET_CF_SN(pci,buf)      BL_GET_LOW_HALF((buf)[(pci)->pciPos])

/** \brief Get the frame type in the a frame.*/
#define CANTP_GET_FRAME_TYPE(pci,buf) BL_GET_LOW_HALF((buf)[(pci)->pciPos]>> 4)
/** \brief Get the FlowStatus in the a Flow Control Frame.*/
#define CANTP_GET_FC_FS(pci,buf)      BL_GET_LOW_HALF((buf)[(pci)->pciPos])
/** \brief Get the BlockSize in the a Flow Control Frame.*/
#define CANTP_GET_FC_BS(pci,buf)        ((buf)[(pci)->fcBsPos])
/** \brief Get the STmin in the a Flow Control Frame.*/
#define CANTP_GET_FC_STMIN(pci,buf)     ((buf)[(pci)->fcStPos])


/** \brief If the status of a channle is Idle return TRUE.*/
#define CANTP_STATUS_IS_IDLE(chn)       (CANTP_STATUS_IDLE == (chn)->status)
/** \brief If the status of a channle is recvSF return TRUE.*/
#define CANTP_STATUS_IS_RECVSF(chn)     (CANTP_STATUS_RECEIVING_SF \
    == (chn)->status)
/** \brief If the status of a channle is recvFF return TRUE.*/
#define CANTP_STATUS_IS_RECVFF(chn)     (CANTP_STATUS_RECEIVING_FF \
    == (chn)->status)
/** \brief If the status of a channle is recvCF return TRUE.*/
#define CANTP_STATUS_IS_RECVCF(chn)     (CANTP_STATUS_RECEIVING_CF \
    == (chn)->status)
/** \brief If the status of a channle is recvFC return TRUE.*/
#define CANTP_STATUS_IS_RECVFC(chn)     (CANTP_STATUS_RECEIVING_FC \
    == (chn)->status)
/** \brief If the status of a channle is tranSF return TRUE.*/
#define CANTP_STATUS_IS_TRANSF(chn)     (CANTP_STATUS_TRANSMITTING_SF \
    == (chn)->status)
/** \brief If the status of a channle is tranFF return TRUE.*/
#define CANTP_STATUS_IS_TRANFF(chn)     (CANTP_STATUS_TRANSMITTING_FF \
    == (chn)->status)
/** \brief If the status of a channle is tranCF return TRUE.*/
#define CANTP_STATUS_IS_TRANCF(chn)     (CANTP_STATUS_TRANSMITTING_CF \
    == (chn)->status)
/** \brief If the status of a channle is tranFC return TRUE.*/
#define CANTP_STATUS_IS_TRANFC(chn)     (CANTP_STATUS_TRANSMITTING_FC \
    == (chn)->status)

/*  When continuous FF or SF is received in one channel during
    a timeout period,It maybe break other physical channel.
    So If a channel is receiving a FF or a SF, do NOT indicate.*/
/** \brief Check if the buffer is gotten from Dcm module.*/
#define CANTP_IS_GETTING_BUFFER(chn)    ((chn)->status > \
    CANTP_STATUS_RECEIVING_FF)

/** \brief If the status of a channle is NOT Idle return TRUE.*/
#define CANTP_STATUS_IS_NOT_IDLE(chn)   (!CANTP_STATUS_IS_IDLE(chn))

/** \brief If the status of a channle is NOT recvSF return TRUE.*/
#define CANTP_STATUS_IS_NOT_RECVSF(chn) (!CANTP_STATUS_IS_RECVSF(chn))
/** \brief If the status of a channle is NOT recvFF return TRUE.*/
#define CANTP_STATUS_IS_NOT_RECVFF(chn) (!CANTP_STATUS_IS_RECVFF(chn))
/** \brief If the status of a channle is NOT recvCF return TRUE.*/
#define CANTP_STATUS_IS_NOT_RECVCF(chn) (!CANTP_STATUS_IS_RECVCF(chn))
/** \brief If the status of a channle is NOT recvFC return TRUE.*/
#define CANTP_STATUS_IS_NOT_RECVFC(chn) (!CANTP_STATUS_IS_RECVFC(chn))
/** \brief If the status of a channle is NOT tranSF return TRUE.*/
#define CANTP_STATUS_IS_NOT_TRANSF(chn) (!CANTP_STATUS_IS_TRANSF(chn))
/** \brief If the status of a channle is NOT tranFF return TRUE.*/
#define CANTP_STATUS_IS_NOT_TRANFF(chn) (!CANTP_STATUS_IS_TRANFF(chn))
/** \brief If the status of a channle is NOT tranCF return TRUE.*/
#define CANTP_STATUS_IS_NOT_TRANCF(chn) (!CANTP_STATUS_IS_TRANCF(chn))
/** \brief If the status of a channle is NOT tranFC return TRUE.*/
#define CANTP_STATUS_IS_NOT_TRANFC(chn) (!CANTP_STATUS_IS_TRANFC(chn))

/** \brief If the sub status of a channle is Idle return TRUE.*/
#define CANTP_SUB_STATUS_IS_IDLE(chn)   (CANTP_SUB_STATUS_IDLE \
    == (chn)->subStatus)
/** \brief If the sub status of a channle is Receiving return TRUE.*/
#define CANTP_SUB_STATUS_IS_RECV(chn)   (CANTP_SUB_STATUS_RECEIVING \
    == (chn)->subStatus)
/** \brief If the sub status of a channle is Transmitting return TRUE.*/
#define CANTP_SUB_STATUS_IS_TRAN(chn)   (CANTP_SUB_STATUS_TRANSMITTING \
    == (chn)->subStatus)

/** \brief If the sub status of a channle is NOT Idle return TRUE.*/
#define CANTP_SUB_STATUS_IS_NOT_IDLE(chn)   (!CANTP_SUB_STATUS_IS_IDLE(chn))
/** \brief If the sub status of a channle is NOT Receiving return TRUE.*/
#define CANTP_SUB_STATUS_IS_NOT_RECV(chn)   (!CANTP_SUB_STATUS_IS_RECV(chn))
/** \brief If the sub status of a channle is NOT Transmitting return TRUE.*/
#define CANTP_SUB_STATUS_IS_NOT_TRAN(chn)   (!CANTP_SUB_STATUS_IS_TRAN(chn))

/** \brief Set the status of a channel to Idle.*/
#define CANTP_STATUS_GOTO_IDLE(chn)     ((chn)->status = CANTP_STATUS_IDLE)
/** \brief Set the status of a channel to recvSF.*/
#define CANTP_STATUS_GOTO_RECVSF(chn)   ((chn)->status = \
        CANTP_STATUS_RECEIVING_SF)
/** \brief Set the status of a channel to recvFF.*/
#define CANTP_STATUS_GOTO_RECVFF(chn)   ((chn)->status = \
        CANTP_STATUS_RECEIVING_FF)
/** \brief Set the status of a channel to recvCF.*/
#define CANTP_STATUS_GOTO_RECVCF(chn)   ((chn)->status = \
        CANTP_STATUS_RECEIVING_CF)
/** \brief Set the status of a channel to recvFC.*/
#define CANTP_STATUS_GOTO_RECVFC(chn)   ((chn)->status = \
        CANTP_STATUS_RECEIVING_FC)

/** \brief Set the status of a channel to tranSF.*/
#define CANTP_STATUS_GOTO_TRANSF(chn)   ((chn)->status = \
        CANTP_STATUS_TRANSMITTING_SF)
/** \brief Set the status of a channel to tranFF.*/
#define CANTP_STATUS_GOTO_TRANFF(chn)   ((chn)->status = \
        CANTP_STATUS_TRANSMITTING_FF)
/** \brief Set the status of a channel to tranCF.*/
#define CANTP_STATUS_GOTO_TRANCF(chn)   ((chn)->status = \
        CANTP_STATUS_TRANSMITTING_CF)
/** \brief Set the status of a channel to tranFC.*/
#define CANTP_STATUS_GOTO_TRANFC(chn)   ((chn)->status = \
        CANTP_STATUS_TRANSMITTING_FC)

/** \brief Set the sub status of a channel to Idle.*/
#define CANTP_SUB_STATUS_GOTO_IDLE(chn) ((chn)->subStatus = \
        CANTP_SUB_STATUS_IDLE)

/** \brief Set the sub status of a channel to receiving.*/
#define CANTP_SUB_STATUS_GOTO_RECV(chn) ((chn)->subStatus = \
        CANTP_SUB_STATUS_RECEIVING)

/** \brief Set the sub status of a channel to transmitting.*/
#define CANTP_SUB_STATUS_GOTO_TRAN(chn) ((chn)->subStatus = \
        CANTP_SUB_STATUS_TRANSMITTING)

/** \brief Set the private data of a channel.*/
#define CANTP_SET_PRIVATE_DATA(chn,_pData) ((chn)->pData = (bl_u8_t)(_pData))

/** \brief Set the BS of a channel.*/
#define CANTP_SET_BS(chn,_bs)       ((chn)->bs = (bl_u8_t)(_bs))
/** \brief Set the STmin of a channel.*/
#define CANTP_SET_STMIN(chn,_st)    ((chn)->st = (bl_u8_t)(_st))
/** \brief Set the maxWFT of a channel.*/
#define CANTP_SET_MAXWFT(chn,_wft)  ((chn)->wft = (bl_u8_t)(_wft))
/** \brief Set the TA type of a channel.*/
#define CANTP_SET_TATYPE(chn,type)  ((chn)->taType = (bl_u8_t)(type))

/** \brief Initialize the BS of a channel by channel configurations.*/
#define CANTP_INIT_BS_BY_CFG(chn,cfg)       CANTP_SET_BS(chn,(cfg)->bs)
/** \brief Initialize the STmin of a channel by channel configurations.*/
#define CANTP_INIT_STMIN_BY_CFG(chn,cfg)    CANTP_SET_STMIN(chn,(cfg)->st)
/** \brief Initialize the maxWFT of a channel by channel configurations.*/
#define CANTP_INIT_MAXWFT_BY_CFG(chn,cfg)   CANTP_SET_MAXWFT(chn,(cfg)->wft)
/** \brief Initialize the TA type of a channel by channel configurations.*/
#define CANTP_INIT_TATYPE_BY_CFG(chn,cfg)   CANTP_SET_TATYPE(chn,(cfg)->taType)

/** \brief Initialize the BS of a channel.*/
#define CANTP_INIT_BS(chn)      CANTP_INIT_BS_BY_CFG((chn),(chn)->chnCfg)
/** \brief Initialize the STmin of a channel.*/
#define CANTP_INIT_STMIN(chn)   CANTP_INIT_STMIN_BY_CFG((chn),(chn)->chnCfg)
/** \brief Initialize the maxWFT of a channel.*/
#define CANTP_INIT_MAXWFT(chn)  CANTP_INIT_MAXWFT_BY_CFG((chn),(chn)->chnCfg)
/** \brief Initialize the TA type of a channel.*/
#define CANTP_INIT_TATYPE(chn)  CANTP_INIT_TATYPE_BY_CFG((chn),(chn)->chnCfg)

/** \brief Set the timeout value of the timer in a channel.*/
#define CANTP_SET_TIMER(chn,timeout)    ((chn)->timer = (bl_u16_t)(timeout))
/** \brief Get the current value of the timer in a channel.*/
#define CANTP_GET_TIMER(chn)            ((chn)->timer)

/** \brief Initialize the timer in a channel.*/
#define CANTP_INIT_TIMER(chn)           CANTP_SET_TIMER(chn,0u)
/** \brief Use the timerA to initialize the timer in a channel.*/
#define CANTP_INIT_TIMER_A(chn) CANTP_SET_TIMER(chn,(chn)->chnCfg->timerA)
/** \brief Use the timerB to initialize the timer in a channel.*/
#define CANTP_INIT_TIMER_B(chn) CANTP_SET_TIMER(chn,(chn)->chnCfg->timerB)
/** \brief Use the timerC to initialize the timer in a channel.*/
#define CANTP_INIT_TIMER_C(chn) CANTP_SET_TIMER(chn,(chn)->chnCfg->timerC)

/** \brief Check if the timer in a channel is timeout.*/
#define CANTP_IS_TIMEOUT(chn)           (0u == CANTP_GET_TIMER(chn))

/** \brief Set the SN of a channel.*/
#define CANTP_SET_SN(chn,_sn)       ((chn)->sn = (bl_u8_t)(_sn))
/** \brief Set the CF counter of a channel.*/
#define CANTP_SET_CFCNT(chn,_cfCnt) ((chn)->cfCnt = (bl_u16_t)(_cfCnt))
/** \brief Set the size of the last CF of a channel.*/
#define CANTP_SET_LAST_SIZE(chn,_lastSize)  ((chn)->lastSize =\
        (bl_u8_t)(_lastSize))
/** \brief Set the total size of a channel.*/
#define CANTP_SET_TOTAL_SIZE(chn,_totalSize)    ((chn)->totalSize =\
        (bl_BufferSize_t)(_totalSize))

/** \brief Set the tx delay of a channel.*/
#define CANTP_SET_TXDELAY(chn,delay)    ((chn)->txDelay = (bl_u8_t)(delay))

/** \brief Initialize the SN of a channel.*/
#define CANTP_INIT_SN(chn)          CANTP_SET_SN(chn,0u)
/** \brief Initialize the CF counter of a channel.*/
#define CANTP_INIT_CFCNT(chn)       CANTP_SET_CFCNT(chn,0u)
/** \brief Initialize the size of the last CF of a channel.*/
#define CANTP_INIT_LAST_SIZE(chn)   CANTP_SET_LAST_SIZE(chn,0u)
/** \brief Initialize the total size of a channel.*/
#define CANTP_INIT_TOTAL_SIZE(chn)  CANTP_SET_TOTAL_SIZE(chn,0u)
/** \brief Initialize the tx delay of a channel.*/
#define CANTP_INIT_TXDELAY(chn)     CANTP_SET_TXDELAY(chn,(chn)->st)
/** \brief Add the SN of a channel.*/
#define CANTP_ADD_SN(chn)            ((chn)->sn += 1u)
/** \brief Check if a channel is functional.*/
#define CANTP_IS_FUNCTIONAL_CHANNEL(chn)    (CANTP_TATYPE_FUNCTIONAL \
    == (chn)->chnCfg->taType)
/** \brief Check if a channel is physical.*/
#define CANTP_IS_PHYSICAL_CHANNEL(chn)      (CANTP_TATYPE_PHYSICAL \
    == (chn)->chnCfg->taType)

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/
/** \brief A alias of the struct _tag_CanTpChannel.*/
typedef struct _tag_CanTpChannel bl_CanTpChannel_t;
/** \brief A alias of the struct _tag_CanTpPciInfo.*/
typedef struct _tag_CanTpPciInfo bl_CanTpPciInfo_t;
/** \brief A alias of the struct _tag_CanTpPeriodInterface.*/
typedef struct _tag_CanTpPeriodInterface bl_CanTpPeriodIF_t;
/** \brief A interface used to process received frame.*/
typedef bl_Return_t (*bl_CanTpRxProcess_t)(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);
/** \brief A interface used to confirm that a frame is transmitted.*/
typedef void (*bl_CanTpTxConfirm_t)(bl_CanTpChannel_t *channel);
/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/
/** \brief The PCI Informations for a can tp channel.*/
struct _tag_CanTpPciInfo {
    bl_u8_t pciPos;     /**< The PCI position in a can frame.*/
    bl_u8_t dataPos;    /**< The valid data position in a SF or CF frame.*/
    bl_u8_t ffDataPos;  /**< The valid data position in a FF frame.*/
    bl_u8_t fcBsPos;    /**< The BS position in a FC frame.*/
    bl_u8_t fcStPos;    /**< The STmin position in a FC frame.*/
    bl_u8_t maxDataSize;/**< The max size of data in a SF or CF frame.*/
    bl_u8_t maxFFDataSize;  /**< The max size of data in a FF frame.*/
    bl_u8_t maxFCDataSize;  /**< The max size of data in a FC frame.*/
};

/** \brief The channel of the CAN TP.*/
struct _tag_CanTpChannel {
    bl_u8_t status;     /**< The current status of a channel.*/
    bl_u8_t subStatus;  /**< The current sub status of a channel.*/
    bl_u8_t pData;      /**< The private data of a channel.*/
    bl_u8_t sn;         /**< The expected SN used to receive CF frames.*/
    bl_u8_t txDelay;    /**< The Delay timer for sending a CF frame.*/
    bl_u8_t st;         /**< The STmin copy from chnCfg->st or a FC frame.*/
    bl_u8_t bs;         /**< The BS copy from chnCfg->bs or a FC frame.*/
    bl_u8_t wft;        /**< The WFT copy from chnCfg->wft.*/
    bl_u8_t taType;      /**< The TA type copy from chnCfg->taType.*/
    bl_u8_t lastSize;   /**< The Size of the last CF or SF frame.*/
    bl_Buffer_t frame[CAN_MAX_SIZE_OF_DATA];  /**< The local frame buffer.*/
    bl_u16_t timer;     /**< The timer*/
    bl_u16_t cfCnt;     /**< The counter of CF frames.*/
    bl_BufferSize_t totalSize; /**< The total size of Tx or Rx.*/
    const struct _tag_CanTpChannelCfg *chnCfg; /**< Channel configurations*/
    const struct _tag_CanTpPciInfo *pciInfo;   /**< PCI information*/
};

/** \brief The period process interface of the CAN TP channel.*/
struct _tag_CanTpPeriodInterface {
    void (*Period)(bl_CanTpChannel_t *channel); /**< period of status.*/
    bl_Return_t (*Timeout)(bl_CanTpChannel_t *channel);/**< timeout of status*/
};

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
/** \brief Use the configuration to initialize a cantp channel.*/
static void _Cantp_InitChannel(bl_CanTpChannel_t *channel,
    const bl_CanTpChannelCfg_t *channelCfg);
/** \brief Use the Rx handle of the canif module to get a channel.*/
static bl_CanTpChannel_t *_Cantp_GetChannelByRxId(bl_ComIfHandle_t handle,
    bl_u16_t chnNum,
    bl_CanTpChannel_t *chnList);
/** \brief Use the size to set the CF counter and last size of a channel.*/
static void _Cantp_SetMultipleFrameSize(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size);

/** \brief Get the STmin to used to transmit CF.*/
static bl_u8_t _Cantp_GetSTMinFromFC(bl_u8_t st);

/** \brief The period function.*/
static void _Cantp_PeriodFunction(bl_u16_t num,
    bl_CanTpChannel_t *channelList,
    const bl_CanTpPeriodIF_t *periodList);
/** \brief The Rx indication function used by Rx channel.*/
static bl_Return_t _Cantp_RxIndToRxChannel(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);

/** \brief The Rx indication function used by Tx channel.*/
static bl_Return_t _Cantp_RxIndToTxChannel(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);

/** \brief Transmit a CF.*/
static void _Cantp_TransmitCF(bl_CanTpChannel_t *channel);
/** \brief Make the PCI of SF into the local frame.*/
static void _Cantp_MakePciOfSF(bl_CanTpChannel_t *channel);
/** \brief Make the PCI of FF into the local frame.*/
static void _Cantp_MakePciOfFF(bl_CanTpChannel_t *channel);
/** \brief Make the PCI of CF into the local frame.*/
static void _Cantp_MakePciOfCF(bl_CanTpChannel_t *channel);
/** \brief Make the PCI of FC into the local frame.*/
static void _Cantp_MakePciOfFC(bl_CanTpChannel_t *channel);
/** \brief The Tx comfrimation function used for the Idle status of a channel.*/
static void _Cantp_TxConfirmIdle(bl_CanTpChannel_t *channel);
/** \brief The Tx comfrimation function used for the tranSF status of a channel.*/
static void _Cantp_TxConfirmSF(bl_CanTpChannel_t *channel);
/** \brief The Tx comfrimation function used for the tranFF status of a channel.*/
static void _Cantp_TxConfirmFF(bl_CanTpChannel_t *channel);
/** \brief The Tx comfrimation function used for the tranCF status of a channel.*/
static void _Cantp_TxConfirmCF(bl_CanTpChannel_t *channel);
/** \brief The Tx comfrimation function used for the tranFC status of a channel.*/
static void _Cantp_TxConfirmFC(bl_CanTpChannel_t *channel);
/** \brief The Change the status of a channel to Idle.*/
static void _Cantp_GotoIdle(bl_CanTpChannel_t *channel);
/** \brief The Change the status of a channel to TranSF.*/
static void _Cantp_GotoTranSF(bl_CanTpChannel_t *channel);
/** \brief The Change the status of a channel to TranFF.*/
static void _Cantp_GotoTranFF(bl_CanTpChannel_t *channel);
/** \brief The Change the status of a channel to TranCF.*/
static void _Cantp_GotoTranCF(bl_CanTpChannel_t *channel);
/** \brief The Change the status of a channel to TranFC.*/
static void _Cantp_GotoTranFC(bl_CanTpChannel_t *channel, bl_u8_t fs);
/** \brief Receive a Single Frame.*/
static bl_Return_t _Cantp_ReceiveSF(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);
/** \brief Receive a First Frame.*/
static bl_Return_t _Cantp_ReceiveFF(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);
/** \brief Receive a Consecutive Frame.*/
static bl_Return_t _Cantp_ReceiveCF(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);
/** \brief Receive a Flow Control Frame.*/
static bl_Return_t _Cantp_ReceiveFC(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer);

/** \brief The period function of the Idle status of a channel.*/
static void _Cantp_PeriodIdle(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the Idle status of a channel.*/
static bl_Return_t _Cantp_TimeoutIdle(bl_CanTpChannel_t *channel);
/** \brief The period function of the RecvSF status of a channel.*/
static void _Cantp_PeriodRecvSF(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the RecvSF status of a channel.*/
static bl_Return_t _Cantp_TimeoutRecvSF(bl_CanTpChannel_t *channel);
/** \brief The period function of the RecvFF status of a channel.*/
static void _Cantp_PeriodRecvFF(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the RecvFF status of a channel.*/
static bl_Return_t _Cantp_TimeoutRecvFF(bl_CanTpChannel_t *channel);
/** \brief The period function of the RecvCF status of a channel.*/
static void _Cantp_PeriodRecvCF(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the RecvCF status of a channel.*/
static bl_Return_t _Cantp_TimeoutRecvCF(bl_CanTpChannel_t *channel);
/** \brief The period function of the TranFC status of a channel.*/
static void _Cantp_PeriodTranFC(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the TranFC status of a channel.*/
static bl_Return_t _Cantp_TimeoutTranFC(bl_CanTpChannel_t *channel);
/** \brief The period function of the TranSF status of a channel.*/
static void _Cantp_PeriodTranSF(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the TranSF status of a channel.*/
static bl_Return_t _Cantp_TimeoutTranSF(bl_CanTpChannel_t *channel);
/** \brief The period function of the TranFF status of a channel.*/
static void _Cantp_PeriodTranFF(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the TranFF status of a channel.*/
static bl_Return_t _Cantp_TimeoutTranFF(bl_CanTpChannel_t *channel);
/** \brief The period function of the TranCF status of a channel.*/
static void _Cantp_PeriodTranCF(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the TranCF status of a channel.*/
static bl_Return_t _Cantp_TimeoutTranCF(bl_CanTpChannel_t *channel);
/** \brief The period function of the RecvFC status of a channel.*/
static void _Cantp_PeriodRecvFC(bl_CanTpChannel_t *channel);
/** \brief The timeout function of the RecvFC status of a channel.*/
static bl_Return_t _Cantp_TimeoutRecvFC(bl_CanTpChannel_t *channel);

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
#if 0
/** \brief The PCI Informations for a can tp channel.*/
struct _tag_CanTpPciInfo {
    bl_u8_t pciPos;     /**< The PCI position in a can frame.*/
    bl_u8_t dataPos;    /**< The valid data position in a SF or CF frame.*/
    bl_u8_t ffDataPos;  /**< The valid data position in a FF frame.*/
    bl_u8_t fcBsPos;    /**< The BS position in a FC frame.*/
    bl_u8_t fcStPos;    /**< The STmin position in a FC frame.*/
    bl_u8_t maxDataSize;/**< The max size of data in a SF or CF frame.*/
    bl_u8_t maxFFDataSize;  /**< The max size of data in a FF frame.*/
    bl_u8_t maxFCDataSize;  /**< The max size of data in a FC frame.*/
};
#endif
/** \brief The PCI used to standard, extanded or mixed channel.*/
STATIC const bl_CanTpPciInfo_t gs_CanTpPciInfo[CANTP_NUMBER_OF_PCI_INFO] = {
    /*The channel type is CANTP_TYPE_STANDARD.*/
    {
        0, 1, 2, 1, 2, CANTP_MAX_FRAME_SIZE - 1u, CANTP_MAX_FRAME_SIZE - 2u, 3u // CANTP_MAX_FRAME_SIZE  原来的 0,1,2,1,2,7,6,3
    },
    /*The channel type is CANTP_TYPE_EXTENDED or CANTP_TYPE_MIXED.*/
    {
        1, 2, 3, 2, 3, CANTP_MAX_FRAME_SIZE - 2u, CANTP_MAX_FRAME_SIZE - 3u, 4u // 1,2,3,2,3,6,5,4
    },
};

/** \brief The period schedule interface of the Rx channel.*/
STATIC const bl_CanTpPeriodIF_t gs_RxPeriodList[CANTP_NUMBER_OF_RX_STATUS] = {
    {
        &_Cantp_PeriodIdle, &_Cantp_TimeoutIdle
    },
    {
        &_Cantp_PeriodRecvSF, &_Cantp_TimeoutRecvSF
    },
    {
        &_Cantp_PeriodRecvFF, &_Cantp_TimeoutRecvFF
    },
    {
        &_Cantp_PeriodRecvCF, &_Cantp_TimeoutRecvCF
    },
    {
        &_Cantp_PeriodTranFC, &_Cantp_TimeoutTranFC
    },
};

/** \brief The period schedule interface of the Tx channel.*/
STATIC const bl_CanTpPeriodIF_t gs_TxPeriodList[CANTP_NUMBER_OF_TX_STATUS] = {
    {
        &_Cantp_PeriodIdle, &_Cantp_TimeoutIdle
    },
    {
        &_Cantp_PeriodTranSF, &_Cantp_TimeoutTranSF
    },
    {
        &_Cantp_PeriodTranFF, &_Cantp_TimeoutTranFF
    },
    {
        &_Cantp_PeriodTranCF, &_Cantp_TimeoutTranCF
    },
    {
        &_Cantp_PeriodRecvFC, &_Cantp_TimeoutRecvFC
    },
};
/** \brief The Tx confirmation interface of a channel.*/
STATIC const bl_CanTpTxConfirm_t gs_TxConfirmList[CANTP_NUMBER_OF_TX_STATUS] = {
    &_Cantp_TxConfirmIdle,
    &_Cantp_TxConfirmSF,
    &_Cantp_TxConfirmFF,
    &_Cantp_TxConfirmCF,
    &_Cantp_TxConfirmFC
};

/** \brief The Rx process interface of a channel.*/
STATIC const bl_CanTpRxProcess_t gs_RxProcessList[CANTP_NUMBER_OF_FRAME_TYPE] = {
    &_Cantp_ReceiveSF,
    &_Cantp_ReceiveFF,
    &_Cantp_ReceiveCF,
    &_Cantp_ReceiveFC
};


/** \brief The Rx channel list.*/
STATIC bl_CanTpChannel_t gs_CanTpRxChannel[CANTP_NUMBER_OF_RX_CHANNEL];
/** \brief The Tx channel list.*/
STATIC bl_CanTpChannel_t gs_CanTpTxChannel[CANTP_NUMBER_OF_TX_CHANNEL];

/** \brief The transmitting channel list.*/
STATIC bl_CanTpChannel_t *gs_TransmittingChannel[CANTP_NUMBER_OF_TX_CHANNEL];

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details    Initialize the can TP module.
 *
 *  \return None.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Cantp_Init(void) {
    bl_CanTpHandle_t handle;
    bl_CanTpChannel_t *channel;
    const bl_CanTpChannelCfg_t *channelCfg;
    
    /*Initialize the RX channels*/
    for (handle = 0u; handle < CANTP_NUMBER_OF_RX_CHANNEL; handle++) {
        channel = &gs_CanTpRxChannel[handle];
        channelCfg = &g_CanTpRxChnsCfg[handle];
        _Cantp_InitChannel(channel, channelCfg);
    }
    
    /*Initialize the TX channels*/
    for (handle = 0u; handle < CANTP_NUMBER_OF_TX_CHANNEL; handle++) {
        channel = &gs_CanTpTxChannel[handle];
        channelCfg = &g_CanTpTxChnsCfg[handle];
        _Cantp_InitChannel(channel, channelCfg);
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details Transmit the data.
 *
 *  \param[in]  handle - Tx handle.
 *  \param[in]  size - the size of the data.
 *
 *  \return None.
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
bl_Return_t Cantp_Transmit(bl_CanTpHandle_t handle, bl_BufferSize_t size) {
    bl_Return_t  ret = BL_ERR_NOT_OK;
    bl_CanTpChannel_t *channel;
    const bl_CanTpPciInfo_t *pci;
    
    BL_DEBUG_ASSERT_PARAM(handle < CANTP_NUMBER_OF_TX_CHANNEL);
    BL_DEBUG_ASSERT_PARAM(size != 0);
    /* -----------------------------------------------------------------------A12
    channel 指向 gs_CanTpTxChannel 
    ----------------------------------------------------------------------- */
    channel = &gs_CanTpTxChannel[handle];
    if (CANTP_STATUS_IS_IDLE(channel)) {
        pci = channel->pciInfo;
        /* -----------------------------------------------------------------------A12
        pci->maxDataSize = 7
        ----------------------------------------------------------------------- */
        if (size > pci->maxDataSize) {
            if (CANTP_IS_PHYSICAL_CHANNEL(channel)) {
                _Cantp_SetMultipleFrameSize(channel, size);
                _Cantp_GotoTranFF(channel);
                
                ret = BL_ERR_OK;
            }
        } else {
            channel->lastSize = (bl_u8_t)size;
            _Cantp_GotoTranSF(channel);
            
            ret = BL_ERR_OK;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details    If a tx channel wait for receiving a FC frame, The tx channel
 *              process this frame. otherwise the rx channel process it.
 *
 *  \param[in]  handle - rx handle of the canif.
 *  \param[in]  size - the size of the data.
 *  \param[in]  buffer - the content of the data.
 *
 *  \return None.
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
void Cantp_RxIndication(bl_ComIfHandle_t handle,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    bl_CanTpChannel_t *channel;
    bl_Return_t ret;
    
    /*Check whether the parameters are valid.*/
    #if (CANTP_FUN_FRAME_PADDING == BL_FUN_ON)
    if ((CANTP_MAX_FRAME_SIZE == size) && (buffer != BL_NULL_PTR))
    #else
    if ((size > 0) && (buffer != BL_NULL_PTR))
    #endif
    {
        channel = _Cantp_GetChannelByRxId(handle,
                CANTP_NUMBER_OF_TX_CHANNEL,
                gs_CanTpTxChannel);
        ret = _Cantp_RxIndToTxChannel(channel, size, buffer);
        if (ret != BL_ERR_OK) { /*the Tx channel is not process this frame.*/
            channel = _Cantp_GetChannelByRxId(handle,
                    CANTP_NUMBER_OF_RX_CHANNEL,
                    gs_CanTpRxChannel);
            (void)_Cantp_RxIndToRxChannel(channel, size, buffer);
            
        }
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    If a channel wait for a confirmation, call the confirmation
 *              function of this channel.
 *
 *  \param[in]  handle - tx handle of the canif.
 *
 *  \return None.
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
void Cantp_TxConfirmation(bl_ComIfHandle_t handle) {
    bl_CanTpChannel_t *channel;
    bl_CanTpTxConfirm_t confirm;
    bl_u16_t i;
    
    for (i = 0u; i < CANTP_NUMBER_OF_TX_CHANNEL; i++) {
        channel = gs_TransmittingChannel[i];
        if ((channel != BL_NULL_PTR) && (channel->chnCfg->txId == handle)) {
            BL_DEBUG_ASSERT_NO_RET(channel->status < CANTP_NUMBER_OF_TX_STATUS);
            
            confirm = gs_TxConfirmList[channel->status];
            
            confirm(channel);
            
            gs_TransmittingChannel[i] = BL_NULL_PTR;
            
            break;
        }
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    When the timer of a channel is not timeout, call the period
 *              function. If the timer is timeout, call the timeout function.
 *
 *  \return None.
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
void Cantp_PeriodFunction(void) {
    _Cantp_PeriodFunction(CANTP_NUMBER_OF_RX_CHANNEL,
        gs_CanTpRxChannel,
        gs_RxPeriodList);
        
    _Cantp_PeriodFunction(CANTP_NUMBER_OF_TX_CHANNEL,
        gs_CanTpTxChannel,
        gs_TxPeriodList);
}

/**************************************************************************//**
 *
 *  \details Initialize the channel of the cantp module.
 *
 *  \param[in]  channel     - the pointer of a cantp channel.
 *  \param[in]  channelCfg  - the pointer of a cantp channel cfg in ROM.
 *
 *  \return None.
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
static void _Cantp_InitChannel(bl_CanTpChannel_t *channel,
    const bl_CanTpChannelCfg_t *channelCfg) {
    BL_DEBUG_ASSERT_NO_RET(channel != BL_NULL_PTR);
    BL_DEBUG_ASSERT_NO_RET(channelCfg != BL_NULL_PTR);
    
    CANTP_STATUS_GOTO_IDLE(channel);
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    
    CANTP_INIT_STMIN_BY_CFG(channel, channelCfg);
    CANTP_INIT_BS_BY_CFG(channel, channelCfg);
    CANTP_INIT_MAXWFT_BY_CFG(channel, channelCfg);
    CANTP_INIT_TATYPE_BY_CFG(channel, channelCfg);
    
    CANTP_INIT_TIMER(channel);
    
    if (CANTP_TYPE_STANDARD == channelCfg->type) {
        channel->pciInfo = &gs_CanTpPciInfo[CANTP_STANDARD_PCI_INFO];
    } else {
        channel->pciInfo = &gs_CanTpPciInfo[CANTP_EXTENDED_PCI_INFO];
    }
    
    channel->chnCfg = channelCfg;
    return ;
}

/**************************************************************************//**
 *
 *  \details Get the cantp channel by the receiving handle.
 *
 *  \param[in]  handle  - the receiving handle of the canif moudle.
 *  \param[in]  chnNum  - the number of channels.
 *  \param[in]  chnList - the channels
 *
 *  \return If the rxId of a channel equal to the handle then return a pointer
 *          of the channel, otherwise returns BL_NULL_PTR.
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
static bl_CanTpChannel_t *_Cantp_GetChannelByRxId(bl_ComIfHandle_t handle,
    bl_u16_t chnNum,
    bl_CanTpChannel_t *chnList) {
    bl_CanTpChannel_t *channel = BL_NULL_PTR;
    bl_u16_t i;
    
    for (i = 0; i < chnNum; i++) {
        if (chnList[i].chnCfg->rxId == handle) {
            channel = &chnList[i];
            break;
        }
    }
    
    return channel;
}

/**************************************************************************//**
 *
 *  \details calculate the CF count and the size of last CF by the total size
 *           of transmitting or receiving data.
 *
 *  \param[in]  channel - the pointer of a cantp channel.
 *  \param[in]  size    - the total size of transmitting or receiving data.
 *
 *  \return None.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_SetMultipleFrameSize(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size) {
    bl_BufferSize_t tmpSize;
    bl_u16_t tmpCnt;
    const bl_CanTpPciInfo_t *pci = channel->pciInfo;
    
    BL_DEBUG_ASSERT_NO_RET(size > pci->maxDataSize);
    BL_DEBUG_ASSERT_NO_RET(pci->maxDataSize != 0);
    
    tmpCnt = size / pci->maxDataSize;
    
    tmpSize = (size % pci->maxDataSize) + 1u;
    
    CANTP_SET_TOTAL_SIZE(channel, size);
    CANTP_SET_LAST_SIZE(channel, tmpSize);
    CANTP_SET_CFCNT(channel, tmpCnt);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Calculate the STmin according to ISO 15765-2
 *
 *  \param[in]  st - The STmin is received from the tester.
 *
 *  \return the STmin used by cantp channel.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_u8_t _Cantp_GetSTMinFromFC(bl_u8_t st) {
    bl_u8_t retSTMin;
    
    if (st > CANTP_MAX_STMIN_VALUE) {
        if ((st > CANTP_MIN_STMIN_VALUE_US) && (st < CANTP_MAX_STMIN_VALUE_US)) {
            retSTMin = CANTP_SCHEDULE_PERIOD;
        } else {
            #if (CANTP_SCHEDULE_PERIOD == 1)
            retSTMin = CANTP_MAX_STMIN_VALUE;
            #else
            retSTMin = (CANTP_MAX_STMIN_VALUE / CANTP_SCHEDULE_PERIOD) + 1;
            #endif
        }
        
    } else {
        #if (CANTP_SCHEDULE_PERIOD == 1)
        retSTMin = st;
        #else
        retSTMin = (st / CANTP_SCHEDULE_PERIOD) + 1;
        #endif
        
    }
    
    return retSTMin;
}

/**************************************************************************//**
 *
 *  \details the period function of the cantp module.
 *
 *  \param[in]  num - The number of the channels in the list.
 *  \param[in]  channelList - the list of the channels.
 *  \param[in]  periodList - the list of the period and timeout interface of
 *                           the channel.
 *
 *  \return None.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Cantp_PeriodFunction(bl_u16_t num,
    bl_CanTpChannel_t *channelList,
    const bl_CanTpPeriodIF_t *periodList) {
    bl_CanTpChannel_t *channel;
    bl_Return_t ret;
    bl_u16_t i;
    
    for (i = 0; i < num; i++) {
        channel = &channelList[i];
        if (CANTP_IS_TIMEOUT(channel)) {
            ret = periodList[channel->status].Timeout(channel);
            if (BL_ERR_OK == ret) {
                _Cantp_GotoIdle(channel);
            }
        } else {
            channel->timer -= 1u;
            periodList[channel->status].Period(channel);
        }
    }
}

/**************************************************************************//**
 *
 *  \details Indicate a rx channel to receive a frame data.
 *
 *  \param[in]  channel - the pointer of a rx channel.
 *  \param[in]  size - the size of received data
 *  \param[in]  buffer - the contents of received data.
 *
 *  \return If the received data can be processed by the rx channel, returns
 *          the BL_ERR_OK. Otherwise returns BL_ERR_NOT_OK.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_RxIndToRxChannel(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    bl_CanTpRxProcess_t processor;
    bl_u8_t frameType;
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (channel != BL_NULL_PTR) {
        if ((channel->chnCfg->type == CANTP_TYPE_STANDARD)
            || (channel->chnCfg->ta == buffer[CANTP_TA_OFFSET])) {
            frameType = CANTP_GET_FRAME_TYPE(channel->pciInfo, buffer); /*PRQA S 2985*/
            if (frameType < CANTP_FRAME_TYPE_FC) {
                processor = gs_RxProcessList[frameType];
                ret = processor(channel, size, buffer);
            }
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Indicate a tx channel to receive a frame data.
 *
 *  \param[in]  channel - the pointer of a tx channel.
 *  \param[in]  size - the size of received data
 *  \param[in]  buffer - the contents of received data.
 *
 *  \return If the received data can be processed by the tx channel, returns
 *          the BL_ERR_OK. Otherwise returns BL_ERR_NOT_OK.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_RxIndToTxChannel(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    bl_CanTpRxProcess_t processor;
    bl_u8_t frameType;
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (channel != BL_NULL_PTR) {
        if ((channel->chnCfg->type == CANTP_TYPE_STANDARD)
            || (channel->chnCfg->ta == buffer[CANTP_TA_OFFSET])) {
            frameType = CANTP_GET_FRAME_TYPE(channel->pciInfo, buffer); /*PRQA S 2985*/
            if (frameType == CANTP_FRAME_TYPE_FC) {
                processor = gs_RxProcessList[frameType];
                ret = processor(channel, size, buffer);
            }
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details    receive a frame according to the Single Frame in
 *              the ISO 15765-2.
 *
 *  \param[in]  channel - the pointer of a channel.
 *  \param[in]  size - the size of received data
 *  \param[in]  buffer - the contents of received data.
 *
 *  \return If the received data can be processed by the channel, returns
 *          the BL_ERR_OK. Otherwise returns BL_ERR_NOT_OK.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_ReceiveSF(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    const bl_CanTpPciInfo_t *pci;
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u8_t tmpSize;
    bl_u8_t l_offset = 0u;
    
    pci = channel->pciInfo;
    #if 0
    #if CANTP_MAX_FRAME_SIZE  <= 8
    l_offset = 0u;
    tmpSize = CANTP_GET_SF_DATASIZE(buffer[pci->pciPos]);
    #else
    l_offset = 1u;
    tmpSize = (buffer[pci->pciPos + l_offset]); //如果是超過8的
    #endif
    #else
    tmpSize = CANTP_GET_SF_DATASIZE(buffer[pci->pciPos]);
    if (tmpSize == 0) { //CANFD
        l_offset = 1u;
        tmpSize = (buffer[pci->pciPos + l_offset]); //如果是超過8的
    } else {
        l_offset = 0u;
        //  tmpSize
    }
    #endif
    if ((tmpSize != 0) && (tmpSize <= pci->maxDataSize) && (tmpSize < size)) {
        /*  When continuous SF is received in one channel during
            a timeout period,It maybe break other channel.
            So If a channel do not get the buffer from DCM module,
            do NOT indicate.*/
        if (CANTP_IS_GETTING_BUFFER(channel))
        
        {
            Dcm_RxIndication(channel->taType, BL_ERR_UNEXPECTED_FRAME);
        }
        
        Bl_MemCpy(channel->frame, &buffer[pci->dataPos + l_offset], (bl_Size_t)tmpSize);
        
        channel->lastSize = tmpSize;
        
        CANTP_STATUS_GOTO_RECVSF(channel);
        CANTP_INIT_TIMER_B(channel);
        
        ret = BL_ERR_OK;
    }
    
    return ret;
}
/**************************************************************************//**
 *
 *  \details    receive a frame according to the First Frame in
 *              the ISO 15765-2.
 *
 *  \param[in]  channel - the pointer of a channel.
 *  \param[in]  size - the size of received data
 *  \param[in]  buffer - the contents of received data.
 *
 *  \return If the received data can be processed by the channel, returns
 *          the BL_ERR_OK. Otherwise returns BL_ERR_NOT_OK.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_ReceiveFF(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    const bl_CanTpPciInfo_t *pci;
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_BufferSize_t totalSize;
    
    if (CANTP_IS_PHYSICAL_CHANNEL(channel)) {
        pci = channel->pciInfo;
        
        totalSize = CANTP_GET_FF_DATASIZE(&buffer[pci->pciPos]);
        #if (CANTP_FUN_FRAME_PADDING == BL_FUN_ON)
        if (totalSize > pci->maxDataSize)
        #else
        if ((totalSize > pci->maxDataSize) && (CANTP_MAX_FRAME_SIZE == size))
        #endif
        {
            /*  When continuous FF is received in one channel during
                a timeout period,It maybe break other physical channel.
                So If a channel do not get the buffer from DCM module,
                do NOT indicate.*/
            if (CANTP_IS_GETTING_BUFFER(channel)) {
                Dcm_RxIndication(channel->taType, BL_ERR_UNEXPECTED_FRAME);
            }
            
            Bl_MemCpy(channel->frame,
                &buffer[pci->ffDataPos],
                (bl_Size_t)(size - (bl_Size_t)pci->pciPos));
                
            _Cantp_SetMultipleFrameSize(channel, totalSize);
            
            CANTP_INIT_SN(channel);
            CANTP_INIT_MAXWFT_BY_CFG(channel, channel->chnCfg);
            CANTP_STATUS_GOTO_RECVFF(channel);
            CANTP_INIT_TIMER_B(channel);
            
            ret = BL_ERR_OK;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details    receive a frame according to the Consecutive Frame in
 *              the ISO 15765-2.
 *
 *  \param[in]  channel - the pointer of a channel.
 *  \param[in]  size - the size of received data
 *  \param[in]  buffer - the contents of received data.
 *
 *  \return If the received data can be processed by the channel, returns
 *          the BL_ERR_OK. Otherwise returns BL_ERR_NOT_OK.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_ReceiveCF(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    const bl_CanTpPciInfo_t *pci;
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u8_t expectedSN;
    bl_u8_t recvSN;
    bl_u8_t tmpSize;
    
    pci = channel->pciInfo;
    
    do {
        if (CANTP_STATUS_IS_NOT_RECVCF(channel)
            || (0u == channel->cfCnt)) { /*avoid unwanted CF*/
            break;
        }
        #if (CANTP_FUN_FRAME_PADDING == BL_FUN_OFF)
        if (1 == channel->cfCnt) {      /*the last CF*/
            if (size < channel->lastSize) {
                break;
            }
        } else {
            if (size < CANTP_MAX_FRAME_SIZE) {
                break;
            }
        }
        #else
        (void)size;
        #endif
        
        expectedSN = BL_GET_LOW_HALF(channel->sn + 1u);
        recvSN = CANTP_GET_CF_SN(pci, buffer);
        if (recvSN == expectedSN) {
            channel->sn = expectedSN;
        } else {
            Dcm_RxIndication(channel->taType, BL_ERR_WRONG_SN);
            _Cantp_GotoIdle(channel);
            break;
        }
        
        channel->cfCnt -= 1u;
        if (0u == channel->cfCnt) {
            tmpSize = channel->lastSize;
        } else {
            tmpSize = pci->maxDataSize;
        }
        
        /*Immediately copy data to buffer avoid continuous CF during a period*/
        ret = Dcm_CopyRxData(tmpSize, &buffer[pci->dataPos]);
        if (BL_ERR_OK == ret) {
            if (channel->bs != 0u) {
                channel->bs -= 1u;
                if (0u == channel->bs) {
                    _Cantp_GotoTranFC(channel, CANTP_FC_FRAME_CTS);
                }
            } else {
                CANTP_INIT_TIMER_C(channel);
            }
        } else {
            BL_DEBUG_ASSERT_NO_RET(0u);
            Dcm_RxIndication(channel->taType, BL_ERR_NOT_OK);
            _Cantp_GotoIdle(channel);
        }
    } while (0); /*lint !e717*/
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details    receive a frame according to the Flow Control Frame in
 *              the ISO 15765-2.
 *
 *  \param[in]  channel - the pointer of a channel.
 *  \param[in]  size - the size of received data
 *  \param[in]  buffer - the contents of received data.
 *
 *  \return If the received data can be processed by the channel, returns
 *          the BL_ERR_OK. Otherwise returns BL_ERR_NOT_OK.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_ReceiveFC(bl_CanTpChannel_t *channel,
    bl_BufferSize_t size,
    const bl_Buffer_t *buffer) {
    const bl_CanTpPciInfo_t *pci;
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u8_t fs;
    bl_u8_t tmpSt;
    pci = channel->pciInfo;
    
    /*if the status of this channel is not waiting for FC*/
    #if (CANTP_FUN_FRAME_PADDING == BL_FUN_OFF)
    if (CANTP_STATUS_IS_RECVFC(channel) && (size < pci->maxFCDataSize))
    #else
    (void)size;
    
    if (CANTP_STATUS_IS_RECVFC(channel))
    #endif
    {
        fs = CANTP_GET_FC_FS(channel->pciInfo, buffer);
        
        switch (fs) {
            case CANTP_FC_FRAME_CTS:
                channel->bs = CANTP_GET_FC_BS(channel->pciInfo, buffer);
                tmpSt = CANTP_GET_FC_STMIN(pci, buffer);
                channel->st = _Cantp_GetSTMinFromFC(tmpSt);
                
                _Cantp_GotoTranCF(channel);
                break;
            case CANTP_FC_FRAME_WAIT:
                CANTP_INIT_TIMER_B(channel);
                break;
            case CANTP_FC_FRAME_OVERFLOW:
                Dcm_TxConfirmation(BL_ERR_BUFFER_OVERFLOW);
                _Cantp_GotoIdle(channel);
                break;
            default:
                Dcm_TxConfirmation(BL_ERR_INVALID_FS);
                _Cantp_GotoIdle(channel);
                break;
        }
        
        ret = BL_ERR_OK;
    }
    
    return ret;
    
}

/**************************************************************************//**
 *
 *  \details    transmit a frame according to the Flow Control Frame in
 *              the ISO 15765-2.
 *              If the frame is not transmitted, try again during the next
 *              period.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Cantp_TransmitCF(bl_CanTpChannel_t *channel) {
    bl_Return_t ret;
    bl_u8_t dataPos;
    bl_BufferSize_t dataSize;
    bl_BufferSize_t frameSize;
    bl_Buffer_t *frame;
    bl_ComIfHandle_t handle;
    bl_u16_t cfCounter;
    
    _Cantp_MakePciOfCF(channel);
    
    cfCounter = channel->cfCnt - 1u;
    if (0u == cfCounter) {
        dataSize = channel->lastSize;
    } else {
        dataSize = channel->pciInfo->maxDataSize;
    }
    
    frame = channel->frame;
    dataPos = channel->pciInfo->dataPos;
    handle = channel->chnCfg->txId;
    frameSize = dataSize + dataPos;
    
    BL_DEBUG_ASSERT_NO_RET(dataSize != 0);
    BL_DEBUG_ASSERT_NO_RET(frameSize <= CANTP_MAX_FRAME_SIZE);
    
    ret = Dcm_CopyTxData(dataSize, &frame[dataPos]);
    
    BL_DEBUG_ASSERT_NO_RET(BL_ERR_OK == ret);
    
    #if (CANTP_FUN_FRAME_PADDING == BL_FUN_ON)
    Bl_MemSet(&frame[frameSize],
        CANTP_FRAME_PADDING_VALUE,
        (bl_Size_t)((bl_Size_t)CANTP_MAX_FRAME_SIZE - frameSize));
        
    ret = Canif_Write(handle, CANTP_MAX_FRAME_SIZE, frame);
    #else
    ret = Canif_Write(handle, frameSize, frame);
    #endif
    
    
    if (BL_ERR_OK == ret) {
        channel->cfCnt = cfCounter;
        CANTP_SUB_STATUS_GOTO_TRAN(channel);
        gs_TransmittingChannel[handle] = channel;
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Make the PCI information according to Single Frame and save it
 *              to the local frame of the channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/

static void _Cantp_MakePciOfSF(bl_CanTpChannel_t *channel) {
    const bl_CanTpPciInfo_t *pci = channel->pciInfo;
    bl_Buffer_t *frame = channel->frame;
    
    if (channel->chnCfg->type != CANTP_TYPE_STANDARD) {
        frame[CANTP_TA_OFFSET] = channel->chnCfg->ta;
    }
    
    #if CANTP_MAX_FRAME_SIZE <=8
    frame[pci->pciPos] = (channel->lastSize);    //这里要不要取低4位，应该不用  BL_GET_LOW_HALF(channel->lastSize)
    #else
    frame[pci->pciPos] = 0x00;
    frame[pci->pciPos + 1u] = (channel->lastSize);
    #endif
    return ;
}

/**************************************************************************//**
 *
 *  \details    Make the PCI information according to First Frame and save it
 *              to the local frame of the channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_MakePciOfFF(bl_CanTpChannel_t *channel) {
    const bl_CanTpPciInfo_t *pci = channel->pciInfo;
    bl_Buffer_t *frame = channel->frame;
    bl_BufferSize_t totalSize = channel->totalSize;
    
    if (channel->chnCfg->type != CANTP_TYPE_STANDARD) {
        frame[CANTP_TA_OFFSET] = channel->chnCfg->ta;
    }
    
    totalSize = (totalSize & CANTP_FRAME_FF_DATASIZE_MASK)
        + CANTP_FRAME_FF_VAULE_16BITS;
    BL_MCU_TO_BE16(&frame[pci->pciPos], totalSize);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Make the PCI information according to Consecutive Frame and
 *              save it to the local frame of the channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_MakePciOfCF(bl_CanTpChannel_t *channel) {
    const bl_CanTpPciInfo_t *pci = channel->pciInfo;
    bl_Buffer_t *frame = channel->frame;
    
    if (channel->chnCfg->type != CANTP_TYPE_STANDARD) {
        frame[CANTP_TA_OFFSET] = channel->chnCfg->ta;
    }
    
    frame[pci->pciPos] = CANTP_FRAME_CF_VALUE + BL_GET_LOW_HALF(channel->sn);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Make the PCI information according to Flow Control Frame and
 *              save it to the local frame of the channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_MakePciOfFC(bl_CanTpChannel_t *channel) {
    const bl_CanTpPciInfo_t *pci = channel->pciInfo;
    bl_Buffer_t *frame = channel->frame;
    
    if (channel->chnCfg->type != CANTP_TYPE_STANDARD) {
        frame[CANTP_TA_OFFSET] = channel->chnCfg->ta;
    }
    
    frame[pci->pciPos] = CANTP_FRAME_FC_VALUE
        + BL_GET_LOW_HALF(channel->pData);
    frame[pci->fcBsPos] = channel->bs;
    frame[pci->fcStPos] = channel->st;
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The tx confirm function is used for the idle statue of channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Cantp_TxConfirmIdle(bl_CanTpChannel_t *channel) {
    (void)channel;
    
    BL_DEBUG_ASSERT_NO_RET(CANTP_STATUS_IS_NOT_IDLE(channel));
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The tx confirm function is used for the tranSF statue of
 *              channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Cantp_TxConfirmSF(bl_CanTpChannel_t *channel) {
    Dcm_TxConfirmation(BL_ERR_OK);
    
    _Cantp_GotoIdle(channel);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The tx confirm function is used for the tranFF statue of
 *              channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Cantp_TxConfirmFF(bl_CanTpChannel_t *channel) {
    CANTP_ADD_SN(channel);  /*the First Frame use first SN.*/
    CANTP_STATUS_GOTO_RECVFC(channel);
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    CANTP_INIT_TIMER_B(channel);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The tx confirm function is used for the tranCF statue of
 *              channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Cantp_TxConfirmCF(bl_CanTpChannel_t *channel) {
    BL_DEBUG_ASSERT_NO_RET(CANTP_SUB_STATUS_IS_TRAN(channel));
    
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    CANTP_ADD_SN(channel);
    
    if (channel->cfCnt == 0u) {
        Dcm_TxConfirmation(BL_ERR_OK);
        _Cantp_GotoIdle(channel);
    } else {
        if (channel->bs != 0u) {
            channel->bs -= 1u;
            if (0u == channel->bs) {
                CANTP_STATUS_GOTO_RECVFC(channel);
                CANTP_INIT_TIMER_B(channel);
            }
        } else {
            CANTP_INIT_TIMER_A(channel);
            CANTP_INIT_TXDELAY(channel);
        }
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The tx confirm function is used for the tranFC statue of
 *              channel.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Cantp_TxConfirmFC(bl_CanTpChannel_t *channel) {
    switch (channel->pData) {
        case CANTP_FC_FRAME_CTS:
            CANTP_STATUS_GOTO_RECVCF(channel);
            CANTP_INIT_TIMER_C(channel);
            break;
        case CANTP_FC_FRAME_WAIT:
            CANTP_STATUS_GOTO_RECVFF(channel);
            CANTP_INIT_TIMER_B(channel);
            break;
        default:
            _Cantp_GotoIdle(channel);
            break;
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Change the status of the channel to tranFC status.
 *
 *  \param[in/out]  channel - the pointer of a rx channel.
 *  \param[in] fs - the FS value of the FC frame.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_GotoTranFC(bl_CanTpChannel_t *channel, bl_u8_t fs) {
    CANTP_STATUS_GOTO_TRANFC(channel);
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    CANTP_INIT_TIMER_A(channel);
    CANTP_INIT_BS_BY_CFG(channel, channel->chnCfg);
    CANTP_SET_PRIVATE_DATA(channel, fs);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Change the status of the channel to Idle status.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_GotoIdle(bl_CanTpChannel_t *channel) {
    CANTP_STATUS_GOTO_IDLE(channel);
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    CANTP_INIT_TIMER(channel);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Change the status of the channel to tranSF status.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_GotoTranSF(bl_CanTpChannel_t *channel) {
    CANTP_STATUS_GOTO_TRANSF(channel);
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    CANTP_INIT_TIMER_A(channel);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Change the status of the channel to tranFF status.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_GotoTranFF(bl_CanTpChannel_t *channel) {
    CANTP_STATUS_GOTO_TRANFF(channel);
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    CANTP_INIT_SN(channel);
    CANTP_INIT_TIMER_A(channel);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    Change the status of the channel to tranCF status.
 *
 *  \param[in/out]  channel - the pointer of a tx channel.
 *
 *  \return None
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_GotoTranCF(bl_CanTpChannel_t *channel) {
    CANTP_STATUS_GOTO_TRANCF(channel);
    CANTP_SUB_STATUS_GOTO_IDLE(channel);
    CANTP_INIT_TIMER_A(channel);
    CANTP_INIT_TXDELAY(channel);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the Idle status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \note   This function is not called except the timer is not set to zero
 *          during the status of this channel is changed to IDLE.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodIdle(bl_CanTpChannel_t *channel) {
    (void)channel;
    /*  This function is not called except the timer is not set to zero
        during the status of this channel is changed to IDLE.*/
    BL_DEBUG_ASSERT_NO_RET(0);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the Idle status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \note   Avoid unnecessary status switching, this function always returns
 *          BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutIdle(bl_CanTpChannel_t *channel) {
    (void)channel;
    
    return BL_ERR_NOT_OK;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the recvSF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodRecvSF(bl_CanTpChannel_t *channel) {
    bl_Return_t ret;
    /*alloc the buffer from the Dcm*/
    ret = Dcm_StartOfReception(channel->lastSize);
    /*if success then copy frame to buffer and goto idle*/
    if (BL_ERR_OK == ret) {
        ret = Dcm_CopyRxData(channel->lastSize, channel->frame);
        if (BL_ERR_OK == ret) {
            Dcm_RxIndication(channel->taType, BL_ERR_OK);
        } else {
            BL_DEBUG_ASSERT_NO_RET(0);
            Dcm_RxIndication(channel->taType, BL_ERR_NOT_OK);
        }
        _Cantp_GotoIdle(channel);
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the recvSF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutRecvSF(bl_CanTpChannel_t *channel) {
    (void)channel;
    
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the recvFF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodRecvFF(bl_CanTpChannel_t *channel) {
    bl_Return_t ret;
    /*Apply for the buffer from the Dcm*/
    ret = Dcm_StartOfReception(channel->totalSize);
    
    if (BL_ERR_OK == ret) {
        ret = Dcm_CopyRxData(channel->pciInfo->maxFFDataSize, channel->frame);
        if (BL_ERR_OK == ret) {
            _Cantp_GotoTranFC(channel, CANTP_FC_FRAME_CTS);
        } else {
            BL_DEBUG_ASSERT_NO_RET(0);
            Dcm_RxIndication(channel->taType, BL_ERR_NOT_OK);
            _Cantp_GotoIdle(channel);
        }
    } else if (BL_ERR_BUFFER_OVERFLOW == ret) {
        _Cantp_GotoTranFC(channel, CANTP_FC_FRAME_OVERFLOW);
    } else {
        /* The buffer busy, wait the next period to alloc again.*/
    }
    
    return ;
    
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the recvFF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutRecvFF(bl_CanTpChannel_t *channel) {
    bl_Return_t ret = BL_ERR_OK;
    
    if (channel->wft != 0u) {
        channel->wft -= 1u;
        
        _Cantp_GotoTranFC(channel, CANTP_FC_FRAME_WAIT);
        
        ret = BL_ERR_NOT_OK;
    }
    
    /* Br timeout, do not indicate the DCM.*/
    return ret;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the recvCF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodRecvCF(bl_CanTpChannel_t *channel) {
    if (0u == channel->cfCnt) {
        /*All CF is successfully received.*/
        Dcm_RxIndication(channel->taType, BL_ERR_OK);
        
        _Cantp_GotoIdle(channel);
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the recvCF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutRecvCF(bl_CanTpChannel_t *channel) {
    (void)channel;
    
    /*Cr timeout!*/
    Dcm_RxIndication(channel->taType, BL_ERR_TIMEROUT_CR);
    
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the tranFC status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodTranFC(bl_CanTpChannel_t *channel) {
    bl_Return_t ret;
    bl_BufferSize_t frameSize;
    bl_Buffer_t *frame;
    bl_ComIfHandle_t handle;
    
    
    BL_DEBUG_ASSERT_NO_RET(channel != BL_NULL_PTR);
    BL_DEBUG_ASSERT_NO_RET(CANTP_STATUS_IS_TRANFC(channel));
    
    if (CANTP_SUB_STATUS_IS_IDLE(channel)) {
        _Cantp_MakePciOfFC(channel);
        
        frameSize = channel->pciInfo->maxFCDataSize;
        frame = channel->frame;
        handle = channel->chnCfg->txId;
        
        BL_DEBUG_ASSERT_NO_RET(frameSize != 0);
        
        #if (CANTP_FUN_FRAME_PADDING == BL_FUN_ON)
        Bl_MemSet(&frame[frameSize],
            CANTP_FRAME_PADDING_VALUE,
            (bl_Size_t)((bl_Size_t)CANTP_MAX_FRAME_SIZE - frameSize));
            
        ret = Canif_Write(handle, CANTP_MAX_FRAME_SIZE, frame);
        #else
        ret = Canif_Write(handle, frameSize, frame);
        #endif
        
        if (BL_ERR_OK == ret) {
            CANTP_SUB_STATUS_GOTO_TRAN(channel);
            gs_TransmittingChannel[handle] = channel;
        }
    }
    
    return ;
    
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the tranFC status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
/*lint -e{818}*/
static bl_Return_t _Cantp_TimeoutTranFC(bl_CanTpChannel_t *channel) {
    if (channel->pData != CANTP_FC_FRAME_WAIT) {
        /*  When a FC frame with wait flag is transmitted,
            this channel do not get a buffer from DCM module.
            So do NOT indicate the DCM module in order to avoid
            break other channel.*/
        Dcm_RxIndication(channel->taType, BL_ERR_TIMEROUT_A);
    }
    
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the tranSF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodTranSF(bl_CanTpChannel_t *channel) {
    bl_Return_t ret;
    bl_u8_t dataPos;
    bl_BufferSize_t dataSize;
    bl_BufferSize_t frameSize;
    bl_Buffer_t *frame;
    bl_ComIfHandle_t handle;
    #if CANTP_MAX_FRAME_SIZE <=8
    bl_u8_t l_offset = 0;
    #else
    bl_u8_t l_offset = 1;
    #endif
    
    BL_DEBUG_ASSERT_NO_RET(channel != BL_NULL_PTR);
    BL_DEBUG_ASSERT_NO_RET(CANTP_STATUS_IS_NOT_TRANSF(channel));
    
    if (CANTP_SUB_STATUS_IS_IDLE(channel)) {
        _Cantp_MakePciOfSF(channel);
        
        dataSize = channel->lastSize;
        frame = channel->frame;
        dataPos = channel->pciInfo->dataPos + l_offset;
        handle = channel->chnCfg->txId;
        frameSize = dataSize + dataPos;
        
        BL_DEBUG_ASSERT_NO_RET(dataSize != 0);
        BL_DEBUG_ASSERT_NO_RET(frameSize <= CANTP_MAX_FRAME_SIZE);
        
        ret = Dcm_CopyTxData(dataSize, &frame[dataPos]);
        
        BL_DEBUG_ASSERT_NO_RET(BL_ERR_OK == ret);
        
        #if (CANTP_FUN_FRAME_PADDING == BL_FUN_ON)
        Bl_MemSet(&frame[frameSize],
            CANTP_FRAME_PADDING_VALUE,
            (bl_Size_t)((bl_Size_t)CANTP_MAX_FRAME_SIZE - frameSize));
            
        ret = Canif_Write(handle, CANTP_MAX_FRAME_SIZE, frame);
        #else
        ret = Canif_Write(handle, frameSize, frame);
        #endif
        if (BL_ERR_OK == ret) {
            CANTP_SUB_STATUS_GOTO_TRAN(channel);
            gs_TransmittingChannel[handle] = channel;
        }
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the tranSF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutTranSF(bl_CanTpChannel_t *channel) {
    (void)channel;
    /*The As timeout.*/
    Dcm_TxConfirmation(BL_ERR_TIMEROUT_A);
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the tranFF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodTranFF(bl_CanTpChannel_t *channel) {
    bl_Return_t ret;
    bl_u8_t dataPos;
    bl_BufferSize_t dataSize;
    bl_Buffer_t *frame;
    bl_ComIfHandle_t handle;
    
    BL_DEBUG_ASSERT_NO_RET(channel != BL_NULL_PTR);
    BL_DEBUG_ASSERT_NO_RET(CANTP_STATUS_IS_TRANFF(channel));
    BL_DEBUG_ASSERT_NO_RET(channel->totalSize != 0);
    
    if (CANTP_SUB_STATUS_IS_IDLE(channel)) {
        _Cantp_MakePciOfFF(channel);
        
        frame = channel->frame;
        dataPos = channel->pciInfo->ffDataPos;
        dataSize = channel->pciInfo->maxFFDataSize;
        handle = channel->chnCfg->txId;
        
        ret = Dcm_CopyTxData(dataSize, &frame[dataPos]);
        
        BL_DEBUG_ASSERT_NO_RET(BL_ERR_OK == ret);
        
        ret = Canif_Write(handle, CANTP_MAX_FRAME_SIZE, frame);
        
        if (BL_ERR_OK == ret) {
            CANTP_SUB_STATUS_GOTO_TRAN(channel);
            gs_TransmittingChannel[handle] = channel;
        }
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the tranFF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutTranFF(bl_CanTpChannel_t *channel) {
    (void)channel;
    /*The As timeout.*/
    Dcm_TxConfirmation(BL_ERR_TIMEROUT_A);
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the tranCF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodTranCF(bl_CanTpChannel_t *channel) {
    BL_DEBUG_ASSERT_NO_RET(channel != BL_NULL_PTR);
    BL_DEBUG_ASSERT_NO_RET(CANTP_STATUS_IS_TRANCF(channel));
    BL_DEBUG_ASSERT_NO_RET(channel->cfCnt > 0);
    
    if (CANTP_SUB_STATUS_IS_IDLE(channel)) {
        if (channel->txDelay != 0u) {
            channel->txDelay -= 1u;
        }
        
        if (channel->txDelay == 0u) {
            _Cantp_TransmitCF(channel);
        }
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the tranCF status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutTranCF(bl_CanTpChannel_t *channel) {
    (void)channel;
    /*The As timeout.*/
    Dcm_TxConfirmation(BL_ERR_TIMEROUT_A);
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details    The period function is used for the recvFC status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return None
 *
 *  \since V5.1.0
 *
 *****************************************************************************/
static void _Cantp_PeriodRecvFC(bl_CanTpChannel_t *channel) {
    (void)channel;
    return ;
}

/**************************************************************************//**
 *
 *  \details    The timeout function is used for the recvFC status of a channel.
 *
 *  \param[in/out]  channel - the pointer of a channel.
 *
 *  \return If the timeout event is processed return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static bl_Return_t _Cantp_TimeoutRecvFC(bl_CanTpChannel_t *channel) {
    (void)channel;
    /*The Bs timeout.*/
    Dcm_TxConfirmation(BL_ERR_TIMEROUT_BS);
    return BL_ERR_OK;
}

