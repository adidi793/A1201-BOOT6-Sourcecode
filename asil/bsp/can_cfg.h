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




#ifndef __CAN_CFG_H__
#define __CAN_CFG_H__


#include "common_types.h"
#include "Cpu.h"



/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

////非常重要的宏；是否用恒潤的can即采購的AUTOSAR
//#define CAN_AUTOSAR_EN  1
//#define  CAN_DRIVE_QUEUE_ENABLE   //開啟底層隊列   -- 和宏 CAN_AUTOSAR_EN  相互斥
#define CAN_LS_NM 	1  //和網絡管理相關的底層

#define CAN_ID_0_EN  //決定通產是否開啟
//#define CAN_ID_1_EN  //決定通產是否開啟
//#define CAN_ID_2_EN  //決定通產是否開啟

//#define CAN_ID_0_FD_EN 1  //开启 CANFD功能
//#define CAN_ID_1_FD_EN 1  //开启 CANFD功能
//#define CAN_ID_2_FD_EN 1  //开启 CANFD功能


#define CAN_ID_TOTAL 1u   //和  CAN_USED_CONTROLLER_NUM 密切相關
#define CAN_LOGIC_ID_MAX   CAN_ID_TOTAL


typedef u8 can_ide_t;
typedef u8 can_rtr_t;


typedef u8 flexcan_event_type_t  ;
typedef u8 flexcan_user_config_t  ;
typedef u8 flexcan_msgbuff_id_type_t  ;
typedef struct {
    u8 state;
} FlexCANState;

/* Private typedef -----------------------------------------------------------*/
typedef  struct {
    unsigned char   SJW;
    unsigned char   BS1;
    unsigned char   BS2;
    unsigned short  PreScale;
} tCAN_BaudRate;


//////////////////s32k
#ifdef CAN_ID_0_FD_EN
#else
//#define CAN_ID_0_USE_RX_FIFO  1   //是否啟用；rx fifo   在CANFD模式下，不能使用
#endif

#ifdef CAN_ID_1_FD_EN
#else
//#define CAN_ID_1_USE_RX_FIFO  1   //是否啟用；rx fifo   在CANFD模式下，不能使用
#endif

#ifdef CAN_ID_2_FD_EN
#else
//#define CAN_ID_2_USE_RX_FIFO  1   //是否啟用；rx fifo   在CANFD模式下，不能使用
#endif


#ifdef CAN_ID_0_USE_RX_FIFO
#define CAN_ID_0_RX_FIFO_EN  true
#else
#define CAN_ID_0_RX_FIFO_EN  false
#endif

#ifdef CAN_ID_1_USE_RX_FIFO
#define CAN_ID_1_RX_FIFO_EN  true
#else
#define CAN_ID_1_RX_FIFO_EN  false
#endif

#ifdef CAN_ID_2_USE_RX_FIFO
#define CAN_ID_2_RX_FIFO_EN  true
#else
#define CAN_ID_2_RX_FIFO_EN  false
#endif

#define FLEXCAN_USE_PAYLOAD_SIZE_8  0u
#define FLEXCAN_USE_PAYLOAD_SIZE_16  1u
#define FLEXCAN_USE_PAYLOAD_SIZE_32  2u
#define FLEXCAN_USE_PAYLOAD_SIZE_64  3u


#ifdef CAN_ID_0_FD_EN
#define CAN_ID_0_USER_REG_PAYLOAD_SIZE  FLEXCAN_USE_PAYLOAD_SIZE_64
#define CAN_ID_0_FD_CFG  true
#else
#define CAN_ID_0_USER_REG_PAYLOAD_SIZE  FLEXCAN_USE_PAYLOAD_SIZE_8
#define CAN_ID_0_FD_CFG  false
#endif

#ifdef CAN_ID_1_FD_EN
#define CAN_ID_1_USER_REG_PAYLOAD_SIZE  FLEXCAN_USE_PAYLOAD_SIZE_64
#define CAN_ID_1_FD_CFG  true
#else
#define CAN_ID_1_USER_REG_PAYLOAD_SIZE  FLEXCAN_USE_PAYLOAD_SIZE_8
#define CAN_ID_1_FD_CFG  false
#endif

