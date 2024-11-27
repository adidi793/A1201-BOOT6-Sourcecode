﻿/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**	Filename  : eeprom_drive.h
**	Project   : KUWE (PM01 PEPS)
**	Module	  :
**	Processor : Freescale mc9s12g128
**	Date/Time : 2015-07-28, 9:00
**	Abstract  :
**	Contents  :
**	Author	  :
**	http	  :
**	mail	  :
**	--------------------------------------------------------------------------
**
**	--------------------------------------------------------------------------
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*****************************************************************************
* Include Files
*****************************************************************************/
#include "common_types.h"
#include "common_cfg.h"
#include "common_memory.h"
#include "bl_typedefs.h"
#include "error.h"
#include "nvm.h"
#include "nvm_cfg.h"
#include "flash.h"
#include "bl_common.h"
#include "bl_rte.h"
#include "bl_memory.h"
#include "bl_flash_if.h"
#include "bl_flash_if_cfg.h"
#include "bl_flash.h"
#include "gpio.h"
#include "Cpu.h"
#include "wdog.h"

void flash_test(void);

#define FLASH_CFG_EN_INT

#define FLASH_SR_ALLOC()  //CPU_SR_ALLOC()
#define FLASH_CRITICAL_ENTER() __disable_irq()
#define FLASH_CRITICAL_EXIT()  __enable_irq()


#define FLASH_SECTOR_SIZES         FEATURE_FLS_PF_BLOCK_SECTOR_SIZE          /* Size of w_data source */

tFlashOptInfo *g_pstFlashOptInfo = (void *)0;

#pragma pack(4)
typedef struct {
    u8 state;
    u8 read_project;
    u8 write_project;
    status_t result;
    u16 page_id;
    u32 addr;
    u32 size;
    u8 w_data[FLASH_SECTOR_SIZES];
    u8 r_data[FLASH_SECTOR_SIZES];
    u8 test[20];
} flash_s;
#pragma pack()
static flash_s  g_flash;

#if 0 //A12

