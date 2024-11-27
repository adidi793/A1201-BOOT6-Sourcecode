/**************************************************************************//**
 *
 *  \copyright  This software is the property of HiRain Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              HiRain Technologies.
 *
 *  \brief      This is a head file to configurate the date of the logical
 *              block manager module.
 *
 *  \file       bl_logical_block_cfg.h
 *  \ingroup    lb_manager_module
 *  \author     mingqing.tang <mingqing.tang@hirain.com>
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
#ifndef _BL_LOGICAL_BLOCK_CFG_H_
#define _BL_LOGICAL_BLOCK_CFG_H_
#include "bl_logical_block.h"
#include "boot_app_cfg.h"

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
#define LBM_LOOKFOR_OFFSET                  (0UL)   //雖然有用到，但是還是缺少了理解;
/**
 *  the size of a buffer is used to calculate the hash value.
 *
 *  \warning The size must be the n-th power of 2.
 */
#define LBM_HASH_CALCULATION_BUFFER_SIZE    (512UL)		//緩存，用於 安全算法，或者 校驗 數據--如checksum
#define LBM_HASH_CALCULATION_TIME           (8UL)		//每幀的  計算時間
#define LBM_HASH_CALCULATION_TIMEOUT        (12000UL) /*ms*/  //總超時-hash計算總超時；

//这两行是添加的--表示bllock的排序
#define LBM_LB1_ID_APP  0u
#define LBM_LB1_ID_DRV  1u
#define LBM_LB1_ID_CAL  2u

#define LBM_NUMBER_OF_LOGICAL_BLOCK         (3u)
#define LBM_NUMBER_OF_APPLICATION           (1u)
#define LBM_NUMBER_OF_CALDATA               (0u)
#define LBM_NUMBER_OF_DRIVER                (1u)
#define LBM_NUMBER_OF_VIRTUAL               (0u)


#define LBM_LB1_TYPE                LBM_TYPE_APPLICATION
#define LBM_LB1_INDEX               (0u)
#define LBM_LB1_STATUS_PTR          (&gs_LogicalBlockStatus[0])
#define LBM_LB1_MEMID               (1u)  //flash 在memory 排1
#define LBM_LB1_GETID_MASK          (0x00000000UL)
#define LBM_LB1_WRITE_MASK          RTE_SYSFLAG_ERASE_BLOCK		//這句沒有看懂，0x04 ；需先理解 RTE_SYSFLAG 等宏
#define LBM_LB1_ERASE_MASK          (0x00000000UL)
#define LBM_LB1_VERIFY_MASK         (0x00000000UL)
#define LBM_LB1_ADDRESS             (APP_ADRRESS_START)	//这里是0x8000 实际app起始地址0x8100
#define LBM_LB1_SIZE                (FLASH_ASW_MAX_ADDR-APP_ADRRESS_START) //0x1C000 - 0x8000
#define LBM_LB1_OFFSET_MASK         (0x0FFFFFFFUL)
#define LBM_LB1_VALID_FLAG_ID       (0x01u) /* pgm app valid flag ID */  //其實就是 bl_data_cfg.h上的 application valid flag
#define LBM_LB1_FILE_HEADER_ID      (0x03u) /* used for PGM DATE  ID */
#define LBM_LB1_FINGERPRINT_ID      (0x02u) /* PGM FP DID */
#define LBM_LB1_SUCCESS_COUNT_ID    (0x03u) /* PGM SUCCESS COUNTER DID */
#define LBM_LB1_ATTEMPT_COUNT_ID    (0x03u) /* PGM FAILED ATTMP DID */
#define LBM_LB1_MAX_ATTEMPT_COUNT   0xFFU //  但是D50工程是 LBM_INVALID_ATTEMPT_COUNT
#define LBM_LB1_SEG_NUMBER          (0u)
#define LBM_LB1_SEG_PTR             BL_NULL_PTR

