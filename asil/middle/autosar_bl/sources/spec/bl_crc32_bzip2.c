/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file of a crc algorithm, this algorithm is used
 *              by BZIP2.
 *
 *  \file       bl_crc32_bzip2.c
 *  \ingroup    security_manager_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2018
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 25/03/2018  | xucong     | N/A | Boot010001
 *
 *****************************************************************************/

#include "bl_common.h"
#include "bl_security_funcfg.h"
#include "bl_crc32_bzip2.h"


/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 506,602,1252,1532,2100,2103,3212,3227 EOF*/
/*
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * 1252:suffixed integer constant implicitly converted to different integer.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 2100:Integral promotion : unsigned char promoted to signed int.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * 3212:This explicit cast is redundant and could be removed.
 * BL_BE32_TO_MCU is not explicit cast.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Macro
 *****************************************************************************/
#if ((SECM_CRC_WIDTH != 32u) || (SECM_CRC_VALUE_SIZE != 4u))
#error "This crc algorithm is not used for current security manager module."
#endif

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
#define BZIP2_CRC_SIZE_OPIMITE_MASK (0x0Fu)
#define BZIP2_CRC_SHIFT_BITS        (SECM_CRC_WIDTH - BZIP2_CALCULATED_WIDTH)

#define BZIP2_INIT_CRC_TABLE_MASK   (0x80000000UL)
#define BZIP2_INIT_CRC_TABLE_SHIFT  (24UL)
#define BZIP2_INIT_CRC_TABLE_BITS   (8UL)

#if (SECM_CRC_OPIMITE_BY_SIZE == BL_FUN_ON) //OFF
#define BZIP2_TABLE_SIZE            (16UL)
#define BZIP2_CALCULATED_WIDTH      (4UL)
#else
#define BZIP2_TABLE_SIZE            (256u)  //�x���@��
#define BZIP2_CALCULATED_WIDTH      (8u)
#endif


/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
static void _Bzip2_InitTable(void);
static void Crc32_ieee802_3_init(void);
/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
STATIC bl_u32_t gs_bzip2CrcRemainder;  //�������У�ĽY��
STATIC bl_u32_t gs_bzip2CrcTable[BZIP2_TABLE_SIZE];  //256
bl_u32_t crc_test;
/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
#define CRC32_ORG
#ifdef CRC32_ORG
//λ��ת
static bl_u32_t Crc32_ieee802_3_bitrev(bl_u32_t input, bl_u16_t bw)
{
	bl_u16_t i;
	bl_u32_t var = 0u;
	bl_u32_t l_input = input;
	for(i=0;i<bw;i++)
	{
		if( (l_input & 0x01U) !=0u)
		{
			var |= (((bl_u32_t)0x01UL)<<(bw-1u-i));
		}
		l_input>>=1;
	}
	return var;
}


//�������
//��:X32+X26+...X1+1,poly=(1<<26)|...|(1<<1)|(1<<0)
void Crc32_ieee802_3_init(void)
{
	bl_u16_t i;
	bl_u16_t j;
	bl_u32_t c, poly;
	poly = Crc32_ieee802_3_bitrev(CRC32_BZIP2_POLYNOMIAL,32);  //
	for(i=0u; i<256u; i++)
	{
		c = i;
		for (j=0u; j<8u; j++)
		{
			if((c&1u)!=0u)
			{
				c=poly^(c>>1);
			}
			else
			{
				c=c>>1;
			}
		}
		gs_bzip2CrcTable[i] = c;
	}
	//__asm( NOP);
}


#else

/**************************************************************************//**
 *
 *  \details Initialize the crc table of the bzip2 algorithm.
 *
 *  \since  V2.0.0
 * Ŀǰ����Ҳû���õ�
 *****************************************************************************/
