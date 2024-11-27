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




#ifndef __CAN_H__
#define __CAN_H__

#include "common_types.h"
//#include "stm32f4xx_can.h"
#include "can_cfg.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

/*can 序號--物理通道*/
typedef enum {
    CAN_ID_0 = 0u,
    CAN_ID_1 = 1u,
    CAN_ID_2 = 2u,
    CAN_ID_NULL = 0xffu
} can_id_e;

/*#define CAN_ID_NULL 0xffu*/
/*PCLINT_ERROS*/ /*lint --e(961)*/ /* 961 不要過份依賴運算優重出級  */
#define IS_CAN_ALL_ID(ID) ( (ID) < CAN_ID_TOTAL)

//CAN邏輯通道
#define CAN_LOGIC_ID_0   0
#define CAN_LOGIC_ID_1   1
#define CAN_LOGIC_ID_2   2

//标准帧
#define CAN_MSG_ID_STD 0
#define CAN_MSG_ID_EXT 1

//远程帧
#define CAN_MSG_RTR_DATA 0
#define CAN_MSG_RTR_REMOTE 1

//FD
#define CAN_MSG_TYPE_NORMAL 0
#define CAN_MSG_TYPE_FD 1

//BRS
#define CAN_MSG_BRS_NORMAL_LOW 0
#define CAN_MSG_BRS_FAST 1

/*按鍵命令*/
typedef struct {
    flexcan_msgbuff_id_type_t type;   //标识符类型
    u8 phy_id;                        //instance 物理CAN通道
    u8 mailbox_id;	                  //郵箱id
    u32 msg_id;		                  //CAN ID  -- 和 msg中的复用
    u8 len;		                      //len -- 和 msg中的复用
    u8 fd_enable;                     // 和 msg中的复用
    u8 fd_padding;                     /*!< Set a value for padding. It will be used when the data length code (DLC)
										 specifies a bigger payload size than data_length to fill the MB */
    u8 enable_brs;                        /*!< Enable bit rate switch inside a CAN FD format frame*/
    u8 is_remote;                         /*!< Specifies if the frame is standard or remote */
    CAN_MsgInfoType msg;
    u8 buf[CAN_FRAME_MAX_DLC];
} can_queue_elem_s;

#if 0
uint32_t    ID;                                /*!< CAN identifier */
uint32_t    RTS;                               /*!< Receive time stamps */
uint8_t     ESI;                               /*!< Transmit time-stamp enable or error state indicator */
uint8_t     DLC;                               /*!< Data length code */
uint8_t     BRS;                               /*!< Bit rate switch */
uint8_t     FDF;                               /*!< FD format indicator */
uint8_t     RTR;                               /*!< Remote transmission request */
uint8_t     IDE;                               /*!< Identifier extension */
uint8_t     *DATA;                             /*!< Data */
#endif

typedef struct {
    u8 phy_id;
    CAN_MsgInfoType tx;
    CAN_MsgInfoType rx;
    u8 tx_buf[CAN_FRAME_MAX_DLC];  //最大64字节
    u8 rx_buf[CAN_FRAME_MAX_DLC];
    u8 result;
    u8 count;
    u8 state_tx;  //状态
    u8 mailbox_id;  //發送的郵箱id
    #ifdef CAN_LOGIC_DEBUG
    u8 test[16];
    #if 0
    u8 normal_int_event;
    u8 normal_int_bufid;
    u8 normal_int_count[32][3];
    u8 normal_queue_count[32][3];
    u8 error_int_event;
    u8 error_int_count;
    u16 count1[2];
    #endif
    #endif
    
} can_s;


void can_init(void);
void can_de_init(void);
void can_task(void);
void can_tx_start(const can_queue_elem_s *p_tx);
void can_op_tx_task(void);

void can_phy_drive_init(can_id_e phy_id);


#if 0

void can1_init(void);

void can_driver_busoff_config(uint32_t instance, bool enable);

void can_phy_enable(can_id_e phy_id);

///////////////////////////////s32k

void can_tx_test(void);


#endif

#define CAN_TypeDef             CAN_Type
#define CAN_FilterInitTypeDef   FL_CAN_FilterInitTypeDef

/** @defgroup CAN_identifier_type
  * @{
  */

