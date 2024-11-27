/**
 * @copyright: KUWE technologies Co.,Ltd. 2013-2015. all rights reserved.
 * @file: calibration.h
 * @brief: This file contains all the functions prototypes for the calibration.
 * @author: 
 * @version: V1.0.0
 * @date: 2016-03-31
 * @history:
 *      1.
 *	author:
 *	version:
 *	date:
 *	modification:
 */
#include "error.h"
#include "calibration.h"
#include "cpu.h"
#include "nvm.h"


//#include "nvm_drive.h"
//#include "EEPROM.h"
//#include "vehicle_config.h"
void cal_did_test(void);


#ifdef __ICCARM__
//110078207 0110078207/001
const u8 g_sysdid_f180[SYSDID_F180_CK_LEN] @SYSDID_F180_ADDR =
{
    0x30,0x31,0x31,0x30,0x30,0x37,0x38,0x32,0x30,0x37,0x00
};
#else
const u8 g_sysdid_f180[SYSDID_F180_CK_LEN] __attribute__((at(SYSDID_F180_ADDR))) =
{
    0x39,0x31,0x31,0x30,0x30,0x37,0x38,0x32,0x30,0x37,0x00
};
#endif

#ifdef __ICCARM__
//110078207 0110078207/001
const u8 g_sysdid_f181[SYSDID_F181_CK_LEN] @SYSDID_F181_ADDR =
#else
const u8 g_sysdid_f181[SYSDID_F181_CK_LEN] __attribute__((at(SYSDID_F181_ADDR))) =
#endif
{
    0x30,0x31,0x31,0x30,0x30,0x37,0x38,0x32,0x30,0x37,0x00
};


const did_attribute_t did_attribute_table[DID_ATTRIBUTE_TABLE_LENGTH] =
{
	{0xF180u, DID_TYPE_CONST,   READ_ONLY, (u32)&g_sysdid_f180[0], SYSDID_F180_CK_LEN},
	{0xF181u, DID_TYPE_CONST,   READ_ONLY, (u32)&g_sysdid_f181[0], SYSDID_F181_CK_LEN},
	{0xF1F2u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F1F2_ADDR, SYSDID_F1F2_CK_LEN},
	{0xF182u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F182_ADDR, SYSDID_F182_CK_LEN},
	{0xF187u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F187_ADDR, SYSDID_F187_CK_LEN},
	{0xF188u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F188_ADDR, SYSDID_F188_CK_LEN},
	{0xF18Au, DID_TYPE_CONST,   READ_ONLY, SYSDID_F18A_ADDR, SYSDID_F18A_CK_LEN},
	{0xF18Bu, DID_TYPE_CONST,   READ_ONLY, SYSDID_F18B_ADDR, SYSDID_F18B_CK_LEN},
	{0xF18Cu, DID_TYPE_VOLATILE,   READ_AND_WRITE, SYSDID_ECU_SN_ADDR, SYSDID_ECU_SN_CK_LEN},
	{0xF191u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F191_ADDR, SYSDID_F191_CK_LEN},
	{0xF192u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F192_ADDR, SYSDID_F192_CK_LEN},
	{0xF193u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F193_ADDR, SYSDID_F193_CK_LEN},
	{0xF194u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F194_ADDR, SYSDID_F194_CK_LEN},
	{0xF195u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F195_ADDR, SYSDID_F195_CK_LEN},    //(uint32_t)&g_sysdid_f195[0]
	{0xF196u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F196_ADDR, SYSDID_F196_CK_LEN},    //(uint32_t)&g_sysdid_f195[0]
	{0xF1F1u, DID_TYPE_CONST,   READ_ONLY, SYSDID_F1F1_ADDR, SYSDID_F1F1_CK_LEN},
	{0xF19eu, DID_TYPE_CONST,   READ_ONLY, SYSDID_F19E_ADDR, SYSDID_F19E_CK_LEN},
	{0xF1A0u, DID_TYPE_VOLATILE,   READ_AND_WRITE, SYSDID_VEHICLE_CONFIG_ADDR, SYSDID_VEHICLE_CONFIG_CK_LEN},
	{0x01A0u, DID_TYPE_VOLATILE,   READ_AND_WRITE, SYSDID_MANUFAC_LIFECYCLE_COUNT_ADDR, SYSDID_MANUFAC_LIFECYCLE_COUNT_LEN},
	{0x01B0u, DID_TYPE_VOLATILE,   WRITE_ONLY, SYSDID_SECURITY_KEY_ADDR, SYSDID_SECURITY_KEY_CK_LEN},
	{0xF190u, DID_TYPE_VOLATILE,   READ_AND_WRITE, SYSDID_F190_VIN_ADDR, SYSDID_F190_VIN_CK_LEN},
	{0xF198u, DID_TYPE_VOLATILE,   READ_ONLY, SYSDID_F198_REPAIR_SN_ADDR, SYSDID_F198_REPAIR_SN_CK_LEN},
	{0xF199u, DID_TYPE_VOLATILE,   READ_ONLY, SYSDID_F199_DATE_ADDR, SYSDID_F199_DATE_CK_LEN},
	{0xF100u, DID_TYPE_VOLATILE,   READ_ONLY, SYSDID_ATTEMP_ADDR, SYSDID_ATTEMP_LEN+SYSDID_ATTEMP_MAX_LEN}
};