#ifdef CAN_ID_2_FD_EN
#define CAN_ID_2_USER_REG_PAYLOAD_SIZE  FLEXCAN_USE_PAYLOAD_SIZE_64
#define CAN_ID_2_FD_CFG  true
#else
#define CAN_ID_2_USER_REG_PAYLOAD_SIZE  FLEXCAN_USE_PAYLOAD_SIZE_8
#define CAN_ID_2_FD_CFG  false
#endif


#define CAN_FRAME_MAX_DLC (u16)8u  /* CAN底層的一幀字節數 --最大可能的值*/  //如果CANFD要改成64
//#define CAN_DATA_DLC               ((uint8)CAN_FRAME_MAX_DLC)

#ifdef CAN_ID_0_FD_EN
#if (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_64)
#define CAN_ID_0_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_64
//#define CAN_MCR_MAXMB_VALUE 7  //最大組數
#define CAN_ID_0_FRAME_DLC 64u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_32)
#define CAN_ID_0_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_32
//#define CAN_MCR_MAXMB_VALUE 12  //最大組數
#define CAN_ID_0_FRAME_DLC 32u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_16)
#define CAN_ID_0_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_16
//#define CAN_MCR_MAXMB_VALUE 21  //最大組數
#define CAN_ID_0_FRAME_DLC 16u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_8)
#define CAN_ID_0_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_8
//#define CAN_MCR_MAXMB_VALUE 32  //最大組數
#define CAN_ID_0_FRAME_DLC 8u  /* CAN底層的一幀字節數 */
#endif
#else
#define CAN_ID_0_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_8
//#define CAN_MCR_MAXMB_VALUE 32  //最大組數
#define CAN_ID_0_FRAME_DLC 8u  /* CAN底層的一幀字節數 */
#endif

#ifdef CAN_ID_1_FD_EN
#if (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_64)
#define CAN_ID_1_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_64
//#define CAN_MCR_MAXMB_VALUE 7  //最大組數
#define CAN_ID_1_FRAME_DLC 64u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_32)
#define CAN_ID_1_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_32
//#define CAN_MCR_MAXMB_VALUE 12  //最大組數
#define CAN_ID_1_FRAME_DLC 32u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_16)
#define CAN_ID_1_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_16
//#define CAN_MCR_MAXMB_VALUE 21  //最大組數
#define CAN_ID_1_FRAME_DLC 16u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_8)
#define CAN_ID_1_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_8
//#define CAN_MCR_MAXMB_VALUE 32  //最大組數
#define CAN_ID_1_FRAME_DLC 8u  /* CAN底層的一幀字節數 */
#endif
#else
#define CAN_ID_1_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_8
//#define CAN_MCR_MAXMB_VALUE 32  //最大組數
#define CAN_ID_1_FRAME_DLC 8u  /* CAN底層的一幀字節數 */
#endif

#ifdef CAN_ID_2_FD_EN
#if (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_64)
#define CAN_ID_2_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_64
//#define CAN_MCR_MAXMB_VALUE 7  //最大組數
#define CAN_ID_2_FRAME_DLC 64u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_32)
#define CAN_ID_2_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_32
//#define CAN_MCR_MAXMB_VALUE 12  //最大組數
#define CAN_ID_2_FRAME_DLC 32u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_16)
#define CAN_ID_2_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_16
//#define CAN_MCR_MAXMB_VALUE 21  //最大組數
#define CAN_ID_2_FRAME_DLC 16u  /* CAN底層的一幀字節數 */
#elif (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_8)
#define CAN_ID_2_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_8
//#define CAN_MCR_MAXMB_VALUE 32  //最大組數
#define CAN_ID_2_FRAME_DLC 8u  /* CAN底層的一幀字節數 */
#endif
#else
#define CAN_ID_2_REG_PAYLOAD_SIZE FLEXCAN_PAYLOAD_SIZE_8
//#define CAN_MCR_MAXMB_VALUE 32  //最大組數
#define CAN_ID_2_FRAME_DLC 8u  /* CAN底層的一幀字節數 */
#endif

//可以參照
#define CAN_FRAME_DLC  CAN_ID_0_FRAME_DLC   //記得修改，for不同的需求


#define CAN_MSG_MAX_FRMAES  4u  /*目前一個完整的數據包 最多的幀數  */
/*數據緩存區大小*/
#define CAN_MSG_MAX_SIZES  (u16)(CAN_MSG_MAX_FRMAES * CAN_FRAME_DLC)  /*目前一個完整的數據包 最多的字節數  */


