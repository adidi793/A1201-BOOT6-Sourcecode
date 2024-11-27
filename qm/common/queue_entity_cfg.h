/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief  消息隊列擴展版本;
			和舊版本的不同之處:
			1. 沒有空閑buf,全部利用;
			2. 加了可覆蓋的功能.
			3. 每個函數加了合法的判斷.
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

/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/

#ifndef  __QUEUE_ENTITY_CFG__
#define  __QUEUE_ENTITY_CFG__

#include "common_types.h"
#include "Cpu.h"


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/




/*#define QUEUE_MOD_ENABLE 1 */ /*開啟求余運算*/
#define QUEUE_MOD_ENABLE 1  /*開啟求余運算*/


//#define QUEUE_SWITCH_CONTEXT_ENABLE  1u 	 /*隊列操作時開啟上下文切換*/


#ifdef QUEUE_SWITCH_CONTEXT_ENABLE
#define QUEUE_CPU_SR_ALLOC()  //CPU_SR_ALLOC()
#define QUEUE_CPU_CRITICAL_ENTER() ENTER_CRITICAL()
#define QUEUE_CPU_CRITICAL_EXIT()  EXIT_CRITICAL()
#else
#define 	QUEUE_CPU_SR_ALLOC()
#define 	QUEUE_CPU_CRITICAL_ENTER()
#define 	QUEUE_CPU_CRITICAL_EXIT()
#endif




/* #define TEST_MODEL_QUEUE  1 */	/*測試消息隊列*/
/*#define TEST_MODEL_QUEUE  1*/ 	/*測試消息隊列*/
#ifdef TEST_MODEL_QUEUE

void queue_test_task(void);

#endif




#endif                                                  /*  __QUEUE_EXT_CFG__                  */

