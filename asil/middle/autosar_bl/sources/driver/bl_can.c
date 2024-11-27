/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the can module.
 *
 *  \file       bl_can.c
 *  \ingroup    driver_module
 *  \author
 *
 *  \version    1.1.0
 *  \date       14/05/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | -------------
 *      01.00.00 | 14/05/2014 | ning.chen        | N/A | BootDrv010001
 *      01.01.00 | 21/08/2014 | ning.chen        | N/A | BootDrv010002
 *
 *****************************************************************************/
#include "bl_common.h"
#include "bl_rte.h"
#include "bl_system.h"
#include "bl_can.h"
#include "queue_entity.h"
#include "bl_system_cfg.h"
#include "Cpu.h"
#include "bl_can_if_cfg.h"
#include "Ls_Nm.h"
#include "can.h"


/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 277,303,312,506,602,781,1251,1252,1257,1258,1532,2100,2103,2465 EOF*/
/*PRQA S 3112,3120,3204,3212,3218,3227,3441,3442,3453,4130 EOF*/
/*
 * 277:An integer constant expression with negative value is being converted
 * to an unsigned type.
 *
 * MISRA-C:2004 11.3(303):Cast a pointer to an integral type.
 * To address a register must cast an integral type to a pointer.
 *
 * 312:Dangerous pointer cast results in loss of volatile qualification.
 *
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter pdu is not a null pointer.
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
 * 1252:suffixed integer constant implicitly converted to different integer.
 *
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1258:Suffixed integer constant explicitly cast to another type.
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
 * 2465:This 'for' loop will only be executed once.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3120:Hard-coded magic integer constant.
 * This is a only configuration.
 *
 * 3204:The variable is only set once and so it could be declared with the
 * 'const' qualifier.
 *
 * 3212:This explicit cast is redundant and could be removed.
 *
 * MISRA-C:2004 8.7(3218):File scope static identifier is only accessed in one
 * function.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * 3441:Function call argument contains side effects.
 *
 * 3442:Operator other than & or = applied to a volatile object.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 *
 * MISRA-C:2004 12.7(4130):Bitwise operations on signed data will give
 * implementation defined results.
 *
 */

static void can1_phy_drive_init(void);

#if 1
#define   CAN_DRIVE_SR_ALLOC()   // CPU_SR_ALLOC()
#define   CAN_DRIVE_CRITICAL_ENTER()   // CPU_CRITICAL_ENTER()
#define   CAN_DRIVE_CRITICAL_EXIT()   // CPU_CRITICAL_EXIT()
#else
#define CAN_DRIVE_SR_ALLOC()  //CPU_SR_ALLOC()
#define CAN_DRIVE_CRITICAL_ENTER() INT_SYS_DisableIRQGlobal()
#define CAN_DRIVE_CRITICAL_EXIT()  INT_SYS_EnableIRQGlobal()
#endif

typedef enum {
    CAN_T_START = 0,
    CAN_T_STOP = 1,
    CAN_T_SLEEP = 2,
    CAN_T_WAKEUP = 3,
    CAN_T_NONE = 4
} Can_StateTransitionType;
typedef enum {
    CAN_DRIVE_STATE_INIT = 0u,  //就是STOP
    CAN_DRIVE_STATE_NORMAL = 1u,
    CAN_DRIVE_STATE_BUSOFF = 2u,
    CAN_DRIVE_STATE_SLEEP = 3u,
} can_drive_state_e;

typedef struct $ {
    u8 buf[12];
    u8 tx[12];
    u8 rx[12];
} bl_can_test_s;
static bl_can_test_s g_bl_can_test[2] = {0};

typedef struct {
    u8 state;
    u16 count;
} can_driver_busoff_s;
typedef struct {
    u8 state;
    u16 count;
} can_driver_wakeup_s;
typedef struct {
    u8 state;
    can_driver_busoff_s busoff;
    can_driver_wakeup_s wakeup;
    can_queue_elem_s tx;
    can_queue_elem_s rx;
    #ifdef CAN_LOGIC_DEBUG
    u8 buf[20];
    #endif
} can_driver_s;
static can_driver_s g_can_driver[CAN_ID_TOTAL] = {0u};
#ifdef CAN_LOGIC_DEBUG
static u8 g_can_busoff_test[CAN_ID_TOTAL][9] = {0};
//static can_drive_debug_s g_can_drive_debug = {0u};
#endif

#ifdef CAN_RX_INTERRUPT_EN
#define CAN1_QUEUE_RX_DEPTH  8
typedef can_queue_elem_s can_rx_q_t;
sequential_queue_s can1_rx_queue;
bl_CanRxPdu_t can1_rx_qbuf[CAN1_QUEUE_RX_DEPTH];
#endif

/*****************************************************************************
 *  Verify The Configurations of Macro
 *****************************************************************************/
#if ((CAN_USED_CONTROLLER_NUMBER == 0)\
|| (CAN_USED_CONTROLLER_NUMBER > CAN_MAX_NUMBER_OF_CONTROLLER))
#error "the number of used controller is error."
#endif

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/


/*lint -emacro(778,CAN_MAKE_BUF_ID,CAN_MAKE_CAN_ID)*/
/*lint -emacro(572,CAN_MAKE_BUF_ID,CAN_MAKE_CAN_ID)*/
#if (CANIF_FUN_EXTERNED_ID == BL_FUN_ON)
#define CAN_MAKE_BUF_ID(canId)  (bl_u32_t)((((canId)<<3) & 0xFFE00000UL)|\
    (((canId) << 1) & 0x0007FFFEUL) |\
    0x00180000UL)
