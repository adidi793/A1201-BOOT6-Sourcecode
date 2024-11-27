/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-08-07 15:45:13
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-08-27 00:40:18
 * @FilePath: \A1201 BOOT4\asil\middle\autosar_bl\cfg\bl_adpt_uds_platform_cfg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the head file to configurate the adapter module based
 *              on uds platform.
 *
 *  \file       bl_adpt_uds_platform_cfg.h
 *  \ingroup    communication_protocol_stack_module
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
#ifndef _BL_ADPT_UDS_PLATFORM_CFG_H_
#define _BL_ADPT_UDS_PLATFORM_CFG_H_

#include "nvm.h"

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief the 11 service save the reset flag.*/
#if 0  //先注釋
#define ADPT_RESETBYSERVICE_FLAGID      (0x01u)
#define ADPT_FINGERPRINT_FLAGID         (0x02)
#define ADPT_PROGRAMMING_APP_VALID_FLAGID    (0x01u)
#define ADPT_PROGRAMMING_DATE_FLAGID    (0x03u)
#define ADPT_PGM_F_ATMP_CNTID    (0x03)
#endif

#define ADPT_SESSION_RESPONSE_LEN       (0x04u)  //會話模式應答，後面跟着的時間參數；

#define ADPT_COMMCONTROLTYPE_DATA       (0x01u)

#define ADPT_MAXNUM_OF_BLOCKLENGTH      (FLASH_P_BLOCK_SECTOR_SIZES+2u) //(0x402u) 1026
#define ADPT_MAXNUM_OF_TRANS_DATA       (ADPT_MAXNUM_OF_BLOCKLENGTH - 2u)

#define ADPT_UDS_TX_HANDLE              (0u)
#define ADPT_UDS_TX_TIMEOUT             (200000UL)
#define ADPT_UDS_CANFD_TX_TIMEOUT             (ADPT_UDS_TX_TIMEOUT)

#define ADPT_UDS_COMOPS_HANDLE          BL_ERROR_ID_16BIT
#define ADPT_UDS_NUMBER_OF_GATEWAY      (0x01u)
#define ADPT_UDS_CONVERSION_DATA_LENGTH (0x02u)

#define ADPT_STAY_IN_BOOT_RESPONSE_HID  (0xF5u)
#define ADPT_STAY_IN_BOOT_RESPONSE_LID  (0x18u)

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

#endif

