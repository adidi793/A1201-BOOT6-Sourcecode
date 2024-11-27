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


#ifndef __WDOG_CFG_H__
#define __WDOG_CFG_H__

#include "Cpu.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

//#define WDT_ENABLE_SELF_CHECK 1  /*开启自检*/
#define WDT_SELF_CHECK_DATA  0x5e715896


#define WDT_TIME_UNIT  1 // 1MS任务里跑


#define WDT_OPERATION_NORMAL  0u
#define WDT_OPERATION_WINDOWS  1u
#define WDT_OPERATION_DISABLE  2u
#define WDT_OPERATION_MODE   WDT_OPERATION_NORMAL

#if 0
#define WDT_PRE_256_EN  1 // 1开启，256分频  0不开启   --切記不能用枚舉

#if 0   //在中断时，0.5ms
#define WDT_CR_VALUE  2u
#define WDT_WINDOWS_TIMEOUT  27u //104
#else
#define WDT_CR_VALUE  (1u/WDT_TIME_UNIT)
#define WDT_WINDOWS_TIMEOUT  (14u/WDT_TIME_UNIT)
#endif

#if WDT_PRE_256_EN == 1
#define WDT_PRE_256_VALUE  ENABLE
#define WDG_CLK_S(n)     (APB_BUS_FREQ/256u * n -1u)
#define WDG_CLK_MS(n)     ( ((APB_BUS_FREQ/1000u) * n)/256u  -1u)
#else
#define WDT_PRE_256_VALUE  DISABLE
#define WDG_CLK_S(n)     (APB_BUS_FREQ * n -1u)
#define WDG_CLK_MS(n)     ( (APB_BUS_FREQ/1000u) * n -1u)
#endif

#endif

void wdog_irq_task(void);



//#define WDOGx_IRQn WDOG_EWM_IRQn


/*! @brief Device instance number */
#define INST_WATCHDOG1 0U

extern const FL_IWDT_InitTypeDef g_wdg_cfg ;


/*#define WWDG_INTERRUPT_ENABLE 1  */ /*使能中斷*/

#define WDOG_DEBUG_EN //测试宏


#endif   /*  __WDOG_APP_CFG_H__  */






