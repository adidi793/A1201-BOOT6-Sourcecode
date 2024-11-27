/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-06 09:12:20
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-06-06 09:17:44
 * @FilePath: \A12 BOOT1\asil\middle\autosar_bl\cfg\bl_can_if_funcfg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file to configurate the can_if module.
 *
 *  \file       bl_can_if_cfg.h
 *  \ingroup    flash_if_manager_module
 *  \author
 *
 *  \version    5.2.1
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      02.00.00 | 20/08/2013 | haiping.wang     | N/A | Boot020001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *      05.02.01 | 21/08/2014 | mingqing.tang    | N/A | BootSpec050002
 *
******************************************************************************/
#ifndef _BL_CAN_IF_FUNCFG_H_
#define _BL_CAN_IF_FUNCFG_H_

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief The canid can be set by other module.*/
#define CANIF_FUN_DYNAMIC_CANID             BL_FUN_OFF
/** \brief The can controller is full can.*/
#define CANIF_FUN_FULL_CAN_CONTROLLER       BL_FUN_OFF
/** \brief The can transeciver whether need be initialized.*/
#define CANIF_FUN_INIT_TRANSCEIVER          BL_FUN_ON    // Transceiver 的控制  即 第8腳 sb
/** \brief The can IF whether use 29bits id*/
#define CANIF_FUN_EXTERNED_ID               BL_FUN_OFF
/** \brief The number of controller used by can IF module.*/
//#define CANIF_USED_CONTROLLER_NUM           (0x01u)   //好像沒用到   CAN_USED_CONTROLLER_NUMBER

//目的是為了 動態 修改 CANID
#define CANIF_PHY_RX_HANDLE                 (0x00u)   // 相當於  gs_CanIfPduCfg  數組的 bufid 0-1
#define CANIF_TX_HANDLE                     (0x00u)   // 相當於  gs_CanIfPduCfg  數組的 bufid 2起始

/* only use F60 PEPS prj */
//#define CANIF_PHY_RX_CANID                  (0x685UL) //(0x723UL)
//#define CANIF_PHY_TX_CANID                  (0x605UL) //(0x7A3UL)
//#define CANIF_FUN_RX_CANID                  (0x6EFUL) //(0x7DFUL)
#define CANIF_PHY_RX_CANID                  (0x782UL) //(0x723UL)
#define CANIF_PHY_TX_CANID                  (0x78AUL) //(0x7A3UL)
#define CANIF_FUN_RX_CANID                  (0x7DFUL)

#endif

