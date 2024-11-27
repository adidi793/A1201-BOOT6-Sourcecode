/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief
	* @function List:
	******************************************************************************
	* @attention
	*
	*
	* <h2><center>&copy; COPYRIGHT 2021 </center></h2>
	******************************************************************************
	* @History:
	* @Author:
	* @Data:
	* @Version:
*/
#include "common_types.h"
#include "common_memory.h"
#include "queue_entity.h"
#include "can.h"
#include "nvic.h"
#ifdef CAN_LS_NM
#include "Ls_Nm.h"
#endif
#ifdef CAN_AUTOSAR_EN
#include "can_drive.h"
#include "CanIf.h"
#endif
#include "bl_can.h"
#include "bl_common.h"

static void can_tx_task(void);

#ifdef CAN_DRIVE_QUEUE_ENABLE
static sequential_queue_s  can_tx_queue;
static can_queue_elem_s  can_tx_qbuf[CAN_ID_0_TX_QUEUE_DEPTH]; /* 隊列 */
static sequential_queue_s  can_rx_queue;
static can_queue_elem_s  can_rx_qbuf[CAN_ID_0_RX_QUEUE_DEPTH]; /* 隊列 */
#else
#endif

static can_s g_can[CAN_ID_TOTAL] = {0};


/**
	* @brief  通过波特率的值获取波特率参数表索引值
	* @param  BaudRate CAN总线波特率，单位为bps
	* @retval 波特率参数表索引值
	*/
uint32_t CAN_GetBaudRateNum(uint32_t BaudRate) {
    switch (BaudRate) {
        case 1000000 :
            return 0;
        case 900000 :
            return 1;
        case 800000 :
            return 2;
        case 666000 :
            return 3;
        case 600000 :
            return 4;
        case 500000 :
            return 5;
        case 400000 :
            return 6;
        case 300000 :
            return 7;
        case 250000 :
            return 8;
        case 225000:
            return 9;
        case 200000 :
            return 10;
        case 160000:
            return 11;
        case 150000 :
            return 12;
        case 144000:
            return 13;
        case 125000 :
            return 14;
        case 120000:
            return 15;
        case 100000 :
            return 16;
        case 90000 :
            return 17;
        case 80000 :
            return 18;
        case 75000:
            return 19;
        case 60000 :
            return 20;
        case 50000 :
            return 21;
        case 40000 :
            return 22;
        case 30000 :
            return 23;
        case 20000 :
            return 24;
        default:
            return 0;
    }
}

/**
* @brief  CAN中断服务函数
	* @param  None
	* @retval None
	*/
