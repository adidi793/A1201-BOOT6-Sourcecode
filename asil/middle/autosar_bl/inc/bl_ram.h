/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the ram module.
 *
 *  \file       bl_ram.h
 *  \ingroup    ram_module
 *  \author     
 *
 *  \version    2.0.0
 *  \date       25/09/2012
 *
 *  \par        Changelist
 *      Version  | Date       | Authors    | CR# | Descriptions
 *      -------- | ---------- | ---------- | --- | ------------
 *      02.00.00 | 25/09/2012 | xin.shang  | N/A | Boot020001
 *
 *****************************************************************************/
#ifndef _BL_RAM_H_
#define _BL_RAM_H_
#include "bl_typedefs.h"

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
/** \brief This function is used to write ram memory.*/
extern bl_Return_t Ram_Write(bl_Address_t address,
                             bl_Size_t size,
                             const bl_Buffer_t *buffer);
/** \brief This function is used to read ram memory.*/
extern bl_Return_t Ram_Read(bl_Address_t address,
                            bl_Size_t size,
                            bl_Buffer_t *buffer);

#endif