#define CAN_MAKE_CAN_ID(bufId)  (bl_u32_t)((((bufId) >> 1) & 0x0003FFFFUL) |\
    (((bufId) >> 3) & 0x1FFC0000UL))
#else
#define CAN_MAKE_BUF_ID(canId)  (bl_u32_t)((canId) << 21)
#define CAN_MAKE_CAN_ID(bufId)  (bl_u32_t)(((bufId) >> 21) & 0x000007FFUL)
#endif
/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
/** \brief Deinitialize a can controller.*/
static void _Can_DeInit(volatile bl_CanControllerBaseRegs_t *regs);
/** \brief Initialize a can controller.*/
static bl_Return_t _Can_Init(volatile bl_CanControllerBaseRegs_t *regs,
       const bl_CanControllerBaseRegs_t *initReg);

#if (CANIF_FUN_DYNAMIC_CANID == BL_FUN_ON)
/** \brief Set the can id to the Can IF module.*/
static void _Can_SetCanId(void);
#endif

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
STATIC __IO bl_CanControllerRegs_t *
const gs_CanControllerRegs[CAN_MAX_NUMBER_OF_CONTROLLER] = {
    (__IO bl_CanControllerRegs_t *)CAN_BASE,
    //(__IO bl_CanControllerRegs_t*)CAN0_BASE,
    //(__IO bl_CanControllerRegs_t*)CAN2_BASE,
};

/* -----------------------------------------------------------------------A12
CAN_USED_CONTROLLER_NUMBER =1
----------------------------------------------------------------------- */
STATIC bl_CanController_t gs_CanController[CAN_USED_CONTROLLER_NUMBER];

STATIC const bl_CanControllerBaseRegs_t
gs_CanControllerInitReg[CAN_USED_CONTROLLER_NUMBER] ;
#if 0
= {
    {
        CAN0_CTL0_INIT,
        (CAN_CTL1_CANE | CAN0_CLKSRC_CFG),
        CAN0BTR0_Def,
        CAN0BTR1_Def,
        0xffu,
        CAN0_RIER_INIT,
        0,
        CAN0_TIER_INIT,
        0,
        0,
        0,
        CANIDAC_Def,
        0,
        0,
        0,
        0,
        (CAN_RX_ID0 >> 24) & 0xFFu, /*lint !e778 !e572*/
        (CAN_RX_ID0 >> 16) & 0xFFu, /*lint !e778 !e572*/
        (CAN_RX_ID0 >> 8) & 0xFFu,  /*lint !e778 !e572*/
        (CAN_RX_ID0 >> 0) & 0xFFu,  /*lint !e778 !e572*/
        CAN_IDMR_INIT,
        CAN_IDMR_INIT,
        CAN_IDMR_INIT,
        CAN_IDMR_INIT,
        (CAN_RX_ID1 >> 24) & 0xFFu, /*lint !e778 !e572*/
        (CAN_RX_ID1 >> 16) & 0xFFu, /*lint !e778 !e572*/
        (CAN_RX_ID1 >> 8) & 0xFFu,  /*lint !e778 !e572*/
        (CAN_RX_ID1 >> 0) & 0xFFu,  /*lint !e778 !e572*/
        CAN_IDMR_INIT,
        CAN_IDMR_INIT,
        CAN_IDMR_INIT,
        CAN_IDMR_INIT,
    },
};
#endif

STATIC const bl_CanControllerCfg_t
gs_CanControllerCfg[CAN_USED_CONTROLLER_NUMBER] = {
    {
        CAN_USED_CONTROLLER_ID, &gs_CanControllerInitReg[0]
    },
    /**
    {
    	CAN_USED_CONTROLLER_ID_1,&gs_CanControllerInitReg[1]
    },**/
};

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Initialize can module.
 *
 *  \return If the can driver is initialized successfully return
 *          BL_ERR_OK. otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK     - can driver is initialized successfully.
 *  \retval BL_ERR_NOT_OK - can driver is not initialized successfully.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Can_Init(void) {
    bl_u8_t i;
    bl_u8_t phyId;
    bl_CanControllerRegs_t *regs;
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    for (i = 0; i < CAN_USED_CONTROLLER_NUMBER; i++) {
        if (gs_CanController[i].status != CAN_STATUS_IDLE) {
            phyId = gs_CanControllerCfg[i].phyId; //
            regs = (bl_CanControllerRegs_t *)gs_CanControllerRegs[phyId]; //CAN寄存器基地址
            
            ret = _Can_Init(&regs->base, gs_CanControllerCfg[i].initReg);
            if (BL_ERR_OK == ret) {
                gs_CanController[i].status = CAN_STATUS_IDLE;
                gs_CanController[i].regs = regs;
            } else {
                gs_CanController[i].status = CAN_STATUS_NOINIT;
                gs_CanController[i].regs = BL_NULL_PTR;
                break;
            }
        } else {
            ret = BL_ERR_OK;
        }
    }
    
    #if (CANIF_FUN_DYNAMIC_CANID == BL_FUN_ON)
    if (BL_ERR_OK == ret) {
        _Can_SetCanId();
        
    }
    #endif
    
    #ifdef CAN_RX_INTERRUPT_EN
    #endif
    /* -----------------------------------------------------------------------A12
    can1_rx_queue.base定义为can1_rx_qbuf
    ----------------------------------------------------------------------- */
    queue_init(&can1_rx_queue, (sequential_queue_elem *)&can1_rx_qbuf, CAN1_QUEUE_RX_DEPTH, sizeof(bl_CanRxPdu_t));
    return ret;
}

