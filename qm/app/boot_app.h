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




#ifndef __BOOT_APP_H__
#define __BOOT_APP_H__




#include "common_types.h"
#include "boot_app_cfg.h"


/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/


void boot_app_init(void);
void boot_app_task(void);

void  boot_app_jump_to_app(void);

extern void Boot_ResetProcess(void);

extern void Boot_ReprogramCallBack(void);

extern void Boot_ResetMCUCallBack(void);

extern u8 Boot_FingerprintCallBack(u8 * readBuf,u32 readSize);
void Boot_Reprogram_only_writeram(void);

void boot_app_test(void);



//片区开头的标志
//extern  const u8 AppValid[16]  ;  // APP_VALID_FLAG1_ADDR




#endif   /*  __BOOT_APP_H__  */






