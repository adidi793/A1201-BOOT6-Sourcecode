/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the application callback module.
 *
 *  \file       bl_app.c
 *  \ingroup    application_callback_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 15/03/2010 | hewei.zhang      | N/A | Boot010001
 *      02.00.00 | 11/05/2011 | mingqing.tang    | N/A | Boot020001
 *      02.05.00 | 08/06/2011 | mingqing.tang    | N/A | Boot020006
 *      05.00.00 | 19/06/2013 | xin.shang        | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_app.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1503,1532,3112,3227,3673 EOF*/
/*
 * MISRA-C:2004 14.1(1503):The function is defined but is not used within this
 * project.
 * these functions will be used When correlative macro is enable.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are used in the Booting and LBM Module.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * App_GetIntegralityValue is reserved to future.(void)dct is used by IDE to
 * avoid a warning.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * MISRA-C:2004 16.7(3673): the pointer could be of type 'pointer to const'.
 * The App_GetIntegralityValue is used to get integrality value from the head
 * of the application, so it will modify the buffer.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details .
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
bl_Return_t App_PreprocessData(bl_DownContext_t *dct,
                               bl_Size_t size,
                               const bl_Buffer_t *buffer)
{
    (void)dct;
    (void)size;
    (void)buffer;
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details .
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
bl_Return_t App_GetIntegralityValue(bl_DownContext_t *dct,
                                    bl_Size_t *size,
                                    bl_Buffer_t *buffer)
{
    (void)dct;
    (void)size;
    (void)buffer;
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details .
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
bl_Return_t App_CheckCompatibility(void)
{
    return BL_ERR_OK;
}

