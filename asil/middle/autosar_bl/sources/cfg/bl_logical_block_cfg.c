/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a source file to configurate the logical block manager
 *              module.
 *
 *  \file       bl_logical_block_cfg.c
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
 *      03.11.00 | 15/02/2012 | mingqing.tang    | N/A | Boot030012
 *      04.00.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040001
 *      04.01.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040002
 *      04.03.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040004
 *      04.21.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040022
 *      04.30.00 | 10/04/2013 | mingqing.tang    | N/A | Boot040031
 *      05.00.00 | 19/06/2013 | mingqing.tang    | N/A | Boot030001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_data.h"
#include "bl_logical_block_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1257,1531,1533,3211,3406 EOF*/
/*
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1531:The object is referenced in only one translation unit.
 * g_LogicalBlockList is a configurations.
 *
 * 1533:The object is referenced in only one translation unit.
 *
 * 3211:The global identifier is defined but not used.
 * The g_BootingCheckerList is used in the bl_booting.c file.
 *
 * MISRA-C:2004 8.5(3406):Object with external linkage has been defined in a
 * header file.
 * The g_HashCalculation is not defined in a header file.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Function Macro
 *****************************************************************************/
#if (((LBM_HASH_CALCULATION_BUFFER_SIZE)\
& (LBM_HASH_CALCULATION_BUFFER_SIZE - 1UL)) != 0UL)
#error "the size of the buffer used to calculate the hash value is invalid."
#endif
/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/

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
/**************************************************************************//**
 *  \details A list of status of the logical block.
 *****************************************************************************/
STATIC bl_u16_t gs_LogicalBlockStatus[LBM_NUMBER_OF_LOGICAL_BLOCK];

/**************************************************************************//**
 *  \details A array of the segment informations.
 *****************************************************************************/

/**************************************************************************//**
 *  \details A array of the logical block informations.
 * struct _tag_LogicalBlock {
    type;
    index;
    *status;
    memId;
    getLbMask;
    writeLbMask;
    eraseLbMask;
    verifyLbMask;
    address;
    size;
    offsetMask;
    validFlagId;
    headerId;
    fpId;
    succCntId;
    attemptCntId;
    maxAttemptCnt;
    segments;
};
 *****************************************************************************/