#ifdef __ICCARM__
//這是 flash driver的生成hex
const unsigned char g_flash_drv_output[] = { // end addr is 20004bd8
    0x01, 0x80, 0x00, 0x00, 0x36, 0x39, 0x31, 0x47, 0x00, 0x00, 0x01, 0x00, 0x79, 0x74, 0x00, 0x20,
    0xBB, 0x74, 0x00, 0x20, 0x0B, 0x75, 0x00, 0x20, 0x41, 0x75, 0x00, 0x20, 0x65, 0x75, 0x00, 0x20,
    0x9E, 0x49, 0x0A, 0x68, 0x10, 0x43, 0x08, 0x60, 0x70, 0x47, 0x9C, 0x49, 0x0A, 0x68, 0x82, 0x43,
    0x0A, 0x60, 0x70, 0x47, 0x9A, 0x49, 0x0A, 0x68, 0x10, 0x43, 0x08, 0x60, 0x70, 0x47, 0x98, 0x49,
    0x0A, 0x68, 0x82, 0x43, 0x0A, 0x60, 0x70, 0x47, 0x42, 0x69, 0x96, 0x4B, 0x13, 0x40, 0x19, 0x43,
    0x41, 0x61, 0x70, 0x47, 0x41, 0x69, 0x01, 0x22, 0x91, 0x43, 0x41, 0x61, 0x41, 0x69, 0x02, 0x22,
    0x0A, 0x43, 0x42, 0x61, 0x70, 0x47, 0x41, 0x69, 0x02, 0x22, 0x91, 0x43, 0x41, 0x61, 0x41, 0x69,
    0x01, 0x22, 0x0A, 0x43, 0x42, 0x61, 0x70, 0x47, 0x81, 0x61, 0x70, 0x47, 0x00, 0x21, 0xC9, 0x43,
    0x81, 0x61, 0x70, 0x47, 0x01, 0x6A, 0xE0, 0x20, 0x00, 0x03, 0x08, 0x40, 0x70, 0x47, 0x00, 0x6A,
    0x81, 0x0A, 0x08, 0x00, 0xC0, 0x07, 0xC0, 0x0F, 0x70, 0x47, 0x80, 0x21, 0xC9, 0x00, 0x01, 0x62,
    0x70, 0x47, 0x00, 0x6A, 0x41, 0x0A, 0x08, 0x00, 0xC0, 0x07, 0xC0, 0x0F, 0x70, 0x47, 0x00, 0x6A,
    0x01, 0x0A, 0x08, 0x00, 0xC0, 0x07, 0xC0, 0x0F, 0x70, 0x47, 0x80, 0x21, 0x49, 0x00, 0x01, 0x62,
    0x70, 0x47, 0x00, 0x6A, 0x41, 0x08, 0x08, 0x00, 0xC0, 0x07, 0xC0, 0x0F, 0x70, 0x47, 0x02, 0x21,
    0x01, 0x62, 0x70, 0x47, 0x01, 0x6A, 0x08, 0x00, 0xC0, 0x07, 0xC0, 0x0F, 0x70, 0x47, 0x01, 0x21,
    0x01, 0x62, 0x70, 0x47, 0xFE, 0xB5, 0x00, 0x24, 0x02, 0x94, 0x01, 0x22, 0x6B, 0x46, 0x1A, 0x70,
    0xA3, 0x4D, 0xA8, 0x42, 0x00, 0xD0, 0xFE, 0xE7, 0x01, 0x91, 0x08, 0x46, 0x91, 0x04, 0x88, 0x42,
    0x00, 0xD3, 0xFE, 0xE7, 0x20, 0x20, 0xFF, 0xF7, 0x8B, 0xFF, 0x01, 0x98, 0x80, 0x07, 0x06, 0xD1,
    0x28, 0x00, 0xFF, 0xF7, 0xB7, 0xFF, 0x80, 0x21, 0x09, 0x03, 0x88, 0x42, 0x01, 0xD1, 0x20, 0x46,
    0xFE, 0xBD, 0xCF, 0x02, 0x38, 0x00, 0xFF, 0xF7, 0x85, 0xFF, 0x21, 0x46, 0x28, 0x00, 0xFF, 0xF7,
    0x8B, 0xFF, 0x28, 0x00, 0xFF, 0xF7, 0x97, 0xFF, 0xEF, 0xF3, 0x10, 0x86, 0x72, 0xB6, 0x91, 0x49,
    0x28, 0x00, 0xFF, 0xF7, 0x99, 0xFF, 0x90, 0x49, 0x28, 0x00, 0xFF, 0xF7, 0x95, 0xFF, 0x86, 0xF3,
    0x10, 0x88, 0x28, 0x00, 0xFF, 0xF7, 0xC3, 0xFF, 0x28, 0x00, 0xFF, 0xF7, 0xAE, 0xFF, 0x28, 0x00,
    0xFF, 0xF7, 0x9B, 0xFF, 0x89, 0x48, 0x01, 0x99, 0x08, 0x60, 0x02, 0x9E, 0x76, 0x1C, 0xA8, 0x0B,
    0x86, 0x42, 0x22, 0xD2, 0x28, 0x00, 0xFF, 0xF7, 0x9A, 0xFF, 0x00, 0x28, 0x1D, 0xD1, 0x28, 0x00,
    0xFF, 0xF7, 0x8F, 0xFF, 0x00, 0x28, 0x18, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0x80, 0xFF, 0x00, 0x28,
    0x13, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0x9E, 0xFF, 0x00, 0x28, 0xE7, 0xD0, 0x28, 0x00, 0xFF, 0xF7,
    0x9E, 0xFF, 0x28, 0x00, 0xFF, 0xF7, 0x6A, 0xFF, 0x38, 0x00, 0xFF, 0xF7, 0x48, 0xFF, 0x20, 0x20,
    0xFF, 0xF7, 0x3B, 0xFF, 0x68, 0x46, 0x00, 0x78, 0xFE, 0xBD, 0x68, 0x46, 0x04, 0x70, 0xF0, 0xE7,
    0xFE, 0xB5, 0x00, 0x24, 0x02, 0x94, 0x01, 0x22, 0x6B, 0x46, 0x1A, 0x70, 0x6C, 0x4D, 0xA8, 0x42,
    0x00, 0xD0, 0xFE, 0xE7, 0x01, 0x91, 0x08, 0x46, 0x91, 0x04, 0x88, 0x42, 0x00, 0xD3, 0xFE, 0xE7,
    0x20, 0x20, 0xFF, 0xF7, 0x1D, 0xFF, 0x01, 0x98, 0x80, 0x07, 0x06, 0xD1, 0x28, 0x00, 0xFF, 0xF7,
    0x49, 0xFF, 0x80, 0x21, 0x09, 0x03, 0x88, 0x42, 0x01, 0xD1, 0x20, 0x46, 0xFE, 0xBD, 0xCF, 0x02,
    0x38, 0x00, 0xFF, 0xF7, 0x17, 0xFF, 0xA9, 0x0D, 0x28, 0x00, 0xFF, 0xF7, 0x1D, 0xFF, 0x28, 0x00,
    0xFF, 0xF7, 0x29, 0xFF, 0xEF, 0xF3, 0x10, 0x86, 0x72, 0xB6, 0x5A, 0x49, 0x28, 0x00, 0xFF, 0xF7,
    0x2B, 0xFF, 0x5B, 0x49, 0x28, 0x00, 0xFF, 0xF7, 0x27, 0xFF, 0x86, 0xF3, 0x10, 0x88, 0x28, 0x00,
    0xFF, 0xF7, 0x55, 0xFF, 0x28, 0x00, 0xFF, 0xF7, 0x40, 0xFF, 0x28, 0x00, 0xFF, 0xF7, 0x2D, 0xFF,
    0x52, 0x48, 0x01, 0x99, 0x08, 0x60, 0x02, 0x9E, 0x76, 0x1C, 0xA8, 0x0B, 0x86, 0x42, 0x22, 0xD2,
    0x28, 0x00, 0xFF, 0xF7, 0x2C, 0xFF, 0x00, 0x28, 0x1D, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0x21, 0xFF,
    0x00, 0x28, 0x18, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0x12, 0xFF, 0x00, 0x28, 0x13, 0xD1, 0x28, 0x00,
    0xFF, 0xF7, 0x30, 0xFF, 0x00, 0x28, 0xE7, 0xD0, 0x28, 0x00, 0xFF, 0xF7, 0x30, 0xFF, 0x28, 0x00,
    0xFF, 0xF7, 0xFC, 0xFE, 0x38, 0x00, 0xFF, 0xF7, 0xDA, 0xFE, 0x20, 0x20, 0xFF, 0xF7, 0xCD, 0xFE,
    0x68, 0x46, 0x00, 0x78, 0xFE, 0xBD, 0x68, 0x46, 0x04, 0x70, 0xF0, 0xE7, 0x38, 0x24, 0x00, 0x40,
    0x4C, 0x24, 0x00, 0x40, 0xFF, 0xFC, 0xFF, 0xFF, 0xF8, 0xB5, 0x84, 0xB0, 0x16, 0x00, 0x00, 0x24,
    0x02, 0x94, 0x01, 0x22, 0x6B, 0x46, 0x1A, 0x70, 0x31, 0x4D, 0xA8, 0x42, 0x00, 0xD0, 0xFE, 0xE7,
    0x01, 0x91, 0x08, 0x46, 0x91, 0x04, 0x88, 0x42, 0x00, 0xD3, 0xFE, 0xE7, 0x20, 0x20, 0xFF, 0xF7,
    0xA7, 0xFE, 0x01, 0x98, 0x80, 0x07, 0x06, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0xD3, 0xFE, 0x80, 0x21,
    0x09, 0x03, 0x88, 0x42, 0x01, 0xD1, 0x20, 0x46, 0x44, 0xE0, 0x03, 0x96, 0xCF, 0x02, 0x38, 0x00,
    0xFF, 0xF7, 0xA0, 0xFE, 0x28, 0x00, 0xFF, 0xF7, 0xAD, 0xFE, 0xEF, 0xF3, 0x10, 0x86, 0x72, 0xB6,
    0x59, 0x49, 0x28, 0x00, 0xFF, 0xF7, 0xB8, 0xFE, 0x58, 0x49, 0x28, 0x00, 0xFF, 0xF7, 0xB4, 0xFE,
    0x86, 0xF3, 0x10, 0x88, 0x28, 0x00, 0xFF, 0xF7, 0xD0, 0xFE, 0x28, 0x00, 0xFF, 0xF7, 0xBD, 0xFE,
    0x03, 0x98, 0x01, 0x99, 0x08, 0x60, 0x02, 0x9E, 0x76, 0x1C, 0xA8, 0x0B, 0x86, 0x42, 0x23, 0xD2,
    0x28, 0x00, 0xFF, 0xF7, 0xBC, 0xFE, 0x00, 0x28, 0x1E, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0xB1, 0xFE,
    0x00, 0x28, 0x19, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0xA2, 0xFE, 0x00, 0x28, 0x14, 0xD1, 0x28, 0x00,
    0xFF, 0xF7, 0xB7, 0xFE, 0x00, 0x28, 0xE7, 0xD0, 0x28, 0x00, 0xFF, 0xF7, 0xB8, 0xFE, 0x28, 0x00,
    0xFF, 0xF7, 0x8C, 0xFE, 0x38, 0x00, 0xFF, 0xF7, 0x6A, 0xFE, 0x20, 0x20, 0xFF, 0xF7, 0x5D, 0xFE,
    0x68, 0x46, 0x00, 0x78, 0x05, 0xB0, 0xF0, 0xBD, 0x68, 0x46, 0x04, 0x70, 0xEF, 0xE7, 0x00, 0x00,
    0x00, 0x10, 0x00, 0x40, 0x96, 0x96, 0x96, 0x96, 0xEA, 0xEA, 0xEA, 0xEA, 0xCD, 0xAB, 0x34, 0x12,
    0x3C, 0x3C, 0x3C, 0x3C, 0xF7, 0xB5, 0x01, 0x24, 0x35, 0x4D, 0xA8, 0x42, 0x00, 0xD0, 0xFE, 0xE7,
    0x80, 0x29, 0x00, 0xD3, 0xFE, 0xE7, 0xC8, 0x02, 0x00, 0x90, 0x00, 0x98, 0x40, 0x05, 0x40, 0x0D,
    0x56, 0xD1, 0x20, 0x20, 0xFF, 0xF7, 0x34, 0xFE, 0xA0, 0x07, 0xFF, 0xF7, 0x3B, 0xFE, 0x28, 0x00,
    0xFF, 0xF7, 0x48, 0xFE, 0xEF, 0xF3, 0x10, 0x86, 0x72, 0xB6, 0x27, 0x49, 0x28, 0x00, 0xFF, 0xF7,
    0x53, 0xFE, 0x26, 0x49, 0x28, 0x00, 0xFF, 0xF7, 0x4F, 0xFE, 0x86, 0xF3, 0x10, 0x88, 0x28, 0x00,
    0xFF, 0xF7, 0x6B, 0xFE, 0x28, 0x00, 0xFF, 0xF7, 0x58, 0xFE, 0x00, 0x27, 0x02, 0x9E, 0x00, 0xE0,
    0x3F, 0x1D, 0x20, 0x48, 0x87, 0x42, 0x28, 0xD8, 0x00, 0x24, 0x28, 0x00, 0xFF, 0xF7, 0x2A, 0xFE,
    0x30, 0x68, 0x00, 0x99, 0x08, 0x60, 0x00, 0x98, 0x00, 0x1D, 0x00, 0x90, 0x36, 0x1D, 0x64, 0x1C,
    0xA8, 0x0B, 0x84, 0x42, 0x25, 0xD2, 0x28, 0x00, 0xFF, 0xF7, 0x49, 0xFE, 0x00, 0x28, 0x20, 0xD1,
    0x28, 0x00, 0xFF, 0xF7, 0x3E, 0xFE, 0x00, 0x28, 0x1B, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0x2F, 0xFE,
    0x00, 0x28, 0x16, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0x44, 0xFE, 0x00, 0x28, 0xE7, 0xD0, 0x28, 0x00,
    0xFF, 0xF7, 0x45, 0xFE, 0x01, 0x24, 0x20, 0x00, 0xD2, 0xD1, 0x28, 0x00, 0xFF, 0xF7, 0x16, 0xFE,
    0x80, 0x20, 0xC0, 0x05, 0xFF, 0xF7, 0xF3, 0xFD, 0x20, 0x20, 0xFF, 0xF7, 0xE6, 0xFD, 0x20, 0x00,
    0xFE, 0xBD, 0x00, 0x24, 0xEF, 0xE7, 0x00, 0x00, 0xA5, 0xA5, 0xA5, 0xA5, 0xF1, 0xF1, 0xF1, 0xF1,
    0x00, 0x10, 0x00, 0x40, 0xFF, 0x07, 0x00, 0x00, 0xF8, 0xB5, 0x0F, 0x00, 0x16, 0x00, 0x00, 0x24,
    0x25, 0x00, 0x21, 0x00, 0x6A, 0x46, 0x11, 0x70, 0x80, 0x69, 0x00, 0x28, 0x00, 0xD0, 0x80, 0x47,
    0x68, 0x46, 0x00, 0x78, 0x02, 0xE0, 0x80, 0x21, 0x09, 0x01, 0x6D, 0x18, 0xB5, 0x42, 0x06, 0xD2,
    0x79, 0x19, 0x41, 0x48, 0xFF, 0xF7, 0x8C, 0xFE, 0x01, 0x00, 0x01, 0x29, 0xF3, 0xD0, 0x01, 0x28,
    0x01, 0xD1, 0x20, 0x00, 0xF2, 0xBD, 0x01, 0x20, 0xF2, 0xBD, 0xF8, 0xB5, 0x0C, 0x00, 0x15, 0x00,
    0x1E, 0x00, 0x00, 0x21, 0x6A, 0x46, 0x11, 0x70, 0x80, 0x69, 0x00, 0x28, 0x00, 0xD0, 0x80, 0x47,
    0x31, 0x00, 0xAE, 0x08, 0xB6, 0xB2, 0x00, 0x22, 0x27, 0x00, 0x68, 0x46, 0x00, 0x78, 0x0D, 0x00,
    0x14, 0x00, 0x00, 0xE0, 0x64, 0x1C, 0xB4, 0x42, 0x09, 0xD2, 0x2A, 0x68, 0xA0, 0x00, 0x39, 0x18,
    0x2D, 0x48, 0xFF, 0xF7, 0xD9, 0xFE, 0x2D, 0x1D, 0x01, 0x00, 0x01, 0x29, 0xF2, 0xD0, 0x01, 0x28,
    0x01, 0xD1, 0x00, 0x20, 0xF2, 0xBD, 0x01, 0x20, 0xF2, 0xBD, 0xFE, 0xB5, 0x00, 0x26, 0x01, 0x90,
    0x00, 0x91, 0x17, 0x00, 0x1C, 0x00, 0x08, 0x9D, 0x00, 0xE0, 0x76, 0x1C, 0xBE, 0x42, 0x0D, 0xD2,
    0x01, 0x98, 0x80, 0x69, 0x00, 0x28, 0x00, 0xD0, 0x80, 0x47, 0x00, 0x98, 0x80, 0x19, 0x01, 0x78,
    0xA2, 0x5D, 0x91, 0x42, 0xF1, 0xD0, 0x28, 0x60, 0x01, 0x20, 0xFE, 0xBD, 0x00, 0x20, 0xFE, 0xBD,
    0x38, 0xB5, 0x0D, 0x00, 0x1C, 0x00, 0x80, 0x69, 0x00, 0x28, 0x00, 0xD0, 0x80, 0x47, 0x22, 0x00,
    0xE9, 0x0A, 0x15, 0x48, 0xFF, 0xF7, 0x1E, 0xFF, 0x01, 0x28, 0x01, 0xD1, 0x00, 0x20, 0x32, 0xBD,
    0x01, 0x20, 0x32, 0xBD, 0xF8, 0xB5, 0x0F, 0x00, 0x16, 0x00, 0x00, 0x24, 0x25, 0x00, 0x21, 0x00,
    0x6A, 0x46, 0x11, 0x70, 0x80, 0x69, 0x00, 0x28, 0x00, 0xD0, 0x80, 0x47, 0x68, 0x46, 0x00, 0x78,
    0x02, 0xE0, 0x80, 0x21, 0x89, 0x00, 0x6D, 0x18, 0xB5, 0x42, 0x06, 0xD2, 0x79, 0x19, 0x06, 0x48,
    0xFF, 0xF7, 0xA8, 0xFD, 0x01, 0x00, 0x01, 0x29, 0xF3, 0xD0, 0x01, 0x28, 0x01, 0xD1, 0x20, 0x00,
    0xF2, 0xBD, 0x01, 0x20, 0xF2, 0xBD, 0x00, 0x00, 0x00, 0x10, 0x00, 0x40
};
#else

