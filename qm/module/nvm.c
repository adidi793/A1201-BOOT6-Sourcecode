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
//#include "eeprom.h"
#include "nvm.h"


#if 0
typedef struct
{
	u8 write_len;
	u8 read_len;
	u8 write_data[20];
	u8 read_data[20];
	u8 result ;
}nvm_test_s;
nvm_test_s g_nvm_test={0};
#endif


/**
	* @brief  ram_read_bytes
	* @param input none
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
u8 ram_read_bytes(u32 addr,u8 *p_data,u16 len)
{
	u8 l_result;
	u8 *p_src;
	u16 i;

	if( (addr<RAM_START_ADDR) || (addr>RAM_END_ADDR) || ((addr+len)>(RAM_END_ADDR+1)) )
	{
		l_result = ERR_FAILED;
	}
	else
	{
		p_src = (u8 *)(addr);

		for (i = 0; i < len; i++)
		{
			p_data[i] = p_src[i]; /*lint !e511*/
		}

		l_result = ERR_OK;
	}

	return l_result;
}

/**
	* @brief  ram_write_bytes
	* @param input none
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others  主要用于 program flag
	*/
u8 ram_write_bytes(u32 addr,u8 *p_data,u16 len)
{
	u8 l_result;
	u8 *p_addr;
	u16 i;

	if( (addr<RAM_START_ADDR) || (addr>RAM_END_ADDR) || ((addr+len)>(RAM_END_ADDR+1)) )
	{
		l_result = ERR_FAILED;
	}
	else
	{
		p_addr = (u8 *)(addr);

		for (i = 0; i < len; i++)
		{
			p_addr[i] = p_data[i] ;  /*lint !e511*/
		}

		l_result = ERR_OK;
	}

	return l_result;
}

/**
	* @brief  flash_read_bytes
	* @param input none
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
u8 flash_read_bytes(u32 addr,u8 *p_data,u16 len)
{
	u8 l_result;
	u8 *p_src;
	u16 i;

	if( (addr<FLASH_START_ADDR) || (addr>FLASH_END_ADDR) || ((addr+len)>(FLASH_END_ADDR+1)) )
	{
		l_result = ERR_FAILED;
	}
	else
	{
		p_src = (u8 *)(addr);

		for (i = 0; i < len; i++)
		{
			p_data[i] = p_src[i]; /*lint !e511*/
		}

		l_result = ERR_OK;
	}

	return l_result;
}

/**
	* @brief  nvm_init
	* @param input none
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void nvm_init(void)
{
	//u8 i;
	flash_init();
	#ifdef NVM_EEPROM_EN
	eeprom_init();
	#endif

	//nvm_eeprom_init();

	//eeprom_ext_init();
	#if 0
	g_nvm_test.result = nvm_read_bytes(NVM_TYPE_EEPROM,0x100300,&g_nvm_test.read_data[0],16);
	for(i=0;i<20;i++)
	{
		g_nvm_test.read_data[i] = 0;
		g_nvm_test.write_data[i] = 0x36+i;
	}
	g_nvm_test.result = nvm_write_bytes(NVM_TYPE_EEPROM,0x100300,&g_nvm_test.write_data[0],16);
	g_nvm_test.result = nvm_read_bytes(NVM_TYPE_EEPROM,0x100300,&g_nvm_test.read_data[0],16);

	g_nvm_test.result = nvm_read_bytes(NVM_TYPE_FLASH,0xfe0000,&g_nvm_test.read_data[0],16);
	g_nvm_test.result = nvm_read_bytes(NVM_TYPE_RAM,0x001000ul,&g_nvm_test.read_data[0],8);
	for(i=0;i<20;i++)
	{
		g_nvm_test.write_data[i] = 0x36+i;
	}
	g_nvm_test.result = nvm_write_bytes(NVM_TYPE_RAM,0x001000ul,&g_nvm_test.write_data[0],8);
	g_nvm_test.result = nvm_read_bytes(NVM_TYPE_RAM,0x001000ul,&g_nvm_test.read_data[0],8);
	#endif
}


/**
	* @brief  nvm_task
	* @param input none
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void nvm_task(void)
{
	#ifdef EEPROM_STATE_MACHINE_EN
	eeprom_task();
	#endif
}




/**
	* @brief  nvm_read_bytes
	* @param input none
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
u8 nvm_read_bytes(nvm_type_e type,u32 addr, u8 *p_data, u16 num)
{
	u8 l_result = ERR_FAILED;
	switch(type)
	{
		case NVM_TYPE_EEPROM:
			#ifdef NVM_EEPROM_EN
			l_result =eeprom_read_u8_ext(addr,p_data,num);
			#endif
		break;
		case NVM_TYPE_RAM:
			l_result =ram_read_bytes(addr,p_data,num);
		break;
		case NVM_TYPE_FLASH:
			l_result =flash_read_bytes(addr,p_data,num);
		break;
		default:
			//code
		break;
	}
	return l_result;
}

/**
	* @brief  nvm_write_bytes
	* @param input none
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
u8 nvm_write_bytes(nvm_type_e type,u32 addr, u8 *p_data, u16 num)
{
	u8 l_result = ERR_FAILED;
	switch(type)
	{
		case NVM_TYPE_EEPROM:
			#ifdef NVM_EEPROM_EN
			l_result =eeprom_write_u8_ext(addr,p_data,num);
			#endif
		break;
		case NVM_TYPE_RAM:
			l_result =ram_write_bytes(addr,p_data,num);
		break;
		case NVM_TYPE_FLASH:
			//l_result =flash_read_bytes(addr,p_data,num);
		break;
		default:
			//code
		break;
	}
	return l_result;
}


