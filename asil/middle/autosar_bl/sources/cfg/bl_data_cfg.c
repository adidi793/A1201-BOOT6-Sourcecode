/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a source file to configurate the data manager module.
 *
 *  \file       bl_data_cfg.c
 *  \ingroup    data_manager_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      04.00.00 | 06/03/2013 | xin.shang        | N/A | Boot040001
 *      04.02.00 | 11/03/2013 | xin.shang        | N/A | Boot040003
 *      04.03.00 | 11/03/2013 | xin.shang        | N/A | Boot040004
 *      04.11.00 | 13/03/2013 | xin.shang        | N/A | Boot040012
 *      04.13.00 | 15/03/2013 | xin.shang        | N/A | Boot040014
 *      04.14.00 | 20/03/2013 | xin.shang        | N/A | Boot040015
 *      04.21.00 | 21/03/2013 | xin.shang        | N/A | Boot040022
 *      04.31.00 | 04/04/2013 | xin.shang        | N/A | Boot040032
 *      05.00.00 | 19/06/2013 | xin.shang        | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_data_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1257,1531,3120,3211 EOF*/
/*
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1531:The object is referenced in only one translation unit.
 * g_DataInfoList is a configurations.
 *
 * 3120:Hard-coded magic integer constant.
 * This is a only configuration.
 *
 * 3211:The global identifier is defined but not used.
 * The g_DataInfoList is used in the bl_data.c file.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
/** Calculate the sum of data size.*/

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
/******************************************************************************
 *  \details The default data list.
 *****************************************************************************/
const bl_Buffer_t gs_defaultDataList[DM_DATA_SIZE_SUM] =
{
    0xA5u,0xA5u,0xE5u,0x34u,  //REPROGRAM_DATA
    0xAAu,0xAAu,0x04u,0xE5u,  ///application valid flag*/
    0xAAu,0xAAu,0x04u,0xE5u,  ///application valid flag1 */  //
    0xA5u,0xA5u,0xE5u,0x34u,  // app REPROGRAM_DATA
};


/**************************************************************************//**
 *  \details The data informations.
 * 
 *****************************************************************************/
//    bl_u16_t type;              /**< the type of data, RO or RW.*/
//    bl_u16_t memId;             /**< the memory id of data.*/
//    bl_Address_t localAddr;     /**< the memory local offset of data.*/
//    bl_Size_t size;             /**< the size of data.*/
//    const bl_Buffer_t *defData; /**< the default data.*/
STATIC const bl_DataInfo_t gs_DataInfo[DM_NUMBER_OF_MANAGED_DATA] =
{
    {
        DM_DATA_0_TYPE,DM_DATA_0_MEMID,DM_DATA_0_LOCAL_ADDR,  //REPROGRAM_DATA   BOOTM_REPROGRAM_FLAG_DID
        DM_DATA_0_SIZE,DM_DATA_0_DDP
    },
    {
        DM_DATA_1_TYPE,DM_DATA_1_MEMID,DM_DATA_1_LOCAL_ADDR,  /*application program Valid  */  //BOOTM_RESET_FLAG_DID
        DM_DATA_1_SIZE,DM_DATA_1_DDP
    },
};


/******************************************************************************
 *  \details The data informations list.
 *
 *  \warning Do not change the name of this variable.
 *****************************************************************************/
const bl_DataInfoList_t g_DataInfoList =
{
    DM_NUMBER_OF_MANAGED_DATA,gs_DataInfo,
};
/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