void CAN_IRQHandler(void) {
    u8 l_buf_index = 0xffu;
    u8 buffIdx = 0u;
    static uint8_t last_ESTAT = 5;
    CanRxMsg tempCanRxMsg = {0u};
    can_queue_elem_s l_rx;
    
    //if ((FL_ENABLE == FL_CAN_IsEnabledIT_RXOK(CAN))
    ////		&& (FL_SET == FL_CAN_IsActiveFlag_RXOK(CAN)))
    if ((FL_ENABLE == FL_CAN_IsEnabledIT_RXNotEmpty(CAN))
        && (FL_SET == FL_CAN_IsActiveFlag_RXNotEmpty(CAN))) {
        uint32_t data1, data2;
        
        tempCanRxMsg.ID = FL_CAN_ReadRXMessageID(CAN);      //接收的CANID
        tempCanRxMsg.DLC = FL_CAN_ReadRXMessageLength(CAN); //数据段长度
        
        data1 = FL_CAN_ReadRXMessageWord1(CAN);
        data2 = FL_CAN_ReadRXMessageWord2(CAN);
        
        tempCanRxMsg.Data[0] = (uint8_t)data1 & 0xff;
        tempCanRxMsg.Data[1] = (uint8_t)(data1 >> 8) & 0xff;
        tempCanRxMsg.Data[2] = (uint8_t)(data1 >> 16) & 0xff;
        tempCanRxMsg.Data[3] = (uint8_t)(data1 >> 24) & 0xff;
        tempCanRxMsg.Data[4] = (uint8_t)data2 & 0xff;
        tempCanRxMsg.Data[5] = (uint8_t)(data2 >> 8) & 0xff;
        tempCanRxMsg.Data[6] = (uint8_t)(data2 >> 16) & 0xff;
        tempCanRxMsg.Data[7] = (uint8_t)(data2 >> 24) & 0xff;
        
        #ifdef CAN_LOGIC_DEBUG
        g_can[CAN_ID_0].test[0]++;
        #endif
        
        /* 拆分ID */
        if (tempCanRxMsg.ID & (1 << 12)) {
            #ifdef CAN_LOGIC_DEBUG
            g_can[CAN_ID_0].test[1]++;
            #endif
            
            /* 扩展ID */
            tempCanRxMsg.IDE = CAN_ID_EXT;
            tempCanRxMsg.ID = ((tempCanRxMsg.ID & 0x7FF) << 18) + ((tempCanRxMsg.ID >> 13) & 0x3FFFF);
            
            if (tempCanRxMsg.ID & 0x80000000) {
                /* 扩展远程帧 */
                tempCanRxMsg.RTR = CAN_RTR_REMOTE;
            } else {
                /* 扩展数据帧 */
                tempCanRxMsg.RTR = CAN_RTR_Data;
            }
        } else {
            #ifdef CAN_LOGIC_DEBUG
            g_can[CAN_ID_0].test[2]++;
            #endif
            
            /* 标准ID */
            tempCanRxMsg.IDE = CAN_ID_STD;
            if (tempCanRxMsg.ID & (1 << 11)) {
                /* 标准远程帧 */
                tempCanRxMsg.RTR = CAN_RTR_REMOTE;
            } else {
                /* 标准数据帧 */
                tempCanRxMsg.RTR = CAN_RTR_Data;
            }
            tempCanRxMsg.ID = tempCanRxMsg.ID & 0x7FF;
        }
        /* 默认只有一个FIFO */
        tempCanRxMsg.FMI = 0;
        FL_CAN_ClearFlag_RXNotEmpty(CAN);
        ///FL_CAN_ClearFlag_RXOK(CAN);
        
        //加入队列
        /* -----------------------------------------------------------------------A12
        tempCanRxMsg ：ID,IDE,RTR,DLC,Data,FMI 都能确定
        ----------------------------------------------------------------------- */
        common_memory_copys((u8 *)&l_rx.msg, (u8 *)&tempCanRxMsg, sizeof(CAN_MsgInfoType));
        common_memory_copys((u8 *)&l_rx.buf[0], (u8 *)&tempCanRxMsg.Data[0], CAN_FRAME_MAX_DLC);
        //转成协议栈
        if (l_rx.msg.IDE == CAN_ID_EXT) {
            l_rx.msg.IDE = EXTENDED_CAN;
        }
        
        l_rx.phy_id = INST_CANCOM0;
        l_rx.mailbox_id = 0u;
        l_rx.msg.DLC =  tempCanRxMsg.DLC;//can_sw_dlc_to_len(l_rx.msg.DLC);
        #ifdef CAN_DRIVE_QUEUE_ENABLE
        (void)queue_add_element(&can_rx_queue, (const sequential_queue_elem *)&l_rx); //NOLINT
        #endif
        l_rx.msg_id = l_rx.msg.ID; //msg_id就是收到的CANID
        l_rx.len = l_rx.msg.DLC;   //len就是接收到的数据段长度
        l_rx.type = l_rx.msg.IDE;  //ID类型
        //#ifdef CAN_AUTOSAR_EN
        can_id_phy_rx_irq_task(CAN_ID_0, &l_rx);
        //#endif
        #ifdef CAN_LS_NM
        if (CanNm_is_busoff() == TRUE) {
            #ifdef CAN_LS_NM
            //Can_SetTxStatus_finish(CAN_ID_0);
            CanNm_TxConfirmation_com(0, CAN_ID_0);
            #endif
        }
        #endif
    }
    if ((FL_ENABLE == FL_CAN_IsEnabledIT_TXOK(CAN)) && (FL_SET == FL_CAN_IsActiveFlag_TXOK(CAN))) {
        #ifdef CAN_LOGIC_DEBUG
        g_can[CAN_ID_0].test[3]++;
        #endif
        
        #ifdef CAN_AUTOSAR_EN
        l_buf_index = g_can[CAN_ID_0].mailbox_id;
        buffIdx = 0u;
        can_id_phy_tx_irq_task(CAN_ID_0, l_buf_index, buffIdx);
        #else
        #ifdef CAN_LS_NM
        Can_SetTxStatus_finish(CAN_ID_0);
        CanNm_TxConfirmation_com(0, CAN_ID_0);
        #endif
        #endif
        FL_CAN_ClearFlag_TXOK(CAN);
    }
    #ifdef CAN_ENABLE_HIGH_TX
    if ((FL_ENABLE == FL_CAN_IsEnabledIT_TXHighPriorBuffFull(CAN)) && (FL_SET == FL_CAN_IsActiveFlag_TXOK(CAN))) {
        #ifdef CAN_LOGIC_DEBUG
        g_can[CAN_ID_0].test[4]++;
        #endif
        
        #ifdef CAN_AUTOSAR_EN
        l_buf_index = g_can[CAN_ID_0].mailbox_id;
        buffIdx = 0u;
        can_id_phy_tx_irq_task(CAN_ID_0, l_buf_index, buffIdx);
        #else
        #ifdef CAN_LS_NM
        Can_SetTxStatus_finish(CAN_ID_0);
        CanNm_TxConfirmation_com(0, CAN_ID_0);
        #endif
        #endif
        FL_CAN_ClearFlag_TXOK(CAN);
        FL_CAN_ClearFlag_TXHighPriorBuffFull(CAN);
    } else {
        if ((FL_ENABLE == FL_CAN_IsEnabledIT_TXHighPriorBuffFull(CAN)) && (FL_CAN_IsActiveFlag_TXHighPriorBuffFull(CAN))) {
            FL_CAN_ClearFlag_TXHighPriorBuffFull(CAN);
            #ifdef CAN_LOGIC_DEBUG
            g_can[CAN_ID_0].test[5]++;
            #endif
        }
    }
    #endif
    
    if (CAN->ISR & CAN_ISR_ERROR_Msk) {
        #if 0
        /* 错误中断 */
        /* 主动错误转换为被动错误时刻检测 */
        if (last_ESTAT == 1) {
            if (((CAN->SR & (3 << 7)) >> 7) == 3) {
                ;
            }
        }
        last_ESTAT = ((CAN->SR & (3 << 7)) >> 7);
        #endif
        #ifdef CAN_LS_NM
        //CanNm_PhysErrorInd(CAN_ID_0,NM_DLL_BUS_OFF_PRE);
        #endif
        #ifdef CAN_LOGIC_DEBUG
        g_can[CAN_ID_0].test[6]++;
        #endif
        CAN->ICR = CAN_ICR_CERROR_Msk;
    }
    //if (CAN->ISR & CAN_ISR_BSOFF_Msk)
    if ((FL_CAN_IsActiveFlag_BusOff(CAN) == TRUE) && (FL_CAN_IsEnabledIT_BusOff(CAN) == TRUE)) {
        /* busoff中断 */
        /* CEN禁止后再使能即可清除can busoff的状态，在CAN模块稳定后可以立即发送数据 */
        //CAN->CR = 0;
        //CAN->CR = 1;
        FL_CAN_SetSoftwareReset(CAN, FL_CAN_SOFTWARE_RESET);
        //CAN->ICR = CAN_ICR_CBSOFF_Msk;
        #if 1
        #ifdef CAN_LS_NM
        CanNm_PhysErrorInd(CAN_ID_0, NM_DLL_BUS_OFF);
        #endif
        #endif
        #ifdef CAN_LOGIC_DEBUG
        g_can[CAN_ID_0].test[7]++;
        #endif
        FL_CAN_ClearFlag_BusOff(CAN);
    }
    #ifdef CAN_LOGIC_DEBUG
    g_can[CAN_ID_0].test[8]++;
    #endif
}


