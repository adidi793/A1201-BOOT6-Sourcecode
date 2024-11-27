/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the application callback module.
 *
 *  \file       bl_app.h
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
#ifndef _BL_APP_H_
#define _BL_APP_H_
#include "bl_typedefs.h"
#include "bl_rte.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_dcm_cfg.c file.
 * The g_UdsPlatformAdapter is used in the bl_process_driver_cfg.c file.
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
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief This function is used to preprocess data.*/
extern bl_Return_t App_PreprocessData(bl_DownContext_t *dct,
                                      bl_Size_t size,
                                      const bl_Buffer_t *buffer);
/** \brief This function is used to get the integrality value.*/
extern bl_Return_t App_GetIntegralityValue(bl_DownContext_t *dct,
                                           bl_Size_t *size,
                                           bl_Buffer_t *buffer);
/** \brief This function is used to check the compatibility.*/
extern bl_Return_t App_CheckCompatibility(void);

#endif