/**************************************************************************//**
 *
 *  \details Deinitialize can module.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Can_Deinit(void) {
    bl_u8_t i;
    
    for (i = 0; i < CAN_USED_CONTROLLER_NUMBER; i++) {
        if (gs_CanController[i].status != CAN_STATUS_NOINIT) {
            _Can_DeInit(&gs_CanController[i].regs->base);
            
            gs_CanController[i].status = CAN_STATUS_NOINIT;
            gs_CanController[i].regs = BL_NULL_PTR;
        }
    }
    
    return ;
}

#define CANx_TXMASK(hth)   ((u8)(0x01<<(hth)))

/**************************************************************************//**
 *
 *  \details Transmit can message.
 *
 *  \param[in/out] pdu  - the pointer of can Tx PDU.
 *
 *  \return If the can driver is transmited successfully return BL_ERR_OK.
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - can message is transmited successfully.
 *  \retval BL_ERR_NOT_OK   - the controller is busy or not initialized.
 *
 *  \since V2.0.0
 *struct _tag_CanTxPdu
{
	bl_ComIfHandle_t handle;
	bl_BufferSize_t dlc;
	bl_u32_t canId;
	const bl_Buffer_t *frame;
};
 *****************************************************************************/
bl_Return_t Can_Write(const bl_CanTxPdu_t *pdu) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_ComIfHandle_t handle = pdu->handle;
    volatile bl_CanControllerBaseRegs_t *regs;
    //volatile bl_CanFrameBuffer_t *txBuf;
    u8 phyhth;
    u8 l_tmp;
    u8 phyhthTXEMask;
    u8 hthmatchflag = 0;
    can_queue_elem_s l_frm_tx;
    u32 l_id_mask;
    u8 i;
    //const can_logic_cfg_txrx_s *p_tx_cfg;
    BL_DEBUG_ASSERT_PARAM(handle < CAN_USED_CONTROLLER_NUMBER);
    
    if (CAN_STATUS_IDLE == gs_CanController[handle].status) {
        #if 0
        regs = &gs_CanController[handle].regs->base;
        
        if ((regs->tflg & 0x01u) != 0u) {
            gs_CanController[handle].status = CAN_STATUS_TRANSMITTING;
            txBuf = &gs_CanController[handle].regs->txBuf;
            
            regs->tbsel = 0x01u;
            
            txBuf->id = CAN_MAKE_BUF_ID(pdu->canId);
            txBuf->dlc = (bl_u8_t)BL_GET_LOW_HALF(pdu->dlc);  //注意，CANFD要超过 15
            Bl_MemCpy((bl_Buffer_t *)txBuf->data,
                pdu->frame,
                (bl_Size_t)CAN_MAX_SIZE_OF_DATA);
            regs->tflg = 0x01u;
            
            ret = BL_ERR_OK;
        }
        #else
        if (((pdu->canId) & MSK_EXT_ID_MSB)) {
            l_frm_tx.type = CAN_MSG_ID_EXT;
            l_frm_tx.msg_id = pdu->canId;
            l_id_mask = 0x1fffffff;
        } else {
            l_frm_tx.type = CAN_MSG_ID_STD;
            l_frm_tx.msg_id = pdu->canId;
            l_id_mask = 0x7ff;
        }
        regs = &gs_CanController[handle].regs->base;
        if (regs == CAN0) {
            l_frm_tx.phy_id = CAN_ID_0;
            //p_tx_cfg = &g_can_id_0_cfg_tx_table[0];
        }
        #if 0
        else if (regs == CAN1) {
            //l_tmp = ((CAN2->TSR) >> 26) & 0X07;
            l_frm_tx.phy_id = CAN_ID_0;
            //p_tx_cfg = &g_can_id_0_cfg_tx_table[0];
        }
        #endif
        else {
            #ifdef CAN_ID_0_EN
            l_frm_tx.phy_id = CAN_ID_0;
            #endif
            #ifdef CAN_ID_1_EN
            l_frm_tx.phy_id = CAN_ID_1;
            #endif
            return BL_ERR_NOT_OK;
        }
        
        for (i = 0; i < CAN_ID_0_TX_MAILBOX_ID_MAX; i++) {
            #if 1
            if (i == 2) {
                if (FL_CAN_IsActiveFlag_TXHighPriorBuffFullSignal(CAN) == FL_RESET
                    //&& ( (g_can_id_1_cfg_tx_table[i].can_id&l_id_mask) == (l_frm_tx.msg_id&l_id_mask)  )
                ) {
                    phyhthTXEMask |= (0x01 << i);
                } else {
                    phyhthTXEMask &= ~(0x01 << i);
                }
            } else {
                if (FL_CAN_IsActiveFlag_TXBuffFullSignal(CAN) == FL_RESET
                    //&& ( (g_can_id_1_cfg_tx_table[i].can_id&l_id_mask) == (l_frm_tx.msg_id&l_id_mask)  )
                ) {
                    phyhthTXEMask |= (0x01 << i);
                } else {
                    phyhthTXEMask &= ~(0x01 << i);
                }
            }
            #else
            if (FL_CAN_IsActiveFlag_TXBuffFullSignal(CAN) == FL_RESET
                //&& ( (g_can_id_1_cfg_tx_table[i].can_id&l_id_mask) == (l_frm_tx.msg_id&l_id_mask)  )
            ) {
                phyhthTXEMask |= (0x01 << i);
                //break;
            } else {
                phyhthTXEMask &= ~(0x01 << i);
            }
            #endif
        }
        
        //判斷任何一個郵箱非空
        #if 1
        /*look for the empty phyth to transmit*/
        for (phyhth = 0; phyhth < 3; phyhth++) { //CAN_MULTIPLEXED_HTH_MBNUM宏代替数字3
            l_tmp = 0x01 << phyhth;
            //phyhthTXEMask = CANx_TXMASK(phyhth);
            if ((0 != (l_tmp & phyhthTXEMask)) 	/*bit 1 表示空     可以發送 和 STM32的 TME位一樣*/
            ) {	/*恆潤 同時也多定義了一個全局變量和他相對應，安全一點*/
                hthmatchflag = 1;
                break;
            }
        }
        #else
        phyhth = CanHardwareObjectConfig[Hth].phyHohRef;
        phyhthTXEMask = CAN_TXMASK(phyhth);
        if ((0 != (l_tmp & phyhthTXEMask))
            && (TRUE == EmptyFlagForHth[Controller][phyhth])) {
            hthmatchflag = CAN_MATCH;
        }
        #endif
        if (hthmatchflag == 1) {
            gs_CanController[handle].status = CAN_STATUS_TRANSMITTING;
            //txBuf = &gs_CanController[handle].regs->txBuf;
        
            //if(0 == ((pdu->canId) & 0x10000000)) //((uint32)(0x10000000)) MSK_EXT_ID_MSB  //
            #if 0
            //l_frm_tx.len = (bl_u8_t)BL_GET_LOW_HALF(pdu->dlc);  //注意，这里修改，不判断低4位
            #else
            l_frm_tx.len = pdu->dlc;
            if (pdu->dlc > CAN_MAX_SIZE_OF_DATA) {
                l_frm_tx.len = pdu->dlc;
            }
            #endif
            l_frm_tx.fd_enable = pdu->canfd;
            //这里要有BRS的处理
            l_frm_tx.enable_brs = pdu->brs;
            Bl_MemCpy((bl_Buffer_t *)l_frm_tx.buf,
                pdu->frame,
                (bl_Size_t)CAN_MAX_SIZE_OF_DATA);
            Bl_MemCpy((bl_Buffer_t *)&g_can_driver[l_frm_tx.phy_id].tx, (bl_Buffer_t *)&l_frm_tx, sizeof(can_queue_elem_s));
            can_tx_start(&g_can_driver[l_frm_tx.phy_id].tx);
        
            ret = BL_ERR_OK;
        }
        
        #endif
    }
    
    return ret;
    
}

