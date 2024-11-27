/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the memory manager module.
 *
 *  \file       bl_memory.c
 *  \ingroup    memory_manager_module
 *  \author
 *
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 17/03/2010 | hewei.zhang      | N/A | Boot010001
 *      01.01.00 | 20/04/2010 | hewei.zhang      | N/A | Boot010002
 *      01.04.00 | 26/06/2010 | hewei.zhang      | N/A | Boot010005
 *      02.00.00 | 21/02/2011 | mingqing.tang    | N/A | Boot020001
 *      02.02.00 | 15/03/2011 | mingqing.tang    | N/A | Boot020003
 *      02.07.00 | 14/06/2011 | mingqing.tang    | N/A | Boot020008
 *      03.00.00 | 12/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.03.00 | 17/10/2011 | mingqing.tang    | N/A | Boot030004
 *      03.04.00 | 24/10/2011 | mingqing.tang    | N/A | Boot030005
 *      03.10.00 | 03/01/2012 | mingqing.tang    | N/A | Boot030011
 *      03.12.00 | 18/01/2012 | mingqing.tang    | N/A | Boot030013
 *      03.18.00 | 29/05/2012 | mingqing.tang    | N/A | Boot030019
 *      05.00.00 | 19/06/2013 | mingqing.tang    | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_rte.h"
#include "bl_memory.h"
#include "bl_memory_cfg.h"

#include "common_types.h"
#include "bl_system_cfg.h"

#ifdef MEMORY_DEBUG_EN
typedef struct {
    u8 id[2];
    u32 addr;
    u32 size;
} memory_test_s;
volatile memory_test_s g_memory_test = {0};
#endif

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 306,506,602,1251,1503,1532,2103,3112,3227,3453 EOF*/
/*
 * MISRA-C:2004 11.3(306):Cast a pointer to an integral type.
 *
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 20.2(602):The identifier is reserved for use by the library.
 * The Bootloader do not use any library funtion and use the underscores to
 * indicate that the function is a interal function.
 *
 * 1251:Suffixed integer constant causes implicit conversion of other
 * (integral) operand.
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
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 * These macros have no side-effect.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
/** This macro is used to check a driver whether is a external driver.*/
#define MEM_IS_EXTERNAL_DRV(id)  ((g_MemoryInfoList.memList[id].type \
        & MEM_MEMORY_TYPE_EXTERNAL_DRV) != 0u)
/** This macro is used to check a driver whether is a internal driver.*/
#define MEM_IS_INTERNAL_DRV(id)  ((g_MemoryInfoList.memList[id].type \
        & MEM_MEMORY_TYPE_EXTERNAL_DRV) == 0u)
/** Check the id of the memory whether is valid.*/
#define MEM_IS_VALID_MEMORY_ID(id)  ((id) < g_MemoryInfoList.number)
/** Check a memory whether is readable or not.*/
#define MEM_IS_READABLE_MEMORY(id)  (g_MemoryInfoList.memList[id].ops->Read \
    != BL_NULL_PTR)

#define MEM_IS_WRITABLE_MEMORY(id)  (g_MemoryInfoList.memList[id].ops->Write\
    != BL_NULL_PTR)

#define MEM_IS_ERASABLE_MEMORY(id)  (g_MemoryInfoList.memList[id].ops->Erase\
    != BL_NULL_PTR)

#define MEM_READ(id,addr,size,buf)  g_MemoryInfoList.memList[id].ops->Read\
    ((addr),(size),(buf))

#define MEM_WRITE(id,addr,size,buf) g_MemoryInfoList.memList[id].ops->Write\
    ((addr),(size),(buf))

