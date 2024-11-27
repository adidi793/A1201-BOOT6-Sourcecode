/**************************************************************************//**
 *
 *  \copyright  This software is the property of Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              Technologies.
 *
 *  \brief      This is a head file to configurate the data of the booting
 *              manager module.
 *
 *  \file       bl_booting_cfg.h
 *  \ingroup    booting_manager_module
 *  \author
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 15/03/2010 | hewei.zhang      | N/A | Boot010001
 *      01.02.00 | 16/06/2010 | hewei.zhang      | N/A | Boot010003
 *      01.03.00 | 18/08/2010 | hewei.zhang      | N/A | Boot010004
 *      02.00.00 | 16/05/2011 | mingqing.tang    | N/A | Boot020001
 *      02.05.00 | 25/05/2011 | mingqing.tang    | N/A | Boot020006
 *      03.00.00 | 15/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.02.00 | 20/10/2011 | mingqing.tang    | N/A | Boot030003
 *      05.00.00 | 19/06/2013 | xin.shang        | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_BOOTING_CFG_H_
#define _BL_BOOTING_CFG_H_
#include "bl_booting.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The g_BootingCheckerList is used in the bl_booting.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The g_BootingCheckerList is Declared in the header file.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/*the booting configurations*/
#define BOOTM_NUMBER_OF_BOOTING_CHECKER     (2u)

#define BOOTM_REPROGRAM_FLAG_DID            (0u)	// ram上的標志。重編程標志
#define BOOTM_RESET_FLAG_DID                (1u) /* app valid id*/
#if 0
#define BOOTM_FP_DID                        (2u) /* pgm fingerprinter id*/
#define BOOTM_PROG_ATTEMP_CNT_DID           (3u) /* pgm f_attmp, success count and PGM date id*/
#endif
//#define BOOTM_PROGRAM_CNT_DID            	(2u) 

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
/** \brief The booting checker list.*/
extern const bl_BootingCheckerList_t g_BootingCheckerList;

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

