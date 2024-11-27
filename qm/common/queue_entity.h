/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief 消息隊列
 							順序循環消息隊列
					約定:1.隊列尾指針為實際末尾無素的下一位置.   2.浪費一個元素空間,以"隊列頭指
						針在隊列尾指針的下一位置(指環狀的下一位置)上作為隊列呈"滿"狀態的標誌.
						seq為空循環隊列的判定條件為:頭尾重合      seq.front==seq.rear
						seq為滿循環隊列的判定條件為:隊尾趕上隊頭  (seq.rear+1)%MAXSIZE==seq.front
				    注: 申請的隊列空間,buf至少2個.
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


#ifndef _QUEUE_ENTITY_H_
#define _QUEUE_ENTITY_H_


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

#include "common_types.h"
#include "queue_entity_cfg.h"


typedef enum {
    QUEUE_ERRO = 0u,
    QUEUE_OK = !QUEUE_ERRO
} QUEUE_OPERATE_FLAG_E;

typedef enum {
    QUEUE_FALSE = 0u,
    QUEUE_TRUE = !QUEUE_FALSE
} QUEUE_RIGHT_FLAG_E;


typedef enum {
    QUEUE_MINUS = 0u,  /* 元素減少發送次數 */
    QUEUE_DEL = !QUEUE_MINUS     /* 元素直接刪除 */
} QUEUE_DEL_FLAG_E;



typedef u8 sequential_queue_elem;

typedef struct {
    u16 length;  /*隊列長度*/
    sequential_queue_elem *base; //指向 can1_rx_qbuf，包含handle，dlc，canId，frame[CANIF_MAX_CAN_FRAME_SIZE]
    u8 unit_sizes;  /*can1_rx_qbuf的字節長度*/
    u16 front;  /*頭指針*/
    u16 rear;   /*尾指針*/
} sequential_queue_s;





/**
* @brief  初始化一個空順序隊列
* @param  input:  seq : 順序隊列；pdata 隊列元素的數據; length隊列長度,unit_sizes 元素的字節長度;
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
void queue_init(sequential_queue_s *seq, sequential_queue_elem *pdata, u8 length, u8 unit_sizes);




/**
* @brief  清空隊列
* @param  input:  seq : 順序隊列；
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
void queue_clear(sequential_queue_s *seq);




/**
 * @brief  判斷隊列是否為空;
 * @param  input:  seq : 順序隊列；
 * @param  output:
 * @return   QUEUE_RIGHT_FLAG_E: 若隊列seq為空隊列，則返回ture 否則返回false
 * @calls
 * @calls by
 * @others
 */
QUEUE_RIGHT_FLAG_E queue_get_empty(sequential_queue_s seq);


QUEUE_OPERATE_FLAG_E queue_get_element(sequential_queue_s seq, u8 i, sequential_queue_elem *element);

/**
 * @brief  求出隊列不為空元素的數目;
 * @param  input:  seq : 順序隊列；
 * @param  output:
 * @return   長度數目
 * @calls
 * @calls by
 * @others
 */
u16 queue_get_length(sequential_queue_s seq);



/**
* @brief  若隊列不為空，則用e返回seq的隊頭元素
* @param  input: seq : 順序隊列列
* @param  output:  element: 將頭元素數據傳給外部的變量;
* @return   是否操作成功
* @calls
* @calls by
* @others
*/

QUEUE_OPERATE_FLAG_E queue_get_head(sequential_queue_s *seq, sequential_queue_elem *element);



/**
 * @brief   若隊列未滿，則插入元素e為seq新的隊尾元素
 * @param  input: seq : 順序隊列；
 * @param  output:  element: 將頭元素數據傳給外部的變量;
 * @return   是否操作成功
 * @calls
 * @calls by
 * @others
 */
QUEUE_OPERATE_FLAG_E queue_add_element(sequential_queue_s *seq, const sequential_queue_elem *element);







/**
* @brief   若隊列未滿，則刪除元素e為seq新的隊尾元素
* @param  input: seq : 順序隊列；
* @param  output:  element: 將頭元素數據傳給外部的變量;
* @return   是否操作成功
* @calls
* @calls by
* @others
*/
QUEUE_OPERATE_FLAG_E queue_del_element(sequential_queue_s *seq, sequential_queue_elem *element);



#if 1

/**
* @brief   若隊列不空，則刪除Q中的某一個元素
* @param  input: seq : 順序隊列；i表示元素地址; fdo 刪除的方法
* @param  output:  element: 將頭元素數據傳給外部的變量;
* @return   是否操作成功
* @calls
* @calls by
* @others
*/
QUEUE_OPERATE_FLAG_E queue_del_one_element_by_flag(sequential_queue_s *seq, u16 i, sequential_queue_elem *element, QUEUE_DEL_FLAG_E fdo);





QUEUE_OPERATE_FLAG_E queue_del_one_element(sequential_queue_s *seq, u16 i, sequential_queue_elem *element);


#endif







#endif






