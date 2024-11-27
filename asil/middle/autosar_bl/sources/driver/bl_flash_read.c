/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the flash module.
 *
 *  \file       bl_flash_read.c
 *  \ingroup    driver_module
 *  \author
 *
 *  \version    1.1.0
 *  \date       14/05/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | -------------
 *      01.00.00 | 14/05/2014 | ning.chen        | N/A | BootDrv010001
 *      01.01.00 | 21/08/2014 | ning.chen        | N/A | BootDrv010002
 *
 *****************************************************************************/
#include "bl_flash_if.h"
#include "bl_flash_if_cfg.h"


/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 306,506,781,1015,1532,3112,3227,3408 EOF*/
/*
 * MISRA-C:2004 11.3(306):Cast a pointer to an integral type.
 * To address a flash must cast an integral type to a pointer.
 *
 * 506:Dereferencing pointer value that is possibly NULL.
 * The parameter delay is not a null pointer.
 *
 * MISRA-C:2004 5.6(781):The identifier is used as a structure/union member.
 * The member of the structure is used through the structure object.
 *
 * 1015:Far is not a legal keyword in ISO C
 * This is a valid keyword in the CodeWarrior.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3227:The parameter is never modified and so it could be declared with const.
 *
 * MISRA-C:2004 8.8(3408):The identifier has external linkage but is being
 * defined without any previous declaration.
 * The Flash_Read has a previous declaraton in the bl_flash.h file.
 *
 */

#if 0
#define FLASH_PPAGE     (*(volatile bl_u8_t *)(0x15u))

#define FLASH_PAGE_START_ADDRESS            (0x8000u)

#define FLASH_PAGE_BIT_OFFSET               (14)
#endif
#define FLASH_PAGE_SIZE                     FLASHIF_BLOCK0_PAGE_SIZE
#define FLASH_GET_LOW_ADDRESS(address)      ((address) & 0x0FFFu)   //����� stm32f103ze f107vc �t�� 0x800

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Read the data from the internal Flash device.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Flash_Read(bl_FlashDrvParam_t *par)
{
    #if 1
    //bool_m_t l_resut;
    status_t l_resut = STATUS_SUCCESS;    /* Return code variable */
    BL_DEBUG_ASSERT_NO_RET(par != BL_NULL_PTR);

    l_resut = flash_drv_pflash_read(par->address,par->size,par->buffer);

    if(l_resut == STATUS_SUCCESS)
    {
        par->errCode = BL_ERR_OK;
    }

    #else

        bl_Size_t i;
    bl_Size_t size;
    bl_Buffer_t *buffer;
    bl_Address_t address;
    bl_Buffer_t *src;
    bl_u8_t ppage;
    bl_u16_t readSize;
    bl_u16_t remainSize;

    BL_DEBUG_ASSERT_NO_RET(par != BL_NULL_PTR);

    size = par->size;
    buffer = par->buffer;
    address = par->address;
    remainSize = (bl_u16_t)(FLASH_PAGE_SIZE - FLASH_GET_LOW_ADDRESS(address));
    src = (bl_Buffer_t *)(FLASH_PAGE_START_ADDRESS
                            + FLASH_GET_LOW_ADDRESS(address));

    if (remainSize > size)
    {
        readSize = (bl_u16_t)size;
    }
    else
    {
        readSize = remainSize;
    }

    ppage = FLASH_PPAGE;
    FLASH_PPAGE = (bl_u8_t)(address >> FLASH_PAGE_BIT_OFFSET);

    for (i = 0; i < readSize; i++)
    {
        buffer[i] = src[i]; /*lint !e511*/
    }

    size -= readSize;
    buffer = &buffer[readSize];

    while(size > 0)
    {
        FLASH_PPAGE = FLASH_PPAGE + 1;
        src = (bl_Buffer_t *)FLASH_PAGE_START_ADDRESS;

        if (size > FLASH_PAGE_SIZE)
        {
            readSize = FLASH_PAGE_SIZE;
        }
        else
        {
            readSize = (bl_u16_t)size;
        }

        for (i = 0; i < readSize; i++)
        {
            buffer[i] = src[i]; /*lint !e511*/
        }

        size -= readSize;
        buffer = &buffer[readSize];
    }

    FLASH_PPAGE = ppage;

    par->errCode = BL_ERR_OK;

    #endif

    return ;
}

