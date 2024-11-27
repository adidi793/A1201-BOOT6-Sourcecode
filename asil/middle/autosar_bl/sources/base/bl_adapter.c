/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the adapter module.
 *
 *  \file       bl_adapter.c
 *  \ingroup    communication_adapter_module
 *  \author
 *
 *  \version    5.2.0
 *  \date       19/06/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions

 *
 *****************************************************************************/
#include "bl_common.h"
#include "bl_rte.h"
#include "bl_logical_block.h"
#include "bl_app.h"
#include "bl_data.h"
#include "bl_adapter.h"
#include "bl_adapter_cfg.h"
#include "bl_logical_block_cfg.h"
#include "bl_booting_cfg.h"
#include "common_memory.h"
#include "flash.h"
#include "eeprom_cfg.h"
#include "eeprom.h"
/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 506,1532,2103,3112,3227 EOF*/
/*
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * 1532:The function is only referenced in one translation unit.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro was used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 */

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
 *  \details Initialize the security access delay info.
 *
 *  \pre The delay must be not a BL_NULL_PTR.
 *
 *  \param[in]  delay  - the pointer of security access delay info in the RAM.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
void Adpt_InitDelayInfo(bl_SecurityDelay_t *delay) {
    bl_u8_t i;
    bl_u32_t attemptcount_ee_buff;
    
    #if(ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
    bl_Return_t ret;
    #endif
    
    BL_DEBUG_ASSERT_NO_RET(delay != BL_NULL_PTR);
    
    #if (ADPT_FUN_ADVANCED_ATTEMPT_DELAY == BL_FUN_ON)
    delay->failedCntFlagId = g_SecurityDelayInfo.failedCntFlagId;
    #endif
    
    #if (ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
    delay->delayFlagId = g_SecurityDelayInfo.delayFlagId;
    
    BL_DEBUG_ASSERT_NO_RET(Dm_IsValidDataId(delay->delayFlagId) == BL_ERR_OK);
    
    /*check whether delayflag is valid.*/
    ret = Dm_IsDefaultData(delay->delayFlagId);
    if (BL_ERR_OK == ret) {
        delay->delayTime    = g_SecurityDelayInfo.delayTime;   // ADPT_SECURITY_DELAY_TIME  =40000ms
        delay->attemptCount = 0u;
    } else {
        delay->delayTime    = 0u;
        delay->attemptCount = g_SecurityDelayInfo.attemptCount;   // ADPT_SECURITY_ATTEMPT_COUNT =3
    }
    #else
    /**/
    delay->delayTime = 0;
    delay->attemptCount = g_SecurityDelayInfo.attemptCount;
    for (i = 0; i < security_access_attemptcount_EE_FLASH_SIZE; i ++) {
        if (EE_ReadVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0 + i, (uint32_t *)&attemptcount_ee_buff) == 1) {
        } else {
            if ((bl_u16_t)attemptcount_ee_buff == 0xFFFF) {//错误访问计数器还未写入EE
                attemptcount_ee_buff = (attemptcount_ee_buff & 0xFFFF0000) | delay->attemptCount;
                EE_WriteVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0, attemptcount_ee_buff);
            }
        }
    }
    
    #endif
    return ;
}

/**************************************************************************//**
 *
 *  \details Call the Lbm_GetBlock() to check the address and size information
 *           whether are valid. If it is valid, save them to Download Context.
 *
 *  \param[in]  dct     - the pointer of current download context.
 *  \param[in]  address - the address of request download.
 *  \param[in]  size    - the size of request download.
 *
 *  \return If Activation is successfully return BL_ERR_OK, otherwise return
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - Activation successfully.
 *  \retval BL_ERR_NOT_OK   - Activation unsuccessfully.
 *
 *  \since  V4.0.0
 *   一般情況下，是指激活下載，如34 診斷。
 *****************************************************************************/