#define CAN_Id_Standard             ((uint32_t)0x00000000)  /*!< Standard Id */
#define CAN_Id_Extended             ((uint32_t)0x00000004)  /*!< Extended Id */
#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_Id_Standard) || \
    ((IDTYPE) == CAN_Id_Extended))

/** @defgroup CAN_remote_transmission_request
  * @{
  */

#define CAN_RTR_Data                ((uint32_t)0x00000000)  /*!< Data frame */
#define CAN_RTR_Remote              ((uint32_t)0x00000002)  /*!< Remote frame */
#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_Data) || ((RTR) == CAN_RTR_Remote))

#define CAN_ID_STD                  CAN_Id_Standard
#define CAN_ID_EXT                  CAN_Id_Extended
#define CAN_RTR_DATA                CAN_RTR_Data
#define CAN_RTR_REMOTE              CAN_RTR_Remote

/**
  * @}
  */

/** @defgroup CAN_synchronisation_jump_width
  * @{
  */

#define CAN_SJW_1tq                 FL_CAN_SJW_1Tq  /*!< 1 time quantum */
#define CAN_SJW_2tq                 FL_CAN_SJW_2Tq  /*!< 2 time quantum */
#define CAN_SJW_3tq                 FL_CAN_SJW_3Tq  /*!< 3 time quantum */
#define CAN_SJW_4tq                 FL_CAN_SJW_4Tq  /*!< 4 time quantum */

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq)|| \
    ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_1
  * @{
  */

#define CAN_BS1_1tq                 FL_CAN_TS1_1Tq  /*!< 1 time quantum */
#define CAN_BS1_2tq                 FL_CAN_TS1_2Tq  /*!< 2 time quantum */
#define CAN_BS1_3tq                 FL_CAN_TS1_31Tq /*!< 3 time quantum */
#define CAN_BS1_4tq                 FL_CAN_TS1_4Tq  /*!< 4 time quantum */
#define CAN_BS1_5tq                 FL_CAN_TS1_5Tq  /*!< 5 time quantum */
#define CAN_BS1_6tq                 FL_CAN_TS1_6Tq  /*!< 6 time quantum */
#define CAN_BS1_7tq                 FL_CAN_TS1_7Tq  /*!< 7 time quantum */
#define CAN_BS1_8tq                 FL_CAN_TS1_8Tq  /*!< 8 time quantum */
#define CAN_BS1_9tq                 FL_CAN_TS1_9Tq  /*!< 9 time quantum */
#define CAN_BS1_10tq                FL_CAN_TS1_10Tq  /*!< 10 time quantum */
#define CAN_BS1_11tq                FL_CAN_TS1_11Tq  /*!< 11 time quantum */
#define CAN_BS1_12tq                FL_CAN_TS1_12Tq  /*!< 12 time quantum */
#define CAN_BS1_13tq                FL_CAN_TS1_13Tq  /*!< 13 time quantum */
#define CAN_BS1_14tq                FL_CAN_TS1_14Tq  /*!< 14 time quantum */
#define CAN_BS1_15tq                FL_CAN_TS1_15Tq  /*!< 15 time quantum */
#define CAN_BS1_16tq                FL_CAN_TS1_16Tq  /*!< 16 time quantum */

#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16tq)
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_2
  * @{
  */

#define CAN_BS2_1tq                 FL_CAN_TS2_1Tq  /*!< 1 time quantum */
#define CAN_BS2_2tq                 FL_CAN_TS2_2Tq  /*!< 2 time quantum */
#define CAN_BS2_3tq                 FL_CAN_TS2_3Tq  /*!< 3 time quantum */
#define CAN_BS2_4tq                 FL_CAN_TS2_4Tq  /*!< 4 time quantum */
#define CAN_BS2_5tq                 FL_CAN_TS2_5Tq  /*!< 5 time quantum */
#define CAN_BS2_6tq                 FL_CAN_TS2_6Tq  /*!< 6 time quantum */
#define CAN_BS2_7tq                 FL_CAN_TS2_7Tq  /*!< 7 time quantum */
#define CAN_BS2_8tq                 FL_CAN_TS2_8Tq  /*!< 8 time quantum */

#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8tq)

void can0_de_init(void);
void can0_init(void);

#endif   /*  __CAN_H__  */






