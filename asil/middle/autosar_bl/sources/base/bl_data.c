/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the data manager module.
 *
 *  \file       bl_data.c
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
#include "bl_common.h"
#include "bl_memory.h"
#include "bl_data.h"
#include "bl_data_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 506,602,781,1251,1257,1503,1532,2100,2103,3112,3227,3453 EOF*/
/*
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * MISRA-C:2004 5.6(781):The identifier is used as a structure/union member.
 * The member of the structure is used through the structure object.
 *
 * 1251:Suffixed integer constant causes implicit conversion of other
 * (integral) operand.
 *
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * MISRA-C:2004 14.1(1503):The function is defined but is not used within this
 * project.
 * these functions will be used When correlative macro is enable.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 2100:Integral promotion : unsigned char promoted to signed int.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * These macros do not have side-effect.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
/** check the id whether is a configured data.*/
#define DM_IS_VALID_ID(did)     ((did) < g_DataInfoList.number) // 原D50也是((did) < g_DataInfoList.number)  //徐工改的DM_NMBr_OF_MANAGED_Data_ID
/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
/** \brief check the parameters.*/
static bl_Return_t _Dm_IsDefaultData(const bl_DataInfo_t *data);
/** \brief read the data.*/
static bl_Return_t _Dm_ReadData(const bl_DataInfo_t *data,
    bl_Size_t size,
    bl_Buffer_t *buffer);

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
 *  \details Read a data from the memory.
 *
 *  \param[in]  did    - the index of a data in the data information list.
 *  \param[in]  size   - the size of a data to be read.
 *  \param[out] buffer - the buffer of a data to be read.
 *
 *  \return If the data is read successfully return BL_ERR_OK, otherwise return
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the data is read successfully.
 *  \retval BL_ERR_NOT_OK - the data is not read successfully.
 *
 *  \warning If the size of the data is zero, this data can be read any size.
 *
 *  \since V4.0.0
 *
 *****************************************************************************/
