/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the memory manager module.
 *
 *  \file       bl_memory.h
 *  \ingroup    memory_manager_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      05.01.00 | 27/03/2014 | cong.xu    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_MEMORY_H_
#define _BL_MEMORY_H_
#include "bl_typedefs.h"
#include "bl_memory_funcfg.h"

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
#define MEM_MEMORY_TYPE_FLASH           (0x0001u) /**< A flash.*/
#define MEM_MEMORY_TYPE_EEPROM          (0x0002u) /**< A eeprom.*/
#define MEM_MEMORY_TYPE_RAM             (0x0004u) /**< A ram.*/
#define MEM_MEMORY_TYPE_VIRTUAL         (0x0008u) /**< A logical memory.*/
#define MEM_MEMORY_TYPE_EXT_FLASH       (0x0010u) /**< like flash.*/
#define MEM_MEMORY_TYPE_EXT_EEPROM      (0x0020u) /**< like eeprom.*/
#define MEM_MEMORY_TYPE_EXT_RAM         (0x0040u) /**< like ram.*/
#define MEM_MEMORY_TYPE_EXT_VIRTUAL     (0x0080u) /**< like virtual.*/
#define MEM_MEMORY_TYPE_EXTERNAL_DRV    (0x8000u) /**< the extrnal driver.*/

#define MEM_MEMORY_TYPE_SIZE        (2) /** the size of type of the memory.*/
#define MEM_MEMORY_INDEX_SIZE       (2) /** the size of index of the memory.*/

#define MEM_ERROR_DRVID     BL_ERROR_ID_16BIT   /**< invalid driver id.*/
#define MEM_ERROR_MEMID     BL_ERROR_ID_16BIT   /**< invalid memory id.*/

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias of the struct _tag_MemoryOperations.*/
typedef struct _tag_MemoryOperations bl_MemoryOperations_t;
/** \brief A alias of the struct _tag_MemoryInfo.*/
typedef struct _tag_MemoryInfo bl_MemoryInfo_t;
/** \brief A alias of the struct _tag_MemoryInfoList.*/
typedef struct _tag_MemoryInfoList bl_MemoryInfoList_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/**
 *  \brief the memory operations.
 */
struct _tag_MemoryOperations
{
    bl_Return_t (*Init)(void);      /**< the initialization interface.*/
    bl_Return_t (*Deinit)(void);    /**< the deinitialization interface.*/
    bl_Return_t (*Write)(bl_Address_t address,
                          bl_Size_t size,
                          const bl_Buffer_t *buffer); /**< the write interface.*/
    bl_Return_t (*Read)(bl_Address_t address,
                         bl_Size_t size,
                         bl_Buffer_t *buffer);  /**< the read interface.*/
    bl_Return_t (*Erase)(bl_Address_t address,
                          bl_Size_t size);      /**< the erase interface.*/
};

/**
 *  \brief the memory informations.
 */
struct _tag_MemoryInfo
{
    bl_u16_t type;              /**< the type of a memory.*/
    bl_u16_t index;             /**< the index of a memory.*/
    bl_Address_t addr;          /**< the base address of a memory.*/
    bl_Size_t size;             /**< the size of a memory.*/
    const struct _tag_MemoryOperations * ops;/**< the operations list.*/
};

/**
 *  \brief the list of the memory informations.
 */
struct _tag_MemoryInfoList
{
    bl_u16_t number;            /**< the number of memory managed by module.*/
    const struct _tag_MemoryInfo *memList;  /**< the memory information list.*/
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief This function is used to initialize the memory manager module.*/
extern bl_Return_t bl_Mem_Init(void);
/** \brief This function is used to initialize a memory by id.*/
extern bl_Return_t Mem_InitMemory(bl_u16_t id);
/** \brief This function is used to de-initialize a memory by id.*/
extern bl_Return_t Mem_DeinitMemory(bl_u16_t id);
/** \brief This function is used to read data from a memory.*/
extern bl_Return_t Mem_Read(bl_u16_t id,
                             bl_Address_t address,
                             bl_Size_t size,
                             bl_Buffer_t *buffer);
/** \brief This function is used to write data to a memory.*/
extern bl_Return_t Mem_Write(bl_u16_t id,
                              bl_Address_t address,
                              bl_Size_t size,
                              const bl_Buffer_t *buffer);
/** \brief This function is used to erase a memory.*/
extern bl_Return_t Mem_Erase(bl_u16_t id,bl_Address_t address,bl_Size_t size);
/** \brief This function is used to look for a memory.*/
extern bl_Return_t Mem_LookforMemId(bl_u16_t type,
                                     bl_u16_t index,
                                     bl_u16_t *mid);
/** \brief This function is used to check the driver status.*/
extern bl_Return_t Mem_IsExternalDrv(bl_u16_t id);
/** \brief This function is used to check the memory status.*/
extern bl_Return_t Mem_IsReadable(bl_u16_t id);

#endif

