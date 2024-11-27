/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file of the DCM module.
 *
 *  \file       bl_dcm.h
 *  \ingroup    communication_protocol_stack_module
 *  \author
 *
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 08/04/2010 | hewei.zhang      | N/A | Boot010001
 *      01.02.00 | 19/04/2010 | hewei.zhang      | N/A | Boot010003
 *      01.04.00 | 06/05/2010 | hewei.zhang      | N/A | Boot010005
 *      02.00.00 | 18/03/2011 | mingqing.tang    | N/A | Boot020001
 *      02.01.00 | 31/03/2011 | mingqing.tang    | N/A | Boot020002
 *      02.03.00 | 07/04/2011 | mingqing.tang    | N/A | Boot020004
 *      03.00.00 | 12/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.05.00 | 11/10/2011 | mingqing.tang    | N/A | Boot030006
 *      03.06.00 | 26/10/2011 | mingqing.tang    | N/A | Boot030007
 *      04.00.00 | 02/01/2012 | mingqing.tang    | N/A | Boot040001
 *      04.02.00 | 16/01/2012 | mingqing.tang    | N/A | Boot040003
 *      04.08.00 | 04/04/2012 | mingqing.tang    | N/A | Boot040009
 *      05.00.00 | 19/06/2013 | heli.zhang       | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_DCM_H_
#define _BL_DCM_H_
#include "bl_typedefs.h"
#include "bl_dcm_funcfg.h"
#include "bl_can_tp.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 783,3205,3210,3447,3448,3453 EOF*/
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
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * These macros have no side-effect.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief Buffer status.*/
#define DCM_STATUS_BUFFER_RECV_LOCK         (0x0001U)
#define DCM_STATUS_BUFFER_PROC_LOCK         (0x0002U)
#define DCM_STATUS_BUFFER_SEND_LOCK         (0x0004U)
#define DCM_STATUS_BUFFER_UNLOCK            (0x0000U)

#define DCM_DOWNSTATUS_INDEX        (0x01U)

#define DCM_MAKE_FLAG(flag)         (RTE_USER_FLAG_GROUP_1 | (flag))

#define DCM_DEFAULT_STATUS_MASK     DCM_MAKE_FLAG(RTE_DOWN_STATUS_USER_MASK)

#define DCM_STATUS_SESSION_DEFAULT          DCM_MAKE_FLAG(0x01UL)
#define DCM_STATUS_SESSION_PROGRAMMING      DCM_MAKE_FLAG(0x02UL)
#define DCM_STATUS_SESSION_EXTENDED         DCM_MAKE_FLAG(0x04UL)
//#define DCM_STATUS_SESSION_EXT_USER         DCM_MAKE_FLAG(0x08UL)

#define DCM_STATUS_FUNCTION_REQUEST         DCM_MAKE_FLAG(0x08UL)
#define DCM_STATUS_PHYSICAL_REQUEST         DCM_MAKE_FLAG(0x10UL)

#define DCM_STATUS_SECURITY_LOCK_1          DCM_MAKE_FLAG(0x100UL)
#define DCM_STATUS_SECURITY_UNLOCK_1        DCM_MAKE_FLAG(0x200UL)
#define DCM_STATUS_SECURITY_WAITKEY_1       DCM_MAKE_FLAG(0x400UL)

#define DCM_STATUS_SECURITY_LOCK_2          DCM_MAKE_FLAG(0x800UL)
#define DCM_STATUS_SECURITY_UNLOCK_2        DCM_MAKE_FLAG(0x1000UL)
#define DCM_STATUS_SECURITY_WAITKEY_2       DCM_MAKE_FLAG(0x2000UL)

#define DCM_STATUS_SECURITY_LOCK_3          DCM_MAKE_FLAG(0x4000UL)
#define DCM_STATUS_SECURITY_UNLOCK_3        DCM_MAKE_FLAG(0x8000UL)
#define DCM_STATUS_SECURITY_WAITKEY_3       DCM_MAKE_FLAG(0x10000UL)

#define DCM_SESSION_MASK            (DCM_STATUS_SESSION_DEFAULT \
    | DCM_STATUS_SESSION_PROGRAMMING \
    | DCM_STATUS_SESSION_EXTENDED)
#define DCM_FUNCTIONAL_MASK         DCM_STATUS_FUNCTION_REQUEST

#define DCM_FUNANDPHY_MASK          (DCM_STATUS_FUNCTION_REQUEST \
    | DCM_STATUS_PHYSICAL_REQUEST)

#define DCM_SECURITYACCESS_1_MASK   (DCM_STATUS_SECURITY_LOCK_1 \
    | DCM_STATUS_SECURITY_UNLOCK_1 \
    | DCM_STATUS_SECURITY_WAITKEY_1)

#define DCM_SECURITYACCESS_2_MASK   (DCM_STATUS_SECURITY_LOCK_2 \
    | DCM_STATUS_SECURITY_UNLOCK_2 \
    | DCM_STATUS_SECURITY_WAITKEY_2)

#define DCM_SECURITYACCESS_3_MASK   (DCM_STATUS_SECURITY_LOCK_3 \
    | DCM_STATUS_SECURITY_UNLOCK_3 \
    | DCM_STATUS_SECURITY_WAITKEY_3)

#define DCM_SECURITYACCESS_MASK     (DCM_STATUS_SECURITY_LOCK_1 \
    | DCM_STATUS_SECURITY_UNLOCK_1 \
    | DCM_STATUS_SECURITY_LOCK_2 \
    | DCM_STATUS_SECURITY_UNLOCK_2 \
    | DCM_STATUS_SECURITY_LOCK_3 \
    | DCM_STATUS_SECURITY_UNLOCK_3)