/**************************************************************************//**
 *
 *  \details Receive can message.
 *
 *  \param[in/out] pdu  - the pointer of can Rx PDU.
 *
 *  \return If the can message is received successfully return BL_ERR_OK.
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - can message is received successfully.
 *  \retval BL_ERR_NOT_OK   - no can message can be received.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
/*
struct _tag_CanRxPdu
{
	bl_ComIfHandle_t handle;    //這裡的hander  是指buf 的id ； 即 gs_CanIfPduCfg數據的buf id
	bl_BufferSize_t dlc;
	bl_u32_t canId;
	bl_Buffer_t frame[CANIF_MAX_CAN_FRAME_SIZE];
};
 */
bl_Return_t Can_Read(bl_CanRxPdu_t *pdu) {
    bl_u8_t i;
    bl_CanControllerBaseRegs_t *regs;
    can_id_e l_can_id;
    bl_Return_t ret = BL_ERR_NOT_OK;
    ITStatus itstatus = RESET;
    //can_rx_q_t l_frm_rx ;
    bl_CanRxPdu_t l_pdu_rx; //结构体指针
    QUEUE_OPERATE_FLAG_E l_result;
    
    #ifdef CAN_RX_INTERRUPT_EN
    //處理隊列中的數據
    l_result = queue_get_head(&can1_rx_queue, (sequential_queue_elem *)&l_pdu_rx); //l_pdu_rx这里转为变量指针
    if (l_result == QUEUE_OK) {
        queue_del_element(&can1_rx_queue, (sequential_queue_elem *)&l_pdu_rx); //将can1_rx_queue中的先传给 l_pdu_rx
        //*pdu = l_pdu_rx;
        Bl_MemCpy((bl_Buffer_t *)pdu, (const bl_Buffer_t *)&l_pdu_rx, (bl_Size_t)sizeof(bl_CanRxPdu_t)); //将l_pdu_rx中的数据传给pdu
        ret = BL_ERR_OK;
        //break;
    } else {
    }
    #else
    for (i = 0; i < CAN_USED_CONTROLLER_NUMBER; i++) {
        regs = &gs_CanController[i].regs->base;
        /* Check CAN_RF0R_FMP0 bit */
        itstatus = CAN_GetFlagStatus(regs, CAN_FLAG_FMP0);
        if (itstatus != RESET) {
            if (regs == CAN1) {
                l_can_id = CAN_ID_1;
            } else {
                l_can_id = CAN_ID_2;
            }
            can_frame_rx(l_can_id, CAN_FIFO_ID_0, &l_frm_rx);
            pdu->handle = i;
            pdu->dlc = l_frm_rx.dlc;
            pdu->canId = l_frm_rx.identifier.st_ex_id;
            Bl_MemCpy(pdu->frame,
                (const bl_Buffer_t *)l_frm_rx.data,
                (bl_Size_t)CAN_MAX_SIZE_OF_DATA);
            regs->RF0R |= CAN_RF0R_RFOM0;
            //CAN_ClearFlag(regs,CAN_FLAG_FF0|CAN_FLAG_FOV0);
            ret = BL_ERR_OK;
            break;
        }
    }
    #endif
    return ret;
}


