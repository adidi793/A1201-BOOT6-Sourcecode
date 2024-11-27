/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file to make seed and verify a key.
 *
 *  \file       bl_access_udsdemo.h
 *  \ingroup    security_manager_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2018
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 25/03/2018 | xucong      | N/A | Boot010001
 *
 *****************************************************************************/
#ifndef _BL_ACC_UDSDEMO_H_
#define _BL_ACC_UDSDEMO_H_
#include "bl_typedefs.h"
/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447 EOF*/
/*
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_security_cfg.c file.
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
/** \brief Initialize the demo algorithm used by UDS.*/
extern void Acc_UdsDemoInit(void);
/** \brief Make a new seed.*/
extern void Acc_UdsDemoMakeNewSeed(void);
/** \brief Get current seed value.*/
extern void Acc_UdsDemoGetSeed(bl_Buffer_t *seed);
/** \brief Verify the transmitted key whether is right.*/
extern bl_Return_t Acc_UdsDemoVerifyKey(const bl_Buffer_t *key);

bl_u32_t _Acc_UdsDemoCalculateKey(void);
 
#endif

