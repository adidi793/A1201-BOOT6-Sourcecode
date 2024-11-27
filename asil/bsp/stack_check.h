/**
	******************************************************************************
	* @file    boot_app.h
	* @author  Mingyea Asamiya
	* @version V1.0.0
	* @date    2017-01-19
	* @brief   測試用
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





#ifndef __STACK_CHECK_H__
#define __STACK_CHECK_H__




#include "common_types.h"
#include "stack_check_cfg.h"


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/


MEMORY_MAP_ROM_FOR_ASIL void stack_check_init(void);
MEMORY_MAP_ROM_FOR_ASIL void stack_check_task(void);
MEMORY_MAP_ROM_FOR_ASIL u8 stack_check_get_test_value(u8 id);



#endif   /*  __BOOT_APP_H__  */