void Can_SetTxStatus_finish(bl_ComIfHandle_t handle) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    volatile bl_CanControllerBaseRegs_t *regs;
    u8 l_tmp;
    BL_DEBUG_ASSERT_PARAM(handle < CAN_USED_CONTROLLER_NUMBER);
    gs_CanController[handle].status = CAN_STATUS_TRANSMIT_FINISH;
}
/**************************************************************************//**
 *
 *  \details Check can transmit status.
 *
 *  \param[in] handle  - the index of a frame pdu list.
 *
 *  \return If the can message is transmited successfully return
 *          BL_ERR_OK. otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - the data is transmitted successfully.
 *  \retval BL_ERR_NOT_OK   - can driver is not transmitting data.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Can_CheckTxStatus(bl_ComIfHandle_t handle) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    volatile bl_CanControllerBaseRegs_t *regs;
    u8 l_tmp;
    BL_DEBUG_ASSERT_PARAM(handle < CAN_USED_CONTROLLER_NUMBER); //“断言”不执行
    
    #ifdef CAN_TX_INTERRUPT_EN
    if (CAN_STATUS_TRANSMIT_FINISH == gs_CanController[handle].status) {
        gs_CanController[handle].status = CAN_STATUS_IDLE;
        ret = BL_ERR_OK;
    }
    #else
    if (CAN_STATUS_TRANSMITTING == gs_CanController[handle].status) {
        regs = &gs_CanController[handle].regs->base;
        #if 0
        if ((regs->tflg & 0x01u) > 0u) {
            gs_CanController[handle].status = CAN_STATUS_IDLE;
            ret = BL_ERR_OK;
        }
        #else
        //l_tmp = ((regs->TSR) >> 26) & 0X07;
        //if(l_tmp==0x07) //表示三个邮箱都空了，才是空闲
        if ((regs->TSR & (CAN_TSR_TXOK0 | CAN_TSR_TXOK1 | CAN_TSR_TXOK2))  != 0) { //任一郵箱成功
            gs_CanController[handle].status = CAN_STATUS_IDLE;
            ret = BL_ERR_OK;
        }
        #endif
    }
    #endif
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check can busoff.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Can_BusOff_Check(void) {
    CanNm_MainFunction();
    //can1_busoff_op_task();
}
#if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details Set all can controller into sleep mode.
 *
 *  \return None.
 *
 *  \note   This interface is not set the WUPE in the CTL0 register.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Can_Sleep(void) {
    bl_u8_t i;
    bl_u16_t delay = CAN_TIMEOUT_DURATION;
    volatile bl_CanControllerBaseRegs_t *regs;
    
    for (i = 0; i < CAN_USED_CONTROLLER_NUMBER; i++) {
        #if 0
        regs = &gs_CanController[i].regs->base;
        /* First set hw start,then sleep ,otherwise set sleep is wrong*/
        if ((regs->ctl0 & CAN_CTL0_INITRQ) > 0u) {
            regs->ctl0 &= ((bl_u8_t)~CAN_CTL0_INITRQ);
            BL_WAITFOR_TIMEOUT(delay, ((regs->ctl1 & CAN_CTL1_INITAK) > 0u))
        }
        
        regs->rflg |= CAN_RFLG_ALL;
        
        delay = CAN_TIMEOUT_DURATION;
        /*request enter into sleep mode and  allows the MSCAN to
        restart from sleep mode when traffic on CAN is detected*/
        regs->ctl0 |= CAN_CTL0_SLPRQ;
        BL_WAITFOR_TIMEOUT(delay, ((regs->ctl1 & CAN_CTL1_SLPAK) > 0u))
        #endif
    }
    
    #if 0
    _asm CLI;/*Enable the interrupt.*/
    #endif
    return ;
}

/**************************************************************************//**
 *
 *  \details The interrupt service function is used to can wakeup interrupt.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
interrupt CAN_USED_CONTROLLER_ISR_ID void Can_Wakeup(void) { /*PRQA S 1503,3408*/
    return ;
}
#endif


/**************************************************************************//**
 *
 *  \details Initialize the can controller.
 *
 *  \param[in] regs     - the pointer of the register list of can controller.
 *  \param[in] initReg  - the configurations of the register of can controller.
 *
 *  \return If the can controller is initialized successfully return BL_ERR_OK,
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
static bl_Return_t _Can_Init(volatile bl_CanControllerBaseRegs_t *regs,
    const bl_CanControllerBaseRegs_t *initReg) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    #if 0
    bl_u16_t delay = CAN_TIMEOUT_DURATION;
    
    regs->ctl0 |= CAN_CTL0_INITRQ;
    
    BL_WAITFOR_TIMEOUT(delay, ((regs->ctl1 & CAN_CTL1_INITAK) > 0u))
    if (delay != 0) {
        delay = CAN_TIMEOUT_DURATION;
        
        /* initialise hardware registers.*/
        regs->ctl1 = initReg->ctl1;
        regs->btr0 = initReg->btr0;
        regs->btr1 = initReg->btr1;
        regs->idac = initReg->idac;
        
        Bl_MemCpy((bl_Buffer_t *) & (regs->idar0),
            (const bl_Buffer_t *) & (initReg->idar0),
            (bl_Size_t)CAN_NUMBER_OF_FILTER_REGS);
            
        /* quit the init mode */
        regs->ctl0 &= (~(bl_u8_t)(CAN_CTL0_SLPRQ | CAN_CTL0_INITRQ));
        BL_WAITFOR_TIMEOUT(delay, (0u == (regs->ctl1 & CAN_CTL1_INITAK)))
        /* wait for can quit init mode */
        if (delay != 0) {
            /*Init the WUPE or TIM*/
            regs->ctl0 |= initReg->ctl0;
            regs->rier = initReg->rier;
            regs->tier = 0;
            /* Clear all flags */
            regs->rflg = CAN_RFLG_ALL;
            ret = BL_ERR_OK;
        }
    }
    #else
    if (regs == CAN0) {
        #ifdef CAN_ID_0_EN
        can0_init();
        #endif
        ret = BL_ERR_OK;
    }
    #if 0
    else if (regs == CAN1) {
        #ifdef CAN_ID_1_EN
        //can1_init();
        #endif
        ret = BL_ERR_OK;
    }
    #endif
    /*
    else if(regs == CAN2)
    {
    	//can2_init();
    	ret = BL_ERR_OK;
    }
    	*/
    else {
    
    }
    #endif
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details De-initialize the can controller.
 *
 *  \param[in] regs     - the pointer of the register list of can controller.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
