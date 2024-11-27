/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-17 09:14:34
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-07-08 13:46:41
 * @FilePath: \A12 BOOT1\qm\bsp\tim_cfg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
#include "common_cfg.h"
#include "Cpu.h"
#include "tim.h"
#include "gpio.h"

#include "logic_delay.h"
#include "logic_time.h"
#include "random_pseudo.h"
#include "bl_timer.h"



const FL_ATIM_InitTypeDef  atimer0_Config0 =
{
    FL_CMU_ATIM_CLK_SOURCE_APBCLK,  /* 时钟源选择APB2 */
    479u,                          /* 分频系数480 */ // 479   48MHZ/480=
    FL_ATIM_COUNTER_DIR_UP,         /* 向上计数 */
    99u,                            /* 自动重装载值1000 */  //  1 / (48MHZ/480) * 100 = 0.001 s
    FL_ATIM_CLK_DIVISION_DIV1,      /* 死区和滤波设置 */
    0u, /* 重复计数 */
    FL_DISABLE,          /* 自动重装载禁止preload */
};


/* 以下四个全局变量为具体计数 */
uint16_t g_timer0Count = 0u; ///<timer0 Count 每10ms计数加1

typedef struct
{
    u16 count[4];
}tim_cfg_s;

tim_cfg_s g_tim_cfg = {0u};



/**
  * @brief  ATIM中断函数
  * @param  void
  * @retval void
  */
void ATIM_IRQHandler()
{
    if(FL_ATIM_IsEnabledIT_Update(ATIM) && FL_ATIM_IsActiveFlag_Update(ATIM))
    {
        FL_ATIM_ClearFlag_Update(ATIM);

        /* 处理更新事件 */
        Tim_tick_count();
        logic_time_tick_count();
        g_tim_cfg.count[0]++;
        if(g_tim_cfg.count[0]>=1000u)
        {
            g_tim_cfg.count[0]=0u;
            g_tim_cfg.count[1]++;
        }
        random_input_count();
    }
}






