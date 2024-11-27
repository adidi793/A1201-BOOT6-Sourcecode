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


#include "clk.h"
#include "gpio.h"
#include "queue_entity.h"
#include "logic_time.h"
#include "wdog.h"
#include "fm33lg0xx_fl.h"
//#include "rcm_check.h"


typedef struct
{
	u8 state;
	u16 count;
	#ifdef WDOG_DEBUG_EN
	u8 test[3];
	#endif
}wdt_s;
static wdt_s g_wdt;

typedef struct
{
	u16 count;
	u32 flag_self_check;
}wdt_check_s;
static wdt_check_s g_wdt_check;



/**
 * @brief  初始化
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
void wdog_init(void)
{
	#if 0
	#if WDT_OPERATION_MODE == WDT_OPERATION_NORMAL  || WDT_OPERATION_MODE == WDT_OPERATION_WINDOWS
	#ifdef WDOG_DEBUG_EN
		//TEST1_ON();
		g_wdt.test[0]++;
		//TEST1_OFF();
		g_wdt.test[0]++;
		//TEST1_ON();
		g_wdt.test[0]++;
		//TEST1_OFF();
		//TEST1_ON();
		g_wdt.test[0]++;
		//TEST1_OFF();
		g_wdt.test[0]++;
		//TEST1_ON();
		g_wdt.test[0]++;
		//TEST1_OFF();
	#endif
	if( (rcm_get_reset_source() & RCM_SRS_WDOG32K_MASK) || (rcm_get_reset_source() & RCM_SRS_WDOG_MASK) )
	{
		if(WDT_SELF_CHECK_DATA == g_wdt_check.flag_self_check )
		{
			//code
		}
		else
		{
			//
		}
	}
	else
	{
		g_wdt_check.flag_self_check = ~WDT_SELF_CHECK_DATA;
	}
	g_wdt.count=0u;
	g_wdt.state=1u;
	//WDG_Init(&g_wdg_cfg);
	wdt_disable();
	WDG_Init(&g_wdg_cfg);
	#endif
	#else
	#if WDT_OPERATION_MODE == WDT_OPERATION_NORMAL  || WDT_OPERATION_MODE == WDT_OPERATION_WINDOWS
	///FL_IWDT_Init(IWDT, &g_wdg_cfg);
	#endif
	#endif
       
     
}



void wdog_task(void)
{
	#if 1
		#if WDT_OPERATION_MODE == WDT_OPERATION_NORMAL
		#if 0//def WDOG_DEBUG_EN
		TEST1_TOGGLE();
		g_wdt.test[0]++;
		TEST1_TOGGLE();
		g_wdt.test[0]++;
		TEST1_TOGGLE();
		g_wdt.test[0]++;
		TEST1_TOGGLE();
		#endif

    	FL_IWDT_ReloadCounter(IWDT);

	#endif

	#endif

}



/*
 * @brief  当使用窗口看门狗时的喂狗
 * @param
 * @return
 */
void wdt_feed_windows(void)
{
	#if 1
		#if WDT_OPERATION_MODE  == WDT_OPERATION_WINDOWS

			#if 0//def WDOG_DEBUG_EN
			TEST1_TOGGLE();
			g_wdt.test[0]++;
			TEST1_TOGGLE();
			g_wdt.test[0]++;
			TEST1_TOGGLE();
			g_wdt.test[0]++;
			TEST1_TOGGLE();
			#endif
			g_wdt_check.count++;
			g_wdt.count++;
			if(g_wdt.count>=WDT_WINDOWS_TIMEOUT)
			{
				g_wdt.count=0u;
		    	FL_IWDT_ReloadCounter(IWDT);
			}
			#ifdef WDT_ENABLE_SELF_CHECK
			else if(g_wdt.count >= (WDT_WINDOWS_TIMEOUT/2u))
			{
				if(g_wdt_check.flag_self_check != WDT_SELF_CHECK_DATA)
				{
					g_wdt_check.flag_self_check = WDT_SELF_CHECK_DATA;
			    	FL_IWDT_ReloadCounter(IWDT);
				}
			}
			#endif

		#endif
	#endif
}


void wdt_enable(void)
{
	#if 0
   uint32_t CS0 = WDG->CS0;
   CS0 |= WDG_CS0_EN_Msk;
   WDG_Unlock();
   WDG_SetCS0(CS0);
   WDG_Feed();   
   #endif
}/* End of function wdt_enable*/

/*****************************************************************************
|Prototype         : void wdt_disable(void)
|Description      : 去能看门狗
*****************************************************************************/
void wdt_disable(void)
{
	#if 0
   uint32_t CS0 = WDG->CS0;
   CS0 &= ~WDG_CS0_EN_Msk;
   WDG_Unlock();
   WDG_SetCS0(CS0);
   #endif
}



/*end line*/