#define CAN_FRAME_COM_DLC CAN_FRAME_DLC
#define CAN_FRAME_TP_DLC CAN_FRAME_DLC//32 //CAN_FRAME_DLC


/*********************************************/

/*消息隊列的深度*/
#define CAN_ID_0_TX_QUEUE_DEPTH   8u
#define CAN_ID_0_RX_QUEUE_DEPTH   8u


/*測試一次性發送的字節*/
#define CAN_ID_0_TEST_LENGTH   CAN_MSG_MAX_SIZES


/*****************以上是CAN 0邏輯相關****************************/





/*********************************************/

/*消息隊列的深度*/
#define CAN_ID_1_TX_QUEUE_DEPTH   8u
#define CAN_ID_1_RX_QUEUE_DEPTH   8u

/*測試一次性發送的字節*/
#define CAN_ID_1_TEST_LENGTH   CAN_MSG_MAX_SIZES


/*****************以上是CAN 1邏輯相關****************************/



/*********************************************/

/*消息隊列的深度*/
#define CAN_ID_2_TX_QUEUE_DEPTH   8u
#define CAN_ID_2_RX_QUEUE_DEPTH   8u

/*測試一次性發送的字節*/
#define CAN_ID_2_TEST_LENGTH   CAN_MSG_MAX_SIZES


/*****************以上是CAN 2邏輯相關****************************/









//#define CAN_SWITCH_CONTEXT_ENABLE  1u 	 /*r開啟上下文切換*/


#ifdef CAN_SWITCH_CONTEXT_ENABLE
#define CAN_CPU_SR_ALLOC()  CPU_SR_ALLOC()
#define CAN_CPU_CRITICAL_ENTER() ENTER_CRITICAL()
#define CAN_CPU_CRITICAL_EXIT()  EXIT_CRITICAL()
#else
#define 	CAN_CPU_SR_ALLOC()
#define 	CAN_CPU_CRITICAL_ENTER()
#define 	CAN_CPU_CRITICAL_EXIT()
#endif



/*! @brief Device instance number */
#define INST_CANCOM0 (0U)
#define INST_CANCOM1 (1U)
#define INST_CANCOM2 (2U)

#if 0
/*! @brief Driver state structure which holds driver runtime data */
//extern flexcan_state_t canCom1_State;
extern const flexcan_user_config_t canCom1_InitConfig0;
extern const flexcan_user_config_t canCom1_InitConfig1;
extern const flexcan_user_config_t canCom1_InitConfig2;
#endif

#define CAN_ID_0_RX_FIFO_ID_0  0u
#if 0
#define CAN_LOGIC0_RX_FIFO_ID_1  1
#define CAN_LOGIC0_RX_FIFO_ID_2  2
#define CAN_LOGIC0_RX_FIFO_ID_3  3
#define CAN_LOGIC0_RX_FIFO_ID_4  4
#define CAN_LOGIC0_RX_FIFO_ID_5  5
#define CAN_LOGIC0_RX_FIFO_ID_6  6
#define CAN_LOGIC0_RX_FIFO_ID_7  7
#endif
#define CAN_ID_0_RX_FIFO_ID_MAX  8u

#define CAN_ID_0_TX_MAILBOX_ID_0  0u
#define CAN_ID_0_TX_MAILBOX_ID_1  1u
#define CAN_ID_0_TX_MAILBOX_ID_MAX  3u

//先定義邏輯順序--接收mailbox  --fifo  以及 發送的MAILBOX
#define CAN_ID_0_RX_MAILBOX_ID_0  0u
#define CAN_ID_0_RX_MAILBOX_ID_1  1u

