/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the header file of the can transceiver.
 *
 *  \file       bl_can_transceiver.h
 *  \ingroup    driver_module
 *  \author     
 *
 *  \version    1.0.0
 *  \date       14/05/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | -------------
 *      01.00.00 | 14/05/2014 | ning.chen        | N/A | BootDrv010001
 *****************************************************************************/
#ifndef _BL_CAN_TRC_H_
#define _BL_CAN_TRC_H_
#include "bl_typedefs.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_can_if.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
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
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief Initialize the can transceiver.*/
extern void CanTrcv_Init(void);  // Cantrc_InitTransceiver
extern void CanTrcv_Disable(void);
#endif

