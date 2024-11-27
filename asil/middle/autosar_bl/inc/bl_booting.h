/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the booting manager module.
 *
 *  \file       bl_booting.h
 *  \ingroup    booting_manager_module
 *  \author     
 *
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
#ifndef _BL_BOOTING_H_
#define _BL_BOOTING_H_
#include "bl_typedefs.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 783,3205,3210,3447,3453 EOF*/
/*
 * 783:A subsequent declaration of the identifier means that it is being used
 * both as a structure/union member and also as a label, ordinary identifier.
 * The member of the structure is used through the structure object.
 *
 * 3205:The identifier is not used and could be removed.
 * The bl_BootingCheckerList_t is used in the bl_booting.c file.
 *
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_dcm_cfg.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * These macros have no side-effect.
 *
 */


/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
#define BOOTING_FLAG_NULL               (0x0000u)/**< flag is null.*/
#define BOOTING_FLAG_REPROGRAM_VALID    (0x0001u)/**< reprogram valid.*/
#define BOOTING_FLAG_APPLICATION_VALID  (0x0002u)/**< application valid.*/
#define BOOTING_FLAG_SYSTEM_RESET       (0x0004u)/**< system reset flag.*/
#define BOOTING_FLAG_USER_DEFINE        (0x8000u)/**< user flag mask.*/

#define BOOTING_MAKE_USER_FLAG(flag)    (BOOTING_FLAG_USER_DEFINE \
                                            | (bl_BootingFlag_t)(flag))
/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** a type for _tag_BootingCheckerList.*/
typedef struct _tag_BootingCheckerList bl_BootingCheckerList_t;
/** a function pointer is used to call the checker*/
typedef bl_Return_t (*bl_BootingChecker_t)(bl_BootingFlag_t *flag);

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/** \brief the list of the booting checkers.*/
struct _tag_BootingCheckerList
{
    bl_u16_t number;                        /**< the number of checkers.*/
    const bl_BootingChecker_t *checkerList; /**< the data information list.*/
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief check the booting flags.*/
extern bl_BootingFlag_t Bootm_CheckBootingRequirement(void);
/** \brief go to the application.*/
extern void Bootm_GotoApplication(void);
/** \brief This function is used to check reprogram flag.*/
extern bl_Return_t Bootm_ReprogramFlagChecker(bl_BootingFlag_t *flag);
/** \brief This function is used to check application valid flag.*/
extern bl_Return_t Bootm_AppValidFlagChecker(bl_BootingFlag_t *flag);
/** \brief This function is used to check reset flag.*/
extern bl_Return_t Bootm_ResetFlagChecker(bl_BootingFlag_t *flag);

#endif

