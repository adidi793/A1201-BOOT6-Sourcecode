/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the logical block manager module.
 *
 *  \file       bl_logical_block.h
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
 *      03.01.00 | 16/09/2011 | mingqing.tang    | N/A | Boot030002
 *      03.02.00 | 23/09/2011 | mingqing.tang    | N/A | Boot030003
 *      03.03.00 | 17/10/2011 | mingqing.tang    | N/A | Boot030004
 *      03.05.00 | 27/10/2011 | mingqing.tang    | N/A | Boot030006
 *      03.06.00 | 08/11/2011 | mingqing.tang    | N/A | Boot030007
 *      03.08.00 | 30/11/2011 | mingqing.tang    | N/A | Boot030009
 *      03.11.00 | 15/02/2012 | mingqing.tang    | N/A | Boot030012
 *      03.14.00 | 13/03/2012 | mingqing.tang    | N/A | Boot030015
 *      03.15.00 | 26/03/2012 | mingqing.tang    | N/A | Boot030016
 *      03.18.00 | 29/05/2012 | mingqing.tang    | N/A | Boot030019
 *      04.00.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040001
 *      04.01.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040002
 *      04.03.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040004
 *      04.04.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040005
 *      04.10.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040011
 *      04.12.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040013
 *      04.16.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040016
 *      04.19.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040020
 *      04.21.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040022
 *      04.26.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040026
 *      04.27.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040028
 *      04.30.00 | 10/04/2013 | mingqing.tang    | N/A | Boot040031
 *      05.00.00 | 19/06/2013 | mingqing.tang    | N/A | Boot030001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_LOGICAL_BLOCK_H_
#define _BL_LOGICAL_BLOCK_H_
#include "common_types.h"
#include "bl_typedefs.h"
#include "bl_logical_block_funcfg.h"
#include "bl_rte.h"

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
 *  Macro Definitions
 *****************************************************************************/
#define LBM_TYPE_APPLICATION        (0x0001u)
#define LBM_TYPE_CALDATA            (0x0002u)
#define LBM_TYPE_DRIVER             (0x0004u)
#define LBM_TYPE_VIRTUAL            (0x0008u)
//這幾個狀態。要好好研究
#define LBM_STATUS_NOT_INIT         BL_STATUS_NOT_INIT
#define LBM_STATUS_VALID            (0x0001u)
#define LBM_STATUS_INVALID          (0x0002u)
#define LBM_STATUS_UPDATING         (0x0003u)
#define LBM_STATUS_UPDATED          (0x0004u)
#define LBM_STATUS_NOT_CARE         (0x0005u)
#define LBM_STATUS_NOT_EXIST        (0xFFFFu)

#define LBM_INVALID_ATTEMPT_COUNT   (0)
#define LBM_ERROR_LBID              BL_ERROR_ID_16BIT
#define LBM_COUNTER_MAX_SIZE        (4)

#define LBM_ERR_UPDATED_ID          BL_ERR_USER_CODE_01  //值是100

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias of the struct _tag_Segment.*/
typedef struct _tag_Segment bl_Segment_t;
/** \brief A alias of the struct _tag_SegmentList.*/
typedef struct _tag_SegmentList bl_SegmentList_t;
/** \brief A alias of the struct _tag_LogicalBlock.*/
typedef struct _tag_LogicalBlock bl_LogicalBlock_t;
/** \brief A alias of the struct _tag_LogicalBlockList.*/
typedef struct _tag_LogicalBlockList bl_LogicalBlockList_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/** \brief This is a segment indicated a region in a logical block.*/
struct _tag_Segment {
    bl_u32_t address;   /**< the address of a segment in the logical block.*/
    bl_u32_t size;      /**< the size of a segment in the logical block.*/
};

/**
 *  \brief This is a list of segments, these segments are in the same logical
 *         block.
 */
struct _tag_SegmentList {
    bl_u16_t number;    /**< the number of segments in this list.*/
    const struct _tag_Segment *segList; /**< the segments list.*/
};

/**
 *  \brief This is a logical block, it indicates a independent unit used to
 *         download.
 */