static void _Can_DeInit(volatile bl_CanControllerBaseRegs_t *regs) {
    CAN_TypeDef *p_can = (bl_CanControllerBaseRegs_t *)regs;
    #if 0
    bl_u16_t delay = CAN_TIMEOUT_DURATION;
    
    regs->ctl0 |= CAN_CTL0_SLPRQ;
    BL_WAITFOR_TIMEOUT(delay, ((regs->ctl1 & CAN_CTL1_SLPAK) > 0u))
    
    regs->ctl0 &= (~(bl_u8_t)CAN_CTL0_SLPRQ);
    
    delay = CAN_TIMEOUT_DURATION;
    
    regs->ctl0 |= CAN_CTL0_INITRQ;
    BL_WAITFOR_TIMEOUT(delay, ((regs->ctl1 & CAN_CTL1_INITAK) > 0u))
    #else
    //CAN_DeInit(p_can);
    if (p_can == CAN0) {
        #ifdef CAN_ID_0_EN
        can0_de_init();
        #endif
    }
    #if 0
    else if (p_can == CAN1) {
        #ifdef CAN_ID_1_EN
        //FLEXCAN_DRV_Deinit(CAN_ID_1);
        #endif
    } else if (p_can == CAN2) {
        #ifdef CAN_ID_2_EN
        //FLEXCAN_DRV_Deinit(CAN_ID_2);
        #endif
    }
    #endif
    else {
    
    }
    #endif
    return ;
}

#if (CANIF_FUN_DYNAMIC_CANID == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details Set the can id.
 *
 *  \return None.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
static void _Can_SetCanId(void) {
    bl_u32_t id;
    /*add your code to make the can id*/
#error "add your code to make the can id and delete this message."
    /*set the rx id*/
    Canif_SetRxCanId(CANIF_PHY_RX_HANDLE, id);
    /*set the tx id*/
    Canif_SetTxCanId(CANIF_TX_HANDLE, id);
}
#endif






/**
 * @brief    CAN1  接收0 中斷函數； 發現是以幀的形式接收一次；
 * @param
 * @return   指針地址
 */
void can_id_phy_rx_irq_task(u8 phy_id, const can_queue_elem_s *p_frame_info) {
    u8 i;
    
    #if 0
    can_rx_q_t l_frm_rx;
    
    l_frm_rx.type = p_frame_info->type;
    l_frm_rx.phy_id = p_frame_info->phy_id;
    l_frm_rx.mailbox_id = p_frame_info->mailbox_id;
    l_frm_rx.msg_id = p_frame_info->msg_id;
    l_frm_rx.len = p_frame_info->len;
    for (i = 0; i < 64; i++) {
        l_frm_rx.buf[i] = p_frame_info->buf[i];
    }
    Bl_MemCpy((bl_Buffer_t *)&g_can_driver[CAN_ID_0].rx, (bl_Buffer_t *)&l_frm_rx, sizeof(can_rx_q_t));
    #endif
    
    #if 1
    bl_CanRxPdu_t l_pdu_rx;
    
    bl_ComIfHandle_t handle;    //這裡的hander  是指buf 的id ； 即 gs_CanIfPduCfg 數據的buf id
    bl_BufferSize_t dlc;
    bl_u32_t canId;
    bl_Buffer_t frame[CANIF_MAX_CAN_FRAME_SIZE];
    if (phy_id == CAN_ID_0) { //phy_id = CAN_ID_0
        #ifdef CAN_ID_0_EN
        l_pdu_rx.handle = HANDLE_CAN0_RX0;  // HANDLE_CAN0_RX0
        #endif
    } else if (phy_id == CAN_ID_1) {
        #ifdef CAN_ID_1_EN
        l_pdu_rx.handle = HANDLE_CAN1_RX0;  // HANDLE_CAN0_RX0
        #endif
    } else if (phy_id == CAN_ID_2) {
        #ifdef CAN_ID_2_EN
        l_pdu_rx.handle = HANDLE_CAN2_RX0;  // HANDLE_CAN0_RX0
        #endif
    } else {
        l_pdu_rx.handle = 0xffu;  //
    }
    
    l_pdu_rx.dlc = p_frame_info->len;
    l_pdu_rx.canId = p_frame_info->msg_id;
    for (i = 0; i < CAN_FRAME_MAX_DLC; i++) {
        l_pdu_rx.frame[i] = p_frame_info->buf[i];
    }
    #else
    
    #endif
    
    #ifdef CAN_RX_INTERRUPT_EN
    queue_add_element(&can1_rx_queue, (sequential_queue_elem *)&l_pdu_rx);
    #endif
    g_bl_can_test[0].buf[1]++;
}

/**
 * @brief    喚醒中斷
 * @param
 * @return
 */
