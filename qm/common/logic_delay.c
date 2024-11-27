/**
  ******************************************************************************
  * @file
  * @author
  * @version
  * @date
  * @brief   定時器任務模塊
					定時器分為緊急和非緊急兩種類型;每一種類型分別建立隊列緩存;
					先只對非緊急定時器; 有關緊急的另外處理;
  * @function List:
  ******************************************************************************
  * @attention
  *
  *
  * <h2><center>&copy; COPYRIGHT 2021 </center></h2>
  ******************************************************************************
  * @History: 整理了定時器,週期性定時器重新運行時,加了防止遺漏任務的機制,添加了定時器的簡易模式.
  * @Author:  
  * @Data:
  * @Version:
*/
#include "common_types.h"
#include "Cpu.h"
#include "logic_time.h"
#include "logic_delay.h"






/**
  * @brief  初始化
  * @param input none:
  * @param input none:
  * @param  output:
  * @retval None
 * @calls
 * @calls by
 * @others
  */
void logic_delay_time_start(logic_delay_t* delay_cb, void (*p_fun)(void),logic_timer_t delay_tick)
{

	delay_cb->current_time = logic_time_get_tick();
	delay_cb->delay_time = delay_tick;
	delay_cb->p_func = p_fun;
	delay_cb->state = (u8)FALSE;

	/*delay_cb->abort_en = FALSE;*/
}



/**
  * @brief  初始化
  * @param input none
  * @param input none:
  * @param  output:
  * @retval None
 * @calls
 * @calls by
 * @others
  */
u8 logic_delay_time_out(logic_delay_t* delay_cb)
{
    /*
    if(delay_cb->abort_en)
    return TIME_ABORT;
    */
	if(delay_cb->state == (u8)FALSE)
	{
		delay_cb->get_offset_time = logic_time_get_tick() - delay_cb->current_time;
		if ((delay_cb->get_offset_time) >= delay_cb->delay_time)
		{
			if(delay_cb->p_func != (void*)NULL_POINT)
			{
				delay_cb->p_func();
			}
			logic_delay_time_abort(delay_cb);
			/*return TRUE;*/
		}
		else
		{

		}
	}
	else
	{
	}
	return delay_cb->state; /*return TIME_RUNNING;*/
}



/**
  * @brief  初始化
  * @param input none:
  * @param input none:
  * @param  output:
  * @retval None
 * @calls
 * @calls by
 * @others
  */
void logic_delay_time_abort(logic_delay_t *delay_cb)
{
	/*delay_cb->abort_en = TRUE;*/
	delay_cb->p_func = (void*)NULL_POINT;
	delay_cb->state = (u8)TRUE;
}




/**
  * @brief  初始化
  * @param input none:
  * @param input none:
  * @param  output:
  * @retval None
 * @calls
 * @calls by
 * @others
  */
void logic_delay_tick_start(logic_timer_t *tick_counter, logic_timer_t tick_counts)
{
	*tick_counter = tick_counts;
}




/**
  * @brief  初始化
  * @param input none:
  * @param input none:
  * @param  output:
  * @retval None
 * @calls
 * @calls by
 * @others
  */
bool logic_delay_tick_time_out(logic_timer_t *tick_counter)
{
    bool l_result;

    if(*tick_counter !=0u)
    {
        (*tick_counter)--;
        l_result = FALSE;  /*PRQA S  4420*/
    }
    else
    {
        l_result = FALSE;  /*PRQA S  4420*/
    }

    return l_result;

}





/**
  * @brief  初始化
  * @param input none:
  * @param input none:
  * @param  output:
  * @retval None
 * @calls
 * @calls by
 * @others
  */
void logic_delay_tick_abort(logic_timer_t* tick_counter)
{
	*tick_counter = 0u;
}








