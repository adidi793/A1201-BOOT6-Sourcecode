/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of the data manager module.
 *
 *  \file       bl_data.h
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
#ifndef _BL_DATA_H_
#define _BL_DATA_H_
#include "bl_typedefs.h"

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
#define DM_DATA_TYPE_READ_ONLY       (0x0001u)          /** read-only type.*/
#define DM_DATA_TYPE_READ_AND_WRITE  (0x0002u)          /** read-write type.*/
#define DM_ERROR_DATAID              BL_ERROR_ID_16BIT  /** invalid data id.*/

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/
/** \brief A alias（意思是：别名） of the struct _tag_DataInfo.*/
typedef struct _tag_DataInfo bl_DataInfo_t;

/** \brief A alias of the struct _tag_DataInfoList.*/
typedef struct _tag_DataInfoList bl_DataInfoList_t;

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/
/** \brief the data informations.*/
struct _tag_DataInfo {
    bl_u16_t type;              /**< the type of data, RO or RW.*/
    bl_u16_t memId;             /**< the memory id of data.*/
    bl_Address_t localAddr;     /**< the memory local offset of data.*/
    bl_Size_t size;             /**< the size of data.*/
    const bl_Buffer_t *defData; /**< the default data.*/
};

/** \brief the list of the data informations.*/
struct _tag_DataInfoList {
    bl_u16_t number;          /**< the number of data managed by module.*/
    const struct _tag_DataInfo *dataList;  /**< the data information list.*/
};

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/
/** \brief This function is used to read data from the memory.*/
extern bl_Return_t Dm_ReadData(bl_u16_t did,
    bl_Size_t size,
    bl_Buffer_t *buffer);
/** \brief This function is used to write data to the memory.*/
extern bl_Return_t Dm_WriteData(bl_u16_t did,
    bl_Size_t size,
    const bl_Buffer_t *buffer);
/** \brief This function is used to write data to default data buffer.*/
extern bl_Return_t Dm_WriteDefaultData(bl_u16_t did);
/** \brief This function is used to compare data with default data.*/
extern bl_Return_t Dm_IsDefaultData(bl_u16_t did);
/** \brief Check whether the did is valid.*/
extern bl_Return_t Dm_IsValidDataId(bl_u16_t did);
/** \brief This function is used to clear default data.*/
extern bl_Return_t Dm_ClearData(bl_u16_t did);
/** \brief This function is used to get memory id.*/
extern bl_u16_t Dm_GetMemid(bl_u16_t did);
/** \brief This function is used to get the size of a data.*/
extern bl_Size_t Dm_GetDataSize(bl_u16_t did);

#endif

