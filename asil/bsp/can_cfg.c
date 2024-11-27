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
#include "can.h"


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
volatile uint8_t TimeOutFlag;               /* 定时器超时标志 */
#define TQ_x_7    CAN_SJW_1tq,CAN_BS1_5tq,CAN_BS2_1tq
#define TQ_x_8    CAN_SJW_1tq,CAN_BS1_5tq,CAN_BS2_2tq
#define TQ_x_9    CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_2tq
#define TQ_x_10   CAN_SJW_1tq,CAN_BS1_7tq,CAN_BS2_2tq
#define TQ_x_12   CAN_SJW_1tq,CAN_BS1_9tq,CAN_BS2_2tq
#define TQ_x_13   CAN_SJW_1tq,CAN_BS1_10tq,CAN_BS2_2tq
#define TQ_x_14   CAN_SJW_1tq,CAN_BS1_11tq,CAN_BS2_2tq
#define TQ_x_15   CAN_SJW_1tq,CAN_BS1_12tq,CAN_BS2_2tq  //good timing
#define TQ_x_16   CAN_SJW_1tq,CAN_BS1_13tq,CAN_BS2_2tq   //good timing
#define TQ_x_17   CAN_SJW_1tq,CAN_BS1_14tq,CAN_BS2_2tq   //good timing
#define TQ_x_18   CAN_SJW_1tq,CAN_BS1_15tq,CAN_BS2_2tq   //good timing
#define TQ_x_19   CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_2tq   //good timing
#define TQ_x_20   CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_3tq   //good timing
#define TQ_x_21   CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_4tq
#define TQ_x_22   CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_5tq
#define TQ_x_23   CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_6tq
#define TQ_x_24   CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_7tq
#define TQ_x_25   CAN_SJW_1tq,CAN_BS1_16tq,CAN_BS2_8tq

const tCAN_BaudRate  CAN_BaudRateInitTab[] =       // CLK=8MHz
{
	{TQ_x_8, 0},     // 1M
	{TQ_x_9, 0},     // 900K
	{TQ_x_10, 0},    // 800K
	{TQ_x_12, 0},    // 666K
	{TQ_x_13, 0},    // 600K
	{TQ_x_16, 0},    // 500K
	{TQ_x_20, 0},    // 400K
	{TQ_x_9, 2},     // 300K
	{TQ_x_8, 3},     // 250K
	{TQ_x_18, 1},    // 225K
	{TQ_x_10, 3},    // 200K
	{TQ_x_10, 4},    // 160K
	{TQ_x_18, 2},    // 150K
	{TQ_x_8, 6},     // 144K
	{TQ_x_16, 3},    // 125K
	{TQ_x_13, 9},    // 120K
	{TQ_x_20, 3},    // 100K
	{TQ_x_22, 3},    // 90K
	{TQ_x_20, 4},    // 80K
	{TQ_x_18, 5},    // 75K
	{TQ_x_19, 6},    // 60K
	{TQ_x_20, 7},    // 50K
	{TQ_x_20, 9},    // 40K
	{TQ_x_19, 13},   // 30K
	{TQ_x_20, 19},   // 20K
};


const uint32_t g_can0_baudrate = 500000;



