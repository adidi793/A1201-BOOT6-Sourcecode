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
#ifndef _CRC_H_
#define _CRC_H_

#include "common_types.h"
#include "common_cfg.h"

//#define CRC8_ID_FOR_SBC 0
#define CRC8_ID_FOR_E2E_L1 0u
#define CRC8_ID_FOR_E2E_L2 1u

#define CRC8_ID_MAX 2u

    //{8, 0x1d, 0x00, 0,  0,   0x00},    //CRC8_SAE J1850
#define CRC8_L1_INIT_VALUE 0XFFu  //调试的板子是H
#define CRC8_L1_POLY_VALUE 0x1Du
#define CRC8_L1_XOR_VALUE 0XFFu

#define CRC8_L2_INIT_VALUE 0X00u  //调试的板子是H
#define CRC8_L2_POLY_VALUE 0x1Du
#define CRC8_L2_XOR_VALUE 0X00u

#define CRC_TABLE_IS_CONST  1u   //crc 表，固定数组值


#define CRC_USE_TABLE 1   //利用生成表格
void crc8_table_init(u8 id);
u8 crc8_make_by_table(u8 id,u8 *p_data,u8 len);
u8 crc8_table_set_init_value(u8 id,u8 value);
#if 0
void crc_init(void);
#endif


#endif
