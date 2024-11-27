
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


#include "tim.h"
#include "Cpu.h"



//初始化
void tim_init(void)
{
    //TIMER_Init(TIMER0_CHANNEL_ID_0, &timer0_Config0);    //TIMER0配置初始化
    //TIMER_Init(TIMER1_CHANNEL_ID_1, &timer1_Config0);    //TIMER0配置初始化
    FL_ATIM_Init(ATIM, (FL_ATIM_InitTypeDef*)&atimer0_Config0);

    NVIC_DisableIRQ(ATIM_IRQn);
    NVIC_SetPriority(ATIM_IRQn, 0); /* 中断优先级配置 */
    NVIC_EnableIRQ(ATIM_IRQn);

    FL_ATIM_ClearFlag_Update(ATIM); /* 清除计数器中断标志位 */
    FL_ATIM_EnableIT_Update(ATIM);  /* 开启计数器中断 */

    FL_ATIM_Enable(ATIM);           /* 使能定时器 */
}

void tim_de_init(void)
{
	FL_ATIM_DeInit(ATIM);
}

/**
	* @brief 任務
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void tim_task(void)
{

}


///////////////////////////