void can_id_phy_wakeup_irq_task(u8 phy_id) {
    #if 0
    u8 phyController = CAN_ID_0;
    u8 l_logic_id;
    
    l_logic_id = can_drive_get_logic_id(phyController);	//EmptyFlagForHth 類似這樣的變量 都是以bufid為順序，而並非物理通道
    
    #ifdef CAN_TEST_USE_DRIVE
    #ifdef CAN_LOGIC_DEBUG
    g_can_driver[CAN_ID_0].buf[5]++;
    #endif
    g_can_driver[CAN_ID_0].wakeup.count = 0;
    if (g_can_driver[CAN_ID_0].wakeup.state != 1) {
        g_can_driver[CAN_ID_0].wakeup.state = 1;
        g_can_driver[CAN_ID_0].state = CAN_DRIVE_STATE_INIT;
        (void)Can_SetControllerMode(l_logic_id, CAN_T_WAKEUP);
    }
    #endif
    #ifdef TEST_QUEUE_FOR_TEST_EN
    test_queue_for_test_add(5);
    #endif
    CanIf_ControllerWakeUp(l_logic_id) ;
    #endif
}

#if 0
/**
 * @brief    busoff  >=255
 *           原來的代碼沒有查完寄存器；
 * @param
 * @return
 */
void can_logic0_phy_busoff_irq_task(void) {
    #if 1
    u8 phyController = CAN_ID_0;
    //u8 l_logic_id;
    
    //l_logic_id = can_drive_get_logic_id(phyController);	//EmptyFlagForHth 類似這樣的變量 都是以bufid為順序，而並非物理通道
    //#ifdef CAN_TEST_USE_DRIVE
    #ifdef CAN_LOGIC_DEBUG
    g_can_driver[CAN_ID_0].buf[4]++;
    #endif
    g_can_driver[CAN_ID_0].busoff.count = 0;
    if (g_can_driver[CAN_ID_0].busoff.state != 1) {
        g_can_driver[CAN_ID_0].busoff.state = 1;
        g_can_driver[CAN_ID_0].state = CAN_DRIVE_STATE_BUSOFF;
        //(void)Can_SetControllerMode(l_logic_id, CAN_T_STOP);
    }
    //#endif
    #ifdef TEST_QUEUE_FOR_TEST_EN
    test_queue_for_test_add(1);
    #endif
    
    //CanIf_ControllerBusOff(l_logic_id);			/*是和CanNm_PhysErrorInd 調用相同效果*/
    #endif
}
#endif


/**
* @brief  用於測試 在drive層的busoff恢復
* @param  input:
* @param  output:
* @return	none:
* @calls
* @calls by
* @others
*/
void  can1_busoff_op_task(void) {
    #if 0
    //#ifdef CAN_TEST_USE_DRIVE
    
    //can_init_s l_can;
    u8 l_flag;
    u8 l_flag_wakeup;
    u16 l_count;
    u8 l_logic_id;
    CAN_DRIVE_SR_ALLOC();
    CAN_DRIVE_CRITICAL_ENTER();
    //l_logic_id = can_drive_get_logic_id(CAN_ID_1);	//EmptyFlagForHth 類似這樣的變量 都是以bufid為順序，而並非物理通道
    l_flag = g_can_driver[CAN_ID_0].busoff.state;
    l_flag_wakeup = g_can_driver[CAN_ID_0].wakeup.state;
    CAN_DRIVE_CRITICAL_EXIT();
    /**/
    if (l_flag == 1) {
        CAN_DRIVE_CRITICAL_ENTER();
        #ifdef CAN_LOGIC_DEBUG
        g_can_busoff_test[CAN_ID_0][7]++;
        #endif
        g_can_driver[CAN_ID_0].busoff.count++;
        l_count = g_can_driver[CAN_ID_0].busoff.count;
        CAN_DRIVE_CRITICAL_EXIT();
        if (l_count >= 5) {
            CAN_DRIVE_CRITICAL_ENTER();
            g_can_driver[CAN_ID_0].busoff.count = 0;
            CAN_DRIVE_CRITICAL_EXIT();
            #ifdef CAN_LOGIC_DEBUG
            g_can_busoff_test[CAN_ID_0][8]++;
            #endif
            //can_phy_tx[CAN_ID_1].busoff_flag = 0;
            
            #if 0
            l_can.id = CAN_ID_1;
            /* CAN1 and CAN2  cell init */
            l_can.config.ttcm = DISABLE;  /* time triggered communication mode 時間觸發模式*/
            l_can.config.abom = DISABLE;  /*  Set the automatic bus-off management 不啟用自動離線模式 */
            l_can.config.awum = DISABLE;  /* Set the automatic wake-up mode 由軟件方式喚醒*/
            l_can.config.nart = DISABLE; /* Set the no automatic retransmission 自動重傳開啟*/
            l_can.config.rflm = DISABLE; /* Set the receive FIFO locked mode 鎖定FIFO 即溢出時；下一個收到的報文被丟棄 */
            l_can.config.txfp = ENABLE;  /*1.優先級由發送請求的順序來決定*/ /* 0.優先級由報文的標識符來決定 */
            l_can.config.mode = CAN_Mode_Normal;  /* 本外設正常工作；soft master reset CAN軟件復位 值1表示強行復位；*/
            l_can.config.sjw = CAN_SJW_1tq;   /*重新同步跳躍寬度*/
            l_can.config.bs1 = CAN_BS1_3tq;  /*時間段1*/
            l_can.config.bs2 = CAN_BS2_2tq;  /*時間段2*/
            l_can.config.prescaler = 12u;   /*波特率分頻器*/
            can_init(l_can);
            
            #else
            //can_re_fresh(CAN1);
            //(void)Can_InitController(0,0);
            FLEXCAN_DRV_Deinit(CAN_ID_0);
            can0_init();
            Can_SetTxStatus_finish(CAN_ID_0);
            #endif
            
            #if 0
            if (CANIF_CS_SLEEP == CanIf_ControllerMode[CANIF_CONTROLLER_ID]) {
                ret = (Std_ReturnType)Can_SetControllerMode(CANIF_CONTROLLER_ID,
                        CAN_T_WAKEUP);
            }
            if (E_OK == ret) {
                ret = (Std_ReturnType)Can_SetControllerMode(CANIF_CONTROLLER_ID,
                        CAN_T_START);
            }
            #else
            //Can_SetControllerMode(CAN_CONTROLLER_0_ID,  CAN_T_START);
            #endif
            CAN_DRIVE_CRITICAL_ENTER();
            g_can_driver[CAN_ID_0].busoff.state  = 2;
            CAN_DRIVE_CRITICAL_EXIT();
        }
        
    } else if (l_flag == 2) {
        CAN_DRIVE_CRITICAL_ENTER();
        g_can_driver[CAN_ID_0].busoff.count++;
        if (g_can_driver[CAN_ID_0].busoff.count >= 10) {
            g_can_driver[CAN_ID_0].busoff.count = 0u;
            g_can_driver[CAN_ID_0].busoff.state  = 0u;
            g_can_driver[CAN_ID_0].state = CAN_DRIVE_STATE_NORMAL;
        }
        CAN_DRIVE_CRITICAL_EXIT();
    }
    
    if (l_flag_wakeup == 1) {
        CAN_DRIVE_CRITICAL_ENTER();
        g_can_driver[CAN_ID_0].wakeup.count++;
        if (g_can_driver[CAN_ID_0].wakeup.count >= 10) {
            g_can_driver[CAN_ID_0].wakeup.state = 0u;
            g_can_driver[CAN_ID_0].wakeup.count = 0u;
            //(void)Can_InitController(0,0);
            //Can_SetControllerMode(CAN_CONTROLLER_0_ID,  CAN_T_START);
            g_can_driver[CAN_ID_0].state = CAN_DRIVE_STATE_NORMAL;
            
        }
        CAN_DRIVE_CRITICAL_EXIT();
    }
    
    //#endif
    #endif
}