const unsigned char g_flash_drv_output[] = { // end addr is 20004bd8
    0x01, 0x80, 0x00, 0x00, 0x36, 0x39, 0x31, 0x47, 0x00, 0x00, 0x01, 0x00, 0x29, 0x74, 0x00, 0x20,
    0x65, 0x74, 0x00, 0x20, 0xED, 0x73, 0x00, 0x20, 0xA5, 0x74, 0x00, 0x20, 0xCD, 0x74, 0x00, 0x20,
    0x01, 0x21, 0x04, 0x4B, 0x5A, 0x69, 0x8A, 0x43, 0x5A, 0x61, 0x02, 0x22, 0x59, 0x69, 0x0A, 0x43,
    0x5A, 0x61, 0x70, 0x47, 0x00, 0x10, 0x00, 0x40, 0x36, 0x4B, 0xF7, 0xB5, 0x83, 0x42, 0x00, 0xD0,
    0xFE, 0xE7, 0x80, 0x22, 0xD2, 0x02, 0x91, 0x42, 0x00, 0xD3, 0xFE, 0xE7, 0x20, 0x20, 0x32, 0x4A,
    0x0E, 0x00, 0x94, 0x6B, 0x20, 0x43, 0x03, 0x24, 0x90, 0x63, 0x26, 0x40, 0x00, 0x20, 0x21, 0x42,
    0x53, 0xD1, 0xE0, 0x20, 0x80, 0x25, 0x1C, 0x6A, 0x00, 0x03, 0x04, 0x40, 0x2D, 0x03, 0x30, 0x00,
    0xAC, 0x42, 0x4A, 0xD0, 0x80, 0x20, 0xD4, 0x6C, 0xC0, 0x05, 0x20, 0x43, 0xD0, 0x64, 0x58, 0x69,
    0x26, 0x4C, 0x20, 0x40, 0x02, 0x24, 0x58, 0x61, 0x58, 0x69, 0xA0, 0x43, 0x58, 0x61, 0x58, 0x69,
    0x01, 0x3C, 0x20, 0x43, 0x58, 0x61, 0xEF, 0xF3, 0x10, 0x80, 0x72, 0xB6, 0x20, 0x4D, 0x9D, 0x61,
    0x20, 0x4D, 0x9D, 0x61, 0x80, 0xF3, 0x10, 0x88, 0x80, 0x25, 0x80, 0x26, 0x1E, 0x48, 0x6D, 0x00,
    0xF6, 0x00, 0x1C, 0x62, 0x80, 0x27, 0x1D, 0x62, 0x1E, 0x62, 0x08, 0x60, 0x80, 0x21, 0xBF, 0x00,
    0x49, 0x02, 0x01, 0x39, 0x00, 0x29, 0x21, 0xD0, 0x18, 0x6A, 0x84, 0x46, 0x28, 0x40, 0x00, 0x90,
    0x60, 0x46, 0x28, 0x42, 0x1A, 0xD1, 0x18, 0x6A, 0x84, 0x46, 0x38, 0x40, 0x01, 0x90, 0x60, 0x46,
    0x38, 0x42, 0x13, 0xD1, 0x18, 0x6A, 0x30, 0x42, 0x12, 0xD1, 0x18, 0x6A, 0x20, 0x42, 0xE8, 0xD0,
    0x01, 0x20, 0x1C, 0x62, 0x01, 0x21, 0x49, 0x42, 0x99, 0x61, 0xD3, 0x6C, 0x0B, 0x49, 0x0B, 0x40,
    0x20, 0x21, 0xD3, 0x64, 0x93, 0x6B, 0x8B, 0x43, 0x93, 0x63, 0xFE, 0xBD, 0x00, 0x20, 0xF1, 0xE7,
    0x01, 0x98, 0xEF, 0xE7, 0x00, 0x10, 0x00, 0x40, 0x00, 0x24, 0x00, 0x40, 0xFF, 0xFC, 0xFF, 0xFF,
    0x96, 0x96, 0x96, 0x96, 0xEA, 0xEA, 0xEA, 0xEA, 0xCD, 0xAB, 0x34, 0x12, 0xFF, 0xFF, 0xFF, 0xBF,
    0x37, 0x4B, 0xF7, 0xB5, 0x83, 0x42, 0x00, 0xD0, 0xFE, 0xE7, 0x80, 0x22, 0xD2, 0x02, 0x91, 0x42,
    0x00, 0xD3, 0xFE, 0xE7, 0x20, 0x20, 0x33, 0x4A, 0x0E, 0x00, 0x94, 0x6B, 0x20, 0x43, 0x03, 0x24,
    0x90, 0x63, 0x26, 0x40, 0x00, 0x20, 0x21, 0x42, 0x54, 0xD1, 0xE0, 0x20, 0x80, 0x25, 0x1C, 0x6A,
    0x00, 0x03, 0x04, 0x40, 0x2D, 0x03, 0x30, 0x00, 0xAC, 0x42, 0x4B, 0xD0, 0x80, 0x20, 0xD4, 0x6C,
    0x80, 0x25, 0xC0, 0x05, 0x20, 0x43, 0xD0, 0x64, 0x58, 0x69, 0x27, 0x4C, 0x6D, 0x00, 0x20, 0x40,
    0x02, 0x24, 0x28, 0x43, 0x58, 0x61, 0x58, 0x69, 0xA0, 0x43, 0x58, 0x61, 0x58, 0x69, 0x01, 0x3C,
    0x20, 0x43, 0x58, 0x61, 0xEF, 0xF3, 0x10, 0x80, 0x72, 0xB6, 0x20, 0x4E, 0x9E, 0x61, 0x20, 0x4E,
    0x9E, 0x61, 0x80, 0xF3, 0x10, 0x88, 0x80, 0x26, 0x1E, 0x48, 0xF6, 0x00, 0x1C, 0x62, 0x80, 0x27,
    0x1D, 0x62, 0x1E, 0x62, 0x08, 0x60, 0x80, 0x21, 0xBF, 0x00, 0x49, 0x02, 0x01, 0x39, 0x00, 0x29,
    0x21, 0xD0, 0x18, 0x6A, 0x84, 0x46, 0x28, 0x40, 0x00, 0x90, 0x60, 0x46, 0x28, 0x42, 0x1A, 0xD1,
    0x18, 0x6A, 0x84, 0x46, 0x38, 0x40, 0x01, 0x90, 0x60, 0x46, 0x38, 0x42, 0x13, 0xD1, 0x18, 0x6A,
    0x30, 0x42, 0x12, 0xD1, 0x18, 0x6A, 0x20, 0x42, 0xE8, 0xD0, 0x01, 0x20, 0x1C, 0x62, 0x01, 0x21,
    0x49, 0x42, 0x99, 0x61, 0xD3, 0x6C, 0x0C, 0x49, 0x0B, 0x40, 0x20, 0x21, 0xD3, 0x64, 0x93, 0x6B,
    0x8B, 0x43, 0x93, 0x63, 0xFE, 0xBD, 0x00, 0x20, 0xF1, 0xE7, 0x01, 0x98, 0xEF, 0xE7, 0xC0, 0x46,
    0x00, 0x10, 0x00, 0x40, 0x00, 0x24, 0x00, 0x40, 0xFF, 0xFC, 0xFF, 0xFF, 0x96, 0x96, 0x96, 0x96,
    0x3C, 0x3C, 0x3C, 0x3C, 0xCD, 0xAB, 0x34, 0x12, 0xFF, 0xFF, 0xFF, 0xBF, 0xF7, 0xB5, 0x31, 0x4C,
    0x0E, 0x00, 0x17, 0x00, 0x84, 0x42, 0x00, 0xD0, 0xFE, 0xE7, 0x80, 0x23, 0xDB, 0x02, 0x99, 0x42,
    0x00, 0xD3, 0xFE, 0xE7, 0x20, 0x23, 0x2C, 0x4D, 0x00, 0x20, 0xAA, 0x6B, 0x13, 0x43, 0xAB, 0x63,
    0x0A, 0x00, 0x03, 0x23, 0x1A, 0x40, 0x19, 0x42, 0x47, 0xD1, 0xE0, 0x21, 0x23, 0x6A, 0x09, 0x03,
    0x0B, 0x40, 0x80, 0x21, 0x10, 0x00, 0x09, 0x03, 0x8B, 0x42, 0x3E, 0xD0, 0x80, 0x23, 0xEA, 0x6C,
    0xDB, 0x05, 0x13, 0x43, 0xEB, 0x64, 0xFF, 0xF7, 0xD3, 0xFE, 0xEF, 0xF3, 0x10, 0x83, 0x72, 0xB6,
    0x1E, 0x4A, 0xA2, 0x61, 0x1E, 0x4A, 0xA2, 0x61, 0x83, 0xF3, 0x10, 0x88, 0x80, 0x22, 0x80, 0x23,
    0x52, 0x00, 0xDB, 0x00, 0x22, 0x62, 0x9C, 0x46, 0x23, 0x62, 0x80, 0x23, 0x37, 0x60, 0x80, 0x26,
    0x5B, 0x02, 0xB6, 0x00, 0x01, 0x3B, 0x00, 0x2B, 0x20, 0xD0, 0x27, 0x6A, 0x38, 0x00, 0x10, 0x40,
    0x17, 0x42, 0x1B, 0xD1, 0x27, 0x6A, 0x39, 0x00, 0x31, 0x40, 0x01, 0x91, 0x37, 0x42, 0x09, 0xD1,
    0x61, 0x46, 0x20, 0x6A, 0x08, 0x42, 0x13, 0xD1, 0x02, 0x21, 0x20, 0x6A, 0x08, 0x42, 0xE9, 0xD0,
    0x01, 0x20, 0x21, 0x62, 0x01, 0x23, 0x5B, 0x42, 0xA3, 0x61, 0xEB, 0x6C, 0x09, 0x4A, 0x13, 0x40,
    0x20, 0x22, 0xEB, 0x64, 0xAB, 0x6B, 0x93, 0x43, 0xAB, 0x63, 0xFE, 0xBD, 0x00, 0x20, 0xF1, 0xE7,
    0x01, 0x98, 0xEF, 0xE7, 0x00, 0x10, 0x00, 0x40, 0x00, 0x24, 0x00, 0x40, 0xA5, 0xA5, 0xA5, 0xA5,
    0xF1, 0xF1, 0xF1, 0xF1, 0xFF, 0xFF, 0xFF, 0xBF, 0xF0, 0xB5, 0x33, 0x4C, 0x85, 0xB0, 0x03, 0x92,
    0x84, 0x42, 0x00, 0xD0, 0xFE, 0xE7, 0x7F, 0x29, 0x00, 0xD9, 0xFE, 0xE7, 0xCB, 0x02, 0x01, 0x93,
    0x20, 0x23, 0x2E, 0x4D, 0xAA, 0x6B, 0x13, 0x43, 0xAB, 0x63, 0x80, 0x23, 0xEA, 0x6C, 0xDB, 0x05,
    0x13, 0x43, 0xEB, 0x64, 0xFF, 0xF7, 0x74, 0xFE, 0xEF, 0xF3, 0x10, 0x83, 0x72, 0xB6, 0x28, 0x4A,
    0xA2, 0x61, 0x28, 0x4A, 0xA2, 0x61, 0x83, 0xF3, 0x10, 0x88, 0x80, 0x27, 0x80, 0x23, 0x01, 0x9A,
    0xDB, 0x00, 0x7F, 0x00, 0x27, 0x62, 0x23, 0x62, 0x80, 0x23, 0x94, 0x46, 0x16, 0x00, 0x1B, 0x01,
    0x63, 0x44, 0x02, 0x93, 0xFF, 0xF7, 0x5C, 0xFE, 0x03, 0x9B, 0x01, 0x9A, 0x9B, 0x1A, 0x9B, 0x59,
    0x08, 0xC6, 0x80, 0x23, 0x5B, 0x02, 0x01, 0x3B, 0x00, 0x2B, 0x28, 0xD0, 0x21, 0x6A, 0x08, 0x00,
    0x38, 0x40, 0x84, 0x46, 0x39, 0x42, 0x22, 0xD1, 0x80, 0x20, 0x80, 0x22, 0x21, 0x6A, 0x80, 0x00,
    0x92, 0x00, 0x08, 0x40, 0x11, 0x42, 0x1C, 0xD1, 0x80, 0x22, 0x21, 0x6A, 0xD2, 0x00, 0x11, 0x42,
    0x08, 0xD1, 0x02, 0x22, 0x21, 0x6A, 0x11, 0x42, 0xE5, 0xD0, 0x02, 0x9B, 0x22, 0x62, 0x9E, 0x42,
    0xD8, 0xD1, 0x01, 0x20, 0x01, 0x23, 0x5B, 0x42, 0xA3, 0x61, 0xEB, 0x6C, 0x0A, 0x4A, 0x13, 0x40,
    0x20, 0x22, 0xEB, 0x64, 0xAB, 0x6B, 0x93, 0x43, 0xAB, 0x63, 0x05, 0xB0, 0xF0, 0xBD, 0x00, 0x20,
    0xF0, 0xE7, 0x60, 0x46, 0xEE, 0xE7, 0xC0, 0x46, 0x00, 0x10, 0x00, 0x40, 0x00, 0x24, 0x00, 0x40,
    0xA5, 0xA5, 0xA5, 0xA5, 0xF1, 0xF1, 0xF1, 0xF1, 0xFF, 0xFF, 0xFF, 0xBF, 0xF7, 0xB5, 0x06, 0x00,
    0x0D, 0x00, 0x0C, 0x00, 0x1F, 0x00, 0x53, 0x18, 0x01, 0x93, 0x01, 0x9B, 0x9C, 0x42, 0x01, 0xD1,
    0x00, 0x20, 0x0D, 0xE0, 0xB3, 0x69, 0x00, 0x2B, 0x00, 0xD0, 0x98, 0x47, 0xEB, 0x43, 0x62, 0x1C,
    0xFB, 0x18, 0x21, 0x78, 0x9B, 0x5C, 0x99, 0x42, 0x03, 0xD0, 0x01, 0x20, 0x08, 0x9B, 0x1C, 0x60,
    0xFE, 0xBD, 0x14, 0x00, 0xE9, 0xE7, 0x00, 0x00, 0x83, 0x69, 0x70, 0xB5, 0x0E, 0x00, 0x15, 0x00,
    0x00, 0x2B, 0x00, 0xD0, 0x98, 0x47, 0x00, 0x20, 0x04, 0x00, 0x08, 0xE0, 0x08, 0x48, 0xA1, 0x19,
    0xFF, 0xF7, 0x76, 0xFE, 0x01, 0x28, 0x08, 0xD1, 0x80, 0x23, 0x1B, 0x01, 0xE4, 0x18, 0xAC, 0x42,
    0xF4, 0xD3, 0x01, 0x23, 0x18, 0x1A, 0x80, 0xB2, 0x70, 0xBD, 0x01, 0x20, 0xFC, 0xE7, 0xC0, 0x46,
    0x00, 0x10, 0x00, 0x40, 0xF8, 0xB5, 0x1F, 0x00, 0x83, 0x69, 0x0E, 0x00, 0x14, 0x00, 0x00, 0x2B,
    0x00, 0xD0, 0x98, 0x47, 0x00, 0x25, 0x28, 0x00, 0xA4, 0x08, 0xA4, 0xB2, 0xA5, 0x42, 0x03, 0xD3,
    0x01, 0x23, 0x18, 0x1A, 0x80, 0xB2, 0xF8, 0xBD, 0xAB, 0x00, 0xFA, 0x58, 0x04, 0x48, 0xF1, 0x18,
    0xFF, 0xF7, 0xCC, 0xFE, 0x01, 0x28, 0x01, 0xD1, 0x01, 0x35, 0xEF, 0xE7, 0x01, 0x20, 0xF2, 0xE7,
    0x00, 0x10, 0x00, 0x40, 0x70, 0xB5, 0x1D, 0x00, 0x83, 0x69, 0x0C, 0x00, 0x00, 0x2B, 0x00, 0xD0,
    0x98, 0x47, 0x2A, 0x00, 0xE1, 0x0A, 0x04, 0x48, 0xFF, 0xF7, 0x26, 0xFF, 0x01, 0x38, 0x43, 0x1E,
    0x98, 0x41, 0x80, 0xB2, 0x70, 0xBD, 0xC0, 0x46, 0x00, 0x10, 0x00, 0x40, 0x83, 0x69, 0x70, 0xB5,
    0x0E, 0x00, 0x15, 0x00, 0x00, 0x2B, 0x00, 0xD0, 0x98, 0x47, 0x00, 0x20, 0x04, 0x00, 0x08, 0xE0,
    0x08, 0x48, 0xA1, 0x19, 0xFF, 0xF7, 0xA8, 0xFD, 0x01, 0x28, 0x08, 0xD1, 0x80, 0x23, 0x9B, 0x00,
    0xE4, 0x18, 0xAC, 0x42, 0xF4, 0xD3, 0x01, 0x23, 0x18, 0x1A, 0x80, 0xB2, 0x70, 0xBD, 0x01, 0x20,
    0xFC, 0xE7, 0xC0, 0x46, 0x00, 0x10, 0x00, 0x40
};
#endif

