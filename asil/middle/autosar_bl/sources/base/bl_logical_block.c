/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the logical block manager module.
 *
 *  \file       bl_logical_block.c
 *  \ingroup    lb_manager_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------

 *      05.01.00 | 27/03/2018 | cong.xu    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_common.h"
#include "bl_security.h"
#include "bl_data.h"
#include "bl_memory.h"
#include "bl_logical_block.h"
#include "bl_logical_block_cfg.h"
#include "bl_adpt_uds_platform_cfg.h"
#include "bl_adpt_uds_platform.h"
#include "bl_data_cfg.h"
#include "bl_watchdog.h"
#include "bl_flash_if.h"
#include "bl_memory_cfg.h"
#include "bl_booting.h"
#include "bl_adpt_uds_platform.h"
#include "bl_adpt_uds_platform_cfg.h"

#include "common_types.h"
#include "bl_system_cfg.h"
#include "eeprom_cfg.h"
#include "eeprom.h"


// #define LOGIC_BLOCK_DEBUG_EN
#ifdef LOGIC_BLOCK_DEBUG_EN
typedef struct {
    u8 buf[4];
    u8 old[4];  //
    bl_u16_t foundId;
    bl_Return_t isUpdate;
} logic_block_test_s;
logic_block_test_s g_logic_block_test = {0};
typedef struct {
    u32 addr;
    u8 memid;
    u16 type;
    u16 index;
    u16 tmpId;
    
    u32 start_addr;
    u32 end_addr;
    u32 limit_start_addr;
    u32 limit_end_addr;
} bl_log_bk_s;
static bl_log_bk_s g_bl_log_bk;
#endif

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 306,506,602,771,781,1251,1257,1503,1532,2103,3112,3199,3227,3453 EOF*/
/*
 * MISRA-C:2004 11.3(306):Cast a pointer to an integral type.
 * To address a flash must cast an integral type to a pointer.
 *
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * MISRA-C:2004 14.6(771):More than one break statement is being used to
 * terminate an iteration statement.
 * it can simplify the code and have no side-effect.
 *
 * MISRA-C:2004 5.6(781):The identifier is used as a structure/union member.
 * The member of the structure is used through the structure object.
 *
 * 1251:Suffixed integer constant causes implicit conversion of other
 * (integral) operand.
 *
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * MISRA-C:2004 14.1(1503):The function is defined but is not used within this
 * project.
 * these functions will be used When correlative macro is enable.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 2103:Integral promotion, unsigned short promoted to unsigned int.
 * This message will therefore only be generated when a short and an int are of
 * equal size.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3199:This assignment is redundant. The value is never subsequently used.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * These macros have no side-effect.
 *
 */

/*****************************************************************************
 *  Verify The Configurations of Function Macro
 *****************************************************************************/
#if ((LBM_ATTEMPT_COUNTER_SIZE >= LBM_COUNTER_MAX_SIZE)\
|| (LBM_SUCCESS_COUNTER_SIZE >= LBM_COUNTER_MAX_SIZE)\
|| (LBM_ATTEMPT_COUNTER_SIZE != LBM_SUCCESS_COUNTER_SIZE))
#error "the size of counter is error."
#endif

#if ((RTE_FUN_FINGERPRINT == BL_FUN_OFF) \
&& (LBM_FUN_SAVE_FINGERPRINT == BL_FUN_ON))
#error "Close the fingerprint function of the LBM."
#endif
/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
/** check whether the id indicates a valid logical block.*/
#define LBM_IS_VALID_ID(id)             ((id) < g_LogicalBlockList.number)

#define LBM_IS_IN_REGION(s,e,ts,te)     (((s)>=(ts)) && ((e)<=(te)))

/* -----------------------------------------------------------------------A12
flashdriver的offsetMask=0xFFFFFFFFUL
----------------------------------------------------------------------- */
#define LBM_MAKE_LOCAL_ADDR(lb,address) ((address) & ((lb)->offsetMask))

#define LBM_IS_VALID_MASK(dct,mask)     Rte_IsAllValidStatus((dct),(mask))   //關鍵是要理解； rte模塊中 id = (mask >> 30) & 0xff； 再 dct->status[id]&mask

#define LBM_IS_GETTABLE(dct,lb)     LBM_IS_VALID_MASK(dct,(lb)->getLbMask)
#define LBM_IS_WRITABLE(dct,lb)     LBM_IS_VALID_MASK(dct,(lb)->writeLbMask)  //在寫入時 dct->status[0]=0x34 ；
#define LBM_IS_ERASABLE(dct,lb)     LBM_IS_VALID_MASK(dct,(lb)->eraseLbMask)    //在擦除時 dct->status[0]=0x30 ；
#define LBM_IS_VERIFIABLE(dct,lb)   LBM_IS_VALID_MASK(dct,(lb)->verifyLbMask)   //在校驗時， dct->status[0]=0x38 ；

#define LBM_GET_LBPTR(id)           (&g_LogicalBlockList.lbList[id])   //直接指向了 gs_LogicalBlock[id] -- LBM_LB1_ID_APP

#define LBM_SET_STATUS(lb,st)       ((lb)->status[0] = (st))
#define LBM_SET_LBSTATUS(id,st)     LBM_SET_STATUS(LBM_GET_LBPTR(id),st)  //最終 gs_LogicalBlockStatus[id]=st

#define LBM_GET_STATUS(lb)          ((lb)->status[0])
#define LBM_GET_LBSTATUS(id)        LBM_GET_STATUS(LBM_GET_LBPTR(id))   //最終 返回  gs_LogicalBlockStatus[id]=st

#define LBM_IS_UPDATING_STATUS(lb)  (LBM_STATUS_UPDATING == LBM_GET_STATUS(lb)) //正更新
#define LBM_IS_VALID_STATUS(lb)     (LBM_STATUS_VALID == LBM_GET_STATUS(lb))    //有效
#define LBM_IS_UPDATED_STATUS(lb)   (LBM_STATUS_UPDATED == LBM_GET_STATUS(lb))  //己更新
#define LBM_IS_NOCARE_STATUS(lb)    (LBM_STATUS_NOT_CARE == LBM_GET_STATUS(lb))  //NO CASE 可以是 RAM用完之後的再清除的狀態

#define LBM_IS_A_DRIVER(lb)         (LBM_TYPE_DRIVER == ((lb)->type))   //判斷是否 驅動程序
/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
/** \brief Update the status of all logical blocks.*/
static void _Lbm_UpdateStatus(void);
/** \brief Update the status of a logical block.*/
static void _Lbm_ReadAndUpdateStatus(const bl_LogicalBlock_t *lb);
/** \brief Find a logical block.*/
static bl_Return_t _Lbm_FindLogicalBlock(bl_Address_t address,
    bl_Size_t size,
    bl_u16_t *foundId);
/** \brief Check a region whether is in the logical block.*/
static bl_Return_t _Lbm_IsInLogicalBlock(const bl_LogicalBlock_t *lb,
    bl_Address_t address,
    bl_Address_t limitAddr);
/** \brief Check a region whether is in the segment list.*/
static bl_Return_t _Lbm_IsInSegmentList(const bl_SegmentList_t *segment,
    bl_Address_t address,
    bl_Address_t limitAddr);
/** \brief Check the download context whether need be updated.*/
static bl_Return_t _Lbm_IsUpdatable(bl_u16_t curId, bl_u16_t foundId);
/** \brief Update the lbId and downSeg information in the download context.*/
static void _Lbm_UpdateBlockInfoInDct(bl_DownContext_t *dct, bl_u16_t newId);
/** \brief Check the parameters of functions whether are valid.*/
static bl_Return_t _Lbm_IsValidParam(const bl_DownContext_t *dct,
    const bl_Buffer_t *buf);
/** \brief Perform the write operation of the logical block.*/
static bl_Return_t _Lbm_Write(const bl_LogicalBlock_t *lb,
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer);
/** \brief Clear the valid flag of the logical block.*/
static bl_Return_t _Lbm_TryToClearValidFlag(const bl_LogicalBlock_t *lb);
/** \brief Perform the erase operation of the logical block.*/
static bl_Return_t _lbm_Erase(const bl_LogicalBlock_t *lb);
/** \brief Check the hash value of the logical block whether is valid.*/
static bl_Return_t _Lbm_IsValidHash(const bl_DownContext_t *dct,
    bl_Size_t size,
    const bl_Buffer_t *buf);
/** \brief Calculate the hash value of the download segments.*/
static bl_Return_t _Lbm_CalculateHashByDct(const bl_DownContext_t *dct);
/** \brief Initialize the memory of the logical block.*/
static bl_Return_t _Lbm_InitMemory(const bl_LogicalBlock_t *lb);
/** \brief Look for the memory id.*/
static bl_u16_t _Lbm_LookForMemId(const bl_LogicalBlock_t *lb);
/** \brief Save the status of the logical block to ROM.*/
static bl_Return_t _Lbm_SaveStatus(const bl_LogicalBlock_t *block);

#if (LBM_FUN_SUCCESS_COUNTER == BL_FUN_ON)
/** \brief add the success count of a logical block.*/
/** \brief get the success count of a logical block.*/
//bl_Return_t _Lbm_GetSuccessCount(bl_u16_t did,
//                                    bl_SuccessCounter_t *count);
/** \brief add save success count of a logical block into nonvolatile memory.*/
//bl_Return_t _Lbm_SaveSuccessCount(bl_u8_t did);
#endif

#if (LBM_FUN_ATTEMPT_COUNTER == BL_FUN_ON)


/** \brief add the attempt count of a logical block.*/
static bl_Return_t _Lbm_AddAttemptCnt(void); ///(const bl_LogicalBlock_t *lb);