u8 can_drive_get_logic_id(u8 phy_id) {
    u8 i;
    u8 l_result = CAN_ID_NULL;
    for (i = 0; i < CAN_USED_CONTROLLER_NUMBER; i++) {
        if (phy_id == gs_CanControllerCfg[i].phyId) {
            l_result = i;
            break;
        }
    }
    return l_result;
    
}

u8 can_drive_get_phy_id(u8 logic_id) {
    u8 i;
    u8 l_result = CAN_ID_NULL;
    
    if (logic_id >= CAN_LOGIC_ID_MAX) {
    
    } else {
        l_result = gs_CanControllerCfg[logic_id].phyId;
    }
    
    return l_result;
    
}



bl_Return_t  Can_Controller_enable(u8 Controller, u8 state) {

    bl_Return_t ret = BL_ERR_NOT_OK;
    u8 phyController = gs_CanControllerCfg[Controller].phyId;
    CAN_TypeDef *p_can_base;
    
    if (CAN_ID_0 == phyController) {
        p_can_base = CAN0;
    }
    #ifdef S32K14x_SERIES  //目前先針對144
    else if (CAN_ID_1 == phyController) {
        p_can_base = CAN1;
    }
    #endif
    else {
        p_can_base = CAN0;
    }
    
    if (state == TRUE) {
        //FLEXCAN_Enable(p_can_base);
        //CAN_SetIntEnable(p_can_base,ENABLE);
    } else {
        //FLEXCAN_Disable(p_can_base);
        //CAN_SetIntEnable(p_can_base,DISABLE);
    }
    ret = BL_ERR_OK;
    return ret;
}


/*
	bl_ComIfHandle_t handle;
	bl_BufferSize_t dlc;
	bl_u32_t canId;
	const bl_Buffer_t *frame;
 */


#ifdef CAN_AUTOSAR_EN

#else
////////////////測試
bl_CanTxPdu_t g_bl_can_tx;
u16 g_bl_can_tx_count;
u8 g_bl_can_tx_data[8];


void bl_can_tx_task(void) {
    u8 i;
    bl_CanTxPdu_t l_bl_can_tx;
    g_bl_can_tx.handle = 0;
    
    Can_CheckTxStatus(g_bl_can_tx.handle);
    
    g_bl_can_tx_count++;
    for (i = 0; i < 8; i++) {
        g_bl_can_tx_data[i] = i + g_bl_can_tx_count;
    }
    g_bl_can_tx.dlc = 8;
    g_bl_can_tx.canId = 0x112; //g_can_id_0_cfg_tx_table[0].can_id;
    g_bl_can_tx.frame = &g_bl_can_tx_data[0];
    Can_Write((const bl_CanTxPdu_t *)&g_bl_can_tx);
}


/*
struct _tag_CanRxPdu
{
	bl_ComIfHandle_t handle;    //這裡的hander  是指buf 的id ； 即 gs_CanIfPduCfg數據的buf id
	bl_BufferSize_t dlc;
	bl_u32_t canId;
	bl_Buffer_t frame[CANIF_MAX_CAN_FRAME_SIZE];
};

 */

bl_CanRxPdu_t g_bl_can_rx;
u16 g_bl_can_rx_count;
u8 g_bl_can_rx_data[8];
bl_Return_t g_bl_can_rx_result;

void bl_can_rx_task(void) {
    u8 i;
    //g_bl_can_rx.frame =&g_bl_can_rx_data[0];
    g_bl_can_rx_result = Can_Read(&g_bl_can_rx);
    if (BL_ERR_OK == g_bl_can_rx_result) {
        g_bl_can_rx_count++;
    }
}

#endif
