/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-17 09:14:33
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-07-26 15:45:44
 * @FilePath: \A12 BOOT1\asil\middle\autosar_bl\sources\cfg\bl_can_if_cfg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a source file to configurate the can_if module.
 *
 *  \file       bl_can_if_lcfg.c
 *  \ingroup    can_if_module
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
#include "bl_can_tp.h"
#include "bl_can_if_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1531,3120,3211 EOF*/
/*
 * 1531:The object is referenced in only one translation unit.
 * g_CanIfRxPduCfg and g_CanIfTxPduCfg are configurations.
 *
 * 3120:Hard-coded magic integer constant.
 * This is a only configuration.
 *
 * 3211:The global identifier is defined but not used.
 * The g_CanIfRxPduCfg and g_CanIfTxPduCfg are used in the bl_can_if.c file.
 *
 */


/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/* define status of pdu*/
#define CANIF_RECEIVE                              (0)
#define CANIF_TRANSMITE                            (1)

/* define types of pdu*/
#define CANIF_STANDARD                             (0)
#define CANIF_EXTERNED                             (1)

#define CANIF_NUMBER_OF_PDU         (CANIFRX_TOTAL_NUM + CANIFTX_TOTAL_NUM)
/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/
#if (CANIF_FUN_DYNAMIC_CANID == BL_FUN_ON)  //不支持動態修改
STATIC bl_CanIfPduCfg_t gs_CanIfPduCfg[CANIF_NUMBER_OF_PDU] = { //先是兩個收 再是發
    // status          //type          //handle          //can id            //canfd標志  BRS標志
    {
        CANIF_RECEIVE, CANIF_STANDARD, HANDLE_CAN_RX0, CANIF_PHY_RX_CANID, 1u, 1u   //
    },
    {
        CANIF_RECEIVE, CANIF_STANDARD, HANDLE_CAN_RX0, CANIF_FUN_RX_CANID, 1u, 1u
    },
    {
        CANIF_TRANSMITE, CANIF_STANDARD, HANDLE_CAN_TX0, CANIF_PHY_TX_CANID, 1u, 1u
    },
};
#else   //這是默認選擇路徑
STATIC const bl_CanIfPduCfg_t gs_CanIfPduCfg[CANIF_NUMBER_OF_PDU] = {
    // status          //type          //handle          //can id            //canfd標志  BRS標志
    {
        CANIF_RECEIVE, CANIF_STANDARD, HANDLE_CAN_RX0, CANIF_PHY_RX_CANID, 1u, 1u    //物理尋址  第三handle 第4 id
    },
    {
        CANIF_RECEIVE, CANIF_STANDARD, HANDLE_CAN_RX0, CANIF_FUN_RX_CANID, 1u, 1u       //功能尋址
    },
    {
        CANIF_TRANSMITE, CANIF_STANDARD, HANDLE_CAN_TX0, CANIF_PHY_TX_CANID, 1u, 1u
    },
};
#endif

const bl_CanIfRxList_t g_CanIfRxPduCfg = {
    CANIFRX_TOTAL_NUM, //number 接收标识符数量
    &gs_CanIfPduCfg[0], //rxList
    &Cantp_RxIndication, //RxInd 有兩個  第一參數=2
};

const bl_CanIfTxList_t g_CanIfTxPduCfg = {
    CANIFTX_TOTAL_NUM, //number 发送标识符数量
    &gs_CanIfPduCfg[2], //txList
    &Cantp_TxConfirmation, //TxConf
};