bl_Return_t Dm_ReadData(bl_u16_t did,
    bl_Size_t size,
    bl_Buffer_t *buffer) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    const bl_DataInfo_t *data;
    
    if (DM_IS_VALID_ID(did) && (size > 0UL)) {
        data = &g_DataInfoList.dataList[did];
        
        if ((size == data->size) || (0UL == data->size)) {
            ret = _Dm_ReadData(data, size, buffer);
        }
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details Write a data to the memory.
 *
 *  \param[in]  did    - the index of a data in the data information list.
 *  \param[in]  size   - the size of a data to be written.
 *  \param[in]  buffer - the buffer of a data to be written.
 *
 *  \return If the data is written successfully return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the data is written successfully.
 *  \retval BL_ERR_NOT_OK - the data is not written successfully.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
bl_Return_t Dm_WriteData(bl_u16_t did,
    bl_Size_t size,
    const bl_Buffer_t *buffer) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    const bl_DataInfo_t *data;
    
    if (DM_IS_VALID_ID(did) && (size > 0UL)) { //((did) < g_DataInfoList.number)
        data = &g_DataInfoList.dataList[did];   // gs_DataInfo
        
        BL_DEBUG_ASSERT_CONFIG(data->type == DM_DATA_TYPE_READ_AND_WRITE);
        
        if ((DM_DATA_TYPE_READ_AND_WRITE == data->type)
            && (data->size == size)) {
            ret = Mem_Write(data->memId, data->localAddr, size, buffer);
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Write a default data.
 *
 *  \param[in]  did    - the index of a data in the data information list.
 *
 *  \return If the data is written successfully return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK             - the data is written successfully.
 *  \retval BL_ERR_NOT_OK         - the data is not written successfully.
 *
 *  \note If not configure the memory id, return BL_ERR_NOT_OK.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
bl_Return_t Dm_WriteDefaultData(bl_u16_t did) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    const bl_DataInfo_t *data;
    
    if (DM_IS_VALID_ID(did)) { //did=1,标识application
        data = &g_DataInfoList.dataList[did];   // gs_DataInfo[1]
        if (data->defData != BL_NULL_PTR) {     // &gs_defaultDataList[4]
            /* -----------------------------------------------------------------------A12
            memId=1,localAddr=0x1BFFC,size=4,defData=&gs_defaultDataList[4]
            ----------------------------------------------------------------------- */
            ret = Mem_Write(data->memId,
                    data->localAddr,
                    data->size,
                    data->defData);
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Compare a data with its default data.
 *
 *  \param[in]  did - the index of a data in the data information list.
 *
 *  \return If the data is equal to the default data, return BL_ERR_OK
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK         - the data is read successfully.
 *  \retval BL_ERR_NOT_OK     - the data is not read successfully.
 *
 *  \since  V4.0.0
 *  // 相當於讀取 0x00003FF8UL 或者 0x0003BFF0UL 地址上的數據，然後和 gs_defaultDataList 上的值比較
 *****************************************************************************/
bl_Return_t Dm_IsDefaultData(bl_u16_t did) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    const bl_DataInfo_t *data;
    /* -----------------------------------------------------------------------A12
    检查did是否在g_DataInfoList.number的范围内
    DM_DATA_0_SIZE,DM_DATA_0_DDP
    ----------------------------------------------------------------------- */
    if (DM_IS_VALID_ID(did)) { //did = 0. did的值一直为0
        data = &g_DataInfoList.dataList[did];
        if (data->defData != BL_NULL_PTR) { //&gs_defaultDataList[0]
            ret = _Dm_IsDefaultData(data);
        }
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check whether the did is valid.
 *
 *  \param[in]  did - the index of a data in the data information list.
 *
 *  \return If the did is valid return BL_ERR_OK, otherwise return
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK         - the did is valid.
 *  \retval BL_ERR_NOT_OK     - the did is invalid.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
bl_Return_t Dm_IsValidDataId(bl_u16_t did) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (DM_IS_VALID_ID(did)) { //意思是did < 2
        ret = BL_ERR_OK;
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details Clear a data.
 *
 *  \param[in]  did - the index of a data in the data information list.
 *
 *  \return If the data is cleared successfully return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK             - the data is cleared successfully.
 *  \retval BL_ERR_NOT_OK         - the data is not cleared successfully.
 *
 *  \warning This interface can only clear the data which is configured the
 *           default data.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
bl_Return_t Dm_ClearData(bl_u16_t did) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u32_t i;
    bl_Buffer_t buffer[DM_MAX_DATA_SIZE] = {0};
    const bl_DataInfo_t *data;
    const bl_Buffer_t *tmpBuf;
    
    if (DM_IS_VALID_ID(did)) { //检查did是否在g_DataInfoList.number的范围内
        data = &g_DataInfoList.dataList[did];  //指向實體 gs_DataInfo[did]
        
        if ((DM_DATA_TYPE_READ_AND_WRITE == data->type) //检查数据类型是否为RW
            && (data->defData != BL_NULL_PTR) //默认值不为NULL
            && (data->size > 0UL)) { //数据长度不为0
            tmpBuf = data->defData;   //指向 gs_defaultDataList
            for (i = 0; i < data->size; i++) { //故意取反；
                buffer[i] = ~tmpBuf[i]; //默认值取反后的结果放入buffer中
            }
            //取反後，再存入
            ret = Mem_Write(data->memId,
                    data->localAddr,
                    data->size,
                    buffer); /*lint !e772*//*PRQA S 3354*/
        }
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details Get the memory id of a data.
 *
 *  \param[in]  did - the index of a data in the data information list.
 *
 *  \return Return the memory id of data.
 *
 *  \note   If the data id is invalid, return DM_ERROR_DATAID.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
bl_u16_t Dm_GetMemid(bl_u16_t did) {
    bl_u16_t memId;
    
    if (DM_IS_VALID_ID(did)) {
        memId = g_DataInfoList.dataList[did].memId;
    } else {
        memId = DM_ERROR_DATAID;
    }
    
    return memId;
}

/**************************************************************************//**
 *
 *  \details Get the data size of a data.
 *
 *  \param[in]  did - the index of a data in the data information list.
 *
 *  \return Return the data size of data.
 *
 *  \note   If the data id is invalid, return 0.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
bl_Size_t Dm_GetDataSize(bl_u16_t did) {
    bl_Size_t size;
    
    if (DM_IS_VALID_ID(did)) {
        size = g_DataInfoList.dataList[did].size;
    } else {
        size = 0UL;
    }
    
    return size;
}

/**************************************************************************//**
 *
 *  \details Check the parameters.
 *
 *  \param[in]  data  - the pointer of a data in the data information list.
 *
 *  \return If the the data is checked successfully return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK               - the data is checked successfully.
 *  \retval BL_ERR_NOT_OK           - the data is not checked successfully.
 *
 *  \since  V4.0.0
 *
 *****************************************************************************/
static bl_Return_t _Dm_IsDefaultData(const bl_DataInfo_t *data) {
    bl_Return_t ret;
    bl_Size_t i;
    bl_Buffer_t buffer[DM_MAX_DATA_SIZE];
    /* -----------------------------------------------------------------------A12
    0x20007FC0u地址上的数据读取后，返回值默认为 BL_ERR_OK。
    检查gs_DataInfo中的defData值和地址中读取的是否一致
    ----------------------------------------------------------------------- */
    ret = Mem_Read(data->memId, data->localAddr, data->size, buffer);
    if (BL_ERR_OK == ret) {
        for (i = 0; i < data->size; i++) {
            //检查gs_DataInfo中的defData值和地址中读取的是否一致
            if (data->defData[i] != buffer[i]) {
                ret = BL_ERR_NOT_OK;
                break;
            }
        }
    }
    return ret;
}


/**************************************************************************//**
 *
 *  \details Read a data from the memory.
 *
 *  \param[in]  did    - the index of a data in the data information list.
 *  \param[in]  size   - the size of a data to be read.
 *  \param[out] buffer - the buffer of a data to be read.
 *
 *  \return If the data is read successfully return BL_ERR_OK, otherwise return
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the data is read successfully.
 *  \retval BL_ERR_NOT_OK - the data is not read successfully.
 *
 *  \since V5.0.0
 *
 *****************************************************************************/
static bl_Return_t _Dm_ReadData(const bl_DataInfo_t *data,
    bl_Size_t size,
    bl_Buffer_t *buffer) {
    bl_Return_t ret;
    
    ret = Mem_IsReadable(data->memId);
    if (BL_ERR_OK == ret) {
        ret = Mem_Read(data->memId, data->localAddr, data->size, buffer);
    } else {
        if (data->defData != BL_NULL_PTR) {
            Bl_MemCpy(buffer, data->defData, size);
            
            ret = BL_ERR_OK;
        }
    }
    
    return ret;
}

