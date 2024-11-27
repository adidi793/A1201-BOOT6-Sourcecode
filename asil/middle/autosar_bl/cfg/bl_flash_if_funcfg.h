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
#ifndef _BL_FLASH_IF_FUNCFG_H_
#define _BL_FLASH_IF_FUNCFG_H_

#include "boot_app_cfg.h"

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief If a flash driver is erased by physical block id, enable it.*/
#define FLASHIF_FUN_ERASE_BY_BLOCKID        BL_FUN_OFF
/** \brief If a flash is not immediately read, enable it.*/
#define FLASHIF_FUN_SPECIAL_READ            BL_FUN_ON
/** \brief If logical and physical address are different, enable it.*/
#define FLASHIF_FUN_CONVERT_ADDRESS         BL_FUN_OFF

#define FLASHIF_DRIVER_STARTADDRESS         (FLAG_RAM_DRIVER_ADDR)		//PRM中  將 RAM_FLASH_DRV 定為3E00-3FEF
//#define FLASHIF_DRIVER_MAGIC_VALUE          FLASH_DRIVER_VERSION_MAGIC
//#define FLASHIF_DRIVER_VERSION_VALUE        FLASH_DRIVER_VERSION_VERSION

#define FLASHIF_NUMBER_OF_BLOCK             (1u)

#define FLASHIF_DRIVER_FUN_POS
#define FLASHIF_DRIVER_FUNPTR_POS

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

