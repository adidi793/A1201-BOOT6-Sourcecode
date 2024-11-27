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

/* clockMan1. */

#include "clk_cfg.h"


uint32_t SYSCLKSELErrorState = 0;


/**
  * @brief  CMU时钟中断
  * @param  void
  * @retval void
*/
void FDET_IRQHandler(void)
{

    /* 时钟选择中断处理 */
    if((FL_CMU_IsEnabledIT_SYSCLKSELError() == 0x01UL)
            && (FL_CMU_IsActiveFlag_SYSCLKSELError() == 0x01UL))
    {
        /* 清除时钟选择错误标志 */
        FL_CMU_ClearFlag_SYSCLKSELError();

        SYSCLKSELErrorState = 1;
    }
    if((FL_CMU_IsEnabledIT_XTHFFail() == 0x01UL)
            && (FL_CMU_IsActiveFlag_XTHFFail() == 0x01UL))
    {
        /* 清除高频晶体停振标志 */
        FL_CMU_ClearFlag_XTHFFail();

        /*应用处理逻辑*/
    }
}


/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
