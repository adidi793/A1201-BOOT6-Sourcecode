/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-17 09:14:33
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-07-02 13:30:49
 * @FilePath: \A12 BOOT1\asil\middle\autosar_bl\sources\driver\bl_ram.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the ram module.
 *
 *  \file       bl_ram.c
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
#include "bl_common.h"
#include "bl_ram.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 306,1532,3227 EOF*/
/*
 * MISRA-C:2004 11.3(306):Cast a pointer to an integral type.
 * To address a flash must cast an integral type to a pointer.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 */

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details bl_ram write data.
 *
 *  \param[in]  addr - the memory address of the data to be written in.
 *  \param[in]  size - the size of the data to be written.
 *  \param[out] buffer - the address pointer of the data to be written.
 *
 *  \return If bl_ram write data successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK     - write data successfully.
 *  \retval BL_ERR_NOT_OK - write data unsuccessfully.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Ram_Write(bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer) {
    Bl_MemCpy((bl_Buffer_t *)address, buffer, size); /*lint !e511*/
    
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details bl_ram read data.
 *
 *  \param[in]  addr - the memory address of the data to be read from.
 *  \param[in]  size - the size of the data to be read.
 *  \param[out] buffer - the saving address pointer of the data to be read.
 *
 *  \return If bl_ram reads data successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK     - read data successfully.
 *  \retval BL_ERR_NOT_OK - read data unsuccessfully.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Ram_Read(bl_Address_t address,
    bl_Size_t size,
    bl_Buffer_t *buffer) {
    Bl_MemCpy(buffer, (const bl_Buffer_t *)address, size); // 读FLAG_REPROGRAM_ADDR上的值

    return BL_ERR_OK;
}
