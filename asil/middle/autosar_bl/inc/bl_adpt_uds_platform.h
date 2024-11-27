/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file of the adapter module based on uds
 *              platform.
 *
 *  \file       bl_adpt_uds_platform.h
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
#ifndef _BL_ADPT_UDS_PLATFORM_H_
#define _BL_ADPT_UDS_PLATFORM_H_
#include "bl_typedefs.h"
#include "bl_adapter.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447,4152 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_dcm_cfg.c file.
 * The g_UdsPlatformAdapter is used in the bl_process_driver_cfg.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 * 4152:The identifier may cause confusion.
 * Adpt_UdsCallbackGetSeedForLevel1 is LEVEL One.
 * Adpt_UdsCallbackVerifyKeyForLevel1 is LEVEL One.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
typedef enum {
    MBEDTLS_MD_NONE=0,
    MBEDTLS_MD_MD2,
    MBEDTLS_MD_MD4,
    MBEDTLS_MD_MD5,
    MBEDTLS_MD_SHA1,
    MBEDTLS_MD_SHA224,
    MBEDTLS_MD_SHA256,
    MBEDTLS_MD_SHA384,
    MBEDTLS_MD_SHA512,
    MBEDTLS_MD_RIPEMD160,
    MBEDTLS_MD_SM3,
} mbedtls_md_type_t;
/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/



/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
/** \brief A adapter for uds platform spec*/
extern const bl_Adapter_t g_UdsPlatformAdapter;
extern bl_u16_t crt_tbs_addr_offset;
extern bl_u16_t crt_tbs_len;
extern bl_u16_t crt_issuer_addr_offset;
extern bl_u16_t crt_issuer_len;
extern bl_u8_t recv_data_6000[1322];
extern bl_u16_t pkey_N_addr_offset;
extern bl_u16_t pkey_E_addr_offset;
extern bl_u16_t pkey_sig_addr_offset;
/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the uds module.*/
extern bl_Return_t Adpt_UdsInit(void);
/** \brief Periodic callback of the adapter.*/
extern void Adpt_UdsPeriodicCallback(void);
/** \brief Callback of the adapter.*/
extern void Adpt_UdsCallback(void);
/** \brief Callback of default session.*/
extern bl_ResponseCode_t Adpt_UdsCallback1001(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of programming session.*/
extern bl_ResponseCode_t Adpt_UdsCallback1002(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of extended session.*/
extern bl_ResponseCode_t Adpt_UdsCallback1003(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
#if 0
extern bl_ResponseCode_t Adpt_UdsCallback1004(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
#endif
/** \brief Callback of reset.*/
extern bl_ResponseCode_t Adpt_UdsCallback1101(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
extern bl_ResponseCode_t Adpt_UdsCallback1103(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of reading fingerprint.*/
extern bl_ResponseCode_t Adpt_UdsCallBackReadFingerprint(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F187 */
extern bl_ResponseCode_t Adpt_UdsCallBackReadPartNumber(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F1 80 */
extern bl_ResponseCode_t Adpt_UdsCallBackReadBootSW_ID(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/*  22 F1 8A */
extern bl_ResponseCode_t Adpt_UdsCallBackReadSupplierCode(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F1 86 */
extern bl_ResponseCode_t Adpt_UdsCallBackReadActiveSession(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/*  22 F1 91 */
extern bl_ResponseCode_t Adpt_UdsCallBackReadECU_HardwareVersionNumber(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/*  22 F195 */
extern bl_ResponseCode_t Adpt_UdsCallBackReadECU_SoftwareVersionNumber(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F198 */
extern bl_ResponseCode_t Adpt_UdsCallBackRead_Tester_Serial_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F18E */
extern bl_ResponseCode_t Adpt_UdsCallBackReadSoftCode(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);


/*  22 F193 */
bl_ResponseCode_t Adpt_UdsCallBackReadEcuSoftVersion(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);


/*  22 F199 */
bl_ResponseCode_t Adpt_UdsCallBackReadProgrammingDate(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 0200 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Programming_Counter(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 0201 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Programming_Attempts_Counter(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 FF00 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Diagnosis_Specification_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F188 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Manufacturer_ECU_Software_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F189 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Manufacturer_ECU_Software_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F191 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Manufacturer_ECU_Hardware_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F150 */
bl_ResponseCode_t Adpt_UdsCallBackRead_System_Supplier_ECU_Hardware_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F182 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Calibration_Software_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F197 */
bl_ResponseCode_t Adpt_UdsCallBackRead_System_Name(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F1ED */
bl_ResponseCode_t Adpt_UdsCallBackRead_Secure_Flash(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/*  22 F1EF */
bl_ResponseCode_t Adpt_UdsCallBackRead_BootApp_Identify(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/** \brief Callback of getting seed.*/
extern bl_ResponseCode_t Adpt_UdsCallbackGetSeedForLevel1(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of verifying key.*/
extern bl_ResponseCode_t Adpt_UdsCallbackVerifyKeyForLevel1(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/** \brief Callback of getting seed.*/
extern bl_ResponseCode_t Adpt_UdsCallbackGetSeedForLevel2(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of verifying key.*/
extern bl_ResponseCode_t Adpt_UdsCallbackVerifyKeyForLevel2(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/** \brief Callback of enable or disable communication.*/
extern bl_ResponseCode_t Adpt_UdsCallback28(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of writting fingerprint.*/
extern bl_ResponseCode_t Adpt_UdsCallBackWriteFingerprint(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

extern bl_ResponseCode_t Adpt_UdsCallBackWrite_Tester_Serial_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);

/** \brief Callback of writting programming date.*/
extern bl_ResponseCode_t Adpt_UdsCallBackWriteProgrammingDate(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of checking programming contiditions.*/
extern bl_ResponseCode_t Adpt_UdsCallbackCheckProgramCond(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of erasing memory.*/
extern bl_ResponseCode_t Adpt_UdsCallbackEraseMemory(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of checksum.*/
extern bl_ResponseCode_t Adpt_UdsCallbackCheckSum(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of checking compatibility.*/
extern bl_ResponseCode_t Adpt_UdsCallbackCheckCompatibility(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of checking compatibility.*/
extern bl_ResponseCode_t Adpt_UdsCallbackCheckLegitimacy(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of checking compatibility.*/
extern bl_ResponseCode_t Adpt_UdsCallbackCheckVersion(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of stayinboot.*/
extern bl_ResponseCode_t Adpt_UdsCallbackStayInBoot(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of requesting download.*/
extern bl_ResponseCode_t Adpt_UdsCallback34(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of transferring data.*/
extern bl_ResponseCode_t Adpt_UdsCallback36(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of requesting transfer exit.*/
extern bl_ResponseCode_t Adpt_UdsCallback37(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of keeping session.*/
extern bl_ResponseCode_t Adpt_UdsCallback3E(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of switching on or off dtc.*/
extern bl_ResponseCode_t Adpt_UdsCallback85(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize);
/** \brief Callback of timeout.*/
extern void Adpt_UdsTimeOutCallBack(void);
/** \brief Callback is used when the reprogramming Flag is valid.*/
extern void Adpt_ReprogramValidProcess(void);
extern void Adpt_Res_default_ValidProcess(void);
/** \brief Callback is used when the reset Flag is valid.*/
extern void Adpt_SystemResetProcess(void);

#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
/** \brief Callback is used when the application is valid.*/
extern void Adpt_AppValidProcess(void);
#endif

void blAdptWriteBootSwId(void);

void Adpt_ReprogramValid_nores_Process(void);

extern bl_u8_t g_cur_request_level ;


#endif