/**
	* @brief  CAN接收中断配置
	* @param  None
	* @retval None
	*/
void CAN_NVIC_Configuration(void) {
    FL_CAN_ClearFlag_RXOK(CAN); //Clear Receive OK
    //  FL_CAN_EnableIT_RXOK(CAN);
    
    /* NVIC中断配置 */
    NVIC_DisableIRQ(CAN_IRQn);
    NVIC_SetPriority(CAN_IRQn, NVIC_PRIORITY_CAN0_RX0);
    NVIC_EnableIRQ(CAN_IRQn);
}

/**
	* @brief  配置CAN接收滤波器
	* @param  FilterNumber 过滤器号
	* @param  ReceiveMsgId 接收数据的ID
		* @param  ReceiveMsgIdType 接收数据的ID类型，0-标准帧，1-扩展帧
	* @retval None
	*/
void CAN_ConfigFilter(uint8_t FilterNumber, uint32_t ReceiveMsgId, uint8_t ReceiveMsgIdType) {
    FL_CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};
    
    if (FilterNumber > FL_CAN_FILTER4) {
        return;
    }
    if (ReceiveMsgIdType == CAN_Id_Standard) {
        /* 标准帧 */
        CAN_FilterInitStructure.filterIdStandard = ReceiveMsgId;   /* 标准ID */
        CAN_FilterInitStructure.filterIdSRR = 0;
        CAN_FilterInitStructure.filterIdIDE = 0;
        CAN_FilterInitStructure.filterIdRTR = 0;
        
        CAN_FilterInitStructure.filterMaskIdHigh = 0X7FF;
        CAN_FilterInitStructure.filterMaskIdSRR = 0x01;
        CAN_FilterInitStructure.filterMaskIdIDE = 0x01; /* 滤波器掩码，1，该位参与滤波器比较，0，不参与 */
        CAN_FilterInitStructure.filterMaskIdRTR = 0x01;
        CAN_FilterInitStructure.filterEn = FL_ENABLE;
        FL_CAN_FilterInit(CAN, &CAN_FilterInitStructure, FilterNumber);
    } else {
        CAN_FilterInitStructure.filterIdExtend = ReceiveMsgId; /* 扩展ID */
        CAN_FilterInitStructure.filterIdSRR = 0X01;
        CAN_FilterInitStructure.filterIdIDE = 0X01;
        CAN_FilterInitStructure.filterIdRTR = 0X00;
        
        CAN_FilterInitStructure.filterMaskIdHigh = 0X7FF;
        CAN_FilterInitStructure.filterMaskIdLow = 0X3FFFF;
        CAN_FilterInitStructure.filterMaskIdSRR = 0X01;
        CAN_FilterInitStructure.filterMaskIdIDE = 0X01;   /* 滤波器掩码，1，该位参与滤波器比较，0，不参与 */
        CAN_FilterInitStructure.filterMaskIdRTR = 0x01;
        CAN_FilterInitStructure.filterEn = FL_ENABLE;
        FL_CAN_FilterInit(CAN, &CAN_FilterInitStructure, FilterNumber);
    }
}


