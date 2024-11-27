/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file to configurate the flash_if manager module.
 *
 *  \file       bl_flash_if_cfg.h
 *  \ingroup    flash_if_manager_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      02.00.00 | 19/08/2013 | haiping.wang     | N/A | Boot020001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
******************************************************************************/
#ifndef _BL_FLASH_IF_CFG_H_
#define _BL_FLASH_IF_CFG_H_
#include "bl_flash_if.h"
#include "nvm.h"
#include "boot_app_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447,3453 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The g_FlashIfBlockList is used in the bl_flash_if.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The g_FlashIfBlockList is declared in the header file.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * the FLASHIF_CONVERTADDRESS have no side-effect.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/*If there is a different between the logical address and the physical address,
  configurate the below macro. The physical is a pointer.*/
#define FLASHIF_CONVERTADDRESS(logical,physical)    ((void)0)


/* publish the erase and write time ,
   if different sector have different time,publish the max one
*/
#define FLASHIF_SECTOR_ERASETIME            (80u)  /*unit ms*/
#define FLASHIF_PAGE_WRITETIME              (4u)   /*unit ms*/
/* the max access time ,when time out the hiboot have to send reponse to pc */
#define FLASHIF_ACCESS_TIMELIMITE   24000	//        (12000UL) /*unit ms*/

/* define the physical flash block num*/
#define FLASHIF_ERASED_VALUE                (0xffu)

/* config order :address low-high*/
/* config block 0 parameter - 0x10000-0x3FFFF */
#define FLASHIF_BLOCK0_STARTADDRESS         (APP_ADRRESS_START)   //s21k144 --0x20000
#define FLASHIF_BLOCK0_TOTAL_SIZE           (FLASH_P_BLOCK_SIZES)  //s32k144--0x60000  FLASH_P_BLOCK_SIZES内部己减FLASH_APP_BASE
#define FLASHIF_BLOCK0_SECTOR_SIZE          (FLASH_P_BLOCK_SECTOR_SIZES)  //0x400
#define FLASHIF_BLOCK0_PAGE_SIZE            (FLASH_P_PAGE_SIZES) //s32k 8

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
extern const bl_FlashIfBlockInfo_t g_FlashIfBlockList[FLASHIF_NUMBER_OF_BLOCK];

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

