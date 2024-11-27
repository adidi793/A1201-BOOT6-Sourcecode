/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2024-09-14 11:59:36
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-09-30 10:31:37
 * @FilePath: \A1201 BOOT5\qm\bsp\tim.h
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



#ifndef __TIM_H__
#define __TIM_H__


#include "common_types.h"
#include "tim_cfg.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/




void BSTIM16_Init(void);
void tim_init(void);
void tim_de_init(void);

void tim_task(void);





#endif   /*  __APP_TIM_H__  */






