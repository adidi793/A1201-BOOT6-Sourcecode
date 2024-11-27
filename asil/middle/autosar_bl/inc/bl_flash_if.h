/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of bl_flash_if.
 *
 *  \file       bl_flash_if.h
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
 *****************************************************************************/
#ifndef _BL_FLASH_IF_H_
#define _BL_FLASH_IF_H_
#include "bl_typedefs.h"
#include "bl_rte_funcfg.h"
#include "bl_flash_if_funcfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 783,3205,3210,3447 EOF*/
/*
 * 783:A subsequent declaration of the identifier means that it is being used
 * both as a structure/union member and also as a label, ordinary identifier.
 * The member of the structure is used through the structure object.
 *
 * 3205:The identifier is not used and could be removed.
 *
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_security_cfg.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \breif The flash IF is not initialized.*/
#define FLASHIF_STATUS_NOINIT       (0)
/** \breif The flash IF is idle.*/
#define FLASHIF_STATUS_IDLE         (1)
/** \breif The flash IF is writing data to flash device.*/
#define FLASHIF_STATUS_WRITING      (2)
/** \breif The flash IF is erasing a flash block.*/
#define FLASHIF_STATUS_ERASING      (3)

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias of the struct _tag_FlashIfBlockInfo.*/
typedef struct _tag_FlashIfBlockInfo bl_FlashIfBlockInfo_t;
/** \brief A alias of the struct _tag_FlashIfContext.*/
typedef struct _tag_FlashIfContext bl_FlashIfContext_t;
/** \brief A alias of the struct _tag_FlashDrvParam_t.*/
typedef struct _tag_FlashDrvParam bl_FlashDrvParam_t;
/** \brief A alias of the struct _tag_FlashDrvHeader.*/
typedef struct _tag_FlashDrvHeader bl_FlashDrvHeader_t;

/** \brief A pointer point to the operations function of flash driver.*/
typedef void
	(FLASHIF_DRIVER_FUN_POS * FLASHIF_DRIVER_FUNPTR_POS bl_FlashDrvFun_t)
	(bl_FlashDrvParam_t *par);
/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
struct _tag_FlashIfBlockInfo
{
	bl_Address_t address;   /**< the start address of a block.*/
	bl_Size_t totalSize;    /**< the total size of a block.*/
	bl_Size_t sectorSize;   /**< the eraseable size.*/
	bl_Size_t pageSize;     /**< the writable size.*/
#if (FLASHIF_FUN_ERASE_BY_BLOCKID == BL_FUN_ON)
	bl_u16_t phyBlockId;    /**< the start physical eraseable block id.*/
#endif
};

struct _tag_FlashIfContext
{
	bl_u16_t startId;       /**< the start index of operating block.*/  // 就是 g_FlashIfBlockList 的buf id
	bl_u16_t endId;         /**< the end index of operating block.*/
	bl_Address_t address;   /**< the address of a operation.*/
	bl_Size_t size;         /**< the remain size of a operation.*/
	bl_Size_t opsSize;      /**< the size of one operation.*/
	bl_Size_t doneSize;     /**< the operated size of one operation.*/
	bl_u32_t opsTime;       /**< the time of one operation*/
	bl_u32_t timeout;       /**< the timer of one operation*/
	bl_Buffer_t *buffer;    /**< the buffer of operating block.*/
	const struct _tag_FlashIfBlockInfo *block;   /**< the current block.*/
	bl_Return_t (* Operate)(const struct _tag_FlashIfBlockInfo *block,
								bl_Address_t address,
								bl_Size_t size,
								const bl_Buffer_t *buffer);
};

struct _tag_FlashDrvParam
{
	bl_u8_t type;           /**< the type*/  //好像是没用的變量
	bl_Return_t errCode;    /**< the result of a flash operation*/
#if (FLASHIF_FUN_ERASE_BY_BLOCKID == BL_FUN_ON)
	bl_u16_t phyBlockId;    /**< the physical block id.*/
#endif
	bl_Address_t address;   /**< the start address of a flash operation.*/
	bl_Size_t size;         /**< the size of a flash operation.*/
	bl_Buffer_t *buffer;    /**< the contents of data of a flash operation.*/
	void (*FeedDog)(void);  /**< the feed watchdog interface.*/
};

struct _tag_FlashDrvHeader
{
	bl_u16_t type;          /**< The type of the flash driver.*/
	bl_u16_t index;         /**< The index of the flash driver.*/
	bl_u32_t magic;         /**< The magic value of the flash driver*/
	bl_u32_t version;       /**< The current of the flash driver.*/
	bl_FlashDrvFun_t Init;  /**< Initialize the flash drvier.*/
	bl_FlashDrvFun_t Deinit;/**< Deinitialize the flash drvier*/
	bl_FlashDrvFun_t Erase; /**< Erase a block of the flash device.*/
	bl_FlashDrvFun_t Write; /**< Write data to the flash device.*/
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the flash IF module.*/
extern bl_Return_t Flsif_Init(void);
/** \brief deinitialize the flash IF module.*/
extern bl_Return_t Flsif_DeInit(void);
/** \brief read the data from the flash.*/
extern bl_Return_t Flsif_Read(bl_Address_t address,
							  bl_Size_t size,
							  bl_Buffer_t *buffer);
/** \brief begin to write data to the flash.*/
extern bl_Return_t Flsif_Write(bl_Address_t address,
							   bl_Size_t size,
							   const bl_Buffer_t *buffer);
/** \brief begin to erase the flash.*/
extern bl_Return_t Flsif_Erase(bl_Address_t address,
							   bl_Size_t size);


void flash_if_test(void);

#endif

