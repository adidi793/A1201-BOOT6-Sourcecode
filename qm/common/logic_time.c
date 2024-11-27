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
#include "logic_time.h"


//#define LOGICTIME_CONTEXT_ENABLE  1u 	 /*隊列操作時開啟上下文切換*/


#ifdef LOGICTIME_CONTEXT_ENABLE
#define 	LOGICTIME_SR_ALLOC()    LOGICTIME_SR_ALLOC()
#define 	LOGICTIME_CRITICAL_ENTER()    LOGICTIME_CRITICAL_ENTER()
#define 	LOGICTIME_CRITICAL_EXIT()    LOGICTIME_CRITICAL_EXIT()
#else
#define 	LOGICTIME_SR_ALLOC()
#define 	LOGICTIME_CRITICAL_ENTER()
#define 	LOGICTIME_CRITICAL_EXIT()
#endif


static logic_timer_t g_logic_time_tick;


/**
 * @brief      	初始化定時器
 * @param
 * @return
 */
void  logic_time_tick_init (void)
{

	LOGICTIME_SR_ALLOC();	/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/  /*PRQA S 2981*/

	LOGICTIME_CRITICAL_ENTER();		/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/
	g_logic_time_tick = 0u;
	LOGICTIME_CRITICAL_EXIT();	/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/


}

/**
 * @brief      定時器中斷處理; 計數;;   頻率 CFG_SYSTICK_RATE_HZ
 * @param
 * @return
 */
void  logic_time_tick_count(void)
{

	LOGICTIME_SR_ALLOC();	/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/  /*PRQA S 2981*/

	LOGICTIME_CRITICAL_ENTER();		/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/
	g_logic_time_tick ++;                                             /* Increment the current time                        */
	LOGICTIME_CRITICAL_EXIT();		/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/


}


/**
 * @brief   讀取邏輯定時器的當前計數
 * @param
 * @return
 */
logic_timer_t  logic_time_get_tick(void)
{
	logic_timer_t tmp;

	LOGICTIME_SR_ALLOC();		/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/  /*PRQA S 2981*/

	LOGICTIME_CRITICAL_ENTER();		/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/
	tmp = g_logic_time_tick;                                             /* Increment the current time                        */
	LOGICTIME_CRITICAL_EXIT();		/*PCLINT_ERROS*/ /*lint !e717  */ /*do while*/


	return tmp ;   /* Increment the current time                        */

}