#endif

void eFLASH_wdog_feed1(void) {

}

flash_ssd_config_t g_flash_ssd1 = {
    0,
    0,
    0,
    0,
    0,
    0,
    NULL_PTR,
    //&eFLASH_wdog_feed1,
};

void flash_init(void) {
    u16 i;
    common_memory_clear((uint8_t *)flashDriverStartAdd, 1440); //sizeof(g_flash_drv_output)替换成1440
    //測試時用到
    #ifdef FLASH_TEST_ERASE_WRITE
    common_memory_copys((uint8_t *)flashDriverStartAdd, g_flash_drv_output, sizeof(g_flash_drv_output));  ///<copy driver
    g_pstFlashOptInfo = (tFlashOptInfo *)flashDriverStartAdd;
    #endif
    
    //flash_test();
}

/**
 * @brief  讀取
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
status_t flash_drv_pflash_read(u32 addr, u32 len, u8 *p_data) {
    status_t ret = STATUS_SUCCESS;    /* Return code variable */
    
    //if ((FTFx_FCNFG & FTFx_FCNFG_PFLSH_MASK) == FTFx_FCNFG_PFLSH_MASK)
    {
        while (len > 0U) { //&& (ret == STATUS_SUCCESS))
        
            *p_data = *((uint8_t *)addr++);
            
            len--;
            p_data++;
            
        }
    }
    /*
    else
    {
    	ret = STATUS_UNSUPPORTED;
    }
    */
    return ret;
}

