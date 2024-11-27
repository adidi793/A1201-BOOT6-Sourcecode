/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**	Filename  : eeprom_drive.h
**	Project   : KUWE (PM01 PEPS)
**	Module	  :
**	Processor : Freescale mc9s12g128
**	Date/Time : 2016-07-14, 9:00
**	Abstract  :
**	Contents  :
**	Author	  :
**	http	  :
**	mail	  :
**	--------------------------------------------------------------------------
**
**	--------------------------------------------------------------------------
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef	__NVM_CFG_H__
#define __NVM_CFG_H__

#include "common_types.h"
#include "boot_app_cfg.h"


//整片flash 的大小， 这里要去掉，被映射为eeprom的地址
#define FLASH_P_BLOCK_SIZES (FLASH_ASW_MAX_ADDR-FLASH_APP_BASE)
#define FLASH_P_BLOCK_END_ADDR (FLASH_P_BLOCK_SIZES-1)

#define FEATURE_FLS_PF_BLOCK_SECTOR_SIZE (1024u)
#define FLASH_P_BLOCK_SECTOR_SIZES  (FEATURE_FLS_PF_BLOCK_SECTOR_SIZE)    //1k  //為了配合block大小，//改回2K
#define FLASH_P_PAGE_SIZES 8u  //要不要改成512字节


#define FLASH_START_ADDR  0x00000000UL
#define FLASH_END_ADDR  (FLASH_ASW_MAX_ADDR-1u)   //目前是128k的单片机



//只能对这些区域写，安全
#define RAM_START_ADDR  0x20007FC0
#define RAM_END_ADDR  0x20007FCF


//#define NVM_FLASH_DEBUG_EN  1


#ifdef NVM_FLASH_DEBUG_EN
void nvm_falsh_init(void);
void nvm_flash_test(void);
#endif




#endif