#ifdef CAN_ID_0_FD_EN
#if (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_64)
#define CAN_ID_0_RX_MAILBOX_ID_MAX  4     // 不超過 CAN_MCR_MAXMB_VALUE-TX //7-TX
#define CAN_ID_0_MCR_MAXMB_VALUE (CAN_ID_0_RX_MAILBOX_ID_MAX+CAN_ID_0_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_32)
#define CAN_ID_0_RX_MAILBOX_ID_MAX  9		// 不超過 CAN_MCR_MAXMB_VALUE-TX	//12-TX
#define CAN_ID_0_MCR_MAXMB_VALUE (CAN_ID_0_RX_MAILBOX_ID_MAX+CAN_ID_0_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_16)
#define CAN_ID_0_RX_MAILBOX_ID_MAX  10	// 不超過 CAN_MCR_MAXMB_VALUE-TX	//21-TX
#define CAN_ID_0_MCR_MAXMB_VALUE (CAN_ID_0_RX_MAILBOX_ID_MAX+CAN_ID_0_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_0_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_8)
#define CAN_ID_0_RX_MAILBOX_ID_MAX  10	// 不超過 CAN_MCR_MAXMB_VALUE-TX	//32-TX
#define CAN_ID_0_MCR_MAXMB_VALUE (CAN_ID_0_RX_MAILBOX_ID_MAX+CAN_ID_0_TX_MAILBOX_ID_MAX)
#endif
#else
#ifdef CAN_ID_0_USE_RX_FIFO
#define CAN_ID_0_RX_MAILBOX_ID_MAX  2u
#define CAN_ID_0_MCR_MAXMB_VALUE (16u)
#else
#define CAN_ID_0_RX_MAILBOX_ID_MAX  10u
#define CAN_ID_0_MCR_MAXMB_VALUE (CAN_ID_0_RX_MAILBOX_ID_MAX+CAN_ID_0_TX_MAILBOX_ID_MAX)
#endif
#endif

//can1
#define CAN_ID_1_RX_FIFO_ID_0  0u
#if 0
#define CAN_LOGIC0_RX_FIFO_ID_1  1
#define CAN_LOGIC0_RX_FIFO_ID_2  2
#define CAN_LOGIC0_RX_FIFO_ID_3  3
#define CAN_LOGIC0_RX_FIFO_ID_4  4
#define CAN_LOGIC0_RX_FIFO_ID_5  5
#define CAN_LOGIC0_RX_FIFO_ID_6  6
#define CAN_LOGIC0_RX_FIFO_ID_7  7
#endif
#define CAN_ID_1_RX_FIFO_ID_MAX  8u

#define CAN_ID_1_TX_MAILBOX_ID_0  0u
#define CAN_ID_1_TX_MAILBOX_ID_1  1u
#define CAN_ID_1_TX_MAILBOX_ID_MAX  3u

//先定義邏輯順序--接收mailbox  --fifo  以及 發送的MAILBOX
#define CAN_ID_1_RX_MAILBOX_ID_0  0u
#define CAN_ID_1_RX_MAILBOX_ID_1  1u

#ifdef CAN_ID_1_FD_EN
#if (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_64)
#define CAN_ID_1_RX_MAILBOX_ID_MAX  4     // 不超過 CAN_MCR_MAXMB_VALUE-TX //7-TX
#define CAN_ID_1_MCR_MAXMB_VALUE (CAN_ID_1_RX_MAILBOX_ID_MAX+CAN_ID_1_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_32)
#define CAN_ID_1_RX_MAILBOX_ID_MAX  9		// 不超過 CAN_MCR_MAXMB_VALUE-TX	//12-TX
#define CAN_ID_1_MCR_MAXMB_VALUE (CAN_ID_1_RX_MAILBOX_ID_MAX+CAN_ID_1_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_16)
#define CAN_ID_1_RX_MAILBOX_ID_MAX  10	// 不超過 CAN_MCR_MAXMB_VALUE-TX	//21-TX
#define CAN_ID_1_MCR_MAXMB_VALUE (CAN_ID_1_RX_MAILBOX_ID_MAX+CAN_ID_1_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_1_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_8)
#define CAN_ID_1_RX_MAILBOX_ID_MAX  10	// 不超過 CAN_MCR_MAXMB_VALUE-TX	//32-TX
#define CAN_ID_1_MCR_MAXMB_VALUE (CAN_ID_1_RX_MAILBOX_ID_MAX+CAN_ID_1_TX_MAILBOX_ID_MAX)
#endif
#else
#ifdef CAN_ID_1_USE_RX_FIFO
#define CAN_ID_1_RX_MAILBOX_ID_MAX  2u
#define CAN_ID_1_MCR_MAXMB_VALUE (16u)
#else
#define CAN_ID_1_RX_MAILBOX_ID_MAX  10u
#define CAN_ID_1_MCR_MAXMB_VALUE (CAN_ID_1_RX_MAILBOX_ID_MAX+CAN_ID_1_TX_MAILBOX_ID_MAX)
#endif
#endif

