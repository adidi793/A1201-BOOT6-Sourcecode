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



#ifndef __LOGIC_TIME_H_
#define __LOGIC_TIME_H_


#include "common_types.h"


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/


typedef u16 logic_timer_t;

void  logic_time_tick_init (void);
void  logic_time_tick_count(void);
logic_timer_t  logic_time_get_tick(void);



#endif  /* __LOGIC_TIME_H_ */