MEMORY_MAP_RAM_FOR_ASIL status_t FLASH_ErasePage_ext(uint32_t Page_Address, cpu_wdg_t p_freewdg) {
    status_t l_result = STATUS_ERROR;
    //flash_callback_t pCallBack;
    g_pstFlashOptInfo = (tFlashOptInfo *)flashDriverStartAdd;
    FLASH_SR_ALLOC();
    //u32 checkValue;
    //checkValue = BL_MAKE_LOCAL_U32(FLASHIF_DRIVER_MAGIC_ADDRESS);   //因為是大端，所以直接讀取FLASHIF_DRIVER_MAGIC_ADDRESS地址上的值
    if (g_pstFlashOptInfo->magic ==
        FLASH_DRIVER_VERSION_MAGIC) { //   FLASHIF_DRIVER_MAGIC_VALUE  目前為了編譯g_FlashHeader 才改成這樣，如果是原來的要改回 FLASH_DRIVER_VERSION_MAGIC等
        //checkValue = BL_MAKE_LOCAL_U32(FLASHIF_DRIVER_VERSION_ADDRESS);
        if (g_pstFlashOptInfo->version == FLASH_DRIVER_VERSION_VERSION) { //    FLASHIF_DRIVER_VERSION_VALUE
            //
        } else {
            //l_result = STATUS_ERROR;
            return l_result;
        }
    } else {
        //l_result = STATUS_ERROR;
        return l_result;
    }
    
    g_flash.addr = Page_Address;
    g_flash.size = FLASH_SECTOR_SIZES;
    
    if ((g_flash.addr % FLASH_SECTOR_SIZES) == 0) {
        //pCallBack = (flash_callback_t)CCIF_Callback;
        //flashSSDConfig.CallBack = pCallBack;
        #ifdef FLASH_CFG_EN_INT
        
        FLASH_CRITICAL_ENTER();
        wdog_task();
        // sector erase 7.5ms
        //g_flash.result = FLASH_DRV_EraseSector(&flashSSDConfig, g_flash.addr, g_flash.size);
        g_flash.result =  g_pstFlashOptInfo->FLASH_EraseSector(&g_flash_ssd1, g_flash.addr, g_flash.size);   ///<擦除一个页，地址为当前页首地址
        FLASH_CRITICAL_EXIT();
        
        #else
        //g_flash.result = FLASH_DRV_EraseSector(&flashSSDConfig, g_flash.addr, g_flash.size);
        g_flash.result =  g_pstFlashOptInfo->FLASH_EraseSector(&g_flash_ssd1, g_flash.addr, g_flash.size);   ///<擦除一个页，地址为当前页首地址
        #endif
        //while( (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK) == 0);
        //DEV_ASSERT(STATUS_SUCCESS == g_flash.result);
        //pCallBack = (flash_callback_t)CCIF_Callback;
        //flashSSDConfig.CallBack = NULL_CALLBACK;
        
        if (g_flash.result == STATUS_SUCCESS) {
            l_result = STATUS_SUCCESS;
        }
        #if 0  //不校驗
        if (STATUS_SUCCESS == l_result) {
            #ifdef FLASH_CFG_EN_INT
        
            FLASH_CRITICAL_ENTER();
            //g_flash.result = FLASH_DRV_VerifySection(&flashSSDConfig, g_flash.addr, g_flash.size / FTFx_DPHRASE_SIZE, 1u);
            g_flash.result = g_pstFlashOptInfo->FLASH_VerifySection(0, g_flash.addr, g_flash.size / FLASH_SECTOR_SIZES, 0); ///<验证当前页是否擦除成功
            FLASH_CRITICAL_EXIT();
            
            #else
            /* Verify the erase operation at margin level value of 1, user read */
            //g_flash.result = FLASH_DRV_VerifySection(&flashSSDConfig, g_flash.addr, g_flash.size / FTFx_DPHRASE_SIZE, 1u);
            g_flash.result = g_pstFlashOptInfo->FLASH_VerifySection(0, g_flash.addr, g_flash.size / FLASH_SECTOR_SIZES, 0); ///<验证当前页是否擦除成功
            #endif
            //while( (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK) == 0);
            //DEV_ASSERT(STATUS_SUCCESS == g_flash.result);
            l_result = (status_t)g_flash.result;
        }
        #endif
    } else {
        l_result = STATUS_SUCCESS;
    }
    
    return l_result;
}
#ifdef __ICCARM__
u8 l_page[FLASH_P_PAGE_SIZES] @(0x20007FD0); //把 分配給eeprom 的拿來用
#else
u8 l_page[FLASH_P_PAGE_SIZES] __attribute__((at(0x20007FD0))); //把 分配給eeprom 的拿來用
#endif
MEMORY_MAP_RAM_FOR_ASIL status_t in_flash_wr_byte_for_boot(u32 phy_start_addr, uc8 *pBuffer, u16 numbers, cpu_wdg_t p_freewdg) {
    u32 failAddr;
    status_t l_result = STATUS_ERROR;
    //flash_callback_t pCallBack;
    FLASH_SR_ALLOC();
    u8 *p_buf;
    //u8 l_page[FLASH_P_PAGE_SIZES];
    u16 l_offset;
    u16 i;
    u32 checkValue;
    g_pstFlashOptInfo = (tFlashOptInfo *)flashDriverStartAdd;
    //checkValue = BL_MAKE_LOCAL_U32(FLASHIF_DRIVER_MAGIC_ADDRESS);   //因為是大端，所以直接讀取FLASHIF_DRIVER_MAGIC_ADDRESS地址上的值
    if (g_pstFlashOptInfo->magic ==
        FLASH_DRIVER_VERSION_MAGIC) { //   FLASHIF_DRIVER_MAGIC_VALUE  目前為了編譯g_FlashHeader 才改成這樣，如果是原來的要改回 FLASH_DRIVER_VERSION_MAGIC等
        //checkValue = BL_MAKE_LOCAL_U32(FLASHIF_DRIVER_VERSION_ADDRESS);
        if (g_pstFlashOptInfo->version == FLASH_DRIVER_VERSION_VERSION) { //    FLASHIF_DRIVER_VERSION_VALUE
            //
        } else {
            //l_result = STATUS_ERROR;
            return l_result;
        }
    } else {
        //l_result = STATUS_ERROR;
        return l_result;
    }
    
    g_flash.addr = phy_start_addr;
    g_flash.size = FLASH_P_PAGE_SIZES;
    //l_offset = numbers%FLASH_P_PAGE_SIZES;
    l_offset = numbers & (FLASH_P_PAGE_SIZES - 1); //前提是 numbers<FLASH_P_PAGE_SIZES
    if (l_offset == 0) {
        for (i = 0; i < FLASH_P_PAGE_SIZES; i++) {
            l_page[i] = pBuffer[i];
        }
        p_buf = (u8 *)&l_page[0];
        //p_buf =(u8*)pBuffer;
    } else {
        l_result = flash_drv_pflash_read(phy_start_addr, FLASH_P_PAGE_SIZES, &l_page[0]);
        for (i = 0; i < l_offset; i++) {
            l_page[i] = pBuffer[i];
        }
        p_buf = (u8 *)&l_page[0];
    }
    
    //p_buf =g_flash.w_data;
    #ifdef FLASH_CFG_EN_INT
    
    FLASH_CRITICAL_ENTER();
    wdog_task();
    //大概35ms時間 --4096字節
    //g_flash.result = FLASH_DRV_Program(&flashSSDConfig, g_flash.addr, g_flash.size, p_buf);
    g_flash.result = g_pstFlashOptInfo->FLASH_Program_page(&g_flash_ssd1, g_flash.addr, g_flash.size, p_buf); ///<验证当前页是否擦除成功
    FLASH_CRITICAL_EXIT();
    
    #else
    //g_flash.result = FLASH_DRV_Program(&flashSSDConfig, g_flash.addr, g_flash.size, p_buf);
    g_flash.result = g_pstFlashOptInfo->FLASH_Program_page(&g_flash_ssd1, g_flash.addr, g_flash.size, p_buf); ///<验证当前页是否擦除成功
    #endif
    //while( (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK) == 0);
    //DEV_ASSERT(STATUS_SUCCESS == g_flash.result);
    
    if (STATUS_SUCCESS == g_flash.result) {
        l_result = STATUS_SUCCESS;
    }
    #if 0
    if (l_result == STATUS_SUCCESS) {
        #ifdef FLASH_CFG_EN_INT
    
        FLASH_CRITICAL_ENTER();
        // FLASH_DRV_ProgramCheck 必須是 FLASH_DRV_Program 之後，而且w_data不變  大概75ms時間 --4096字節
        //g_flash.result = FLASH_DRV_ProgramCheck(&flashSSDConfig, g_flash.addr, g_flash.size, p_buf, &failAddr, 1u);
        g_flash.result = g_pstFlashOptInfo->FLASH_ProgramCheck(0,  g_flash.addr,  g_flash.size, p_buf, &failAddr, 0); ///<验证当前页是否擦除成功
        FLASH_CRITICAL_EXIT();
        
        #else
        //g_flash.result = FLASH_DRV_ProgramCheck(&flashSSDConfig, g_flash.addr, g_flash.size, p_buf, &failAddr, 1u);
        g_flash.result = g_pstFlashOptInfo->FLASH_ProgramCheck(0,  g_flash.addr,  g_flash.size, p_buf, &failAddr, 0); ///<验证当前页是否擦除成功
        #endif
        //g_flash.result = FLASH_DRV_ProgramCheck(&flashSSDConfig, g_flash.addr, g_flash.size, g_flash.w_data, &failAddr, 1u);
        //while( (FTFx_FSTAT & FTFx_FSTAT_CCIF_MASK) == 0);
        //DEV_ASSERT(STATUS_SUCCESS == g_flash.result);
        
        #if 0
        FLASH_TEST_GPIO_TOG1();
        flash_drv_pflash_read(g_flash.addr, g_flash.size, g_flash.r_data);
        FLASH_TEST_GPIO_TOG1();
        #endif
        
        l_result = g_flash.result;
    }
    #endif
    return l_result;
}