#define LBM_LB2_TYPE                LBM_TYPE_DRIVER
#define LBM_LB2_INDEX               (0u)   //為什麼 是0 ？？
#define LBM_LB2_STATUS_PTR          (&gs_LogicalBlockStatus[1])
#define LBM_LB2_MEMID               (0u)                      //這和 memeory有關 ，有定義，ram相關的排0
#define LBM_LB2_GETID_MASK          (0x00000000UL)
#define LBM_LB2_WRITE_MASK          (0x00000000UL)
#define LBM_LB2_ERASE_MASK          (0x00000000UL)
#define LBM_LB2_VERIFY_MASK         (0x00000000UL)
#define LBM_LB2_ADDRESS             (FLAG_RAM_RE_START_ADDR) //FLASHDRIVER起始地址为0x20007000
#define LBM_LB2_SIZE                (0x00000800UL)           //长度为0x5AC
#define LBM_LB2_OFFSET_MASK         (0xFFFFFFFFUL)
#define LBM_LB2_VALID_FLAG_ID       DM_ERROR_DATAID          //表示沒有
#define LBM_LB2_FILE_HEADER_ID      DM_ERROR_DATAID
#define LBM_LB2_FINGERPRINT_ID      DM_ERROR_DATAID
#define LBM_LB2_SUCCESS_COUNT_ID    DM_ERROR_DATAID
#define LBM_LB2_ATTEMPT_COUNT_ID    DM_ERROR_DATAID
#define LBM_LB2_MAX_ATTEMPT_COUNT   LBM_INVALID_ATTEMPT_COUNT
#define LBM_LB2_SEG_NUMBER          (0u)
#define LBM_LB2_SEG_PTR             BL_NULL_PTR


#define LBM_LB3_TYPE                LBM_TYPE_CALDATA
#define LBM_LB3_INDEX               (0u)
#define LBM_LB3_STATUS_PTR          (&gs_LogicalBlockStatus[2])
#define LBM_LB3_MEMID               (1u)
#define LBM_LB3_GETID_MASK          (0x00000000UL)
#define LBM_LB3_WRITE_MASK          RTE_SYSFLAG_ERASE_BLOCK		//這句沒有看懂，0x04 ；需先理解 RTE_SYSFLAG 等宏
#define LBM_LB3_ERASE_MASK          (0x00000000UL)
#define LBM_LB3_VERIFY_MASK         (0x00000000UL)
#define LBM_LB3_ADDRESS             (CAL_ADRRESS_START)	//0x1C000
#define LBM_LB3_SIZE                (CAL_ADRRESS_LEN) 	//--STM32F107VC//為什麼只到BFF0?  因為APP程序 PAGE_0E = READ_ONLY   0x0E8000 TO 0x0EBFEF;
#define LBM_LB3_OFFSET_MASK         (0x00FFFFFFUL)
#define LBM_LB3_VALID_FLAG_ID       (DM_ERROR_DATAID) /* pgm app valid flag ID */  //其實就是 bl_data_cfg.h上的 application valid flag
#define LBM_LB3_FILE_HEADER_ID      (0x03u) /* used for PGM DATE  ID */
#define LBM_LB3_FINGERPRINT_ID      (0x02u) /* PGM FP DID */
#define LBM_LB3_SUCCESS_COUNT_ID    (0x03u) /* PGM SUCCESS COUNTER DID */
#define LBM_LB3_ATTEMPT_COUNT_ID    (0x03u) /* PGM FAILED ATTMP DID */
#define LBM_LB3_MAX_ATTEMPT_COUNT   0xFFU //  但是D50工程是 LBM_INVALID_ATTEMPT_COUNT
#define LBM_LB3_SEG_NUMBER          (0)
#define LBM_LB3_SEG_PTR             BL_NULL_PTR

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
/**************************************************************************//**
 *  \brief A list of logical blocks.
 *****************************************************************************/
extern const bl_LogicalBlockList_t g_LogicalBlockList;

/**************************************************************************//**
 *  \brief the buffer is used to calculate the hash.
 *****************************************************************************/
extern bl_Buffer_t g_HashCalculation[LBM_HASH_CALCULATION_BUFFER_SIZE];
/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

