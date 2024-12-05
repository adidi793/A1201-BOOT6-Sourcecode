/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a source file to make seed and verify a key.
 *
 *  \file       bl_access_udsdemo.c
 *  \ingroup    security_manager_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2018
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 25/03/2018 | xucong      | N/A | Boot010001
 *
 *****************************************************************************/
#include "bl_common.h"
#include "bl_security_funcfg.h"
#include "bl_access_udsdemo.h"
#include "bl_adpt_uds_platform.h"
#include "bl_dcm_cfg.h"
#include "bl_crc32_bzip2.h"
#include "bl_timer.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 602,1532,3212,3227,3453 EOF*/
/*
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 3212:This explicit cast is redundant and could be removed.
 * ACC_UDSDEMO_KEY_FROM_BUF and ACC_UDSDEMO_SEED_TO_BUF are not explicit cast.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 * Acc_UdsDemoGetSeed, the seed will be modify.
 * Acc_UdsDemoVerifyKey, the key is declared with const.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * ACC_UDSDEMO_KEY_FROM_BUF macro do not have side-effect.
 * ACC_UDSDEMO_SEED_TO_BUF macro do not have side-effect.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Macro
 *****************************************************************************/
#if ((SECM_ACCESS_KEY_SIZE != 4u) && (SECM_ACCESS_KEY_SIZE != 2u))
#error "The size of a key is only four or two, other values are invalid."
#endif

#if ((SECM_ACCESS_SEED_SIZE != 4u) && (SECM_ACCESS_SEED_SIZE != 2u))
#error "The size of a seed is only four or two, other values are invalid."
#endif
/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
#if (SECM_ACCESS_KEY_SIZE == 4u)    //進入該選項
#define ACC_UDSDEMO_KEY_MASK                (0xA5CEFDB6UL)
#define ACC_UDSDEMO_KEY_FROM_BUF(buf)       BL_BE32_TO_MCU(buf)   //buf = * buf
#elif (SECM_ACCESS_KEY_SIZE == 2u)
#define ACC_UDSDEMO_KEY_MASK                (0xDFB6u)
#define ACC_UDSDEMO_KEY_FROM_BUF(buf)       BL_BE16_TO_MCU(buf)
#endif

#if (SECM_ACCESS_SEED_SIZE == 4u)  //進入該選項
/** \brief It is used to calculate the random seed*/
#define ACC_UDSDEMO_RANDOM_SEED             (16807UL)
#define ACC_UDSDEMO_SEED_INITVALUE          (0x12350012UL)
#define ACC_UDSDEMO_SEED_TO_BUF(seed,buf)   BL_MCU_TO_BE32(buf,seed)  //*buf = seed
#elif (SECM_ACCESS_SEED_SIZE == 2u)
/** \brief It is used to calculate the random seed*/
#define ACC_UDSDEMO_RANDOM_SEED             (16807u)
#define ACC_UDSDEMO_SEED_INITVALUE          (0x1235u)
#define ACC_UDSDEMO_SEED_TO_BUF(seed,buf)   BL_MCU_TO_BE16(buf,seed)
#endif

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/
#if (SECM_ACCESS_SEED_SIZE == 4u)   //是該選項
typedef bl_u32_t bl_UdsDemoSeed_t;
#elif (SECM_ACCESS_SEED_SIZE == 2u)
typedef bl_u16_t bl_UdsDemoSeed_t;
#endif

#if (SECM_ACCESS_KEY_SIZE == 4u)   //是該選項
typedef bl_u32_t bl_UdsDemoKey_t;
#elif (SECM_ACCESS_KEY_SIZE == 2u)
typedef bl_u16_t bl_UdsDemoKey_t;
#endif

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
/** \brief Calculate the key from the gs_UdsDemoSeed.*/
static bl_UdsDemoKey_t _Acc_UdsDemoCalculateKey(void);

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
STATIC bl_UdsDemoSeed_t gs_UdsDemoSeed;  //u32

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Initialize the DEMO security access used for UDS.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Acc_UdsDemoInit(void) {
    gs_UdsDemoSeed = ACC_UDSDEMO_SEED_INITVALUE;
    
    return ;
}

/* -----------------------------------------------------------------------A12
重新上下电后请求种子，Boot下种子值一直同一个，未随机，APP下无此问题
由于是靠时间获取种子值，而测试时上电到获取种子时间固定。
----------------------------------------------------------------------- */
#if 0
void Acc_UdsDemoMakeNewSeed(void) {
    /* A12 初始key 0x5597 9DBE */
    bl_u32_t currenttick;
    currenttick = Tim_seedtick_Get();
    currenttick = currenttick % 1000;
    /*当前时间 ×16807，得到新的随机数seed*/
    gs_UdsDemoSeed = gs_UdsDemoSeed * ACC_UDSDEMO_RANDOM_SEED * currenttick;
    return ;
}
#endif