bl_Return_t Adpt_ActivateDownload(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (0UL != size) {
        /*get the logical block id, according to addr,size and status.*/
        //其實是就想知道 是1 flash driver 還是 0 flash app
        /* -----------------------------------------------------------------------A12
        刷写flashdriver时，address为0x20007000 size为0x5AC
        ----------------------------------------------------------------------- */
        ret = Lbm_GetBlock(dct, address, size);
        
        /*after get logical block id, save address and size, and set status.*/
        if (BL_ERR_OK == ret) {
            //dct->segIndex 決定 存在哪個 dct->segment.segList 中
            ret = Rte_SaveAddressInfo(dct, address, size);
        }
    }
    
    if (ret != BL_ERR_OK) {
        Rte_ClearAddressInfo(dct);  // dct->segIndex 0;
        /*clear erase and write status.*/
        Rte_ClearDownStatus(dct, ADPT_SYSTEM_STATUS_MASK);  // mask = ADPT_SYSTEM_STATUS_MASK =0x0c
        // id  = mask >> 30 再 與上 0xff  =0
        // state &= ~ (mask&0x3ffffffff)  = ~0x0c
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Download the data of application or calibration to nonvolatile
 *           memorys.
 *
 *  \param[in]  dct     - the pointer of current download context.
 *  \param[in]  address - the address of the data.
 *  \param[in]  size    - the size of the data.
 *  \param[in]  buffer  - the contents of the data.
 *
 *  \retval BL_ERR_OK           - Download successfully.
 *  \retval BL_ERR_NOT_OK       - Download unsuccessfully.
 *
 *  \since  V3.0.0
 *   36 服務 傳輸下載 成功 每個 block 最多內容 1024
 *****************************************************************************/
bl_Return_t Adpt_DownloadData(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer) {
    bl_Return_t ret;
    // logic block 層 寫入數據
    ret = Lbm_Write(dct, address, size, buffer);
    if (BL_ERR_OK == ret) {
        /*after writting data sucessfully, set status.*/
        Rte_SetDownStatus(dct, RTE_SYSFLAG_WRITE_DATA);  //或上 RTE_SYSFLAG_WRITE_DATA =0x08
    } else {
        Rte_ClearAddressInfo(dct);   // dct->segIndex 0;
        /*clear erase and write status.*/
        Rte_ClearDownStatus(dct, ADPT_SYSTEM_STATUS_MASK); // mask = ADPT_SYSTEM_STATUS_MASK =0x0c
        // id  = mask >> 30 再 與上 0xff  =0
        // state &= ~ (mask&0x3ffffffff)  = ~0x0c
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Erase a logical block.
 *
 *  \param[in]  dct     - the pointer of current download context.
 *  \param[in]  address - the address of erased logical block.
 *  \param[in]  size    - the size of erased logical block.
 *
 *  \retval BL_ERR_OK           - Erasing successfully.
 *  \retval BL_ERR_NOT_OK       - Erasing unsuccessfully.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Adpt_EraseLB(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size) {
    bl_Return_t ret;
    
    /*get logical block id.*/
    //其實是就想知道 是1 flash driver 還是 0 flash app
    ret = Lbm_GetBlock(dct, address, size);
    if (BL_ERR_OK == ret) {
        // logic block 層 擦除數據
        ret = Lbm_Erase(dct);
        if (BL_ERR_OK == ret) {
            Rte_SetDownStatus(dct, RTE_SYSFLAG_ERASE_BLOCK);   // dct->status[id] | 0x04
        } else {
            Rte_ClearDownStatus(dct, ADPT_SYSTEM_STATUS_MASK);  // dct->status[id] & ~0x0c
        }
    }
    
    Rte_ClearAddressInfo(dct);   //dct->segIndex = 0;
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Verify checksum value of a logical block.
 *
 *  \param[in]  dct   - the pointer of current download context.
 *  \param[in]  size  - the size of checksum value.
 *  \param[in]  value - the pointer of checksum value.
 *
 *  \retval BL_ERR_OK           - Verify successfully.
 *  \retval BL_ERR_NOT_OK       - Verify unsuccessfully.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
/*lint -e{818}*/
bl_Return_t Adpt_VerifyData(bl_DownContext_t *dct, bl_Size_t size, bl_Buffer_t *value) { /*PRQA S 3673*/
    bl_Return_t ret;
    #if(ADPT_FUN_GET_INTEGRALITY_VALUE == BL_FUN_ON)
    /*When the ADPT_FUN_GET_INTEGRALITY_VALUE is enable,
      the value must be modified. So the value is not a const pointer.*/
    App_GetIntegralityValue(dct, &size, value);
    #endif
    // 用哈希算法 校驗 剛才下載的所有數據
    ret = Lbm_Verify(dct, size, value);
    //这里需做的逻辑，把 app flag 置起来
    //#ifdef CHECKMOMERY_USE_ADDR_SIZE
    if (ret == BL_ERR_OK) {
        if (dct->lbId == LBM_LB1_ID_APP) {
        
            //app 烧写成功
            Dm_WriteDefaultData(BOOTM_RESET_FLAG_DID);
            
        }
    } else { //if()
        if (dct->lbId == LBM_LB1_ID_DRV) {
            //清除flash driver的标志
            flash_init();
        }
    }
    //#endif
    /*clear address, size and status.*/
    Rte_ClearAddressInfo(dct);   //dct->segIndex = 0;
    
    Rte_ClearDownStatus(dct, ADPT_SYSTEM_STATUS_MASK);  //dct->status[0]&3FFF FFF3
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check compatibility of all downloaded logical block.
 *
 *  \param[in]  dct   - the pointer of current download context.
 *
 *  \retval BL_ERR_OK      - Check successfully.
 *  \retval BL_ERR_NOT_OK  - Check unsuccessfully.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Adpt_CheckCompatibility(void) {
    bl_Return_t ret = BL_ERR_OK;
    
    #if(BL_FUN_ON == ADPT_FUN_CHECK_COMPATIBILITY)
    /*if not ok, need to set invalid status of logical block. */
    //ret = App_CheckCompatibility();
    #endif
    
    /*  Maybe the checking of Compatiblity is failure when
        ADPT_FUN_CHECK_COMPATIBILITY was BL_FUN_ON.*/
    if (BL_ERR_OK == ret) { /*lint !e774*/
        /*PRQA S 3355,3358,2995,2991*/
        ret = Lbm_IsAllValid();   // 檢查 app flash 和 app driver 是否都可以；
    }
    if (BL_ERR_OK == ret) { //必须在检查OK后，写入成功计数值
        ret = _Lbm_AddSuccessCnt(); //返回值总为OK
    }
    
    /*save status into nonvolatile memory*/ //写入APP VALID FLAG
    (void)Lbm_SaveAllStatus();   // 保存 app flash 和 app driver 最終狀態；
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check whether security access is available.
 *
 *  \pre The delay must be not a BL_NULL_PTR.
 *
 *  \param[in]  delay  - the pointer of security access delay info.
 *
 *  \return If delaytime is zreo, return BL_ERR_OK,
 *          otherwise, return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - Security access is avaliable.
 *  \retval BL_ERR_NOT_OK   - Security access is not allowed.
 *
 *  \since  V3.0.0
 *****************************************************************************/
bl_Return_t Adpt_IsAccessible(const bl_SecurityDelay_t *delay) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    BL_DEBUG_ASSERT_NO_RET(delay != BL_NULL_PTR);
    
    /*only when delaytime is Zero, security access is avaliable.*/
    if (0UL == delay->delayTime) {
        ret = BL_ERR_OK;
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details Decrease the attemptcount of security access.
 *
 *  \pre The delay must be not a BL_NULL_PTR.
 *
 *  \param[in]  delay  - the pointer of security access delay info.
 *
 *  \return Return the remain attemptcount of security access.
 *
 *  \note If attemptcount is zero, updating delaytime is needed.
 *
 *  \since  V5.0.0
 *   當安全算法校驗成功時
 *****************************************************************************/
bl_u16_t Adpt_DecreaseAttemptCount(bl_SecurityDelay_t *delay) {
    bl_u16_t remainCount;
    bl_u8_t i;
    bl_u32_t attemptcount_ee_buff;
    
    BL_DEBUG_ASSERT_NO_RET(delay != BL_NULL_PTR); //空语句
    for (i = 0; i < security_access_attemptcount_EE_FLASH_SIZE; i ++) {
        if (EE_ReadVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0 + i, (uint32_t *)&attemptcount_ee_buff) == 1) {
        } else {
            delay->attemptCount = (bl_u16_t)attemptcount_ee_buff;
        }
    }
    if (delay->attemptCount < 3) {
        delay->attemptCount += 1;
    }
    
    if (3 == delay->attemptCount) {
        delay->delayTime = g_SecurityDelayInfo.delayTime;
    }
    attemptcount_ee_buff = (attemptcount_ee_buff & 0xFFFF0000) | delay->attemptCount;
    EE_WriteVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0, attemptcount_ee_buff);
    remainCount = delay->attemptCount;
    return remainCount;
}

/**************************************************************************//**
 *
 *  \details Decrease the delaytime of security access.
 *
 *  \pre The delay must be not a BL_NULL_PTR.
 *
 *  \param[in]  delay  - the pointer of security access delay info.
 *
 *  \return Return void.
 *
 *  \note If delaytime is zero, updating attemptcount is needed.
 *
 *  \since  V5.0.0
 *   一直在周期循環裡  被 Adpt_UdsPeriodicCallback 函數調用
 *****************************************************************************/
void Adpt_DelayTimeCallback(bl_SecurityDelay_t *delay) {
    bl_u8_t i;
    bl_u32_t attemptcount_ee_buff;
    BL_DEBUG_ASSERT_NO_RET(delay != BL_NULL_PTR);
    
    if (delay->delayTime != 0UL) { //
        delay->delayTime -= 1UL;
    } else {
        for (i = 0; i < security_access_attemptcount_EE_FLASH_SIZE; i ++) {
            if (EE_ReadVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0 + i, (uint32_t *)&attemptcount_ee_buff) == 1) {
            } else {
            }
        }
        if (3 == (bl_u16_t)attemptcount_ee_buff) { //查看EE中的attemptcount
            #if(ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
            (void)Dm_ClearData(delay->delayFlagId);
            #endif
            delay->attemptCount = 2;
        }
    }
    if ((bl_u16_t)attemptcount_ee_buff != delay->attemptCount) {
        attemptcount_ee_buff = (attemptcount_ee_buff & 0xFFFF0000) | delay->attemptCount;
        EE_WriteVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0, attemptcount_ee_buff);
    }
    return ;
}

