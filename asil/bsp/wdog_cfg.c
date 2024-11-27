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

#include "wdog.h"
#include "logic_time.h"
#include "gpio.h"


/**
 * @brief
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */


typedef struct
{
	u16 count[6];
}wdog_cfg_s;
static wdog_cfg_s g_wdog_cfg ={0};

/**
 * @brief  應用
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
void wdog_irq_callback(void *device, uint32_t wpara, uint32_t lpara)
{
	g_wdog_cfg.count[0]++;
}


#if WDT_OPERATION_MODE == WDT_OPERATION_NORMAL

/*! watchdog1 configuration structures */
const FL_IWDT_InitTypeDef g_wdg_cfg =
{
	FL_IWDT_PERIOD_8000MS, /* 最长溢出时间 */
	0u,	/* 默认不使用窗口 */
}
;

#elif WDT_OPERATION_MODE == WDT_OPERATION_WINDOWS

/*! watchdog1 configuration structures */
const FL_IWDT_InitTypeDef g_wdg_cfg =
{
	FL_IWDT_PERIOD_8000MS, /* 最长溢出时间 */
	1u,	/* 默认不使用窗口 */
};


#endif



/*end line*/