/**
	* @brief  初始化---當使用autosar時，不調用該初始化，一般用於測試時。
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void can_init(void) {
    #ifdef CAN_ID_0_EN
    can0_init();
    #endif
    #ifdef CAN_ID_1_EN
    can1_init();
    #endif
}


/**
	* @brief  反
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void can_de_init(void) {
    #ifdef CAN_ID_0_EN
    can0_de_init();
    #endif
    #ifdef CAN_ID_1_EN
    can1_de_init();
    #endif
}

/**
	* @brief  任务
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void can_task(void) {
    can_tx_task();
}

/**
	* @brief  初始化
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
/*PCLINT_ERROS*/ /*lint -efunc(529,can_drive_init)  */  /* 局部變量未被引用,*/
void can_phy_drive_init(can_id_e phy_id) {
    switch (phy_id) {
        case CAN_ID_0:
            #ifdef CAN_ID_0_EN
            can0_init();
            #endif
            break;
        case CAN_ID_1:
            #ifdef CAN_ID_1_EN
            can1_init();
            #endif
            break;
            
        default:
        
            break;
    }
    
}



/**
	* @brief  初始化CAN
	* @param  BaudRate CAN总线波特率
	* @retval None
	*/
void CAN_Configuration(uint32_t BaudRate) {
    FL_CAN_InitTypeDef       CAN_InitStructure = {0};
    /* CAN register init */
    //CAN_NVIC_Configuration();
    //CAN_GPIO_Configuration();
    /* CAN cell init */
    CAN_InitStructure.TS1 = FL_CAN_TS1_12Tq;//CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRate)].BS1;
    CAN_InitStructure.TS2 = FL_CAN_TS2_3Tq;//CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRate)].BS2;        /* 位时序设置 */
    CAN_InitStructure.SJW = FL_CAN_SJW_2Tq;//Synchronization Jump Width 没懂
    CAN_InitStructure.BRP = 0;//CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRate)].PreScale;   /* 波特率预分频 */
    CAN_InitStructure.mode = FL_CAN_MODE_NORMAL;                                          /* 工作模式设置 */
    CAN_InitStructure.clockSource = FL_CMU_CAN_CLK_SOURCE_XTHF;                           /* 时钟源设置 */
    FL_CAN_Init(CAN, &CAN_InitStructure);
    
}