//can2
#define CAN_ID_2_RX_FIFO_ID_0  0u
#if 0
#define CAN_LOGIC0_RX_FIFO_ID_1  1
#define CAN_LOGIC0_RX_FIFO_ID_2  2
#define CAN_LOGIC0_RX_FIFO_ID_3  3
#define CAN_LOGIC0_RX_FIFO_ID_4  4
#define CAN_LOGIC0_RX_FIFO_ID_5  5
#define CAN_LOGIC0_RX_FIFO_ID_6  6
#define CAN_LOGIC0_RX_FIFO_ID_7  7
#endif
#define CAN_ID_2_RX_FIFO_ID_MAX  8u

#define CAN_ID_2_TX_MAILBOX_ID_0  0u
#define CAN_ID_2_TX_MAILBOX_ID_1  1u
#define CAN_ID_2_TX_MAILBOX_ID_MAX  3u

//先定義邏輯順序--接收mailbox  --fifo  以及 發送的MAILBOX
#define CAN_ID_2_RX_MAILBOX_ID_0  0u
#define CAN_ID_2_RX_MAILBOX_ID_1  1u

#ifdef CAN_ID_2_FD_EN
#if (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_64)
#define CAN_ID_2_RX_MAILBOX_ID_MAX  4     // 不超過 CAN_MCR_MAXMB_VALUE-TX //7-TX
#define CAN_ID_2_MCR_MAXMB_VALUE (CAN_ID_2_RX_MAILBOX_ID_MAX+CAN_ID_2_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_32)
#define CAN_ID_2_RX_MAILBOX_ID_MAX  9		// 不超過 CAN_MCR_MAXMB_VALUE-TX	//12-TX
#define CAN_ID_2_MCR_MAXMB_VALUE (CAN_ID_2_RX_MAILBOX_ID_MAX+CAN_ID_2_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_16)
#define CAN_ID_2_RX_MAILBOX_ID_MAX  10	// 不超過 CAN_MCR_MAXMB_VALUE-TX	//21-TX
#define CAN_ID_2_MCR_MAXMB_VALUE (CAN_ID_2_RX_MAILBOX_ID_MAX+CAN_ID_2_TX_MAILBOX_ID_MAX)
#elif (CAN_ID_2_USER_REG_PAYLOAD_SIZE == FLEXCAN_USE_PAYLOAD_SIZE_8)
#define CAN_ID_2_RX_MAILBOX_ID_MAX  10	// 不超過 CAN_MCR_MAXMB_VALUE-TX	//32-TX
#define CAN_ID_2_MCR_MAXMB_VALUE (CAN_ID_2_RX_MAILBOX_ID_MAX+CAN_ID_2_TX_MAILBOX_ID_MAX)
#endif
#else
#ifdef CAN_ID_2_USE_RX_FIFO
#define CAN_ID_2_RX_MAILBOX_ID_MAX  2u
#define CAN_ID_2_MCR_MAXMB_VALUE (16u)
#else
#define CAN_ID_2_RX_MAILBOX_ID_MAX  10u
#define CAN_ID_2_MCR_MAXMB_VALUE (CAN_ID_2_RX_MAILBOX_ID_MAX+CAN_ID_2_TX_MAILBOX_ID_MAX)
#endif
#endif

#if 0
typedef struct {
    u8 mb_fifo_id;	//物理上的MAILBOX 或者FIFO 順序
    flexcan_msgbuff_id_type_t type;
    u32 can_id;	//canid 不論 std或者ext
} can_logic_cfg_txrx_s;

typedef struct {
    u8 mb_fifo_id;	//物理上的MAILBOX 或者FIFO 順序
    bool isRemoteFrame;      /*!< Remote frame*/
    bool isExtendedFrame;    /*!< Extended frame*/
    uint32_t id;             /*!< Rx FIFO ID filter element*/
} can_logic_cfg_rxfifo_s;
#endif

#if 0
extern const can_logic_cfg_txrx_s g_can_id_0_cfg_rx_table[CAN_ID_0_RX_MAILBOX_ID_MAX];
extern const can_logic_cfg_txrx_s g_can_id_0_cfg_rxff_table[CAN_ID_0_RX_FIFO_ID_MAX];
extern const can_logic_cfg_txrx_s g_can_id_0_cfg_tx_table[CAN_ID_0_TX_MAILBOX_ID_MAX];
extern const can_logic_cfg_rxfifo_s g_can_id_0_rxfifo_id_filter[CAN_ID_0_RX_FIFO_ID_MAX] ;

