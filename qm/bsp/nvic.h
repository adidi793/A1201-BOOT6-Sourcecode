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
#ifndef __NVIC_H__
#define __NVIC_H__

#include "common_types.h"
#include "nvic_cfg.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/


void nvic_init(void);
void nvic_set_vector_table(uint32_t NVIC_VectTab, uint32_t Offset);

#if 0
void nvic_disable_primask(void);
void nvic_enable_primask(void);
void nvic_disable_faultmask(void);
void nvic_enable_faultmask(void);
#endif


#endif   /* __NVIC_H__ */




