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
#ifndef __TIM_CFG_H__
#define __TIM_CFG_H__



#include  "common_types.h"
#include  "Cpu.h"


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

#if 1
#define TIME_SR_ALLOC() //CPU_SR_ALLOC()
#define TIME_CRITICAL_ENTER() //ENTER_CRITICAL()
#define TIME_CRITICAL_EXIT() //EXIT_CRITICAL()
#else
#define TIME_SR_ALLOC()  //CPU_SR_ALLOC()
#define TIME_CRITICAL_ENTER() INT_SYS_DisableIRQGlobal()
#define TIME_CRITICAL_EXIT()  INT_SYS_EnableIRQGlobal()
#endif
/*****************普通定時器*******************/

#define TIMER0_CHANNEL_ID_0  TIMER_CHANNEL0
#define TIMER1_CHANNEL_ID_1  TIMER_CHANNEL1


extern const FL_ATIM_InitTypeDef  atimer0_Config0 ;


#endif   /* __TIM_CFG_H__ */
