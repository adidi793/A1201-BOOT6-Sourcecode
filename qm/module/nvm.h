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

#ifndef	__NVM_H__
#define __NVM_H__

#include "common_types.h"
#include "nvm_cfg.h"
#include "gpio.h"
#include "flash.h"

typedef enum
{
	NVM_TYPE_EEPROM=0,
	NVM_TYPE_RAM,
	NVM_TYPE_FLASH
}nvm_type_e;

u8 ram_write_bytes(u32 addr,u8 *p_data,u16 len);
u8 ram_read_bytes(u32 addr,u8 *p_data,u16 len);
u8 flash_read_bytes(u32 addr,u8 *p_data,u16 len);

void nvm_init(void);
void nvm_task(void);
u8 nvm_read_bytes(nvm_type_e type,u32 addr, u8 *p_data, u16 num);
u8 nvm_write_bytes(nvm_type_e type,u32 addr, u8 *p_data, u16 num);

#endif