#define DCM_NEGATIVE_RESPONSE_SIZE          (0x03u)
#define DCM_NEGATIVE_RESPONSE_CODE          (0x7Fu)
#define DCM_NEGATIVE_RESPONSE_ID_POS        (1)
#define DCM_NEGATIVE_RESPONSE_NRC_POS       (2)

/*bl_ResponseCode_t.*/
#define DCM_E_POSITIVERESPONSE                          (0x00u)
#define DCM_E_GENERALREJECT                             (0x10u)
#define DCM_E_SERVICENOTSUPPORTED                       (0x11u)
#define DCM_E_SUBFUNCTIONNOTSUPPORTED                   (0x12u)
#define DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT     (0x13u)
#define DCM_E_CONDITIONSNOTCORRECT                      (0x22u)
#define DCM_E_REQUESTSEQUENCEERROR                      (0x24u)
#define DCM_E_REQUESTOUTOFRANGE                         (0x31u)
#define DCM_E_SECURITYACCESSDENIED                      (0x33u)
#define DCM_E_INVALIDKEY                                (0x35u)
#define DCM_E_EXCEEDNUMBEROFATTEMPTS                    (0x36u)
#define DCM_E_REQUIREDTIMEDELAYNOTEXPIRED               (0x37u)
#define DCM_E_UPLOADDOWNLOADNOTACCEPTED                 (0x70u)
#define DCM_E_TRANSFERDATASUSPENDED                     (0x71u)
#define DCM_E_GENERALPROGRAMMINGFAILURE                 (0x72u)
#define DCM_E_WRONGBLOCKSEQUENCECOUNTER                 (0x73u)
#define DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING  (0x78u)
#define DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION    (0x7Eu)
#define DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION        (0x7Fu)

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias of the struct _tag_DcmServiceHeader.*/
typedef struct _tag_DcmServiceHeader bl_DcmServiceHeader_t;
/** \brief A alias of the struct _tag_DcmService.*/
typedef struct _tag_DcmService bl_DcmService_t;
/** \brief A alias of the struct _tag_DcmBuffer.*/
typedef struct _tag_DcmBuffer bl_DcmBuffer_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/** \brief This is header of service or subfunction or did.*/
struct _tag_DcmServiceHeader {
    bl_u8_t type; //表示识别到的是SID、SUBID还是DID
    bl_ResponseCode_t resCode; //响应码，bl_ResponseCode_t实际是bl_u8_t
    bl_ResponseCode_t sessionResCode; //会话响应码
    bl_BufferSize_t (*MakeId)(const bl_Buffer_t *buffer, bl_u16_t *id); //bl_BufferSize_t实际是bl_u16_t，bl_Buffer_t实际是bl_u8_t
};

/** \brief This is info of service or subfunction or did.*/
struct _tag_DcmService {
    bl_u16_t id;
    bl_u16_t number; //子功能或子ID的数量，没有时为0
    bl_BufferSize_t minSize; //检查请求消息的服务最小长度，从SID开始计算
    bl_BufferSize_t maxSize; //检查请求消息的最大长度
    bl_u32_t flag;   //
    const struct _tag_DcmService *serviceList;
    const struct _tag_DcmServiceHeader *header;
    bl_ResponseCode_t (*Callback)(bl_BufferSize_t size, bl_Buffer_t *buffer, bl_BufferSize_t *respSize);
};

/** \brief This is info of dcm buffer.*/
struct _tag_DcmBuffer {
    bl_u16_t status;
    bl_u16_t index;
    bl_BufferSize_t recvSize; //PCI中的DL
    bl_BufferSize_t tranSize; //
    bl_BufferSize_t totalSize;
    bl_Buffer_t *buffer;
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
extern bl_u8_t gs_SupPosRespFlag;
/** \brief The service header of dcm module.*/
extern const bl_DcmServiceHeader_t g_DcmServiceHeader;

/** \brief The subid header of service.*/
extern const bl_DcmServiceHeader_t g_DcmSubIdHeader;

/** \brief The did header of service.*/
extern const bl_DcmServiceHeader_t g_DcmDidHeader;

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the dcm module.*/
extern void Dcm_Init(void);
/** \brief Mainfunction of the dcm module.*/
extern void Dcm_MainFunction(void);
/** \brief Timerfunction of the dcm module.*/
extern void Dcm_TimerFunction(void);
/** \brief Start the S3 timer.*/
extern void Dcm_StartS3Timer(void);
/** \brief Stop the S3 timer.*/
extern void Dcm_StopS3Timer(void);
/** \brief Check whether the response is suspended.*/
extern bl_Return_t Dcm_IsSuspendedResponse(void);

/** \brief Start reception.*/
extern bl_Return_t Dcm_StartOfReception(bl_BufferSize_t size);
/** \brief Copy received data.*/
extern bl_Return_t Dcm_CopyRxData(bl_BufferSize_t size,
    const bl_Buffer_t *data);
/** \brief Indicate received result to the upper*/
extern void Dcm_RxIndication(bl_u8_t taType, bl_CanTpResult_t result);
/** \brief Copy transmittied data.*/
extern bl_Return_t Dcm_CopyTxData(bl_BufferSize_t size, bl_Buffer_t *data);
/** \brief Confirm transmitting result to the upper.*/
extern void Dcm_TxConfirmation(bl_CanTpResult_t result);

#endif