static uint8_t get_did_headler(did_t did)

{
	uint8_t i;
	u8 l_result =0xffu;
	for(i = 0; i < DID_ATTRIBUTE_TABLE_LENGTH; i++)
	{
		if(did == did_attribute_table[i].id)
		{
			l_result = i;
			break;
		}
	}
	return l_result;
}

u8 cal_read_did(did_t did, uint8_t *p_buf, uint8_t length)
{
	uint8_t headler;
	uint8_t opt_length;
	u8 l_result = TRUE;

	CAL_CPU_SR_ALLOC();

	do
	{
		headler = get_did_headler(did) ;
		if( (headler == 0xffu)  || (did_attribute_table[headler].rw == WRITE_ONLY) )
		{
			l_result =  FALSE;
			break;
		}

		if(length < did_attribute_table[headler].length)
		{
			opt_length = length;
		}
		else
		{
			opt_length = did_attribute_table[headler].length;
		}

		CAL_ENTER_CRITICAL();
		#if 0
		for(i = 0; i < opt_length; i++)
		{
			p_buf[i]  = did_attribute_table[headler].addr[i];
		}
		#endif
		if(did_attribute_table[headler].type == DID_TYPE_CONST)
		{
			(void)nvm_read_bytes(NVM_TYPE_FLASH,did_attribute_table[headler].phy_addr,p_buf,opt_length);
		}
		else
		{
			(void)nvm_read_bytes(NVM_TYPE_EEPROM,did_attribute_table[headler].phy_addr,p_buf,opt_length);
		}
		CAL_EXIT_CRITICAL();
	}while(0);

	return l_result;
}

volatile uint16_t test1;
u8 cal_write_did(did_t did, const uint8_t *p_buf, uint8_t length)
{
	uint8_t l_result = FALSE;
	uint8_t headler;
	uint8_t opt_length;
	CAL_CPU_SR_ALLOC();

	do
	{
		headler = get_did_headler(did) ;
		if( (headler == 0xffu) || (did_attribute_table[headler].rw == READ_ONLY))
		{
			l_result =  FALSE;
			break;
		}

		if(length < did_attribute_table[headler].length)
		{
			opt_length = length;
		}
		else
		{
			opt_length = did_attribute_table[headler].length;
		}
		CAL_ENTER_CRITICAL();
		#if 0
		for(i = 0; i < opt_length; i++)
		{
			did_attribute_table[headler].addr[i] = p_buf[i];
		}
		#endif
		if(did_attribute_table[headler].type == DID_TYPE_CONST)
		{
			l_result = TRUE;
			//(void)nvm_write_bytes(NVM_TYPE_FLASH,did_attribute_table[headler].phy_addr,p_buf,opt_length);
		}
		else
		{
			l_result = nvm_write_bytes(NVM_TYPE_EEPROM,did_attribute_table[headler].phy_addr,(u8*)&p_buf[0],opt_length);
			if(ERR_OK == l_result)
			{
				l_result = TRUE;
			}
		}
		CAL_EXIT_CRITICAL();
	}while(0);

	return l_result;
}

void cal_did_init(void)
{
	uint8_t  i;
	u8 j;
	u16_swap_t l_buf[8];
	u8 l_data[64];

	CAL_CPU_SR_ALLOC();

	for(i = 0; i < DID_ATTRIBUTE_TABLE_LENGTH; i++)
	{

		#ifdef FUN_WDT_ENABLE
		wdt_clr();
		#endif

		if(did_attribute_table[ i ].type == DID_TYPE_CONST)
		{

			continue;
		}
		CAL_ENTER_CRITICAL();

		CAL_EXIT_CRITICAL();


	}

	//cal_did_test();
}


