/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the header file of the flash module.
 *
 *  \file       bl_flash.h
 *  \ingroup    driver_module
 *  \author     
 *
 *  \version    1.0.0
 *  \date       15/05/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | -------------
 *      01.00.00 | 15/05/2014 | ning.chen        | N/A | BootDrv010001
 *****************************************************************************/
#ifndef _BL_FLASH_H_
#define _BL_FLASH_H_
#include "bl_typedefs.h"
#include "bl_flash_if.h"
#include "common_types.h"
#include "bl_system_cfg.h"
#include "boot_app_cfg.h"


/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447,3453 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_security_cfg.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
//實際沒有用到的宏
#define FLASH_DRIVER_VERSION_TYPE       (0x8001u)
#define FLASH_DRIVER_VERSION_INDEX      (0x0000u)
#define FLASH_DRIVER_VERSION_MAGIC      (0x47313936UL)
#define FLASH_DRIVER_VERSION_VERSION    (0x00010000UL)

/*
#define FLASHIF_DRIVER_MAGIC_VALUE          (0x47313936UL)
#define FLASHIF_DRIVER_VERSION_VALUE        (0x00010000UL)
*/

#define FLASH_START_ADRESS              (APP_ADRRESS_START)
#define FLASH_END_ADRESS                (FLASH_P_BLOCK_END_ADDR)  // stm32f107VC
#define FLASH_ERASE_SIZE                (FLASH_P_BLOCK_SECTOR_SIZES)	//一個sector 大小512 --stm32f107vc =  0x800
#define FLASH_ERASE_VALUE               (0xFFu)
#define FLASH_PHRASE_SIZE               (FLASH_P_PAGE_SIZES)  //page大小 = 8  stm32f107vc = 0x800
#define FLASH_PHRASE_SIZE_HALF          (FLASH_P_PAGE_SIZES/2)
#define FLASH_RESET_TIME_CNT            (0)
#define FLASH_OVER_TIME_CNT             (0x10000u)

//可能要干掉
#define FLASH_DRIVER_INIT_OFFSET        (0x0Cu)
#define FLASH_DRIVER_DEINIT_OFFSET      (0x10u)
#define FLASH_DRIVER_ERASE_OFFSET       (0x14u)
#define FLASH_DRIVER_WRITE_OFFSET       (0x18u)


/*****************************************************************************
 * Type Declarations
 *****************************************************************************/


void Flash_Init(bl_FlashDrvParam_t *par);
void Flash_Deinit(bl_FlashDrvParam_t *par);
void Flash_Write(bl_FlashDrvParam_t *par);
void Flash_Erase(bl_FlashDrvParam_t *par);


extern void Flash_Read(bl_FlashDrvParam_t *par);

#define FLASH_DRIVER_INIT(sa, pp)       Flash_Init(pp)
#define FLASH_DRIVER_DEINIT(sa, pp)     Flash_Deinit(pp)
#define FLASH_DRIVER_ERASE(sa, pp)      Flash_Erase(pp)
#define FLASH_DRIVER_WRITE(sa, pp)      Flash_Write(pp)



#endif