#if 1
#define NVM_FLASH_PAGE_SIZE 512u //字节
#define NVM_FLASH_SECTOR_SIZE 2048u //字节
#define NVM_FLASH_TEST_ADDR    0x00016000
#define TestPageNum         ((NVM_FLASH_TEST_ADDR - 0x00000000)/NVM_FLASH_SECTOR_SIZE)
void flash_test(void) {
    u16 i;
    u32 l_result;
    for (i = 0; i < 2048; i++) {
        g_flash.w_data[i] = (uint8_t)i + 0x77;
    }
    do {
        //g_pstFlashOptInfo = (tFlashOptInfo *)flashDriverStartAdd;
        
        TEST1_ON();
        g_flash.result =  g_pstFlashOptInfo->FLASH_EraseSector(&g_flash_ssd1, NVM_FLASH_TEST_ADDR, 0x800);   ///<擦除一个页，地址为当前页首地址
        //g_flash.result =  eFLASH_EraseSector(&g_flash_ssd1, NVM_FLASH_TEST_ADDR, 0x800);   ///<擦除一个页，地址为当前页首地址
        TEST1_OFF();
        if (g_flash.result != STATUS_SUCCESS) {
            //EFLASH_LockCtrl();
            g_flash.test[1]++;
            //break;
        }
        
        TEST1_ON();
        ///写操作，只可以写擦除过的地址，一次限制512字(2K byte)，buff必须四字节对齐。
        g_flash.result = g_pstFlashOptInfo->FLASH_Program_sector(&g_flash_ssd1, NVM_FLASH_TEST_ADDR, 2048,
                (u8 *)&g_flash.w_data[0]); ///<验证当前页是否擦除成功
        //g_flash.result = eFLASH_Program_page(&g_flash_ssd1, NVM_FLASH_TEST_ADDR, 512,(u8*)&g_flash.wr_data[0]); ///<验证当前页是否擦除成功
        TEST1_OFF();
        if (g_flash.result != STATUS_SUCCESS) {
            g_flash.test[3]++;
            //EFLASH_LockCtrl();
            //break;
        }
        
        for (i = 0; i < 2048; i++) {
            //g_flash.wr_data[i] = (uint8_t)i+0x92;
            g_flash.r_data[i] = 0u;
        }
        //讀取扇區
        TEST1_ON();
        flash_read_bytes(NVM_FLASH_TEST_ADDR, g_flash.r_data, 2048);
        TEST1_OFF();
    } while (0);
}
#endif




