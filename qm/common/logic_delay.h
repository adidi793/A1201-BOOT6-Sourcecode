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
#ifndef _LOGIC_DELAY_H
#define _LOGIC_DELAY_H

#include "common_types.h"

#include "logic_time.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

#if 0
typedef enum
{


};
#else

#define LOGIC_DELAY_STATUS_STOP  0u
#define LOGIC_DELAY_STATUS_RUNNING  1u
#define LOGIC_DELAY_STATUS_DELETE  2u



#endif

typedef struct
{
	logic_timer_t get_offset_time;
	logic_timer_t current_time;
	logic_timer_t delay_time;
	u8  state;
	/*  bool_t abort_en;*/
	void (*p_func)(void) ;
}logic_delay_t;





/* 時間以邏輯定時器週期為基準*/
void logic_delay_time_start(logic_delay_t* delay_cb, void (*p_fun)(void),logic_timer_t delay_tick);

/* 超時返回 TRUE, 否則返回FALSE;*/
u8 logic_delay_time_out(logic_delay_t* delay_cb);

void logic_delay_time_abort(logic_delay_t* delay_cb);


/* 時間以被調的任務週期為基準,  tick_counts 倍 , 不適用於任務週期不確定的任務調用*/
void logic_delay_tick_start(logic_timer_t *tick_counter, logic_timer_t tick_counts);
bool logic_delay_tick_time_out(logic_timer_t *tick_counter);
void logic_delay_tick_abort(logic_timer_t *tick_counter);






#endif  /* _LOGIC_DELAY_H */