struct _tag_LogicalBlock {
    bl_u16_t type;
    bl_u16_t index;
    bl_u16_t *status;
    bl_u16_t memId;
    bl_DownStatus_t getLbMask;
    bl_DownStatus_t writeLbMask;
    bl_DownStatus_t eraseLbMask;
    bl_DownStatus_t verifyLbMask;
    bl_Address_t address;
    bl_Size_t size;
    bl_Address_t offsetMask;
    bl_u16_t validFlagId;
    bl_u16_t headerId;
    bl_u16_t fpId;
    bl_u16_t succCntId;
    bl_u16_t attemptCntId;
    bl_AttemptCounter_t maxAttemptCnt;
    const struct _tag_SegmentList segments;
};

/**
 *  \brief This is a list of logical blocks, includes all supported logical
 *         block.
 */
struct _tag_LogicalBlockList {
    bl_u16_t status;
    bl_u16_t number;
    bl_u16_t appNum;
    bl_u16_t calNum;
    bl_u16_t drvNum;
    bl_u16_t virNum;
    const struct _tag_LogicalBlock *lbList;
};

typedef union {
    bl_u32_t dw;
    bl_u8_t byt[4];
    struct {
        bl_u16_t  f_attmp;
        bl_u16_t  success;
    } dt;
} bl_u_cnt_t;

typedef struct {
    bl_u8_t   fingprnt[16];
    bl_u_cnt_t pgm_counter;
    bl_u8_t    pgm_date[4];
    //bl_us32_t  app_valid;
} bl_flash_t;


/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the logical block module.*/
extern bl_Return_t Lbm_Init(void);
/** \brief Get a logical block id.*/
extern bl_Return_t Lbm_GetBlock(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size);
/** \brief Clear the logical block information of the download context*/
extern void Lbm_ClearBlock(bl_DownContext_t *dct);
/** \brief write data into a logical block.*/
extern bl_Return_t Lbm_Write(const bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer);
/** \brief erase a logical block.*/
extern bl_Return_t Lbm_Erase(const bl_DownContext_t *dct);
/** \brief verify whether a logical block is valid.*/
extern bl_Return_t Lbm_Verify(const bl_DownContext_t *dct,
    bl_Size_t size,
    const bl_Buffer_t *value);
/** \brief check whether all of logical blocks are valid.*/
extern bl_Return_t Lbm_IsAllValid(void);
/** \brief update the status of all of logical blocks.*/
extern void Lbm_UpdateStatus(void);
/** \brief save the status of a logical block.*/
extern bl_Return_t Lbm_SaveStatus(bl_u16_t id);  //把 flash app valid 值寫進去
/** \brief save the status of all of logical blocks.*/
extern bl_Return_t Lbm_SaveAllStatus(void);
/** \brief clear the status of a logical block.*/
extern bl_Return_t Lbm_ClearStatus(bl_u16_t id);
/** \brief get the status of a logical block.*/
extern bl_Return_t Lbm_GetStatus(bl_u16_t id, bl_u16_t *status);
#if (LBM_FUN_SAVE_FINGERPRINT == BL_FUN_ON)
/** \brief save the fingerprint information of the logical blocks.*/
extern void Lbm_SaveFingerprint(const bl_Buffer_t *buffer);
#endif

#if (LBM_FUN_SAVE_PROGRAMMING_DATE == BL_FUN_ON)
void Lbm_SaveProgramDate(const bl_Buffer_t *buffer);
#endif

#if (LBM_FUN_ATTEMPT_COUNTER == BL_FUN_ON)
bl_AttemptCounter_t Lbm_GetAttmptCount(void);
//void Lbm_SavePgmFattmpCnt(const bl_Buffer_t *buffer);

#endif
#if (LBM_FUN_SUCCESS_COUNTER == BL_FUN_ON)
bl_Return_t _Lbm_AddSuccessCnt(void);
bl_SuccessCounter_t Lbm_GetSuccesCount(void);
//void Lbm_SavePgmSuccCnt(const bl_Buffer_t *buffer);

#endif

#if 0
bl_Return_t _Lbm_erase_(bl_u16_t did);

bl_Return_t Lbm_read_eeprom_Data(bl_u8_t did);

bl_Return_t _Lbm_SaveDataToRom(bl_u8_t w_index);
#endif


void lb_logic_block_init_flashdrv(void);
u8 lb_logic_block_read_app_valid1(void);
bl_u8_t check_program_valid_nores_is_valid(void);
bl_u8_t check_program_valid_to_app_valid(void);
void clear_program_valid_to_app_valid(void);

#endif