/**
	* @brief
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others  终于找到busoff不能恢复的原因；里面的内容，都先屏蔽--后来更新供应商的
	*/
void can0_de_init(void) {
    #if 0
    CanTrcv_Disable();
    FL_CAN_Disable(CAN);
    FL_CMU_DisableGroup1BusClock(FL_CMU_GROUP1_BUSCLK_PAD);
    FL_CMU_DisableGroup4BusClock(FL_CMU_GROUP4_BUSCLK_ATIM);
    FL_CMU_DisableGroup3OperationClock(FL_CMU_GROUP3_OPCLK_ATIM);
    FL_CMU_DisableGroup3BusClock(FL_CMU_GROUP3_BUSCLK_CAN);
    FL_CMU_DisableGroup3OperationClock(FL_CMU_GROUP3_OPCLK_CAN);
    #else
    /* NVIC DeInit */
    NVIC_DisableIRQ(CAN_IRQn);
    NVIC_ClearPendingIRQ(CAN_IRQn);
    
    /* Enable Peripheral Reset */
    FL_RMU_EnablePeripheralReset(RMU);
    FL_RMU_EnableResetAPBPeripheral(RMU, FL_RMU_RSTAPB_CAN);
    FL_RMU_DisableResetAPBPeripheral(RMU, FL_RMU_RSTAPB_CAN);
    FL_RMU_EnableResetAPBPeripheral(RMU, FL_RMU_RSTAPB_CAN);
    FL_RMU_DisableResetAPBPeripheral(RMU, FL_RMU_RSTAPB_CAN);
    FL_RMU_DisablePeripheralReset(RMU);
    
    /* Close CANBUS Clock */
    FL_CMU_DisableGroup3BusClock(FL_CMU_GROUP3_BUSCLK_CAN);
    FL_CMU_DisableGroup3OperationClock(FL_CMU_GROUP3_OPCLK_CAN);
    
    /* GPIO DeInit */
    //FL_GPIO_DeInit(GPIOA,FL_GPIO_PIN_6 | FL_GPIO_PIN_7);
    //FL_GPIO_DeInit(GPIOC,FL_GPIO_PIN_6);
    
    #endif
}

/**
	* @brief  任务
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void can0_init(void) {
    #ifdef CAN_DRIVE_QUEUE_ENABLE
    queue_init(&can_tx_queue, (sequential_queue_elem *)&can_tx_qbuf, CAN_ID_0_TX_QUEUE_DEPTH, sizeof(can_queue_elem_s));
    queue_init(&can_rx_queue, (sequential_queue_elem *)&can_rx_qbuf, CAN_ID_0_RX_QUEUE_DEPTH, sizeof(can_queue_elem_s));
    #endif
    
    //CAN->CR = 0u;  //为了重新初始化，相当于反初始化
    
    CAN_Configuration(g_can0_baudrate);
    
    
    /* 设置CAN接收滤波器 */
    //CAN_ConfigFilter(0, MSG_RECEIVE_ID, MSG_ID_TYPE);
    #if 1
    FL_CAN_ClearFlag_RXNotEmpty(CAN);
    FL_CAN_EnableIT_RXNotEmpty(CAN);    /* 接收中断使能 */
    #endif
    /* 使能接收中断 */
    //FL_CAN_ClearFlag_RXOK(CAN);
    //FL_CAN_EnableIT_RXOK(CAN);   //接收中断使能
    
    FL_CAN_ClearFlag_Error(CAN);
    FL_CAN_EnableIT_Error(CAN);     //错误中断使能
    
    FL_CAN_ClearFlag_BusOff(CAN);   //BusOff中断使能
    FL_CAN_EnableIT_BusOff(CAN);
    
    //FL_CAN_ClearFlag_TXOK(CAN);
    //FL_CAN_EnableIT_TXOK(CAN);      //发送完成中断使能
    
    g_can[CAN_ID_0].state_tx = CAN_TX_STATE_IDLE;
    
    CAN_NVIC_Configuration();  //调用顺序，会引起busoff不能恢复
    
}



/**
	* @brief  通过TX FIFO发送数据
	* @param  CANx
	* @retval  None
	*/
