/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file to configurate the date of the memory
 *              manager module.
 *
 *  \file       bl_memory_cfg.h
 *  \ingroup    memory_manager_module
 *  \author     
 *              
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 17/03/2010 | hewei.zhang      | N/A | Boot010001
 *      01.01.00 | 20/04/2010 | hewei.zhang      | N/A | Boot010002
 *      01.04.00 | 26/06/2010 | hewei.zhang      | N/A | Boot010005
 *      02.00.00 | 21/02/2011 | mingqing.tang    | N/A | Boot020001
 *      02.02.00 | 15/03/2011 | mingqing.tang    | N/A | Boot020003
 *      03.00.00 | 12/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.18.00 | 29/05/2012 | mingqing.tang    | N/A | Boot030019
 *      05.00.00 | 19/06/2013 | mingqing.tang    | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_MEMORY_CFG_H_
#define _BL_MEMORY_CFG_H_

#include "nvm_cfg.h"
#include "bl_memory.h"
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
//这两行是添加的，--用于排序
#define MEM_MEMORY_ID_RAM 0u
#define MEM_MEMORY_ID_FLASH 1u
/*the memory configurations*/
#define MEM_NUMBER_OF_MEMORY        (2u)
#define MEM_NUMBER_OF_OPSLIST       (2u)

#define MEM_MEMORY_0_TYPE           MEM_MEMORY_TYPE_RAM
#define MEM_MEMORY_0_INDEX          (0u)
#define MEM_MEMORY_0_BASE_ADDRESS   (0x20000000UL)	//這裡起始地址，按最大容量來算
#define MEM_MEMORY_0_SIZE           (RAM_TOTAL_SIZES) //0x2000 0000 ~ 0x2000 8000
#define MEM_MEMORY_0_OPS            (&gs_MemoryOps[0])
#define MEM_MEMORY_0_OPS_INIT       BL_NULL_PTR
#define MEM_MEMORY_0_OPS_DEINIT     BL_NULL_PTR
#define MEM_MEMORY_0_OPS_WRITE      (&Ram_Write)
#define MEM_MEMORY_0_OPS_READ       (&Ram_Read)
#define MEM_MEMORY_0_OPS_ERASE      BL_NULL_PTR

#define MEM_MEMORY_1_TYPE           (MEM_MEMORY_TYPE_EXTERNAL_DRV \
                                      | MEM_MEMORY_TYPE_FLASH)   //這裡為什麼 要或上外部？？？
#define MEM_MEMORY_1_INDEX          (0)   //為什麼這個也是0？？？？？
#define MEM_MEMORY_1_BASE_ADDRESS   (APP_ADRRESS_START)   //這裡起始地址，按最大容量來算
#define MEM_MEMORY_1_SIZE           (FLASH_P_BLOCK_SIZES)	 //0x14000
#define MEM_MEMORY_1_OPS            (&gs_MemoryOps[1])
#define MEM_MEMORY_1_OPS_INIT       (&Flsif_Init)
#define MEM_MEMORY_1_OPS_DEINIT     (&Flsif_DeInit)
#define MEM_MEMORY_1_OPS_WRITE      (&Flsif_Write)
#define MEM_MEMORY_1_OPS_READ       (&Flsif_Read)
#define MEM_MEMORY_1_OPS_ERASE      (&Flsif_Erase)

#if 0  //可以先屏蔽掉，好像D50工程也沒有
#define MEM_MEMORY_2_TYPE           (MEM_MEMORY_TYPE_EXTERNAL_DRV \
                                      | MEM_MEMORY_TYPE_FLASH)
#define MEM_MEMORY_2_INDEX          (0)
#define MEM_MEMORY_2_BASE_ADDRESS   (0x00010000UL)
#define MEM_MEMORY_2_SIZE           (0x0002C000UL)
#define MEM_MEMORY_2_OPS            (&gs_MemoryOps[1])
#define MEM_MEMORY_2_OPS_INIT       (&Flsif_Init)
#define MEM_MEMORY_2_OPS_DEINIT     (&Flsif_DeInit)
#define MEM_MEMORY_2_OPS_WRITE      (&Flsif_Write)
#define MEM_MEMORY_2_OPS_READ       (&Flsif_Read)
#define MEM_MEMORY_2_OPS_ERASE      (&Flsif_Erase)

#define MEM_MEMORY_3_TYPE           (MEM_MEMORY_TYPE_EXTERNAL_DRV \
                                      | MEM_MEMORY_TYPE_FLASH)
#define MEM_MEMORY_3_INDEX          (0)
#define MEM_MEMORY_3_BASE_ADDRESS   (0x00010000UL)
#define MEM_MEMORY_3_SIZE           (0x0002C000UL)
#define MEM_MEMORY_3_OPS            (&gs_MemoryOps[1])
#define MEM_MEMORY_3_OPS_INIT       (&Flsif_Init)
#define MEM_MEMORY_3_OPS_DEINIT     (&Flsif_DeInit)
#define MEM_MEMORY_3_OPS_WRITE      (&Flsif_Write)
#define MEM_MEMORY_3_OPS_READ       (&Flsif_Read)
#define MEM_MEMORY_3_OPS_ERASE      (&Flsif_Erase)
#endif

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
/** \brief The memory informations list.*/
extern const bl_MemoryInfoList_t g_MemoryInfoList;

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

