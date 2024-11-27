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



#ifndef __WDOG_H__
#define __WDOG_H__

#include  "common_types.h"
#include  "wdog_cfg.h"


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/


void wdog_init(void);
void wdog_task(void);

void wdt_feed_windows(void);

void wdt_enable(void);
void wdt_disable(void);


#endif   /*  __WDOG_APP_H__  */