extern const can_logic_cfg_txrx_s g_can_id_1_cfg_rx_table[CAN_ID_1_RX_MAILBOX_ID_MAX];
extern const can_logic_cfg_txrx_s g_can_id_1_cfg_rxff_table[CAN_ID_1_RX_FIFO_ID_MAX];
extern const can_logic_cfg_txrx_s g_can_id_1_cfg_tx_table[CAN_ID_1_TX_MAILBOX_ID_MAX];
extern const can_logic_cfg_rxfifo_s g_can_id_1_rxfifo_id_filter[CAN_ID_1_RX_FIFO_ID_MAX] ;
#endif


#define CAN_ID_0_ORed_0_15_MB_IRQn CAN0_ORed_0_31_MB_IRQn
#define CAN_ID_0_ORed_16_31_MB_IRQn CAN0_ORed_0_31_MB_IRQn
#define CAN_ID_0_Wake_Up_IRQn CAN0_ORed_Err_Wakeup_IRQn
#define CAN_ID_0_Error_IRQn CAN0_ORed_Err_Wakeup_IRQn
//#define CAN_ID_0_ORed_IRQn CAN0_ORed_IRQn

#define CAN_ID_1_ORed_0_15_MB_IRQn CAN1_ORed_0_15_MB_IRQn
//#define CAN_ID_1_ORed_16_31_MB_IRQn CAN1_ORed_16_31_MB_IRQn
//#define CAN_ID_1_Wake_Up_IRQn CAN1_Wake_Up_IRQn
#define CAN_ID_1_Error_IRQn CAN1_Error_IRQn
#define CAN_ID_1_ORed_IRQn CAN1_ORed_IRQn


#define CAN0 CAN
#define CAN_BASE_PTRS    { CAN0 }

typedef enum {
    STANDARD_CAN = 0,
    EXTENDED_CAN = 1
} CanIf_PduCanIdType;


#if 0
extern const CAN_FilterControlType g_can_filter_table[];
extern const CAN_BitrateConfigType g_can_bitrate_cfg_table;
extern const CAN_ConfigType g_can_cfg_table;
extern const CAN_BitrateConfigType g_can_data_bitrate_cfg_table;
#endif
extern const tCAN_BaudRate  CAN_BaudRateInitTab[] ;       // CLK=8MHz
extern const uint32_t g_can0_baudrate ;

void can_id_0_normal_irq_task(void *device, uint32_t wpara, uint32_t lpara);


#if 0
//暫時
typedef struct _CAN_MSG_INFO {
    uint32_t    ID;                                /*!< CAN identifier */
    uint32_t    RTS;                               /*!< Receive time stamps */
    uint8_t     ESI;                               /*!< Transmit time-stamp enable or error state indicator */
    uint8_t     DLC;                               /*!< Data length code */
    uint8_t     BRS;                               /*!< Bit rate switch */
    uint8_t     FDF;                               /*!< FD format indicator */
    uint8_t     RTR;                               /*!< Remote transmission request */
    uint8_t     IDE;                               /*!< Identifier extension */
    uint8_t     *DATA;                             /*!< Data */
} CAN_MsgInfoType;

#else

/**
  * @brief  CAN Tx message structure definition
  */

typedef struct {
    #if 0
    uint32_t StdId;  /*!< Specifies the standard identifier.
						This parameter can be a value between 0 to 0x7FF. */

    uint32_t ExtId;  /*!< Specifies the extended identifier.
						This parameter can be a value between 0 to 0x1FFFFFFF. */
    #else
    uint32_t    ID;                                /*!< CAN identifier */
    #endif

    uint8_t IDE;     /*!< Specifies the type of identifier for the message that
						will be transmitted. This parameter can be a value
						of @ref CAN_identifier_type */

    uint8_t RTR;     /*!< Specifies the type of frame for the message that will
						be transmitted. This parameter can be a value of
						@ref CAN_remote_transmission_request */

    uint8_t DLC;     /*!< Specifies the length of the frame that will be
						transmitted. This parameter can be a value between
						0 to 8 */

    uint8_t Data[CAN_FRAME_MAX_DLC]; /*!< Contains the data to be transmitted. It ranges from 0
						to 0xFF. */

    //以下为预留
    uint32_t    RTS;                               /*!< Receive time stamps */
    uint8_t     ESI;                               /*!< Transmit time-stamp enable or error state indicator */
    uint8_t     BRS;                               /*!< Bit rate switch */
    uint8_t     FDF;                               /*!< FD format indicator */
    
    uint8_t FMI;     /*!< Specifies the index of the filter the message stored in
						the mailbox passes through. This parameter can be a
						value between 0 to 0xFF */
} CAN_MsgInfoType;