#define MEM_ERASE(id,addr,size)     g_MemoryInfoList.memList[id].ops->Erase\
    ((addr),(size))

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
/** \brief initialize a memory.*/
static bl_Return_t _Mem_Init(bl_u16_t id);
/** \brief de-initialize a memory.*/
static bl_Return_t _Mem_Deinit(bl_u16_t id);
/** \brief check the parameters whether are valid or not.*/
static bl_Return_t _Mem_IsValidParam(bl_u16_t id,
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer);
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
 *  \details Initialize all driver of memory except its driver is not in the
 *           internal ROM or it don't need initialize.
 *
 *  \return If the all memory driver is initialized successfully return
 *          BL_ERR_OK. otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK     - this module is initialized successfully.
 *  \retval BL_ERR_NOT_OK - this module is not initialized successfully.
 *
 *  \warning If all memory drvier are external driver, it will return
 *           BL_ERR_NOT_OK. Because in this case, there should be a RAM driver,
 *           but it should not be a external driver.
 *  \warning In this interface, it will return BL_ERR_OK even though
 *           the initialization interface of the memory is BL_NULL_PTR.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
bl_Return_t bl_Mem_Init(void) {
    bl_u16_t i;
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    for (i = 0; MEM_IS_VALID_MEMORY_ID(i); i++) {
        if (MEM_IS_INTERNAL_DRV(i)) {
            ret = _Mem_Init(i);
            if (ret != BL_ERR_OK) {
                break;
            }
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details If the id is valid, initialize the memory.
 *
 *  \param[in]  id  - the index of a memory in the memory information list.
 *
 *  \return If the memory is initialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the memory is initialized successfully.
 *  \retval BL_ERR_NOT_OK   - the memory is not initialized successfully.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Mem_InitMemory(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (MEM_IS_VALID_MEMORY_ID(id)) {
        ret = _Mem_Init(id);
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details If the id is valid, deinitialize the memory.
 *
 *  \param[in]  id  - the index of a memory in the memory information list.
 *
 *  \return If the memory is deinitialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the memory is deinitialized successfully.
 *  \retval BL_ERR_NOT_OK   - the memory is not deinitialized successfully.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Mem_DeinitMemory(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (MEM_IS_VALID_MEMORY_ID(id)) {
        ret = _Mem_Deinit(id);
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Read data from the memory.
 *
 *  \param[in]  id      - the index of a memory in the memory information list.
 *  \param[in]  address - the address of the memory.
 *  \param[in]  size    - the size of the memory.
 *  \param[out] buffer  - the buffer of the read data.
 *
 *  \return If the read operation is processed successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - the data is read successfully.
 *  \retval BL_ERR_NOT_OK   - fail to read.
 *
 *  \since  V1.0.0
 *  id=0是ram  id=1是flash
 *****************************************************************************/
/* -----------------------------------------------------------------------A12
id = 0,address = 0x20007FC0u,size = 4,
刷写flashdriver的id=0，address=0x20007000u，size=0x9A，buffer=g_HashCalculation
31 2020服务flashdriver的 id=0 ，address=0x20007000u，size = 4
----------------------------------------------------------------------- */
bl_Return_t Mem_Read(bl_u16_t id,
    bl_Address_t address,
    bl_Size_t size,
    bl_Buffer_t *buffer) {
    bl_Return_t ret;
    
    //默认ret = BL_ERR_NOT_OK，4个参数均检查有效之后ret = BL_ERR_OK
    ret = _Mem_IsValidParam(id, address, size, buffer);
    if (BL_ERR_OK == ret) {
        if (MEM_IS_READABLE_MEMORY(id)) { //检查g_MemoryInfoList中的Read操作是否有效
            /* -----------------------------------------------------------------------A12
            下列4个形参均为gs_DataInfo中的数据，将0x20007FC0u读取后，返回值默认为BL_ERR_OK。
            ----------------------------------------------------------------------- */
            ret = MEM_READ(id, address, size, buffer);
        } else {
            ret = BL_ERR_NOT_OK;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Write data into the memory.
 *
 *  \param[in]  id      - the index of a memory in the memory information list.
 *  \param[in]  address - the address of the memory.
 *  \param[in]  size    - the size of the memory.
 *  \param[out] buffer  - the buffer of the data.
 *
 *  \return If the write operation is processed successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the data is written successfully.
 *  \retval BL_ERR_NOT_OK   - fail to write.
 *
 *  \note   If MEM_FUN_CALLBACK_PROCESS is enabled, this function will call the
 *          _Mem_DoNextOps().
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Mem_Write(bl_u16_t id,
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer) {
    bl_Return_t ret;
    /* -----------------------------------------------------------------------A12
    memId=1,localAddr=0x1BFFC,size=4,defData=&gs_defaultDataList[4]
    falshdriver的memId=0,localAddr=0x20007000,size=0x400,buffer为36服务接收到的刷写数据
    ----------------------------------------------------------------------- */
    ret = _Mem_IsValidParam(id, address, size, buffer);
    if (BL_ERR_OK == ret) {
        if (MEM_IS_WRITABLE_MEMORY(id)) { //检查g_MemoryInfoList中的write操作是否有效
            ret = MEM_WRITE(id, address, size, buffer);
        } else {
            ret = BL_ERR_NOT_OK;
        }
    }
    
    return ret;
    
}

/**************************************************************************//**
 *
 *  \details Erase memory.
 *
 *  \param[in]  id      - the index of a memory in the memory information list.
 *  \param[in]  address - the address of the memory..
 *  \param[in]  size    - the size of the memory.
 *
 *  \return If the erase operation is processed successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the data is erased successfully.
 *  \retval BL_ERR_NOT_OK   - fail to erase.
 *
 *  \note   If MEM_FUN_CALLBACK_PROCESS is enabled, this function will call the
 *          _Mem_DoNextOps().
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Mem_Erase(bl_u16_t id, bl_Address_t address, bl_Size_t size) {
    bl_Return_t ret;
    
    ret = _Mem_IsValidParam(id, address, size, BL_VIRTUAL_PTR); /*PRQA S 0326*/
    if (BL_ERR_OK == ret) {
        /* -----------------------------------------------------------------------A12
        id=0,Erase为空指针
        id=1,address=0x8000,size=0x14000
        ----------------------------------------------------------------------- */
        if (MEM_IS_ERASABLE_MEMORY(id)) {
            ret = MEM_ERASE(id, address, size);
        } else {
            ret = BL_ERR_NOT_OK;
        }
    }
    
    return ret;
    
}

/**************************************************************************//**
 *
 *  \details By passing the type and index, looking for a match memory. If
 *           found, returns the memory id.
 *
 *  \param[in]  type    - the type of a memory.
 *  \param[in]  index   - the index of a memory.
 *  \param[out] *mid    - the buffer of the memory id.
 *
 *  \return If a memory is found return BL_ERR_OK and the memory id save in the
 *          mid, otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - found a memory.
 *  \retval BL_ERR_NOT_OK   - Did not find any memory.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Mem_LookforMemId(bl_u16_t type, bl_u16_t index, bl_u16_t *mid) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_u16_t i;
    
    BL_DEBUG_ASSERT_PARAM(mid != BL_NULL_PTR);
    
    for (i = 0; MEM_IS_VALID_MEMORY_ID(i); i++) {
        if ((g_MemoryInfoList.memList[i].type == type)
            && (g_MemoryInfoList.memList[i].index == index)) {
            *mid = i;
            ret = BL_ERR_OK;
            break;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check a driver of memory whether is in the internal ROM or not.
 *
 *  \param[in]  id  - the index of a memory in the memory information list.
 *
 *  \return if a driver of memory is a external driver returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK           - The driver is external.
 *  \retval BL_ERR_NOT_OK       - The driver is internal.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Mem_IsExternalDrv(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (MEM_IS_VALID_MEMORY_ID(id)) { //(id) < g_MemoryInfoList.number
        if (MEM_IS_EXTERNAL_DRV(id)) { // g_MemoryInfoList.memList[id].type & MEM_MEMORY_TYPE_EXTERNAL_DRV
            ret = BL_ERR_OK;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check a memory whether is readable or not.
 *
 *  \param[in]  id  - the index of a memory in the memory information list.
 *
 *  \return if the memory is readable returns BL_ERR_OK, otherwise returns
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK           - the memory is reabable.
 *  \retval BL_ERR_NOT_OK       - the memory is NOT reable.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
bl_Return_t Mem_IsReadable(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    
    if (MEM_IS_VALID_MEMORY_ID(id)) {
        if (MEM_IS_READABLE_MEMORY(id)) {
            ret = BL_ERR_OK;
        }
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Initialize the memory except the initialization interface is null.
 *
 *  \param[in]  id  - the index of a memory in the memory information list.
 *
 *  \return If the memory is initialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK           - the memory is initialized successfully.
 *  \retval BL_ERR_NOT_OK       - the memory is not initialized successfully.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
static bl_Return_t _Mem_Init(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_OK;
    
    #ifdef MEMORY_DEBUG_EN
    if (id == 0x00) {
        g_memory_test.id[id]++;
    } else {
        g_memory_test.id[id]++;
    }
    #endif
    
    if (g_MemoryInfoList.memList[id].ops->Init != BL_NULL_PTR) {
        ret = g_MemoryInfoList.memList[id].ops->Init();
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Deinitialize the memory except the deinitialization interface is
 *           null.
 *
 *  \param[in]  id  - the index of a memory in the memory information list.
 *
 *  \return If the memory is deinitialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK           - the memory is deinitialized successfully.
 *  \retval BL_ERR_NOT_OK       - the memory is not deinitialized successfully.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
static bl_Return_t _Mem_Deinit(bl_u16_t id) {
    bl_Return_t ret = BL_ERR_OK;
    
    if (g_MemoryInfoList.memList[id].ops->Deinit != BL_NULL_PTR) {
        ret = g_MemoryInfoList.memList[id].ops->Deinit();
    }
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Check parameters whether are valid.
 *
 *  \param[in]  id      - the index of a memory in the memory information list.
 *  \param[in]  address - the offset of the memory.
 *  \param[in]  size    - the size of the memory.
 *  \param[out] buffer  - the buffer of the data.
 *
 *  \return If all parameters are valid returns BL_ERR_OK, otherwise returns
 *          BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK        - all parameters is valid.
 *  \retval BL_ERR_NOT_OK    - at least one parameters is invalid.
 *
 *  \since  V2.0.0
 *
 *****************************************************************************/
static bl_Return_t _Mem_IsValidParam(bl_u16_t id,
    bl_Address_t address,
    bl_Size_t size,
    const bl_Buffer_t *buffer) {
    bl_Return_t ret = BL_ERR_NOT_OK;
    bl_Address_t memAddr;
    bl_Size_t memSize;
    //id = 0 是有效的，g_DataInfoList.gs_DataInfo[BOOTM_REPROGRAM_FLAG_DID].memId
    BL_DEBUG_ASSERT_PARAM(MEM_IS_VALID_MEMORY_ID(id)); //断言已做屏蔽处理
    BL_DEBUG_ASSERT_PARAM(buffer != BL_NULL_PTR); //断言已做屏蔽处理
    
    //检查g_DataInfoList的ID是否在g_MemoryInfoList的ID范围之内
    if (MEM_IS_VALID_MEMORY_ID(id)) {
        if (((address + size) >= address) && (buffer != BL_NULL_PTR)) { //buffer已在前面的调用函数内定义为局部数组
            memAddr = g_MemoryInfoList.memList[id].addr; //刷写driver时0x20000000UL
            memSize = g_MemoryInfoList.memList[id].size; //刷写driver时0x8000UL
            
            /* -----------------------------------------------------------------------A12
            1. address = 0x20007FC0u,            → memAddr = 0x20000000UL,memSize = 0x00008000UL
            2. address = 0x1C000u,size = 0x400   → memAddr = 0x8000UL,memSize = 0x14000UL
            3. address = 0x20007000u，size = 0x4 → memAddr = 0x20000000UL,memSize = 0x00008000UL
            ----------------------------------------------------------------------- */
            if ((address >= memAddr) && ((address + size) <= (memAddr + memSize))) {
                ret = BL_ERR_OK;
            }
        }
    }
    
    return ret;
}

