/**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
#include "error.h"
#include "gpio.h"
#include "nvm.h"
#include "Cpu.h"





#ifdef NVM_FLASH_DEBUG_EN

#define NVM_FLASH_PAGE_SIZE 512u //字节
#define NVM_FLASH_SECTOR_SIZE 2048u //字节
#define NVM_FLASH_TEST_ADDR    0x00016000
#define TestPageNum         ((NVM_FLASH_TEST_ADDR - 0x00000000)/NVM_FLASH_SECTOR_SIZE)

#pragma pack(4)
typedef struct
{
	u8 state;
	u8 read_project;
	u8 write_project;
	u8 result;
	u16 page_id;
	u32 addr;
	u8 wr_data[NVM_FLASH_SECTOR_SIZE];
	u8 rd_data[NVM_FLASH_SECTOR_SIZE];
	u8 test[20];
}nvm_flash_test_s;
#pragma pack()


nvm_flash_test_s  g_nvm_flash_test;


void nvm_falsh_init(void)
{
	//
}


void nvm_flash_test(void)
{
	#if 1
	u16 i;

	for (i=0;i<NVM_FLASH_SECTOR_SIZE;i++)
	{
		g_nvm_flash_test.rd_data[i] = 0x0u;
		g_nvm_flash_test.wr_data[i] = (uint8_t)i+0x22;
	}

	TEST1_ON();
	g_nvm_flash_test.result=flash_read_bytes(NVM_FLASH_TEST_ADDR,g_nvm_flash_test.rd_data,NVM_FLASH_SECTOR_SIZE);
	TEST1_OFF();
	if(g_nvm_flash_test.state != 0u)
	{
		 //return;
	}
	TEST1_ON();
	g_nvm_flash_test.result=FL_FLASH_PageErase(FLASH, NVM_FLASH_TEST_ADDR);
	TEST1_OFF();
	TEST1_ON();
	g_nvm_flash_test.result=FL_FLASH_Program_Page(FLASH, NVM_FLASH_TEST_ADDR / FL_FLASH_PGAE_SIZE_BYTE, (u32*)&g_nvm_flash_test.wr_data);
	TEST1_OFF();
	for (i=0;i<NVM_FLASH_SECTOR_SIZE;i++)
	{
		g_nvm_flash_test.rd_data[i] = 0x0u;
	}
	TEST1_ON();
	g_nvm_flash_test.result=flash_read_bytes(NVM_FLASH_TEST_ADDR,g_nvm_flash_test.rd_data,NVM_FLASH_SECTOR_SIZE);
	TEST1_OFF();

	TEST1_ON();
	FL_FLASH_SectorErase(FLASH, NVM_FLASH_TEST_ADDR);
	TEST1_OFF();
	for (i=0;i<NVM_FLASH_SECTOR_SIZE;i++)
	{
		g_nvm_flash_test.rd_data[i] = 0x0u;
	}
	TEST1_ON();
	g_nvm_flash_test.result=flash_read_bytes(NVM_FLASH_TEST_ADDR,g_nvm_flash_test.rd_data,NVM_FLASH_SECTOR_SIZE);
	TEST1_OFF();

	for (i=0;i<NVM_FLASH_SECTOR_SIZE;i++)
	{
		g_nvm_flash_test.wr_data[i] = (uint8_t)i+0x44;
	}

	TEST1_ON();
	FL_FLASH_Program_Sector(FLASH, NVM_FLASH_TEST_ADDR / FL_FLASH_SECTOR_SIZE_BYTE, (u32*)&g_nvm_flash_test.wr_data);
	TEST1_OFF();

	for (i=0;i<NVM_FLASH_SECTOR_SIZE;i++)
	{
		g_nvm_flash_test.rd_data[i] = 0x0u;
	}
	TEST1_ON();
	g_nvm_flash_test.result=flash_read_bytes(NVM_FLASH_TEST_ADDR,g_nvm_flash_test.rd_data,NVM_FLASH_SECTOR_SIZE);
	TEST1_OFF();

	#endif
}





#endif