void Acc_UdsDemoMakeNewSeed(void) {
    gs_UdsDemoSeed = GetRandomNumber();
    return ;
}

/**************************************************************************//**
 *
 *  \details Get current seed value.
 *
 *  \param[out]  seed - the value of current seed.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Acc_UdsDemoGetSeed(bl_Buffer_t *seed) {
    ACC_UDSDEMO_SEED_TO_BUF(gs_UdsDemoSeed, seed); // seed = gs_UdsDemoSeed
    
    return ;
}

/**************************************************************************//**
 *
 *  \details Verify the value of transmitted key whether is right.
 *
 *  \param[in]  key - the value of transmitted key.
 *
 *  \return If the transmitted key value is right return BL_ERR_OK, otherwise
 *          return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the transmitted key value is right.
 *  \retval BL_ERR_NOT_OK - the transmitted key value is NOT right.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Acc_UdsDemoVerifyKey(const bl_Buffer_t *key) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_UdsDemoKey_t calKey;
    bl_UdsDemoKey_t tranKey;
    
    calKey = _Acc_UdsDemoCalculateKey();   //這是相當於本地算出來的
    tranKey = ACC_UDSDEMO_KEY_FROM_BUF(key);  // tranKey = *key  --這是相當於 診斷上收回來的數據；
    
    if (calKey == tranKey) {
        ret = BL_ERR_OK;
    }
    
    return ret;
}
bl_UdsDemoKey_t u32seedtokey = 0;
#define  BYD_SACEED
//#define  BJ_ASCEED
#define  LP_SEEDKEY
/**************************************************************************//**
 *
 *  \details An algorithm is used to calculate the key by a seed.
 *
 *  \return the key value
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
bl_UdsDemoKey_t _Acc_UdsDemoCalculateKey(void) {
    #ifdef LP_SEEDKEY
    bl_u8_t MagicSeed[4] = {0};
    bl_u32_t raw_seed;
    bl_u32_t key;
    Acc_UdsDemoGetSeed(MagicSeed);//这里把种子的原始数据按字节排序反一下
    raw_seed = MagicSeed[0];
    raw_seed <<= 8;
    raw_seed |= MagicSeed[1];
    raw_seed <<= 8;
    raw_seed |= MagicSeed[2];
    raw_seed <<= 8;
    raw_seed |= MagicSeed[3];
    key = 0u;
    key = ((((raw_seed >> 4U)^raw_seed) << 3U)^raw_seed);
    return key;
    #elif BYD_SACEED
    ///bl_UdsDemoKey_t key = 0;
    bl_u8_t MagicSeed[4] = {0};
    u32    SaccSeed = 0;
    u32    KeyResult = 0;
    u32    SaccKey;
    static const u32 KeyK = 0x00000017;
    
    Acc_UdsDemoGetSeed(MagicSeed);
    SaccSeed = MagicSeed[0];
    SaccSeed <<= 8;
    SaccSeed |= MagicSeed[1];
    SaccSeed <<= 8;
    SaccSeed |= MagicSeed[2];
    SaccSeed <<= 8;
    SaccSeed |= MagicSeed[3];
    KeyResult = ((((SaccSeed >> 1) ^ SaccSeed) << 3) ^ (SaccSeed >> 2));
    SaccKey = KeyResult ^ KeyK;
    u32seedtokey = SaccKey;
    return SaccKey;
    #else
    bl_UdsDemoKey_t key = 0;
    bl_u8_t i, Key[4], cal_data[4];
    bl_u8_t MagicSeed[4] = {0};
    u8 Const[4];
    u8 Seed2[4];
    u8 Key1[4];
    u8 Key2[4];
    u8 j = 3u;
    static const u32 key_u32 = 0x00005c37;
    Acc_UdsDemoGetSeed(MagicSeed);
    /*lev_index = 11;*/
    Const[3] = (u8)((key_u32 & 0xff000000) >> 24);
    Const[2] = (u8)((key_u32 & 0x00ff0000) >> 16);
    Const[1] = (u8)((key_u32 & 0x0000ff00) >> 8);
    Const[0] = (u8)(key_u32 & 0x000000ff);
    for (i = 0; i < 4; i++) {
        Key1[i] = MagicSeed[i] ^ Const[i];
        Seed2[i] = MagicSeed[j];
        Key2[i] = Seed2[i] ^ Const[i];
        Key[i] = (u8)(Key1[i] + Key2[i] + 0x05);
        j--;
    }
    
    key = ACC_UDSDEMO_KEY_FROM_BUF(Key);  //key = *Key
    
    return key;
    #endif
}