void FL_CAN_FIFO_Write(u8 mailbox_id, uint32_t id, uint32_t len, uint32_t data1, uint32_t data2) {
    #if 0
    uint32_t i = 5;
    
    while ((FL_CAN_IsActiveFlag_TXBuffFull(CAN) != FL_RESET)&i) {
        i--;
        FL_DelayMs(1);
    }
    #endif
    #ifdef CAN_AUTOSAR_EN
    #else
    if (FL_CAN_IsActiveFlag_TXBuffFull(CAN) == FL_RESET)
    #endif
    {
        g_can[CAN_ID_0].mailbox_id = mailbox_id;
        #ifdef CAN_ENABLE_HIGH_TX
        if (mailbox_id == 2u) {
            FL_CAN_ClearFlag_TXHighPriorBuffFull(CAN);
            FL_CAN_WriteHighPriorTXMessageID(CAN, id);
            FL_CAN_WriteHighPriorMessageLength(CAN, len);
            FL_CAN_WriteHighPriorMessageWord1(CAN, data1);
            FL_CAN_WriteHighPriorMessageWord2(CAN, data2);
            FL_CAN_EnableIT_TXHighPriorBuffFull(CAN);
        } else
        #endif
        {
            FL_CAN_ClearFlag_TXBuffFull(CAN);
            FL_CAN_WriteTXMessageID(CAN, id);
            FL_CAN_WriteTXMessageLength(CAN, len);
            FL_CAN_WriteTXMessageWord1(CAN, data1);
            FL_CAN_WriteTXMessageWord2(CAN, data2);
            FL_CAN_EnableIT_TXOK(CAN);
        }
    }
}


/**
	* @brief  发送一帧CAN数据
	* @param  CANx CAN通道号
	* @param  TxMessage CAN消息指针
	* @retval None
	*/
uint8_t CAN_WriteData(u8 mailbox_id, CanTxMsg *TxMessage) {
    uint32_t id = 0;
    uint32_t len = 0;
    uint32_t data1 = 0;
    uint32_t data2 = 0;
    
    if (TxMessage->IDE) {
        /* 扩展 */
        if (TxMessage->RTR) {
            /* 远程帧 */
            id = ((TxMessage->ID & 0x3ffff) << 13) | ((uint32_t)1 << 12) | ((uint32_t)1 << 31) | ((TxMessage->ID & 0x1ffc0000) >> 18);
        } else {
            /* 数据帧 */
            id = ((TxMessage->ID & 0x3ffff) << 13) | ((uint32_t)1 << 12) | ((uint32_t)1 << 11) | ((TxMessage->ID & 0x1ffc0000) >> 18);
        }
    } else {
        /* 标准 */
        if (TxMessage->RTR) {
            /* 远程帧 */
            id = (TxMessage->ID & 0x7ff) | (1 << 11);
        } else {
            /* 数据帧 */
            id = TxMessage->ID & 0x7ff;
        }
    }
    len = TxMessage->DLC;
    data1 = (((uint32_t)TxMessage->Data[3] << 24) |
            ((uint32_t)TxMessage->Data[2] << 16) |
            ((uint32_t)TxMessage->Data[1] << 8) |
            ((uint32_t)TxMessage->Data[0]));
    data2 = (((uint32_t)TxMessage->Data[7] << 24) |
            ((uint32_t)TxMessage->Data[6] << 16) |
            ((uint32_t)TxMessage->Data[5] << 8) |
            ((uint32_t)TxMessage->Data[4]));
            
    FL_CAN_FIFO_Write(mailbox_id, id, len, data1, data2);
    return 0;
}

/**
	* @brief  发送
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
static void can_tx_task(void) {
    //考虙這里加隊列發送；
    #ifdef CAN_DRIVE_QUEUE_ENABLE
    //u8 l_buf_index;
    can_queue_elem_s l_rx;
    can_queue_elem_s l_tx;
    //CAN_Type *p_can;
    //u8 i;
    /*PCLINT_ERROS*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
    if (queue_get_head(&can_tx_queue, (sequential_queue_elem *)&l_tx) == QUEUE_OK) {
        /*PCLINT_ERROS*/ /*lint --e(926) */ /* */
        /*PCLINT_ERROS*/ /*lint --e(641) --e(960) --e(923) */
        queue_del_element(&can_tx_queue, (sequential_queue_elem *)&l_tx);
        
        if (l_tx.phy_id == INST_CANCOM0) {
            //p_can = CAN0;
            //common_memory_copys((u8*)&g_can[INST_CANCOM0].tx,(u8*)&l_tx.msg,sizeof(CAN_MsgInfoType));  //
            //common_memory_copys((u8*)&g_can[INST_CANCOM0].tx_buf[0],(u8*)&l_tx.buf[0],CAN_FRAME_MAX_DLC);
            //g_can[INST_CANCOM0].tx.DATA = &g_can[INST_CANCOM0].tx_buf[0];
            //把dlc转换成寄存器能识别的长度；
            //g_can[INST_CANCOM0].tx.DLC = can_sw_len_to_dlc(g_can[INST_CANCOM0].tx.DLC);
            CAN_WriteData(l_tx.mailbox_id, &l_tx.msg); //发送数据
        }
    }
    
    /*PCLINT_ERROS*/ /*lint --e(928) */ /* 928 11.4（建議）： 不應在某類型對像指針和其他不同類型對像指針之間進行強制轉換。*/
    if (queue_get_head(&can_rx_queue, (sequential_queue_elem *)&l_rx) == QUEUE_OK) {
        /*PCLINT_ERROS*/ /*lint --e(926) */ /* */
        /*PCLINT_ERROS*/ /*lint --e(641) --e(960) --e(923) */
        queue_del_element(&can_rx_queue, (sequential_queue_elem *)&l_rx);
        //
        #ifdef CAN_LOGIC_DEBUG
        g_can[CAN_ID_0].test[9]++;
        #endif
    }
    #endif
}