static u8 g_cal_refresh_flag = 0;


/*讀取數據之後的更新*/
void cal_did_read_refresh(void)
{
	#if 0
	u8 j;
	u16_swap_t l_buf[16]={0};

	if(g_cal_refresh_flag ==0)
	{
		g_cal_refresh_flag = 1;

		#ifdef FUN_WDT_ENABLE
		wdt_clr();
		#endif

		#if 1
		/*F180*/
		for(j=0;j<8;j++)
		{
			#ifdef CAL_USE_NVM
			(void)eeprom_get_u16((eeprom_addr_t)(EEPROM_DATA_BASE_ADDR+EEPROM_F180_ADDR+2*j), &l_buf[j].w);
			#endif
			volatile_did.boot_soft_id[2*j] = l_buf[j].w >> 8;
			volatile_did.boot_soft_id[2*j+1] = l_buf[j].w;
		}
		#endif

		#if 1
		/*F187*/
		for(j=0;j<5;j++)
		{
			l_buf[j].w = (u16)const_did.geely_spare_part_number[2*j] << 8 | const_did.geely_spare_part_number[2*j+1] ;
			#ifdef CAL_USE_NVM
			(void)eeprom_set_u16((eeprom_addr_t)(EEPROM_DATA_BASE_ADDR+EEPROM_F187_ADDR+2*j), l_buf[j].w);
			#endif
		}
		#endif

		#if 1
		/*F189*/
		for(j=0;j<8;j++)
		{
			l_buf[j].w = (u16)const_did.ecu_software_version_number[2*j] << 8 | const_did.ecu_software_version_number[2*j+1] ;
			#ifdef CAL_USE_NVM
			(void)eeprom_set_u16((eeprom_addr_t)(EEPROM_DATA_BASE_ADDR+EEPROM_F189_ADDR+2*j), l_buf[j].w);
			#endif
		}
		#endif

		#if 1

		/*F193*/
		for(j=0;j<8;j++)
		{
			l_buf[j].w = (u16)const_did.system_supplier_ecu_hardware_version_number[2*j] << 8 | const_did.system_supplier_ecu_hardware_version_number[2*j+1] ;
			#ifdef CAL_USE_NVM
			(void)eeprom_set_u16((eeprom_addr_t)(EEPROM_DATA_BASE_ADDR+EEPROM_F193_ADDR+2*j), l_buf[j].w);
			#endif
		}
		#endif


	}
	#endif
}



/** 判斷數組的值是否都為 value;
 * @param  Pdata內容
 * @param
 * @param
 * @return
 */
u8 buf_judge_value_is_same(uc8 *pdata,u8 len ,u8 value)
{

	u8 l_result = TRUE;

	u8 i;

	for(i=0;i<len;i++)
	{
		if( pdata[i] != value )
		{
			l_result = FALSE;
			break;
		}
	}

	return l_result;

}

//u8 *g_cal_paddr;
volatile u8 g_cal_w_buf[50];
volatile u8 g_cal_r_buf[50];
void cal_did_test(void)
{
	//g_cal_paddr = (u8*)&g_sysdid_f195[0];
	cal_read_did(0xF180u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF100u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF181u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF1F2u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF182u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF187u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF188u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF18au,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF18bu,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF18cu,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF191u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF192u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF193u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF194u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF195u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF1f1u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF19eu,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF190u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF190u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF198u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF199u,(u8*)&g_cal_r_buf[0],50);
	cal_read_did(0xF195u,(u8*)&g_cal_w_buf[0],50);
	cal_write_did(0xF190u,(u8*)&g_cal_w_buf[0],50);
	cal_read_did(0xF190u,(u8*)&g_cal_r_buf[0],50);
	//(void)nvm_read_bytes(NVM_TYPE_FLASH,(u32)&g_cal_paddr,&g_cal_r_buf[0],SYSDID_F195_CK_LEN);
	//(void)nvm_read_bytes(NVM_TYPE_FLASH,(u32)&g_sysdid_f195[0],&g_cal_w_buf[0],SYSDID_F195_CK_LEN);
}


/*PRQA S 1503*/
