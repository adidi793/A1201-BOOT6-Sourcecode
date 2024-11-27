/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the security manager module.
 *
 *  \file       bl_security.c
 *  \ingroup    security_manager_module
 *  \author     
 *              
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 25/03/2010 | hewei.zhang      | N/A | Boot010001
 *      01.01.00 | 19/05/2010 | hewei.zhang      | N/A | Boot010002
 *      02.00.00 | 18/05/2011 | mingqing.tang    | N/A | Boot020001
 *      02.01.00 | 15/06/2011 | mingqing.tang    | N/A | Boot020002
 *      02.05.00 | 22/06/2011 | mingqing.tang    | N/A | Boot020006
 *      05.00.00 | 19/06/2013 | xin.shang        | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_common.h"
#include "bl_security.h"
#include "bl_security_cfg.h"
#include "bl_access_udsdemo.h"
/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1252,1532,2103,3112,3227 EOF*/
/*
 * 1252:suffixed integer constant implicitly converted to different integer.
 *
 * 1532:The function is only referenced in one translation unit.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Macro
 *****************************************************************************/
#if ((SECM_ACCESS_SEED_SIZE == 0u) || (SECM_ACCESS_KEY_SIZE == 0u))
#error "the size of the seed and the key is failure."
#endif

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Initialize the security manager module.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Secm_Init(void)
{
    BL_DEBUG_ASSERT_NO_RET(g_VerificationOps.Init != BL_NULL_PTR);
    BL_DEBUG_ASSERT_NO_RET(g_SecurityAccessOps.Init != BL_NULL_PTR);

    g_VerificationOps.Init();

    g_SecurityAccessOps.Init();

    return ;
}