STATIC const bl_LogicalBlock_t gs_LogicalBlock[LBM_NUMBER_OF_LOGICAL_BLOCK] = {
    /*logical block 1----flash application*/
    {
        //type           index=0    *statusָ��gs_LogicalBlockStatus[0]  memId �ӌ�����id=1
        LBM_LB1_TYPE, LBM_LB1_INDEX, LBM_LB1_STATUS_PTR, LBM_LB1_MEMID,
        //getLbMask 00      //writeLbMask RTE_SYSFLAG_ERASE_BLOCK=0x04???��̫����;
        LBM_LB1_GETID_MASK, LBM_LB1_WRITE_MASK,
        //0x00000000UL      //verifyLbMask 00      //address 0x8000
        LBM_LB1_ERASE_MASK, LBM_LB1_VERIFY_MASK, LBM_LB1_ADDRESS,
        //size 14000   //offsetMask FFFFFF  validFlagId
        LBM_LB1_SIZE, LBM_LB1_OFFSET_MASK, LBM_LB1_VALID_FLAG_ID,
        //headerId 3          //fpId 2                 //succCntId 3
        LBM_LB1_FILE_HEADER_ID, LBM_LB1_FINGERPRINT_ID, LBM_LB1_SUCCESS_COUNT_ID,
        //attemptCntId 3           //maxAttemptCnt ff ����D50Ĭ�J0
        LBM_LB1_ATTEMPT_COUNT_ID, LBM_LB1_MAX_ATTEMPT_COUNT,
        //const struct _tag_SegmentList segments ���e���ֵ��0 ��ʾ�]�ж��x segments
        {
            LBM_LB1_SEG_NUMBER, LBM_LB1_SEG_PTR
        },
    },
    /*logical block 2--flash driver,�ஔ� ram */
    {
        //type         index=0??    *statusָ��gs_LogicalBlockStatus[1]  memId �ӌ�����id=0
        LBM_LB2_TYPE, LBM_LB2_INDEX, LBM_LB2_STATUS_PTR, LBM_LB2_MEMID,
        //getLbMask 00      //writeLbMask 00
        LBM_LB2_GETID_MASK, LBM_LB2_WRITE_MASK,
        //0x00000000UL     //verifyLbMask 00   //address 0x20007000
        LBM_LB2_ERASE_MASK, LBM_LB2_VERIFY_MASK, LBM_LB2_ADDRESS,
        //size 0x000001E0UL    //offsetMask FFFF ��ַ��Чλmask    //validFlagId flag error
        LBM_LB2_SIZE, LBM_LB2_OFFSET_MASK, LBM_LB2_VALID_FLAG_ID,
        //headerId ff          //fpId FF               //succCntId FF
        LBM_LB2_FILE_HEADER_ID, LBM_LB2_FINGERPRINT_ID, LBM_LB2_SUCCESS_COUNT_ID,
        //attemptCntId FF         //maxAttemptCnt 0
        LBM_LB2_ATTEMPT_COUNT_ID, LBM_LB2_MAX_ATTEMPT_COUNT,
        //const struct _tag_SegmentList segments ���e���ֵ��0 ��ʾ�]�ж��x segments
        {
            LBM_LB2_SEG_NUMBER, LBM_LB2_SEG_PTR
        },
    },
    /*logical block 3--flash CAL,�ஔ� APP DATA */
    {
        //type         index=0??    *statusָ��gs_LogicalBlockStatus[1]  memId �ӌ�����id=0
        LBM_LB3_TYPE, LBM_LB3_INDEX, LBM_LB3_STATUS_PTR, LBM_LB3_MEMID,
        //getLbMask 00      //writeLbMask 00
        LBM_LB3_GETID_MASK, LBM_LB3_WRITE_MASK,
        //0x00000000UL     //verifyLbMask 00   //address 0x3e00
        LBM_LB3_ERASE_MASK, LBM_LB3_VERIFY_MASK, LBM_LB3_ADDRESS,
        //size 0x000001E0UL    //offsetMask FFFF ��ַ��Чλmask    //validFlagId flag error
        LBM_LB3_SIZE, LBM_LB3_OFFSET_MASK, LBM_LB3_VALID_FLAG_ID,
        //headerId ff          //fpId FF               //succCntId FF
        LBM_LB3_FILE_HEADER_ID, LBM_LB3_FINGERPRINT_ID, LBM_LB3_SUCCESS_COUNT_ID,
        //attemptCntId FF         //maxAttemptCnt 0
        LBM_LB3_ATTEMPT_COUNT_ID, LBM_LB3_MAX_ATTEMPT_COUNT,
        //const struct _tag_SegmentList segments ���e���ֵ��0 ��ʾ�]�ж��x segments
        {
            LBM_LB3_SEG_NUMBER, LBM_LB3_SEG_PTR
        },
    },
};

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/
/**************************************************************************//**
 *  \details A list of logical blocks is managed by the logical block module.
 *****************************************************************************/
const bl_LogicalBlockList_t g_LogicalBlockList = {
    BL_STATUS_INITED,               //status  ����ʼ��
    LBM_NUMBER_OF_LOGICAL_BLOCK,    //number �F���Ѓɂ�block��
    LBM_NUMBER_OF_APPLICATION,      //appNum ��һ�����ÉK
    LBM_NUMBER_OF_CALDATA,          //calNum  0��
    LBM_NUMBER_OF_DRIVER,           //drvNum ��һ�� �ӉK
    LBM_NUMBER_OF_VIRTUAL,          //virNum  0��
    gs_LogicalBlock,                //*lbList
};

/**************************************************************************//**
 *  \details the buffer is used to calculate the hash value.
 *****************************************************************************/
bl_Buffer_t
g_HashCalculation[LBM_HASH_CALCULATION_BUFFER_SIZE];   //�@����512���A�����棻�������� ��� flash�� sector��С�� 512

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