#endif

#if 0
/**
  * @brief  CAN Tx message structure definition
  */

typedef struct {
    uint32_t StdId;  /*!< Specifies the standard identifier.
						This parameter can be a value between 0 to 0x7FF. */

    uint32_t ExtId;  /*!< Specifies the extended identifier.
						This parameter can be a value between 0 to 0x1FFFFFFF. */

    uint8_t IDE;     /*!< Specifies the type of identifier for the message that
						will be transmitted. This parameter can be a value
						of @ref CAN_identifier_type */

    uint8_t RTR;     /*!< Specifies the type of frame for the message that will
						be transmitted. This parameter can be a value of
						@ref CAN_remote_transmission_request */

    uint8_t DLC;     /*!< Specifies the length of the frame that will be
						transmitted. This parameter can be a value between
						0 to 8 */

    uint8_t Data[8]; /*!< Contains the data to be transmitted. It ranges from 0
						to 0xFF. */

    //以下为预留
    uint32_t    RTS;                               /*!< Receive time stamps */
    uint8_t     ESI;                               /*!< Transmit time-stamp enable or error state indicator */
    uint8_t     BRS;                               /*!< Bit rate switch */
    uint8_t     FDF;                               /*!< FD format indicator */
    
} CanTxMsg;

/**
  * @brief  CAN Rx message structure definition
  */

typedef struct {
    uint32_t StdId;  /*!< Specifies the standard identifier.
						This parameter can be a value between 0 to 0x7FF. */

    uint32_t ExtId;  /*!< Specifies the extended identifier.
						This parameter can be a value between 0 to 0x1FFFFFFF. */

    uint8_t IDE;     /*!< Specifies the type of identifier for the message that
						will be received. This parameter can be a value of
						@ref CAN_identifier_type */

    uint8_t RTR;     /*!< Specifies the type of frame for the received message.
						This parameter can be a value of
						@ref CAN_remote_transmission_request */

    uint8_t DLC;     /*!< Specifies the length of the frame that will be received.
						This parameter can be a value between 0 to 8 */

    uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to
						0xFF. */

    uint8_t FMI;     /*!< Specifies the index of the filter the message stored in
						the mailbox passes through. This parameter can be a
						value between 0 to 0xFF */
    //以下为预留
    uint32_t    RTS;                               /*!< Receive time stamps */
    uint8_t     ESI;                               /*!< Transmit time-stamp enable or error state indicator */
    uint8_t     BRS;                               /*!< Bit rate switch */
    uint8_t     FDF;                               /*!< FD format indicator */
} CanRxMsg;

#else

typedef CAN_MsgInfoType CanTxMsg;
typedef CAN_MsgInfoType CanRxMsg ;


#endif



/* 定义数据收发的帧ID，必须跟上位机配置一致 否则无法正常工作 */
/* 定义数据收发帧ID类型，0-标准帧，1-扩展帧 */
#define MSG_ID_TYPE         0
#if (MSG_ID_TYPE == 0)   /* 标准帧 */
/* 对于CAN总线，数据收发ID可以定义为一个ID，也可以定义为不同ID */
#define MSG_RECEIVE_ID      0x3C
#define MSG_SEND_ID         0x3D
#else /* 扩展帧 */
/* 对于CAN总线，数据收发ID可以定义为一个ID，也可以定义为不同ID */
#define MSG_RECEIVE_ID      0x12345
#define MSG_SEND_ID         0x12346
#endif


#define CAN_TX_STATE_IDLE 0u
#define CAN_TX_STATE_BUSY 1u

//#define CAN_ENABLE_HIGH_TX  //

#define CAN_LOGIC_DEBUG   //debug宏


#endif   /*  __CAN_CFG_H__  */






