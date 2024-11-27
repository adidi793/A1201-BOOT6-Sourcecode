/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**	Filename  : flash.h
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

#ifndef	__FLASH_H__
#define __FLASH_H__

#include "common_types.h"
#include "Cpu.h"
//#include "ac780x_eflash.h"
#include "status.h"
#include "common_cfg.h"
#include "bl_flash_if_funcfg.h"


typedef void(*cpu_wdg_t)(void);

#ifndef NULL_PTR
#define NULL_PTR ((void *)0)
#endif

#define FLASH_DRIVER_SECTOR_SIZE     FL_FLASH_SECTOR_SIZE_BYTE    //byte
#define FLASH_DRIVER_PAGE_SIZE     FL_FLASH_PGAE_SIZE_BYTE    //byte

#define FLASH_DRIVER_VERSION_TYPE       (0x8001u)
#define FLASH_DRIVER_VERSION_INDEX      (0x0000u)
#define FLASH_DRIVER_VERSION_MAGIC      (0x47313936UL)
#define FLASH_DRIVER_VERSION_VERSION    (0x00010000UL)

#define FLASHIF_DRIVER_MAGIC_ADDRESS        (FLASHIF_DRIVER_STARTADDRESS + 4u)
#define FLASHIF_DRIVER_VERSION_ADDRESS      (FLASHIF_DRIVER_STARTADDRESS + 8u)

typedef void (* flash_callback_t)(void);
typedef struct
{
		uint32_t PFlashBase;          /*!< The base address of P-Flash memory */
		uint32_t PFlashSize;          /*!< The size in byte of P-Flash memory */
		uint32_t DFlashBase;          /*!< For FlexNVM device, this is the base address of D-Flash memory (FlexNVM memory);
																	 *    For non-FlexNVM device, this field is unused */
		uint32_t DFlashSize;          /*!< For FlexNVM device, this is the size in byte of area
																	 *    which is used as D-Flash from FlexNVM memory;
																	 *    For non-FlexNVM device, this field is unused */
		uint32_t EERAMBase;           /*!< The base address of FlexRAM (for FlexNVM device)
																	 *    or acceleration RAM memory (for non-FlexNVM device) */
		uint32_t EEESize;             /*!< For FlexNVM device, this is the size in byte of EEPROM area which was partitioned
																	 *    from FlexRAM; For non-FlexNVM device, this field is unused */
		flash_callback_t CallBack;    /*!< Call back function to service the time critical events. Any code reachable from this function
																	 *   must not be placed in a Flash block targeted for a program/erase operation */
} flash_ssd_config_t;
typedef struct
{
		uint16_t type;          /**< The type of the flash driver.*/
		uint16_t index;         /**< The index of the flash driver.*/
		uint32_t magic;         /**< The magic value of the flash driver*/
		uint32_t version;       /**< The current of the flash driver.*/
		//status_t (*FLASH_EraseAllBlock)(const flash_ssd_config_t * pSSDConfig);
		//status_t (*FLASH_VerifyAllBlock)(const flash_ssd_config_t * pSSDConfig,uint8_t marginLevel);
		status_t (*FLASH_EraseSector)(const flash_ssd_config_t * pSSDConfig,uint32_t dest,uint32_t size);
		//status_t (*FLASH_VerifySection)(const flash_ssd_config_t * pSSDConfig,uint32_t dest,uint16_t number,uint8_t marginLevel);
		//void (*FLASH_EraseSuspend)(void);
		//void (*FLASH_EraseResume)(void);
		status_t (*FLASH_Program_page)(const flash_ssd_config_t * pSSDConfig,uint32_t dest, uint32_t size,const uint8_t * pData);
		status_t (*FLASH_ProgramCheck)(const flash_ssd_config_t * pSSDConfig,uint32_t dest,uint32_t size,const uint8_t * pExpectedData,uint32_t * pFailAddr,uint8_t marginLevel);
		status_t (*FLASH_Program_sector)(const flash_ssd_config_t * pSSDConfig,uint32_t dest, uint32_t size,const uint8_t * pData);
		status_t (*FLASH_Erase_page)(const flash_ssd_config_t * pSSDConfig, uint32_t dest,uint32_t size);
		//u8 (*FLASH_CommandSequence)(const flash_ssd_config_t * pSSDConfig);
		//status_t (*FLASH_VerifyBlock)(const flash_ssd_config_t * pSSDConfig,uint32_t dest,uint8_t marginLevel);
} tFlashOptInfo;


//extern const MEMORY_MAP_RAM_FLASH_DRV tFlashOptInfo g_stFlashOptInfo ;
//extern const flash_ssd_config_t g_flash_ssd;

#define flashDriverStartAdd      0x20007000  ///<eflash driver start addr
void flash_init(void);
status_t flash_drv_pflash_read(u32 addr,u32 len,u8 *p_data);
status_t in_flash_wr_byte_for_boot(u32 phy_start_addr,uc8* pBuffer,u16 numbers,cpu_wdg_t p_freewdg);
status_t FLASH_ErasePage_ext(uint32_t Page_Address,cpu_wdg_t p_freewdg);

#endif
