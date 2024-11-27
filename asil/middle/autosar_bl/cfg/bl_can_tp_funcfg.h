/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-09-14 11:59:35
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-09-26 14:08:17
 * @FilePath: \A1201 BOOT5\asil\middle\autosar_bl\cfg\bl_can_tp_funcfg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a functional configuration file of the CAN Transport
 *              Protocol module.
 *
 *  \file       bl_can_tp_funcfg.h
 *  \ingroup    cantp_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_CAN_TP_FUNCFG_H_
#define _BL_CAN_TP_FUNCFG_H_

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief The frame padding function.*/
#define CANTP_FUN_FRAME_PADDING         BL_FUN_ON	//填充功能，剩余的字節 都填充成 值 CANTP_FRAME_PADDING_VALUE


/** \brief The frame padding value.*/
#define CANTP_FRAME_PADDING_VALUE      (0x55u) // 0xaa only used for GL DMS, (0x55u) only for E31

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