static void _Bzip2_InitTable(void)
{
	bl_u32_t l_remainder;
	bl_u32_t dividend;
	bl_u8_t  bit;

	/* Compute the l_remainder of each possible dividend.*/
	for (dividend = 0; dividend < BZIP2_TABLE_SIZE; dividend++)
	{
		/* Start with the dividend followed by zeros.*/
		l_remainder = dividend << BZIP2_INIT_CRC_TABLE_SHIFT;

		/* Perform modulo-2 division, a bit at a time.*/
		for (bit = (bl_u8_t)BZIP2_INIT_CRC_TABLE_BITS; bit > 0u; --bit)
		{
			/* Try to divide the current data bit.*/
			if ((l_remainder & BZIP2_INIT_CRC_TABLE_MASK) != 0UL)
			{
				l_remainder = (l_remainder << 1) ^ CRC32_BZIP2_POLYNOMIAL;
			}
			else
			{
				l_remainder = (l_remainder << 1);
			}
		}

		/* Store the result into the table.*/
		gs_bzip2CrcTable[dividend] = l_remainder;
	}

	return ;
}


#endif

/**************************************************************************//**
 *
 *  \details Initialize the bzip2 crc32 algorithm.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Bzip2_Init(void)
{
#ifdef CRC32_ORG
	/* used for Geely DMS */ //����ת
    Crc32_ieee802_3_init();
#else
    _Bzip2_InitTable();    //����ת Bzip2
#endif
}

/**************************************************************************//**
 *
 *  \details Start the bzip2 crc32 algorithm.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Bzip2_Start(void)
{
	gs_bzip2CrcRemainder = CRC32_BZIP2_INIT_VALUE;
}

/**************************************************************************//**
 *
 *  \details Update the crc value.
 *
 *  \param[in]  size - the size of data.
 *  \param[in]  data - the value of data.
 *
 *  \return If the crc value is calculated successfully return BL_ERR_OK,
 *          otherwise crc value BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the crc value is calculated successfully.
 *  \retval BL_ERR_NOT_OK - the crc value is not calculated successfully.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Bzip2_Update(bl_Size_t size, const bl_Buffer_t *data)
{
	//bl_Buffer_t tmp;
	bl_Size_t i;
	bl_u32_t crc = gs_bzip2CrcRemainder;

	for (i = 0; i < size; i++)
	{
		#ifdef CRC32_ORG /* only used for Geely DMS */
			//tmp = (bl_Buffer_t)(crc ^ data[i]);    //tmp = (bl_Buffer_t)((crc ^ data[i]) & 0xffU);
          
			crc = ((crc >> 8) ^ gs_bzip2CrcTable[(crc ^ data[i]) & 0xffu]);
		#else
			#if (SECM_CRC_OPIMITE_BY_SIZE == BL_FUN_ON)
				tmp = (bl_Buffer_t)((crc >> BZIP2_CRC_SHIFT_BITS)
									^ ((data[i] >> BZIP2_CALCULATED_WIDTH)
									& BZIP2_CRC_SIZE_OPIMITE_MASK));
			#else
				tmp = (bl_Buffer_t)((crc >> BZIP2_CRC_SHIFT_BITS) ^ data[i]);
			#endif

			crc = (crc << BZIP2_CALCULATED_WIDTH) ^ gs_bzip2CrcTable[tmp];

			#if (SECM_CRC_OPIMITE_BY_SIZE == BL_FUN_ON)
				tmp = (bl_Buffer_t)((crc >> BZIP2_CRC_SHIFT_BITS)
									^ (data[i] & BZIP2_CRC_SIZE_OPIMITE_MASK));
				crc = (crc << BZIP2_CALCULATED_WIDTH) ^ gs_bzip2CrcTable[tmp];
			#endif
		#endif
	}
	gs_bzip2CrcRemainder = crc;
	return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details The algorithm to calculate the crc value.
 *
 *  \param[in]  size - the size of data.
 *  \param[in]  data - the value of data.
 *
 *  \return If the crc value is right return BL_ERR_OK, otherwise return
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the crc value is right.
 *  \retval BL_ERR_NOT_OK - the crc value is NOT right.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Bzip2_Verify(bl_Size_t size, const bl_Buffer_t *hash)
{
	bl_Return_t ret = BL_ERR_NOT_OK;
	bl_u32_t crc;
	bl_u32_t tranCrc;

	if (SECM_CRC_VALUE_SIZE == size)
	{
		/* Finish the crc algorithm.*/
		crc = gs_bzip2CrcRemainder ^ CRC32_BZIP2_FINAL_XOR_VALUE;
		/* Verify the crc value.*/
		tranCrc = BL_BE32_TO_MCU(hash);
		if (tranCrc == crc)
		{
			ret = BL_ERR_OK;
		}
	}

	return ret;
}