/**************************************************************************//**
 *
 *  \details This function use the current seed to make a new seed.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Secm_MakeNewSeed(void)
{
    BL_DEBUG_ASSERT_NO_RET(g_SecurityAccessOps.MakeNewSeed != BL_NULL_PTR);

    g_SecurityAccessOps.MakeNewSeed();

    return ;
}

/**************************************************************************//**
 *
 *  \details Get a seed of security access.
 *
 *  \param[out] seed - the pointer of a buffer used to save the seed.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Secm_GetSeed(bl_Buffer_t *seed)
{
    BL_DEBUG_ASSERT_NO_RET(g_SecurityAccessOps.GetSeed != BL_NULL_PTR);
    BL_DEBUG_ASSERT_NO_RET(seed != BL_NULL_PTR);

    g_SecurityAccessOps.GetSeed(seed);

    return ;
}
////////////////////////////////////////////////////////////////////////////////////

/**************************************************************************//**
 *
 *  \details An algorithm is used to calculate the key by a seed.
 *
 *  \return the key value
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
  bl_u32_t u32seedtokey0 = 0; 
bl_u32_t _Acc_UdsDemoCalculateKey(void)
{
	
	bl_u8_t     MagicSeed[4] = {0};
        bl_u32_t    SaccSeed=0;
        bl_u32_t    KeyResult=0;
        bl_u32_t    SaccKey;	
	static const bl_u32_t KeyK= 0x00000017;

	Acc_UdsDemoGetSeed(MagicSeed);
        SaccSeed=MagicSeed[0];
        SaccSeed<<=8;
        SaccSeed|=MagicSeed[1];
         SaccSeed<<=8;
        SaccSeed|=MagicSeed[2];
         SaccSeed<<=8;
        SaccSeed|=MagicSeed[3];	
	KeyResult = ((((SaccSeed >> 1) ^ SaccSeed) <<3) ^ (SaccSeed >>2));
        SaccKey = KeyResult ^ KeyK;
       u32seedtokey0=SaccKey;
	return SaccKey;

}
/**************************************************************************//**
 *
 *  \details Verify the result of security access.
 *
 *  \param[in] size - the size of key.
 *  \param[in] key  - the value of key.
 *
 *  \return If the key is verified successfully return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the result is verified successfully.
 *  \retval BL_ERR_NOT_OK - the result is verified failed.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Secm_VerifyKey(bl_Size_t size, const bl_Buffer_t *key)
{
    bl_Return_t ret = BL_ERR_NOT_OK;   
    BL_DEBUG_ASSERT_CONFIG(g_SecurityAccessOps.VerifyKey != BL_NULL_PTR);
    BL_DEBUG_ASSERT_PARAM(key != BL_NULL_PTR);

    if (SECM_ACCESS_KEY_SIZE == size) //EM3 是16字节
    {
        ret = g_SecurityAccessOps.VerifyKey(key);  // 就是函数 Acc_UdsDemoVerifyKey
      
    }

    return ret;
}

/**************************************************************************//**
 *
 *  \details Start a hash algorithm used to verify the data whether are valid.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Secm_StartHash(void)
{
    BL_DEBUG_ASSERT_NO_RET(g_VerificationOps.Start != BL_NULL_PTR);

    g_VerificationOps.Start();   //Bzip2_Starts

    return ;
}


/**************************************************************************//**
 *
 *  \details The data are used to update the hash value.
 *
 *  \param[in]  size - the size of data.
 *  \param[in]  data - the value of data.
 *
 *  \return If the hash value is calculated successfully return BL_ERR_OK,
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the hash value is calculated successfully.
 *  \retval BL_ERR_NOT_OK - the hash value is not calculated successfully.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Secm_UpdateHash(bl_Size_t size, const bl_Buffer_t *data)
{
    bl_Return_t ret;

    BL_DEBUG_ASSERT_CONFIG(g_VerificationOps.Update != BL_NULL_PTR);
    BL_DEBUG_ASSERT_PARAM(data != BL_NULL_PTR);
    BL_DEBUG_ASSERT_PARAM(size != 0);

    ret = g_VerificationOps.Update(size,data);   // Bzip2_Update

    return ret;
}

/**************************************************************************//**
 *
 *  \details Verify the value of transmitted hash whether is right.
 *
 *  \pre when the crc algorithm is used, the value of hash must be BIG endian.
 *
 *  \param[in]  size - the size of transmitted hash.
 *  \param[in]  hash - the value of transmitted hash.
 *
 *  \return If the transmitted hash value is right return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the transmitted hash value is right.
 *  \retval BL_ERR_NOT_OK - the transmitted hash value is NOT right.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Secm_VerifyHash(bl_Size_t size, const bl_Buffer_t *hash)
{

    bl_Return_t ret;

    BL_DEBUG_ASSERT_CONFIG(g_VerificationOps.Verify != BL_NULL_PTR);
    BL_DEBUG_ASSERT_PARAM(hash != BL_NULL_PTR);
    BL_DEBUG_ASSERT_PARAM(size != 0);

    ret = g_VerificationOps.Verify(size,hash);  //校驗  hash   Bzip2_Verify


    return ret;
}

#if ((SECM_SECURITY_CLASS == BL_SECURITY_CLASS_C) \
    || (SECM_SECURITY_CLASS == BL_SECURITY_CLASS_CCC))
/**************************************************************************//**
 *
 *  \details In HIS spec, the contents of verificated data contain the address
 *           and the size of one segment when the security level is class C or
 *           class CCC. The address and size are FOUR bytes.
 *           The data of one segment which has to be hashed is schematically
 *           illustrated in the following way:
 *           Sx =  HAdr || ... || LAdr ||  HLen || ... || LLen || Segment data
 *
 *           If the binary representation of the start address or the length of
 *           one segment is less then four bytes the data is padded to four
 *           bytes with 0x00 in front of the high byte.
 *
 *  \param[in]  addr - the start address of a downloaded segment.
 *  \param[in]  size - the size of a downloaded segment.
 *
 *  \return If the hash value is calculated successfully return BL_ERR_OK,
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the hash value is updated successfully.
 *  \retval BL_ERR_NOT_OK - the hash value is not updated successfully.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
bl_Return_t Secm_UpdateHashByAddr(bl_Address_t addr, bl_Size_t size)
{
    bl_Return_t ret;
    bl_Size_t bufSize = sizeof(bl_Address_t) + sizeof(bl_Size_t);
    bl_Buffer_t buf[sizeof(bl_Address_t) + sizeof(bl_Size_t)];

    BL_DEBUG_ASSERT_CONFIG(g_VerificationOps.Update != BL_NULL_PTR);
    BL_DEBUG_ASSERT_CONFIG(sizeof(bl_Address_t) == 4);
    BL_DEBUG_ASSERT_CONFIG(sizeof(bl_Size_t) == 4);

    /*copy the contents of start address and length to a buffer.*/
    BL_MCU_TO_BE32(buf, addr);
    BL_MCU_TO_BE32(&buf[sizeof(bl_Address_t)], size);

    ret = g_VerificationOps.Update(bufSize,buf);

    return ret;
}

#endif