/**
	* @brief  发送  任務
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void can_op_tx_task(void) {
    #if 1
    static u8 g_cc = 0;
    #ifdef CAN_DRIVE_QUEUE_ENABLE
    can_queue_elem_s l_tx;
    u8 i;
    g_can[CAN_ID_0].count++;
    l_tx.phy_id = INST_CANCOM0;
    //l_tx.mailbox_id = g_can_id_0_cfg_tx_table[0].mb_fifo_id;
    l_tx.msg.ID = 0x228;  //
    l_tx.msg.IDE = CAN_MSG_ID_STD;  //
    l_tx.msg.DLC = CAN_FRAME_DLC;
    #ifdef CAN_ID_0_FD_EN
    l_tx.msg.FDF = CAN_MSG_TYPE_FD;
    l_tx.msg.BRS = CAN_MSG_BRS_FAST;
    #else
    l_tx.msg.FDF = CAN_MSG_TYPE_NORMAL;
    l_tx.msg.BRS = CAN_MSG_BRS_NORMAL_LOW;
    #endif
    l_tx.msg.RTR = CAN_MSG_RTR_DATA;
    //l_tx.msg_id = g_can_id_0_cfg_tx_table[0].can_id;
    g_cc++;
    for (i = 0; i < CAN_FRAME_DLC; i++) {
        l_tx.buf[i] = 0x11u + i + g_cc;
    }
    common_memory_copys((u8 *)&l_tx.msg.Data[0], (u8 *)&l_tx.buf[0], CAN_FRAME_MAX_DLC); //
    //if(g_can[CAN_ID_0].count & 0x01)
    {
        (void)queue_add_element(&can_tx_queue, (const sequential_queue_elem *)&l_tx); //NOLINT
    }
    
    l_tx.phy_id = INST_CANCOM0;
    //l_tx.mailbox_id = g_can_id_0_cfg_tx_table[0].mb_fifo_id;
    l_tx.msg.ID = 0x7654229;  //
    l_tx.msg.IDE = CAN_MSG_ID_EXT;  //
    l_tx.msg.DLC = CAN_FRAME_DLC;
    #ifdef CAN_ID_0_FD_EN
    l_tx.msg.FDF = CAN_MSG_TYPE_FD;
    l_tx.msg.BRS = CAN_MSG_BRS_FAST;
    #else
    l_tx.msg.FDF = CAN_MSG_TYPE_NORMAL;
    l_tx.msg.BRS = CAN_MSG_BRS_NORMAL_LOW;
    #endif
    l_tx.msg.RTR = CAN_MSG_RTR_DATA;
    //l_tx.msg_id = g_can_id_0_cfg_tx_table[0].can_id;
    for (i = 0; i < CAN_FRAME_DLC; i++) {
        l_tx.buf[i] = 0x22u + i + g_cc;
    }
    common_memory_copys((u8 *)&l_tx.msg.Data[0], (u8 *)&l_tx.buf[0], CAN_FRAME_MAX_DLC); //
    //if(g_can[CAN_ID_0].count & 0x01)
    {
        (void)queue_add_element(&can_tx_queue, (const sequential_queue_elem *)&l_tx); //NOLINT
    }
    #else
    
    #endif
    #endif
}






/**
	* @brief  发送接口
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void can_tx_start(const can_queue_elem_s *p_tx) {
    u8 i;
    can_queue_elem_s l_tx;
    //CAN_Type* p_can;
    /*
    l_tx.phy_id = INST_CANCOM0;
    //l_tx.mailbox_id = g_can_id_0_cfg_tx_table[0].mb_fifo_id;
    l_tx.msg.ID = 0x228;  //
    l_tx.msg.IDE = CAN_MSG_ID_EXT;  //
    l_tx.msg.DLC = CAN_FRAME_DLC;
    #ifdef CAN_ID_0_FD_EN
    l_tx.msg.FDF = CAN_MSG_TYPE_FD;
    l_tx.msg.BRS = CAN_MSG_BRS_FAST;
    #else
    l_tx.msg.FDF = CAN_MSG_TYPE_NORMAL;
    l_tx.msg.BRS = CAN_MSG_BRS_NORMAL_LOW;
    #endif
    l_tx.msg.RTR = CAN_MSG_RTR_DATA;
    //l_tx.msg_id = g_can_id_0_cfg_tx_table[0].can_id;
    l_tx.msg.DATA = &l_tx.buf[0];
    g_cc++;
    for(i=0;i<CAN_FRAME_DLC;i++)
    {
    	l_tx.buf[i] = 0x11u+i+g_cc;
    }
     */
    
    //p_can = CAN0;
    if (p_tx->type == CAN_MSG_ID_STD) {
        l_tx.msg.IDE = CAN_ID_STD;  //
    } else {
        l_tx.msg.IDE = CAN_ID_EXT;  //
    }
    //p_tx->phy_id
    //mailbox_id
    l_tx.msg.ID = p_tx->msg_id;
    l_tx.msg.DLC = p_tx->len;
    #ifdef CAN_ID_0_FD_EN
    #if 0
    if (p_tx->fd_enable == 0u) {
        l_tx.msg.FDF = CAN_MSG_TYPE_NORMAL;  //
        l_tx.msg.BRS = CAN_MSG_BRS_NORMAL_LOW;
    } else {
        l_tx.msg.FDF = CAN_MSG_TYPE_FD;  //
        l_tx.msg.BRS = CAN_MSG_BRS_FAST;
    }
    #else
    l_tx.msg.FDF = p_tx->fd_enable;  //
    l_tx.msg.BRS = p_tx->enable_brs;
    #endif
    #else
    l_tx.msg.FDF = CAN_MSG_TYPE_NORMAL;  //
    l_tx.msg.BRS = CAN_MSG_BRS_NORMAL_LOW;
    #endif
    l_tx.msg.RTR = CAN_MSG_RTR_DATA;
    l_tx.msg.ESI = 1u;
    l_tx.msg.RTS = 1u;
    l_tx.mailbox_id = p_tx->mailbox_id;
    #if 1
    //l_tx.msg.DATA = &l_tx.buf[0];
    for (i = 0; i < CAN_FRAME_DLC; i++) {
        l_tx.buf[i] = p_tx->buf[i];
        l_tx.msg.Data[i] = p_tx->buf[i];
    }
    #endif
    #if 0
    common_memory_copys((u8 *)&g_can[INST_CANCOM0].tx, (u8 *)&l_tx.msg, sizeof(CAN_MsgInfoType)); //
    common_memory_copys((u8 *)&g_can[INST_CANCOM0].tx_buf[0], (u8 *)&p_tx->buf[0], CAN_FRAME_MAX_DLC);
    g_can[INST_CANCOM0].tx.DATA = &g_can[INST_CANCOM0].tx_buf[0];
    //把dlc转换成寄存器能识别的长度；
    g_can[INST_CANCOM0].tx.DLC = can_sw_len_to_dlc(g_can[INST_CANCOM0].tx.DLC);
    #if 1
    if (l_tx.mailbox_id == 2) {
        CAN_TransmitMessage(p_can, &g_can[INST_CANCOM0].tx, CAN_TRANSMIT_PRIMARY);//发送数据
    } else {
        CAN_TransmitMessage(p_can, &g_can[INST_CANCOM0].tx, CAN_TRANSMIT_SECONDARY);//发送数据
    }
    #else
    CAN_TransmitMessage(p_can, &g_can[INST_CANCOM0].tx, CAN_TRANSMIT_SECONDARY);//发送数据
    #endif
    #else
    CAN_WriteData(l_tx.mailbox_id, &l_tx.msg); //发送数据
    #endif
}