/** \brief get the attempt count of a logical block.*/
bl_Return_t _Lbm_GetAttemptCount(bl_u16_t did,
    bl_Buffer_t *count);
/** \brief add save attempt count of a logical block into nonvolatile memory.*/
//bl_Return_t _Lbm_SaveAttemptCount(bl_u16_t did);
#endif

#if (LBM_FUN_SAVE_FINGERPRINT == BL_FUN_ON)
/** \brief add save fingerprint of a logical block into nonvolatile memory.*/
//bl_Return_t _Lbm_SaveFingerprint(const bl_LogicalBlock_t *block);
#endif

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
#if (LBM_FUN_SAVE_FINGERPRINT == BL_FUN_ON)
/** \brief the fingerprint buffer of a logical block.*/
//static bl_Buffer_t gs_LbmFingerprint[RTE_FINGERPRINT_SIZE];
#endif

STATIC bl_flash_t gs_LbmData;   //徐工，自己加的，相當於自己修改了 fingprnt f_attmp  success等
/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details initialize the logical block manager module.
 *
 *  \return If this module is initialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - initialize successfully.
 *  \retval BL_ERR_NOT_OK   - initialize unsuccessfully.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Lbm_Init(void) {
    _Lbm_UpdateStatus();
    //blAdptWriteBootSwId();   //2E F180
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details look up a logical block by the address and size information.
 *
 *  \param[in]  dct     - the pointer of current download context.
 *  \param[in]  address - the general address of the logical block.
 *  \param[in]  size    - the size of the logical block.
 *
 *  \retval BL_ERR_OK - the logical block is erased successfully.
 *  \retval BL_ERR_NOT_OK - the logical block is erased unsuccessfully.
 *
 *  \since  V3.0.0
 *  被函數 Adpt_ActivateDownload 調用 ;  Adpt_EraseLB 調用;
 *  主要目標是 dct->lbId = newId;  dct->segIndex = 0;
 *****************************************************************************/
bl_Return_t Lbm_GetBlock(bl_DownContext_t *dct,
    bl_Address_t address,
    bl_Size_t size) {
    bl_Return_t ret;
    bl_Return_t isUpdate;
    bl_u16_t foundId;
    #if (LBM_FUN_AUTO_CALCULATE_HASH == BL_FUN_ON)
    bl_Return_t isRead;
    const bl_LogicalBlock_t *block;
    #endif
    
    BL_DEBUG_ASSERT_PARAM(dct != BL_NULL_PTR);
    #ifdef LOGIC_BLOCK_DEBUG_EN
    g_logic_block_test.buf[0]++;
    #endif
    // foundId为0表示app,1表示driver
    ret = _Lbm_FindLogicalBlock(address, size, &foundId); //從仿真理解； foundId是 gs_LogicalBlock 的 buf id 0是 flash app 1是flash driver
    if (BL_ERR_OK == ret) {
        isUpdate = _Lbm_IsUpdatable(dct->lbId, foundId);	//lbid LBM_LB1_ID_APP
        #ifdef LOGIC_BLOCK_DEBUG_EN
        g_logic_block_test.buf[1]++;
        g_logic_block_test.foundId = foundId;
        g_logic_block_test.isUpdate = isUpdate;
        #endif
        if (BL_ERR_OK == isUpdate) {
            //這函數的功能，相當於 切換app和driver時要更新一下。把segment清0
            _Lbm_UpdateBlockInfoInDct(dct, foundId); //    dct->lbId = newId;  dct->segIndex = 0;
            #ifdef LOGIC_BLOCK_DEBUG_EN
            g_logic_block_test.old[g_logic_block_test.buf[2]] =
                foundId;   //測試發現 順序是 driver(下載RAM) ,app 擦除; 共兩次進來;。因為擦除後就是LBM_STATUS_UPDATING狀態
            g_logic_block_test.buf[2]++;
            if (g_logic_block_test.buf[2] == 1) {
                g_logic_block_test.foundId = foundId;
            } else if (g_logic_block_test.buf[2] == 2) {
                g_logic_block_test.isUpdate = isUpdate;
                g_logic_block_test.foundId = foundId;
            } else if (g_logic_block_test.buf[2] == 3) {
                g_logic_block_test.foundId = foundId;
                g_logic_block_test.isUpdate = isUpdate;
            } else {
                g_logic_block_test.foundId = foundId;
            }
            #endif
            #if (LBM_FUN_AUTO_CALCULATE_HASH == BL_FUN_ON)
            isRead = Mem_IsReadable(block->memId);
            if (isRead != BL_ERR_OK) { /*this logical block is not readable.*/
                Secm_StartHash();
                #if ((RTE_SYSTEM_SECURITY_CLASS == BL_SECURITY_CLASS_C) \
                || (RTE_SYSTEM_SECURITY_CLASS == BL_SECURITY_CLASS_CCC))
                /*  according to the HIS spec. Use the address and size of
                	the downloaded segment to calculate the hash value.*/
                ret = Secm_UpdateHashByAddr(address, size);
                #endif
            }
            #endif
        } else if (isUpdate != LBM_ERR_UPDATED_ID) {
            ret = BL_ERR_NOT_OK;
            #ifdef LOGIC_BLOCK_DEBUG_EN
            g_logic_block_test.buf[3]++;
            //g_logic_block_test.foundId =foundId;
            #endif
        } else {
            /*code*/
        }
    } else {
        ret = BL_ERR_USER_CODE_01; //请求超限错误，ECU否定响应NRC0x31
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Clear the logical block in the download context.
 *
 *  \param[in]  dct - the pointer of current download context.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
void Lbm_ClearBlock(bl_DownContext_t *dct) {
    const bl_LogicalBlock_t *block;
    
    BL_DEBUG_ASSERT_PARAM(dct != BL_NULL_PTR);
    
    if (LBM_IS_VALID_ID(dct->lbId)) { //(dct->lbId) < g_LogicalBlockList.number  LBM_LB1_ID_APP
        block = LBM_GET_LBPTR(dct->lbId);   // gs_LogicalBlock[dct->lbId]  LBM_LB1_ID_APP
        if (LBM_IS_UPDATING_STATUS(block)) { // 如果是正在更新的UPDATING
            LBM_SET_STATUS(block, LBM_STATUS_INVALID); //則置為無效
        }
        
        dct->lbId = BL_ERROR_ID_16BIT;  //而且把 dct->lbId改成 0xffff
    }
    
    return ;
}
/**************************************************************************//**
 *
 *  \details write data to a logical block.
 *
 *  \param[in]  dct     - the pointer of current download context.
 *  \param[in]  address - the general address of the data.
 *  \param[in]  size    - the size of the data.
 *  \param[in]  buffer  - the content of the data.
 *
 *  \retval BL_ERR_OK       - the logical block is written successfully.
 *  \retval BL_ERR_NOT_OK   - the logical block is written unsuccessfully.
 *
 *  \note If the parameters is invalid or the writing operation cannot be
 *        performed, the status of the logical block is not changed.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Lbm_Write(const bl_DownContext_t *dct,    // g_DownContext
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer) {
    bl_Return_t ret;
    const bl_LogicalBlock_t *block = BL_NULL_PTR;
    bl_u16_t tmpStatus = LBM_STATUS_NOT_EXIST;
    
    do {
        ret = _Lbm_IsValidParam(dct, buffer);  //(dct->lbId) < g_LogicalBlockList.number
        if (ret != BL_ERR_OK) {
            break;
        }
        /* -----------------------------------------------------------------------A12
        falshdriver的lbId=1
        ----------------------------------------------------------------------- */
        block = LBM_GET_LBPTR(dct->lbId);  // gs_LogicalBlock[dct->lbId]
        
        /*check the write mask of logical block*/
        ret = LBM_IS_WRITABLE(dct, block); //flashdriver的writeLbMask=0x00000000UL
        if (ret != BL_ERR_OK) {
            break;
        }
        
        tmpStatus = LBM_STATUS_INVALID;
        /* -----------------------------------------------------------------------A12
        将36服务接收到的刷写数据复制到34服务的地址上
        ----------------------------------------------------------------------- */
        ret = _Lbm_Write(block, address, size, buffer); //将buffer中的数据复制到address中
        if (ret != BL_ERR_OK) {
            break;
        }
        
        #if (LBM_FUN_AUTO_CALCULATE_HASH == BL_FUN_ON)  //OFF
        ret = Mem_IsReadable(block->memId);
        if (ret != BL_ERR_OK) { /*this logical block is not readable.*/
            ret = Secm_UpdateHash(size, buffer);
        }
        #endif
        /*  Maybe the updating of hash is failure when
        	LBM_FUN_AUTO_CALCULATE_HASH was BL_FUN_ON.*/
        if (BL_ERR_OK == ret) { /*PRQA S 3355,3358,2995,2991*/
            tmpStatus = LBM_STATUS_UPDATING;
        }
    } while (0); /*lint !e717*/
    
    if ((tmpStatus != LBM_STATUS_NOT_EXIST)
        && (block != BL_NULL_PTR)) {
        LBM_SET_STATUS(block, tmpStatus);
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Erase a logical block.
 *           If the macro 'LBM_FUN_ATTEMPT_COUNTER' is enabled, this function
 *           will try to add the attempt count of the logical block.
 *           If the macro 'LBM_FUN_SAVE_FINGERPRINT' is enabled, this
 *           function will try to save the fingerprint to the data manager
 *           module.
 *
 *  \param[in]  dct - the pointer of current download context.
 *
 *  \retval BL_ERR_OK - the logical block is erased successfully.
 *  \retval BL_ERR_NOT_OK - the logical block is erased unsuccessfully.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Lbm_Erase(const bl_DownContext_t *dct) {
    bl_Return_t ret;
    const bl_LogicalBlock_t *block = BL_NULL_PTR;
    bl_u16_t tmpStatus = LBM_STATUS_NOT_EXIST;
    
    do {
        ret = _Lbm_IsValidParam(dct, BL_VIRTUAL_PTR); /*PRQA S 0326*/ //(dct->lbId) < g_LogicalBlockList.number;  BL_VIRTUAL_PTR 完全是為了通過
        if (ret != BL_ERR_OK) {
            break;
        }
        
        block = LBM_GET_LBPTR(dct->lbId);    // 等价于&g_LogicalBlockList.lbList[dct->lbId] 也即gs_LogicalBlock[dct->lbId]
        /*If the status of this logical block is LBM_STATUS_UPDATING, this
          logical block is not erased again.*/
        if (LBM_IS_UPDATING_STATUS(block)) { //如果正在更新，則退出 不擦除
            ret = BL_ERR_NOT_OK;
            break;
        }
        
        /*Check the erase mask of logical block*/
        ret = LBM_IS_ERASABLE(dct, block); //条件成立
        if (ret != BL_ERR_OK) {
            break;
        }
        /*Set the temp status of this logical block.*/
        tmpStatus = LBM_STATUS_INVALID;
        
        #if 0  //對STM32來說，一定要先擦flash要寫入;;或者把這一步移到 _lbm_Erase 後面；
        if (block->memId == LBM_LB1_MEMID) {
        
        } else {
            ret = _Lbm_TryToClearValidFlag(block);  //清除 flash app的 valid 標志 ;
            if (ret != BL_ERR_OK) {
                break;
            }
        }
        #else
        /* -----------------------------------------------------------------------A12
        起始地址0x1bc00，长度0x400
        ----------------------------------------------------------------------- */
        ret = _Lbm_TryToClearValidFlag(block);  //清除 flash app的 valid 標志 ;
        if (ret != BL_ERR_OK) {
            break;
        }
        #endif
        
        ret = _lbm_Erase(block); //真正擦除flash的app
        if (ret != BL_ERR_OK) {
            break;
        }
        
        
        #if (LBM_FUN_ATTEMPT_COUNTER == BL_FUN_ON)
        /* write pgm attmp counter TO EEPROM */
        ret =  _Lbm_AddAttemptCnt();
        if (ret != BL_ERR_OK) {
            break;
        }
        #endif
        
        if (ret != BL_ERR_OK) {
            break;
        }
        
        tmpStatus = LBM_STATUS_UPDATING;
    } while (0); /*lint !e717*/
    //擦除完成后，条件成立
    if ((tmpStatus != LBM_STATUS_NOT_EXIST) && (block != BL_NULL_PTR)) {
        LBM_SET_STATUS(block, tmpStatus);
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details check the logical block whether is downloaded successfully.
 *
 *  \param[in]  dct     - the pointer of current download context.
 *  \param[in]  size    - the size of a hash value.
 *  \param[in]  value   - the hash value.
 *
 *  \retval BL_ERR_OK - the logical block is downloaded successfully.
 *  \retval BL_ERR_NOT_OK - the logical block is downloaded unsuccessfully.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Lbm_Verify(const bl_DownContext_t *dct,
    bl_Size_t size,
    const bl_Buffer_t *value) {
    bl_Return_t ret, ret1 = BL_ERR_NOT_OK;
    const bl_LogicalBlock_t *block = BL_NULL_PTR;
    bl_u16_t tmpStatus = LBM_STATUS_NOT_EXIST;
    
    bl_DownContext_t raw_dct = *dct;
    //STATIC bl_u16_t test_pgm_result = 0;
    do {
        ret = _Lbm_IsValidParam(dct, value); //(dct->lbId) < g_LogicalBlockList.number;  --value 在函数内部被忽略
        if (ret != BL_ERR_OK) {
            break;
        }
        
        block = LBM_GET_LBPTR(dct->lbId);    // gs_LogicalBlock[dct->lbId]
        /*Check the verify mask of logical block*/
        /* -----------------------------------------------------------------------A12
        三个LB的verifyLbMask都为 0x00000000UL
        ----------------------------------------------------------------------- */
        ret = LBM_IS_VERIFIABLE(dct, block); //可校驗
        if (ret != BL_ERR_OK) {
            break;
        }
        
        /*Set the temp status of this logical block.*/
        tmpStatus = LBM_STATUS_INVALID;
        #if 0
        test_pgm_result++;
        if ((test_pgm_result & 0x1) == 0) { //  & 0x3
            ret = BL_ERR_OK; //debug only
        } else {
            ret = BL_ERR_NOT_OK; //_Lbm_IsValidHash(dct,size,value);
        }
        if (ret != BL_ERR_OK) {
            break;
        }
        #else
        ret = _Lbm_IsValidHash(dct, size, value); //讀取 flash 或者 ram ，然後計算 hash值，並校驗； size=4 value= 44 addr 和 szie
        if (ret != BL_ERR_OK) {
            ret = BL_ERR_USER_CODE_01;
        } else {
            if (raw_dct.lbId == LBM_LB1_ID_DRV) { //1. 是否是driver
                raw_dct.segment.segList[0].size -= 4;
                value = (const bl_Buffer_t *)(raw_dct.segment.segList[0].address + raw_dct.segment.segList[0].size);
                ret = _Lbm_IsValidHash(&raw_dct, size, value); //检查数据块，不包含CRC块,保证flashdriver的末尾一定有CRC块，否则校验不通过
            }
            if (ret != BL_ERR_OK) {
                ret = BL_ERR_USER_CODE_02;
            }
        }
        
        
        // ret = BL_ERR_NOT_OK; //验证否定响应是否执行
        if (ret != BL_ERR_OK) {
            break;
        }
        #endif
        if (LBM_IS_A_DRIVER(block)) { //判斷是否FlashDrv
            ret = _Lbm_InitMemory(block);
            if (ret != BL_ERR_OK) {
                break;
            }
        } else {
            /* write pgm success counter(++) and attmp counter (=0) to eeprom */
            #if (LBM_FUN_SUCCESS_COUNTER == BL_FUN_ON)
            /* -----------------------------------------------------------------------A12
            成功编程计数器移动到FF01
            ----------------------------------------------------------------------- */
            // ret = _Lbm_AddSuccessCnt();
            // if (ret != BL_ERR_OK) { /*PRQA S 2996,2992*/
            //     break;
            // }
            #endif
            
            #if (LBM_FUN_AUTO_DRIVER_DEINIT == BL_FUN_ON)
            ret = Mem_DeinitMemory(block->memId);
            if (ret != BL_ERR_OK) {
                break;
            }
            #endif
        }
        tmpStatus = LBM_STATUS_UPDATED;
    } while (0); /*lint !e717*/
    if ((tmpStatus != LBM_STATUS_NOT_EXIST)
        && (block != BL_NULL_PTR)) {
        LBM_SET_STATUS(block, tmpStatus);
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details check the status of all logical blocks are whether valid or
 *           updated. If the status of a logical block is LBM_STATUS_NOT_CARE,
 *           the status of this logical block do not effect return value.
 *
 *  \return If the status of a logical block is invalid or not_init returns
 *          BL_ERR_NOT_OK, otherwise returns BL_ERR_OK.
 *
 *  \retval BL_ERR_OK - all logical blocks are valid.
 *  \retval BL_ERR_NOT_OK - a logical block is invalid.
 *
 *  \since  V3.0.0
 *
 *
 *
#define LBM_STATUS_NOT_INIT         BL_STATUS_NOT_INIT
#define LBM_STATUS_VALID            (0x0001u)  --- 有效
#define LBM_STATUS_INVALID          (0x0002u)
#define LBM_STATUS_UPDATING         (0x0003u)
#define LBM_STATUS_UPDATED          (0x0004u)  ---- 有效
#define LBM_STATUS_NOT_CARE         (0x0005u)  ----  有效
#define LBM_STATUS_NOT_EXIST        (0xFFFFu)
 *****************************************************************************/
bl_Return_t Lbm_IsAllValid(void) {
    bl_u16_t i;
    bl_Return_t ret = BL_ERR_OK;
    const bl_LogicalBlock_t *block;
    
    BL_DEBUG_ASSERT_CONFIG(g_LogicalBlockList.number != 0); //条件成立，但是断言已取消
    
    for (i = 0; LBM_IS_VALID_ID(i); i++) { // i<=g_LogicalBlockList.number 其中number = 3
        block = LBM_GET_LBPTR(i);        //&g_LogicalBlockList.lbList[i]  就是 gs_LogicalBlock[i]
        if (LBM_IS_VALID_STATUS(block)  //gs_LogicalBlock[i]->status[0]  就是  gs_LogicalBlockStatus[i]
            || LBM_IS_UPDATED_STATUS(block)
            || LBM_IS_NOCARE_STATUS(block)) {
        } else {
            ret = BL_ERR_NOT_OK;
            break;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Read the valid flag of all logical blocks and update the status in
 *           the logical block information. If a logical block do not have its
 *           valid flag (the validFlagId is configurated to BL_ERROR_ID_16BIT),
 *           the status of this logical block will be set LBM_STATUS_NOT_CARE.
 *
 *  \return none
 *
 *  \since  V3.0.0
 *  好像沒有全局調用
 *****************************************************************************/
void Lbm_UpdateStatus(void) {
    _Lbm_UpdateStatus();
}

/**************************************************************************//**
 *
 *  \details save the status of the logical block. When the status of a logical
 *           block is updated, save it.
 *
 *  \param[in]  id - the index of the logical block array.
 *
 *  \return If a logical block statuses are saved into ROM return BL_ERR_OK,
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the status of a logical block is saved successfully.
 *  \retval BL_ERR_NOT_OK - a logical block status is saved failure.
 *
 *  \warning If the status of the logical block is not LBM_STATUS_UPDATED, this
 *           function will return BL_ERR_OK.
 *  \warning If the validFlagId of the logical block is invalid, this function
 *           will return BL_ERR_OK.
 *
 *  \since  V3.0.0  目前沒有被外部調用
 *
 *****************************************************************************/
bl_Return_t Lbm_SaveStatus(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (LBM_IS_VALID_ID(id)) {
        ret = _Lbm_SaveStatus(LBM_GET_LBPTR(id));
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details save the status of all logical block. When the status of a logical
 *           block is updated, save it.
 *
 *  \return If all of logical block statuses are saved into ROM return
 *          BL_ERR_OK, otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - all of statuses are saved successfully.
 *  \retval BL_ERR_NOT_OK - at least one logical block status is saved failure.
 *
 *  \warning If the number of logical block is zero, this function return
 *           BL_ERR_OK.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Lbm_SaveAllStatus(void) {
    bl_Return_t ret = BL_ERR_OK;
    bl_u16_t i;
    
    BL_DEBUG_ASSERT(g_LogicalBlockList.number != 0);
    for (i = 0; LBM_IS_VALID_ID(i); i++) {
        ret = _Lbm_SaveStatus(LBM_GET_LBPTR(i));
        if (ret != BL_ERR_OK) {
            break;
        }
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details Clear the valid flag of a logical block.
 *
 *  \param[in]  id - the index of the logical block array.
 *
 *  \retval BL_ERR_OK - the valid flag is cleared.
 *  \retval BL_ERR_NOT_OK - the status is not cleared.
 *
 *  \since  V3.0.0
 *  目前好像沒有被調用
 *****************************************************************************/
bl_Return_t Lbm_ClearStatus(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    const bl_LogicalBlock_t *block;
    
    if (LBM_IS_VALID_ID(id)) { //(id) < g_LogicalBlockList.number
        block = LBM_GET_LBPTR(id);  // gs_LogicalBlock[id]
        ret = Dm_IsValidDataId(block->validFlagId);  // block->validFlagId =0x01 flash app
        if (ret != BL_ERR_OK) {
            ret = BL_ERR_OK;
            
            LBM_SET_STATUS(block, LBM_STATUS_NOT_CARE);
        } else {
            ret = Dm_ClearData(block->validFlagId);  //清除 flash app valid標志
            
            LBM_SET_STATUS(block, LBM_STATUS_INVALID);
        }
    }
    
    return ret;
    
}

/**************************************************************************//**
 *
 *  \details Get the status of a logical block.
 *
 *  \param[in]  id - the index of logical block array.
 *  \param[out] status - the buffer is used to keep the logical block status.
 *
 *  \return If the id is invalid returns BL_ERR_NOT_OK, otherwise returns
 *          BL_ERR_OK.
 *
 *  \retval BL_ERR_OK     - the status of a logical block is copied to status.
 *  \retval BL_ERR_NOT_OK - the id of a logical block is invalid.
 *
 *  \since  V3.0.0
 *  目前好像沒有被調用
 *****************************************************************************/
bl_Return_t Lbm_GetStatus(bl_u16_t id, bl_u16_t *status) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (LBM_IS_VALID_ID(id)) {
        if (status != BL_NULL_PTR) {
            *status = LBM_GET_LBSTATUS(id);    // gs_LogicalBlockStatus[id]
            ret = BL_ERR_OK;
        }
    }
    
    return ret;
}

#if (LBM_FUN_SAVE_FINGERPRINT == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details Save the fingerprint to RAM.
 *
 *  \param[in]  buffer - the pointer of the fingerprint.
 *
 *  \since  V3.0.0
 *  好像沒有被真正調用
 *****************************************************************************/
void Lbm_SaveFingerprint(const bl_Buffer_t *buffer) {
    Bl_MemCpy(gs_LbmData.fingprnt, buffer, RTE_FINGERPRINT_SIZE); //有16字節
    
    return ;
}
#endif

#if (LBM_FUN_SAVE_PROGRAMMING_DATE == BL_FUN_ON)
/**************************************************************************//**
 *
 *	\details Save the ProgramDate to RAM.
 *
 *	\param[in]	buffer - the pointer of the ProgramDate.
 *
 *	\since	V3.0.0
 *  好像沒有被真正調用
 *****************************************************************************/
void Lbm_SaveProgramDate(const bl_Buffer_t *buffer) {
    Bl_MemCpy(gs_LbmData.pgm_date, buffer, 4);
    
    return ;
}
#endif

#if (LBM_FUN_SUCCESS_COUNTER == BL_FUN_ON)
//void Lbm_SavePgmSuccCnt(const bl_Buffer_t *buffer)
//{
///   Bl_MemCpy(gs_LbmPgmCounter,buffer,LBM_SUCCESS_COUNTER_SIZE);
//    return ;
//}

//void Lbm_SavePgmFattmpCnt(const bl_Buffer_t *buffer)
//{
//    Bl_MemCpy(&gs_LbmPgmCounter[LBM_SUCCESS_COUNTER_SIZE],buffer,LBM_ATTEMPT_COUNTER_SIZE);
//    return ;
//}

#endif

/**************************************************************************//**
 *
 *  \details update the status of all logical blocks.
 *
 *  \return none
 *
 *  \since V3.0.0
 *
 *****************************************************************************/
static void _Lbm_UpdateStatus(void) {
    bl_u16_t i;
    
    BL_DEBUG_ASSERT_NO_RET(g_LogicalBlockList.number != 0);
    
    for (i = 0; LBM_IS_VALID_ID(i); i++) { //(i) < g_LogicalBlockList.number
        _Lbm_ReadAndUpdateStatus(LBM_GET_LBPTR(i));    //_Lbm_ReadAndUpdateStatus(&gs_LogicalBlock[i])
    }
    
    return ;
}

/**************************************************************************//**
 *
 *  \details read and update the status of a logical blocks.
 *
 *  \param[in] lb   - the pointer of a logical block information
 *.
 *  \return none
 *
 *  \note If the memory of the valid flag is not initialized, initialize it.
 *
 *  \since V3.0.0
 *
 *****************************************************************************/
static void _Lbm_ReadAndUpdateStatus(const bl_LogicalBlock_t *lb) { //指向實體 gs_LogicalBlock
    bl_Return_t ret;
    bl_u16_t validFlagMemId;
    bl_u16_t status;
    
    BL_DEBUG_ASSERT_NO_RET(lb != BL_NULL_PTR);
    
    do {
        //只有 flash app才有效 返回 BL_ERR_OK。而flash driver是0xffff 返回 BL_ERR_NOT_OK
        ret = Dm_IsValidDataId(lb->validFlagId);   //flash app的 是 LBM_LB1_VALID_FLAG_ID=0x01 ; flash driver 的是 0xffff； 要求 < DM_NMBr_OF_MANAGED_Data_ID
        if (ret != BL_ERR_OK) {
            status = LBM_STATUS_NOT_CARE;
            break;
        }
        
        //相當於讀取 0x0003BFF0UL 地址上的數據，有效;判斷app valid標志； 正確返回 BL_ERR_OK
        ret = Dm_IsDefaultData(lb->validFlagId);  //flash app的 是 LBM_LB1_VALID_FLAG_ID=0x01; flash driver 的是 0xffff；
        if (BL_ERR_OK == ret) {
            status = LBM_STATUS_VALID;
            break;
        }
        //讀取 flasg_DataInfoList 中的 重編程條件 和 app valid標志 ; app valid 的Memory id 是 1;  validFlagMemId 的返回結果是1
        validFlagMemId = Dm_GetMemid(lb->validFlagId);  //flash app的 是 LBM_LB1_VALID_FLAG_ID=0x01;flash driver 的是 0xffff；
        ret = Mem_IsExternalDrv(validFlagMemId);  //flash app的確是外部的
        if (BL_ERR_OK == ret) {
            BL_DEBUG_ASSERT_NO_RET(0);
            status = LBM_STATUS_INVALID;   //如果valid 讀取不匹配，，則flash app會到這一步中止;
            break;
        }
        
        ret = Mem_InitMemory(validFlagMemId);
        if (ret != BL_ERR_OK) {
            status = LBM_STATUS_INVALID;
            break;
        }
        
        ret = Dm_IsDefaultData(lb->validFlagId);
        if (BL_ERR_OK == ret) {
            status = LBM_STATUS_VALID;
        } else {
            status = LBM_STATUS_INVALID;
        }
    } while (0); /*lint !e717*/
    
    LBM_SET_STATUS(lb, status);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details Find a logical block.
 *
 *  \param[in]  address - the general address of the logical block.
 *  \param[in]  size - the size of the logical block.
 *  \param[out] foundId - the found id.
 *
 *  \return If a logical block is found return BL_ERR_OK, otherwise returns
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - find a logical block successfully.
 *  \retval BL_ERR_NOT_OK  - find a logical block unsuccessfully.
 *
 *  \since  V3.0.0
 *從仿真理解； foundId是 gs_LogicalBlock 的 buf id 0是 flash app 1是flash driver
 *****************************************************************************/
static bl_Return_t _Lbm_FindLogicalBlock(bl_Address_t address,
    bl_Size_t size,
    bl_u16_t *foundId) {
    bl_Address_t limitAddr;
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t i;
    
    BL_DEBUG_ASSERT(foundId != BL_NULL_PTR);
    /* -----------------------------------------------------------------------A12
    刷写APP时，address = 0x8100,size = 13F00 → limitAddr = 1C000
    刷写flashdriver时， address为0x20007000 size为0x5AC
    ----------------------------------------------------------------------- */
    limitAddr = address + size;
    if (limitAddr >= address) {
        for (i = 0; LBM_IS_VALID_ID(i); i++) { //(i) < g_LogicalBlockList.number //app 是0
            ret = _Lbm_IsInLogicalBlock(LBM_GET_LBPTR(i), address, limitAddr); //gs_LogicalBlock[i],address,limitAddr, 在gs_LogicalBlock[i]的範圍內
            if (BL_ERR_OK == ret) {
                *foundId = i; //flashdriver时i=1
                break;
            }
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check a region whether is in the logical block. If this region is
 *           in the logical block, check it whether is in a segment list.
 *
 *  \param[in]  lb - the pointer of a logical block information.
 *  \param[in]  address - the start address of the region.
 *  \param[in]  limitAddr - the limit address of the region.
 *
 *  \return If the region is in the logical block returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the region is in the logical block.
 *  \retval BL_ERR_NOT_OK  - the region is not in the logical block.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static bl_Return_t _Lbm_IsInLogicalBlock(const bl_LogicalBlock_t *lb,    //gs_LogicalBlock[i],address,limitAddr
    bl_Address_t address,
    bl_Address_t limitAddr) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_Address_t lbLimitAddr;
    /* -----------------------------------------------------------------------A12
    LogicalBlock中的
    1. app的 address=0x8000,size=0x14000
    2. flashdriver的 address为0x20007000 size为0x800
    ----------------------------------------------------------------------- */
    lbLimitAddr = lb->address + lb->size;
    #ifdef LOGIC_BLOCK_DEBUG_EN
    g_bl_log_bk.start_addr = address;
    g_bl_log_bk.end_addr = limitAddr;
    g_bl_log_bk.limit_start_addr = lb->address;
    g_bl_log_bk.limit_end_addr = lbLimitAddr;
    #endif
    //以下判断语句的宏定义#define LBM_IS_IN_REGION(s,e,ts,te)     (((s)>=(ts)) && ((e)<=(te)))
    //相當於 address>=lb->address  同時 limitAddr<=lbLimitAddr
    if (LBM_IS_IN_REGION(address, limitAddr, lb->address, lbLimitAddr)) {
        ret = BL_ERR_OK;    //注意，該句成立，則下面的判斷，如果是lb->segments.number==0；相當於忽略
        //因為 segments 沒有配置 ，忽略
        //lb->segments.number 都为0
        if (lb->segments.number != 0u) {
            ret = _Lbm_IsInSegmentList(&lb->segments, address, limitAddr);
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check a region whether is in a segment list.
 *
 *  \param[in]  segment - the pointer of a segment list.
 *  \param[in]  address - the start address of the region.
 *  \param[in]  limitAddr - the limit address of the region.
 *
 *  \return If the region is in the segment list returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the region is in the segment list.
 *  \retval BL_ERR_NOT_OK  - the region is not in the segment list.
 *
 *  \note If the segment number of the segment list is ZERO returns
 *        BL_ERR_NOT_OK.
 *
 *  \since  V3.0.0
 *  好像目前沒有用到
 *****************************************************************************/
static bl_Return_t _Lbm_IsInSegmentList(const bl_SegmentList_t *segment,
    bl_Address_t address,
    bl_Address_t limitAddr) {
    bl_Address_t segLimitAddr;
    const bl_Segment_t *seg;
    bl_u16_t segNum;
    bl_u16_t i;
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    segNum = segment->number;
    seg = segment->segList;
    
    for (i = 0; i < segNum; i++) {
        segLimitAddr = seg[i].address + seg[i].size;
        //#define LBM_IS_IN_REGION(s,e,ts,te)     (((s)>=(ts)) && ((e)<=(te)))
        //相當於 address>=seg[i].address 同時 limitAddr<=segLimitAddr
        if (LBM_IS_IN_REGION(address, limitAddr, seg[i].address, segLimitAddr)) {
            ret = BL_ERR_OK;
            break;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check the found id whether is updated.
 *
 *  \param[in]  curId - the current id in the download context.
 *  \param[in]  foundId - a valid id found by _Lbm_FindLogicalBlock().
 *
 *  \return If the found id is updatable return BL_ERR_OK, otherwise returns
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the found id is updatable.
 *  \retval BL_ERR_NOT_OK  - the found id is not updatable.
 *  \retval LBM_ERR_UPDATED_ID  - the current id and the found id are equal.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static bl_Return_t _Lbm_IsUpdatable(bl_u16_t curId, bl_u16_t foundId) {
    bl_Return_t ret = BL_ERR_OK;
    const bl_LogicalBlock_t *block;
    
    if (LBM_IS_VALID_ID(curId)) { //(curId) < g_LogicalBlockList.number
        if (curId == foundId) { //相同，說明 還在升，不是OK
            ret = LBM_ERR_UPDATED_ID;       //值是100
        } else { //如果不同，而且當前的curId 的state 正在升級 則也不是ok
            block = LBM_GET_LBPTR(curId);
            if (LBM_IS_UPDATING_STATUS(block)) { // gs_LogicalBlock  即 gs_LogicalBlockStatus[curId] == LBM_STATUS_UPDATING
                ret = BL_ERR_NOT_OK;
            }
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Update the logical block id and the down segment index in the
 *           download context.
 *
 *  \param[in/out]  dct   - the pointer of the download context.
 *  \param[in]      newId - the new logical block id.
 *
 *  \return none
 *
 *  \note   This function directly modify the contents of the download context.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static void _Lbm_UpdateBlockInfoInDct(bl_DownContext_t *dct, bl_u16_t newId) { //指向實體 g_DownContext
    dct->lbId = newId;
    dct->segIndex = 0;  //g_DownContext 的segment 相當於 上位機的 一大塊。  block屬於他內部
    
    return ;
}

/**************************************************************************//**
 *
 *  \details Update the logical block id and the down segment index in the
 *           download context.
 *
 *  \param[in]  dct - the pointer of the download context.
 *  \param[in]  buf - the pointer of the data.
 *
 *  \return If the pointers are not null and the logical block id in the
 *          download context is valid returns BL_ERR_OK, otherwise returns
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK     - the parameters are valid.
 *  \retval BL_ERR_NOT_OK - one of parameters is not valid.
 *
 *  \note   This function directly use the contents of the download context.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static bl_Return_t _Lbm_IsValidParam(const bl_DownContext_t *dct,   //指向實體 g_DownContext
    const bl_Buffer_t *buf) {   //數據
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if ((dct != BL_NULL_PTR)
        && (buf != BL_NULL_PTR)) {
        if (LBM_IS_VALID_ID(dct->lbId)) { //(dct->lbId) < g_LogicalBlockList.number
            ret = BL_ERR_OK;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details write data to a logical block.
 *
 *  \param[in]  lb      - the pointer of the logical block information.
 *  \param[in]  address - the general address of the data.
 *  \param[in]  size    - the size of the data.
 *  \param[in]  buffer  - the contents of the data.
 *
 *  \return If the data are written successfully in the logical block returns
 *          BL_ERR_OK, otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the logical block is written successfully.
 *  \retval BL_ERR_NOT_OK - the logical block is written unsuccessfully.
 *
 *  \note this function do not change the status of the logical block.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static bl_Return_t _Lbm_Write(const bl_LogicalBlock_t *lb,   //指向實體 gs_LogicalBlock
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer) {
    bl_Return_t ret;
    bl_Address_t localAddr;
    //#define LBM_MAKE_LOCAL_ADDR(lb,address) ((address) & ((lb)->offsetMask))
    //相當於 localAddr = ((address) & ((lb)->offsetMask));
    localAddr = LBM_MAKE_LOCAL_ADDR(lb, address); //flashdriver的offsetMask=0xFFFFFFFFUL
    /* -----------------------------------------------------------------------A12
    falshdriver的memId=0,localAddr=0x20007000,size=0x400,buffer为接收到的刷写数据
    ----------------------------------------------------------------------- */
    ret = Mem_Write(lb->memId, localAddr, size, buffer);
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details try to clear the valid flag of the logical block.
 *
 *  \param[in]  lb  - the pointer of the logical block information.
 *
 *  \return If the valid flag is cleared returns BL_ERR_OK, otherwise returns
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the valid flag is cleared successfully.
 *  \retval BL_ERR_NOT_OK - the valid flag is cleared unsuccessfully.
 *
 *  \note This function is not modified the status of the logical block.
 *
 *  \since  V3.0.0
 *   好像只被 erase 調用，則只服務於 flash app 即 flash 相關，而和 ram無關
 *****************************************************************************/
static bl_Return_t _Lbm_TryToClearValidFlag(const bl_LogicalBlock_t *lb) { //指向實體 gs_LogicalBlock
    bl_Return_t ret = BL_ERR_OK;
    
    if (LBM_IS_VALID_STATUS(lb)) { //是否是 有效狀態  LBM_STATUS_VALID--因為對於flash app來說，肯定是要valid有效後，才能操作
        //清除 app valid 標志
        #if 0
        ret = Dm_ClearData(lb->validFlagId);  //如果是 flash app 則是 validFlagId=0x01 ;
        #endif
        ret = Mem_Erase(1,  //0 is ram, 1 is app
                0x1bc00,  // ((seg[i].address) & ((lb)->offsetMask))
                1024);
    }
    return ret;
}

/**************************************************************************//**
 *
 *  \details erase a logical block.If this logical block contains multiple
 *           segments, each segment can be independently erased.
 *
 *
 *  \param[in]  lb  - the pointer of the logical block information.
 *
 *  \return If the logical block is erased successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the logical block is erased successfully.
 *  \retval BL_ERR_NOT_OK - the logical block is erased unsuccessfully.
 *
 *  \note this function do not change the status of the logical block.
 *
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static bl_Return_t _lbm_Erase(const bl_LogicalBlock_t *lb) { //指向实体
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t eraseCnt;
    bl_u16_t i;
    const bl_Segment_t *seg;
    bl_Segment_t segment;
    
    /* -----------------------------------------------------------------------A12
    gs_LogicalBlock中segments.number都等于0，条件直接满足
    ----------------------------------------------------------------------- */
    if (0u == lb->segments.number) { //好像都沒有 segment；直接滿足這判定
        /*erase by the logical block address.*/
        eraseCnt = 1;
        seg = &segment;
        /* -----------------------------------------------------------------------A12
        1. app: address=0x8000,size=0x14000
        2. flashdriver: address=0x20007000,size=0x800
        3. flashcal: address=0x1C000,size=0x800
        ----------------------------------------------------------------------- */
        segment.address = lb->address;  //起始地址
        segment.size = lb->size;	    //長度
    } else {
        /*erase by the segment address.*/  //好像都沒有 segment
        eraseCnt = lb->segments.number;
        seg = lb->segments.segList;
    }
    //0 is ram, 1 is app
    for (i = 0; i < eraseCnt; i++) {
        ret = Mem_Erase(lb->memId, LBM_MAKE_LOCAL_ADDR(lb, seg[i].address), seg[i].size);
        if (ret != BL_ERR_OK) {
            ret = BL_ERR_NOT_OK;
            break;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check the hash value of the downloaded logical block whether is
 *           valid.If the marco 'LBM_FUN_AUTO_CALCULATE_HASH' is enabled and
 *           the memory of the logical block is not readable, do not need to
 *           calculate the hash value again.
 *
 *  \param[in]  dct  - the pointer of the download context.
 *  \param[in]  size - the size of the hash value.
 *  \param[in]  buf  - the data of the hash value.
 *
 *  \return If the hash value is match returns BL_ERR_OK, otherwise returns
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the hash value is match.
 *  \retval BL_ERR_NOT_OK - the hash value is not match or calculation is
 *                          failed.
 *
 *  \since  V3.0.0
 *  就是為了校驗 flash區 或者 ram區 用的校驗算法
 *****************************************************************************/
static bl_Return_t _Lbm_IsValidHash(const bl_DownContext_t *dct,   // 指向實體 g_DownContext
    bl_Size_t size,
    const bl_Buffer_t *buf) { //buf就是 傳過來的哈希結果--
    bl_Return_t ret;
    #if (LBM_FUN_AUTO_CALCULATE_HASH == BL_FUN_ON)  //不開啟自動
    const bl_LogicalBlock_t *block;
    #endif
    
    
    #if (LBM_FUN_AUTO_CALCULATE_HASH == BL_FUN_ON)
    block = LBM_GET_LBPTR(dct->lbId);
    
    ret = Mem_IsReadable(block->memId);
    if (BL_ERR_NOT_OK == ret) {
        /*Do not need to calculate the hash value again.*/
        ret = BL_ERR_OK;
    } else
    #endif
    {
        ret = _Lbm_CalculateHashByDct(dct); //啟動哈希記算
    }
    
    if (BL_ERR_OK == ret) {
        ret = Secm_VerifyHash(size, buf); //校驗總的哈希結果；是否正確
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Calculate the hash value of the download segments in same logical
 *           block.
 *
 *  \param[in]  dct  - the pointer of the download context.
 *
 *  \return If the hash value is calculated successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the calculation is successful.
 *  \retval BL_ERR_NOT_OK - the calculation is failed.
 *
 *  \warning The size of each segment that is used to calculate the hash must
 *           be a multiple of BL_BYTE_WIDTH.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static bl_Return_t _Lbm_CalculateHashByDct(const bl_DownContext_t *dct) { // 指向實體 g_DownContext
    bl_DownSegment_t *seg;
    const bl_LogicalBlock_t *block;   //指向 gs_LogicalBlock
    bl_Size_t readSize;
    bl_Size_t calSize;
    bl_Address_t calAddr;
    bl_u16_t i;
    bl_u32_t timeout = 0;
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    Secm_StartHash();  //啟動校驗算法；就只是设置个初始值
    seg = dct->segment.segList;
    block = LBM_GET_LBPTR(dct->lbId);   // gs_LogicalBlock[dct->lbId] 0是flash app 1是 flash driver
    //seg 就是 s19中 有間隔的 分段
    for (i = 0; i < dct->segIndex; i++) {
        #if (BL_BYTE_WIDTH != BL_BYTE_WIDTH_8)
        if ((seg[i].size & BL_BYTE_WIDTH) != 0) {
            /*the segment size must be a multiple of BL_BYTE_WIDTH.*/
            ret = BL_ERR_NOT_OK;
            break;
        }
        #endif
        
        /* -----------------------------------------------------------------------A12
        引导加载程序应计算所有数据的 CRC，包括数据块和 CRC块
        ----------------------------------------------------------------------- */
        calAddr = seg[i].address; //刷写flash, calAddr=0x20007000
        calSize = seg[i].size; //刷写flash, calSize=0x5AC
        readSize = (calSize % LBM_HASH_CALCULATION_BUFFER_SIZE);
        if (0UL == readSize) { //這句水平有點高，除整時余數為0 就是全部
            readSize = LBM_HASH_CALCULATION_BUFFER_SIZE;
        }
        
        do {
            /* -----------------------------------------------------------------------A12
            flashdriver的memid为0 offset为0xFFFFFFFFUL
            ----------------------------------------------------------------------- */
            ret = Mem_Read(block->memId, LBM_MAKE_LOCAL_ADDR(block, calAddr), readSize, g_HashCalculation); //calAddr & block->mask
            if (ret != BL_ERR_OK) {
                ret = BL_ERR_NOT_OK;
                break;
            }
            
            ret = Secm_UpdateHash(readSize, g_HashCalculation); //開始讀取校驗；
            if (ret != BL_ERR_OK) {
                break;
            }
            
            calSize -= readSize; //5AC-1AC
            calAddr += readSize; //200071AC
            readSize = LBM_HASH_CALCULATION_BUFFER_SIZE;
            
            g_WdgHeader.feed();  //  //g_RteInfo.wdgOps->Feed
            timeout += LBM_HASH_CALCULATION_TIME;
            if (timeout >= LBM_HASH_CALCULATION_TIMEOUT) {  //
                timeout = 0UL;
                Rte_ReportEvent(RTE_EVENT_TYPE_MEMOPS_TIMEOUT);  //要發送 78負響應
            }
        } while (calSize > 0UL);
        
        
        if (ret != BL_ERR_OK) {
            break;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Initialize the memory.
 *
 *  \param[in]  lb  - the pointer of the logical block information.
 *
 *  \return If the memory is initialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the memory is initialized successfully.
 *  \retval BL_ERR_NOT_OK - the memory is initialized successfully.
 *
 *  \see _Lbm_LookForMemId
 *
 *  \since  V3.0.0
 *  在本。C中好像是給 flash driver 服務的
 *****************************************************************************/
static bl_Return_t _Lbm_InitMemory(const bl_LogicalBlock_t *lb) { // gs_LogicalBlock
    bl_Return_t ret;
    bl_u16_t memoryId;
    
    memoryId = _Lbm_LookForMemId(lb);  //求出mem id;一般是0  ram，flash driver 的
    
    ret = Mem_InitMemory(memoryId);  //空操作BL_NULL_PTR
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Look for the memory id. Read the data in the logical block and
 *           make these data to the type of memory and the index of memory.
 *           call Mem_LookforMemId() to perform lookup action.
 *
 *  \param[in]  lb  - the pointer of the logical block information.
 *
 *  \return If the memory id is not found returns MEM_ERROR_MEMID, otherwise
 *          returns the found id.
 *
 *  \since  V3.0.0
 *  這函數的意圖 還是為了 flash driver服務的，
 *****************************************************************************/
static bl_u16_t _Lbm_LookForMemId(const bl_LogicalBlock_t *lb) { // gs_LogicalBlock
    bl_Return_t ret;
    bl_u16_t type;
    bl_u16_t index;
    bl_u16_t tmpId = MEM_ERROR_MEMID;
    bl_Address_t addr;
    bl_Buffer_t tmpBuf[MEM_MEMORY_TYPE_SIZE + MEM_MEMORY_INDEX_SIZE];  //就這句的意圖，是把 flash driver 中的 type 和 index 讀出來
    
    //#define LBM_MAKE_LOCAL_ADDR(lb,address) ((address) & ((lb)->offsetMask))
    // addr = (lb->address + LBM_LOOKFOR_OFFSET) & ((lb)->offsetMask)
    addr = LBM_MAKE_LOCAL_ADDR(lb, (lb->address + LBM_LOOKFOR_OFFSET)); /*PRQA S 2985*/
    #ifdef LOGIC_BLOCK_DEBUG_EN
    g_bl_log_bk.addr = addr;
    g_bl_log_bk.memid = lb->memId;
    #endif
    ret = Mem_Read(lb->memId,
            addr,
            (bl_Size_t)(MEM_MEMORY_TYPE_SIZE + MEM_MEMORY_INDEX_SIZE),
            tmpBuf);
    if (BL_ERR_OK == ret) {
        type = BL_MAKE_LOCAL_U16(tmpBuf);  //按buf順序復制
        index = BL_MAKE_LOCAL_U16(&tmpBuf[MEM_MEMORY_TYPE_SIZE]);
        #ifdef LOGIC_BLOCK_DEBUG_EN
        g_bl_log_bk.type = type;
        g_bl_log_bk.index = index;
        #endif
        (void)Mem_LookforMemId(type, index, &tmpId); // 求出 memid, 按這意思，求出的會是0 ram相關  ； g_MemoryInfoList
        #ifdef LOGIC_BLOCK_DEBUG_EN
        g_bl_log_bk.tmpId = tmpId;
        #endif
    }
    
    return tmpId;
}

/**************************************************************************//**
 *
 *  \details Save the status of the logical block to ROM and Update the logical
 *           block status.
 *
 *  \param[in]  block  - the pointer of a logical block.
 *
 *  \return If the status is saved successfully returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the status is saved successfully.
 *  \retval BL_ERR_NOT_OK - the status is saved unsuccessfully.
 *
 *  \warning If the status of the logical block is not LBM_STATUS_UPDATED, this
 *           function will return BL_ERR_OK.
 *  \warning If the validFlagId of the logical block is DM_ERROR_DATAID, this
 *           function will return BL_ERR_OK.
 *
 *  \since  V3.0.0
 *   看意圖 是為了 flash app
 *****************************************************************************/
static bl_Return_t _Lbm_SaveStatus(const bl_LogicalBlock_t *block) { // gs_LogicalBlock
    bl_Return_t ret = BL_ERR_OK;
    bl_Return_t isValidId;
    
    if (LBM_IS_UPDATED_STATUS(block)) { // 狀態是 UPDATED
        /* -----------------------------------------------------------------------A12
        如果是flash app,block->validFlagId=01
        如果是flash driver,block->validFlagId=0xFFFFu
        如果是flash CAL,block->validFlagId=0xFFFFu
        故此判断语句，只有app
        ----------------------------------------------------------------------- */
        isValidId = Dm_IsValidDataId(block->validFlagId);  //如果是flash app,block->validFlagId=01 ;
        if (BL_ERR_OK == isValidId) {
            ret = Dm_WriteDefaultData(block->validFlagId);  // block->validFlagId =0x01 ;  然後把 app valid 的值 寫入到該地址
            // 0xA5u,0xA5u,0xE5u,0x34u,  //REPROGRAM_DATA
            //0xAAu,0xAAu,0x04u,0xE5u,  ///application valid flag*/
            if (BL_ERR_OK == ret) {
                LBM_SET_STATUS(block, LBM_STATUS_VALID);
            } else {
                LBM_SET_STATUS(block, LBM_STATUS_INVALID);
            }
        } else {
            LBM_SET_STATUS(block, LBM_STATUS_NOT_CARE); //如果是 ram 的 flash driver 則是 LBM_STATUS_NOT_CARE
        }
    }
    
    return ret;
}


#if (0) ///(LBM_FUN_SUCCESS_COUNTER == BL_FUN_ON)

/**************************************************************************//**
 *
 *  \details save the success count of the logical block. save to ROM
 *
 *  \param[in]  did   - the index of the data manager module.
 *  \param[out] count - the success count
 *
 *  \return If the count is saved successfully returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the count is saved.
 *  \retval BL_ERR_NOT_OK - the count is failed.
 *
 *  \since  V3.11.0
 *
 *****************************************************************************/
bl_Return_t _Lbm_SaveSuccessCount(bl_u8_t did) {
    #if 0
    bl_Return_t ret;
    bl_Buffer_t buf[LBM_COUNTER_MAX_SIZE];
    
    #if (LBM_SUCCESS_COUNTER_SIZE == 4)
    BL_LOCAL_U32_TO_BUF(buf, count);
    #elif (LBM_SUCCESS_COUNTER_SIZE == 2)
    BL_LOCAL_U16_TO_BUF(buf, count);
    #elif (LBM_SUCCESS_COUNTER_SIZE == 1)
    buf[0] = count;
    #else
#error "The size of the bl_attempt_t is error."
    #endif
    
    ret = Dm_WriteData(did, sizeof(bl_SuccessCounter_t), buf);
    
    return ;
    #else
    bl_Return_t ret;
    //gs_LbmData.pgm_counter.dt.success = count;
    //ret = Mem_Erase(3, DM_DATA_3_LOCAL_ADDR, 8);
    ret = Dm_WriteData(did, 6, (bl_Buffer_t *)&gs_LbmData.pgm_counter.dt.f_attmp);
    return ret;
    #endif
}
#endif

/**************************************************************************//**
 *
 *  \details add the success count of the logical block.
 *
 *  \param[in]  lb  - the pointer of the logical block information.
 *
 *  \return If the count is added successfully returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the count is added.
 *  \retval BL_ERR_NOT_OK - the count is failed.
 *
 *  \since  V3.11.0
 *
 *****************************************************************************/
bl_Return_t _Lbm_AddSuccessCnt(void) {
    uint32_t data;
    uint8_t i;
    gs_LbmData.pgm_counter.dt.f_attmp = 0;
    if (gs_LbmData.pgm_counter.dt.success < 0xFFFF) {
        gs_LbmData.pgm_counter.dt.success++;
    }
    for (i = 0; i < Programming_Counter_EE_FLASH_SIZE; i ++) {
        data = gs_LbmData.pgm_counter.byt[i * 4 + 3];
        data <<= 8;
        data |= gs_LbmData.pgm_counter.byt[i * 4 + 2];
        data <<= 8;
        data |= gs_LbmData.pgm_counter.byt[i * 4 + 1];
        data <<= 8;
        data |= gs_LbmData.pgm_counter.byt[i * 4];
        EE_WriteVariable(Programming_Counter_EE_FLASH_ADDRESS_0 + i, data);
    }
    #ifdef BL_EEPROM_EN
    (void)EEPROM_SetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR + EEPROM_START_ADDRESS + 0x10), gs_LbmData.pgm_counter.dt.f_attmp);
    (void)EEPROM_SetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR + EEPROM_START_ADDRESS + 0x12), gs_LbmData.pgm_counter.dt.success);
    //(void)IntEeprom_WriteBytes(EEPROM_START_ADDRESS, gs_LbmData.pgm_counter.byt, 4);
    #endif
    return BL_ERR_OK;
}


bl_SuccessCounter_t Lbm_GetSuccesCount(void) {
    return (bl_SuccessCounter_t)gs_LbmData.pgm_counter.dt.success;
}
#if 0
bl_Return_t Lbm_read_eeprom_Data(bl_u8_t did) {
    bl_Buffer_t *pdata;
    bl_u32_t addr;
    bl_Size_t size;
    bl_Return_t ret = Dm_IsValidDataId(did);
    if (BL_ERR_OK == ret) {
        switch (did) {
            case 2:
                if (IntEeprom_ReadBytes(EEPROM_START_ADDRESS, gs_LbmData.fingprnt, 16) != FALSE) {
                
                } else {
                    ret = BL_ERR_NOT_OK;
                }
                break;
            case 1:
            
                break;
            case 3:
                if (IntEeprom_ReadBytes(EEPROM_START_ADDRESS + 0x10, gs_LbmData.pgm_counter.byt, 4) != FALSE) {
                    if (gs_LbmData.pgm_counter.dw == 0xffffffffUL) {
                        gs_LbmData.pgm_counter.dw = 0;
                    }
                    if (gs_LbmData.pgm_counter.dt.f_attmp > 0xffUL) {
                        gs_LbmData.pgm_counter.dt.f_attmp = 0xff;
                    }
                    if (gs_LbmData.pgm_counter.dt.success > 0xffUL) {
                        gs_LbmData.pgm_counter.dt.success = 0xff;
                    }
                    ret = BL_ERR_OK;
                } else {
                    gs_LbmData.pgm_counter.dw = 0;
                    ret = BL_ERR_NOT_OK;
                }
                break;
            case 4:
                if (IntEeprom_ReadBytes(EEPROM_START_ADDRESS + 0x14, gs_LbmData.pgm_date, 4) != FALSE) {
                    if ((gs_LbmData.pgm_date[0] == 0xff) && (gs_LbmData.pgm_date[1] == 0xff) &&
                        (gs_LbmData.pgm_date[2] == 0xff) && (gs_LbmData.pgm_date[3] == 0xff)) {
                        gs_LbmData.pgm_date[0] = gs_LbmData.pgm_date[1] = gs_LbmData.pgm_date[2] = gs_LbmData.pgm_date[3] = 0;
                    }
                    ret = BL_ERR_OK;
                } else {
                    ret = BL_ERR_NOT_OK;
                }
                break;
            default:
                ret = BL_ERR_NOT_OK;
                break;
        }
    } else {
        ret = BL_ERR_OK;
    }
    return ret;
}
#endif

#if (LBM_FUN_ATTEMPT_COUNTER == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details add the attempt count of the logical block.
 *
 *  \param[in]  lb  - the pointer of the logical block information.
 *
 *  \return If the count is added successfully returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the count is added.
 *  \retval BL_ERR_NOT_OK - the count is failed.
 *
 *  \since  V3.11.0
 *
 *****************************************************************************/
static bl_Return_t _Lbm_AddAttemptCnt(void) {
    bool_m_t w_s_cnt = FALSE_M;
    uint8_t i;
    bl_u32_t Programming_Counter;
    uint32_t data;
    #ifdef BL_EEPROM_EN
    (void)EEPROM_GetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR + EEPROM_START_ADDRESS + 0x10), &gs_LbmData.pgm_counter.dt.f_attmp);
    (void)EEPROM_GetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR + EEPROM_START_ADDRESS + 0x12), &gs_LbmData.pgm_counter.dt.success);
    #endif
    for (i = 0; i < Programming_Counter_EE_FLASH_SIZE; i ++) {
        if (EE_ReadVariable(Programming_Counter_EE_FLASH_ADDRESS_0 + i, &Programming_Counter) == 1) {
            gs_LbmData.pgm_counter.byt[i * 4] = 0xffu;
            gs_LbmData.pgm_counter.byt[i * 4 + 1] = 0xffu;
            gs_LbmData.pgm_counter.byt[i * 4 + 2] = 0xffu;
            gs_LbmData.pgm_counter.byt[i * 4 + 3] = 0xffu;
        } else {
            gs_LbmData.pgm_counter.byt[i * 4] = (uint8_t)(Programming_Counter);
            gs_LbmData.pgm_counter.byt[i * 4 + 1] = (uint8_t)(Programming_Counter >> 8);
            gs_LbmData.pgm_counter.byt[i * 4 + 2] = (uint8_t)(Programming_Counter >> 16);
            gs_LbmData.pgm_counter.byt[i * 4 + 3] = (uint8_t)(Programming_Counter >> 24);
        }
    }
    if (gs_LbmData.pgm_counter.dw > 0xfffffffdUL) {
        gs_LbmData.pgm_counter.dw = 0;
        w_s_cnt = TRUE_M;
    }
    if (gs_LbmData.pgm_counter.dt.f_attmp > 0xffUL) {
        gs_LbmData.pgm_counter.dt.f_attmp = 0xff;
    }
    if (gs_LbmData.pgm_counter.dt.success >= 0xFFFFUL) {
        gs_LbmData.pgm_counter.dt.success = 0xFFFF;
        w_s_cnt = TRUE_M;
    }
    if (gs_LbmData.pgm_counter.dt.f_attmp < 0xff) {
        gs_LbmData.pgm_counter.dt.f_attmp++;
        for (i = 0; i < Programming_Counter_EE_FLASH_SIZE; i ++) {
            data = gs_LbmData.pgm_counter.byt[i * 4 + 3];
            data <<= 8;
            data |= gs_LbmData.pgm_counter.byt[i * 4 + 2];
            data <<= 8;
            data |= gs_LbmData.pgm_counter.byt[i * 4 + 1];
            data <<= 8;
            data |= gs_LbmData.pgm_counter.byt[i * 4];
            EE_WriteVariable(Programming_Counter_EE_FLASH_ADDRESS_0 + i, data);
        }
        #ifdef BL_EEPROM_EN
        (void)EEPROM_SetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR + EEPROM_START_ADDRESS + 0x10), gs_LbmData.pgm_counter.dt.f_attmp);
        //(void)IntEeprom_WriteBytes(EEPROM_START_ADDRESS+0x10, gs_LbmData.pgm_counter.byt, 4);
        #endif
    }
    if (w_s_cnt != FALSE_M) {
        #ifdef BL_EEPROM_EN
        (void)EEPROM_SetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR + EEPROM_START_ADDRESS + 0x12), gs_LbmData.pgm_counter.dt.success);
        #endif
    }
    return BL_ERR_OK;
}

/**************************************************************************//**
 *
 *  \details get the attempt count of the logical block.
 *
 *  \param[in]  did   - the index of the data manager module.
 *  \param[out] count - the buffer of the attempt count
 *
 *  \return If the count is gotten successfully returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the count is gotten.
 *  \retval BL_ERR_NOT_OK - the count is failed.
 *
 *  \since  V3.11.0
 *
 *****************************************************************************/
bl_AttemptCounter_t Lbm_GetAttmptCount(void) {
    return gs_LbmData.pgm_counter.dt.f_attmp;
}


/**************************************************************************//**
 *
 *  \details save the attempt count of the logical block.
 *
 *  \param[in]  did   - the index of the data manager module.
 *  \param[out] count - the attempt count
 *
 *  \return If the count is saved successfully returns BL_ERR_OK, otherwise
 *          returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the count is saved.
 *  \retval BL_ERR_NOT_OK - the count is failed.
 *
 *  \since  V3.11.0
 *
 *****************************************************************************/
#endif

#if 0
bl_Return_t _Lbm_SaveDataToRom(bl_u8_t w_index) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_Buffer_t *pdata;
    bl_u32_t addr;
    bl_Size_t size;
    if (BL_ERR_OK == ret) {
        switch (w_index) {
        
            case 3:
                addr = EEPROM_START_ADDRESS + 0x10; //DM_DATA_3_LOCAL_ADDR;
                pdata = &(gs_LbmData.pgm_counter.dt.f_attmp);
                size = 2;
                ret = BL_ERR_NOT_OK;
                break;
                
            case 4:
                addr = EEPROM_START_ADDRESS + 0x12; //DM_DATA_3_LOCAL_ADDR;
                pdata = &(gs_LbmData.pgm_counter.dt.success);
                size = 2;
                ret = BL_ERR_NOT_OK;
                break;
                
            case 5:
                addr = EEPROM_START_ADDRESS + 0x10; //DM_DATA_3_LOCAL_ADDR;
                pdata = &(gs_LbmData.pgm_counter.dt.f_attmp);
                size = 4;
                ret = BL_ERR_NOT_OK;
                break;
                
            default:
            
                break;
        }
        if (ret != BL_ERR_OK) {
            if (IntEeprom_WriteBytes(addr, pdata, size) != FALSE) {
                ret = BL_ERR_OK;
            };
        }
    } else {
        ret = BL_ERR_OK;
    }
    return ret;
}
#endif




//
void lb_logic_block_init_flashdrv(void) {
    u8 i;
    u8 l_buf[DM_DATA_3_SIZE] = {0};
    const bl_LogicalBlock_t *block;
    
    if (LBM_IS_VALID_ID(LBM_LB1_ID_DRV)) { //(dct->lbId) < g_LogicalBlockList.number
        block = LBM_GET_LBPTR(LBM_LB1_ID_DRV);   // gs_LogicalBlock[dct->lbId]
        _Lbm_InitMemory(block);
        LBM_SET_STATUS(block, LBM_STATUS_UPDATED);
    }
    
    #ifdef DEFAULT_GOTO_APP_DEFAULT_SEESION_78
    for (i = 0; i < DM_DATA_3_SIZE; i++) {
        l_buf[i] = 0;
    }
    (void)Mem_Write(MEM_MEMORY_ID_RAM, DM_DATA_3_LOCAL_ADDR, DM_DATA_3_SIZE, &l_buf[0]); //
    #endif
}


u8 lb_logic_block_read_app_valid1(void) {
    u8 l_buf[4] = {0};
    u8 l_result = 0;
    u8 i;
    uc8 *p_data;
    p_data = DM_DATA_2_DDP;
    Flsif_Read(APP_VALID_FLAG_1_ADDR, 4, (bl_Buffer_t *)&l_buf[0]);
    l_result = 1;
    for (i = 0; i < 4; i++) {
        if (p_data[i] != l_buf[i]) {
            l_result = 0;
            break;
        }
    }
    return l_result;
}


bl_u8_t check_program_valid_nores_is_valid(void) {
    u8 l_buf[16], i;
    bl_u8_t l_result = BOOTING_FLAG_REPROGRAM_VALID;
    uc8 *p_data;
    p_data = DM_DATA_0_DDP;
    //gs_default_app_flag1
    (void)Mem_Read(MEM_MEMORY_ID_RAM, DM_DATA_0_LOCAL_ADDR, DM_DATA_0_SIZE, &l_buf[0]); //
    for (i = 0; i < DM_DATA_2_SIZE; i++) {
        if (l_buf[i] != (p_data[i] + 1u)) {
            l_result = BOOTING_FLAG_NULL;
            break;
        }
    }
    l_buf[0] = 0;
    l_buf[1] = 0;
    l_buf[2] = 0;
    l_buf[3] = 0;
    (void)Mem_Write(MEM_MEMORY_ID_RAM, DM_DATA_0_LOCAL_ADDR, DM_DATA_0_SIZE, &l_buf[0]); //
    return l_result;
}


//单独判断，program 是否有标志 --用于APP被擦后，1002 跳到 1001的应答
bl_u8_t check_program_valid_to_app_valid(void) {
    u8 l_buf[16], i;
    bl_u8_t l_result = BOOTING_FLAG_REPROGRAM_VALID;
    uc8 *p_data;
    p_data = DM_DATA_3_DDP;
    //gs_default_app_flag1
    (void)Mem_Read(MEM_MEMORY_ID_RAM, DM_DATA_3_LOCAL_ADDR, DM_DATA_3_SIZE, &l_buf[0]); //
    for (i = 0; i < DM_DATA_3_SIZE; i++) {
        if (l_buf[i] != (p_data[i])) {
            l_result = BOOTING_FLAG_NULL;
            break;
        }
    }
    #if 0
    l_buf[0] = 0;
    l_buf[1] = 0;
    l_buf[2] = 0;
    l_buf[3] = 0;
    (void)Mem_Write(MEM_MEMORY_ID_RAM, DM_DATA_3_LOCAL_ADDR, DM_DATA_3_SIZE, &l_buf[0]); //
    #endif
    return l_result;
}


//单独判断，program 是否有标志 --用于APP被擦后，1002 跳到 1001的应答
void clear_program_valid_to_app_valid(void) {
    u8 l_buf[16];
    l_buf[0] = 0;
    l_buf[1] = 0;
    l_buf[2] = 0;
    l_buf[3] = 0;
    (void)Mem_Write(MEM_MEMORY_ID_RAM, DM_DATA_3_LOCAL_ADDR, DM_DATA_3_SIZE, &l_buf[0]); //
}

