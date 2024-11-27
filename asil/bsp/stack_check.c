/**
	******************************************************************************
	* @file    stack_check.c
	* @author  Mingyea Asamiya
	* @version V1.0.0
	* @date    2021-10-19
	* @brief
	* @function List:
	******************************************************************************
	* @attention
	*
	*
	* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
	******************************************************************************
	* @History:
	* @Author:
	* @Data:
	* @Version:
	*/


#include "stack_check.h"
#include "common_memory.h"
#include "rcm_check.h"

typedef struct
{
	u8 time_count;
	#ifdef STACK_CHECK_DEBUG_EN
	u8 test[3];
	#endif
}stack_check_s;
MEMORY_MAP_RAM_FLAG_NOINIT stack_check_s g_stack_check ={0u};


volatile u8 g_stack_check_addr[RAM_STACK_CHECK_MAX_LEN] __attribute__((section(".STACK_CHECK_ADDR"))) =
{
	RAM_STACK_CHECK_VALUE_BUF0,
	RAM_STACK_CHECK_VALUE_BUF1,
	RAM_STACK_CHECK_VALUE_BUF2,
	RAM_STACK_CHECK_VALUE_BUF3,
	RAM_STACK_CHECK_VALUE_BUF0,
	RAM_STACK_CHECK_VALUE_BUF1,
	RAM_STACK_CHECK_VALUE_BUF2,
	RAM_STACK_CHECK_VALUE_BUF3,
	RAM_STACK_CHECK_VALUE_BUF0,
	RAM_STACK_CHECK_VALUE_BUF1,
	RAM_STACK_CHECK_VALUE_BUF2,
	RAM_STACK_CHECK_VALUE_BUF3,
	RAM_STACK_CHECK_VALUE_BUF0,
	RAM_STACK_CHECK_VALUE_BUF1,
	RAM_STACK_CHECK_VALUE_BUF2,
	RAM_STACK_CHECK_VALUE_BUF3,
};

/**
* @brief
* @param  input:
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
MEMORY_MAP_ROM_FOR_ASIL void stack_check_init(void)
{
	#if 0
	u8 i;
	for(i=0;i<RAM_STACK_CHECK_MAX_LEN;i++)
	{
		g_stack_check_addr[i] = g_stack_check_fix_value[i];
	}
	#endif
	common_memory_clear((u8*)&g_stack_check,sizeof(g_stack_check));  //memcpy
}

/**
* @brief
* @param  input:
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
MEMORY_MAP_ROM_FOR_ASIL void stack_check_task(void)
{
	#if 0
	u8 i;
	u8 l_temp=0u;

	for(i=0;i<RAM_STACK_CHECK_MAX_LEN;i++)
	{
		if(g_stack_check_addr[i] != g_stack_check_fix_value[i])
		{
			l_temp =1u;
			break;
		}
	}

	if(l_temp!=0u)
	{
		//执行复位
		//SystemSoftwareReset();
		#ifdef STACK_CHECK_DEBUG_EN
		g_stack_check.test[1]++;
		#endif
	}
	else
	{
		#ifdef STACK_CHECK_DEBUG_EN
		g_stack_check.test[0]++;
		#endif
	}
	#else
	g_stack_check.time_count++;
	if(g_stack_check.time_count>=STACK_CHECK_TASK_TIME)
	{
		g_stack_check.time_count=0u;
		if( (g_stack_check_addr[0] == RAM_STACK_CHECK_VALUE_BUF0)
			&& (g_stack_check_addr[1] == RAM_STACK_CHECK_VALUE_BUF1)
			&& (g_stack_check_addr[2] == RAM_STACK_CHECK_VALUE_BUF2)
			&& (g_stack_check_addr[3] == RAM_STACK_CHECK_VALUE_BUF3)
			&& (g_stack_check_addr[4] == RAM_STACK_CHECK_VALUE_BUF0)
			&& (g_stack_check_addr[5] == RAM_STACK_CHECK_VALUE_BUF1)
			&& (g_stack_check_addr[6] == RAM_STACK_CHECK_VALUE_BUF2)
			&& (g_stack_check_addr[7] == RAM_STACK_CHECK_VALUE_BUF3)
			&& (g_stack_check_addr[8] == RAM_STACK_CHECK_VALUE_BUF0)
			&& (g_stack_check_addr[9] == RAM_STACK_CHECK_VALUE_BUF1)
			&& (g_stack_check_addr[10] == RAM_STACK_CHECK_VALUE_BUF2)
			&& (g_stack_check_addr[11] == RAM_STACK_CHECK_VALUE_BUF3)
			&& (g_stack_check_addr[12] == RAM_STACK_CHECK_VALUE_BUF0)
			&& (g_stack_check_addr[13] == RAM_STACK_CHECK_VALUE_BUF1)
			&& (g_stack_check_addr[14] == RAM_STACK_CHECK_VALUE_BUF2)
			&& (g_stack_check_addr[15] == RAM_STACK_CHECK_VALUE_BUF3)
			)
		{
			#ifdef STACK_CHECK_DEBUG_EN
			g_stack_check.test[0]++;
			#endif
		}
		else
		{
			//执行复位
			#ifdef STACK_CHECK_DEBUG_EN
			g_stack_check.test[1]++;
			#endif
			rcm_check_set_reset_flag(RCM_LOGIC_FLAG_STACK_CHECK);
			SystemSoftwareReset();
		}
	}
	#endif
}



MEMORY_MAP_ROM_FOR_ASIL u8 stack_check_get_test_value(u8 id)
{
	u8 l_temp;
	if(id < 3u)
	{
		#ifdef STACK_CHECK_DEBUG_EN
		l_temp =  g_stack_check.test[id];
		#else
		l_temp =  0;
		#endif
	}
	else
	{
		l_temp =  0;
	}
	return l_temp;
}

