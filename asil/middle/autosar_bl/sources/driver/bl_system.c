/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the system clock module.
 *
 *  \file       bl_system.c
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
#include "bl_system.h"


#include "clk.h"
#include "boot_app.h"
#include "logic_time.h"
#include "Cpu.h"
#include "nvic.h"
//#include "lin_driver.h"

#include "clk.h"
#include "gpio.h"

#include "can.h"
#include "tim.h"

#include "boot_app.h"
#include "nvic.h"


#include "wdog.h"
//#include "lin_app.h"
//#include "device_registers.h"
#include "bl_timer.h"

#include "svd.h"
#include "svd_interrupt.h"
/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 277,303,305,1251,1257,1503,1532,2100,3112,3442,3453,4130 EOF*/
/*
 * 277:An integer constant expression with negative value is being converted
 * to an unsigned type.
 *
 * MISRA-C:2004 11.3(303):Cast a pointer to an integral type.
 * To address a register must cast an integral type to a pointer.
 *
 * MISRA-C:2004 11.3(305):Cast a pointer to an integral type.
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
 * 2100:Integral promotion : unsigned char promoted to signed int.
 *
 * MISRA-C:2004 14.2(3112):This statement has no side-effect.
 * The DEBUG_ASSERT macro is used to debug bootloader when the BL_DEBUG is
 * enable. When the BL_DEBUG is disable, the DEBUG_ASSERT macro is null.
 *
 * 3442:Operator other than & or = applied to a volatile object.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 *
 * MISRA-C:2004 12.7(4130):Bitwise operations on signed data will give
 * implementation defined results.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/



/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/
/** a function pointer is used to go to application*/
typedef void (* theApp)(void);

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Initialize system clock.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Sys_InitClock(void)
{
  
 
    __disable_irq();
    clk_init();
    gpio_init();

    nvm_init();
    
    
    
      
    //spm_init();
    //wdog_init();
}

/**************************************************************************//**
 *
 *  \details Deinitialize system clock.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Sys_DeinitClock(void)
{
    //
}

/**************************************************************************//**
 *
 *  \details Set system clock to sleep mode.
 *
 *  \return None.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Sys_Sleep(void)
{
    /*PRQA S 899*/
    //_asm STOP;          /*into STOP mode*/
    //_asm NOP;
    // _asm NOP;
}

/**************************************************************************//**
 *
 *  \details Remap the interrupt vector table.
 *
 *  \since V1.0.0
 *
 *****************************************************************************/
void Sys_RemapInterrupt(void)
{
    nvic_set_vector_table(FLASH_APP_BASE,BOOT_APP_VECTOR_OFFSET);
}

/**************************************************************************//**
 *
 *  \details Set up the application entry point, and go to the application.
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Sys_GotoApplication(void)
{
    /*when a new application is downloaded by the bootloader, the enter address
      of this application maybe is different from the old one.*/
    /*PRQA S 3204 1*/
    boot_app_jump_to_app();
}

/**************************************************************************//**
 *
 *  \details .
 *
 *  \since  V1.0.0
 *
 *****************************************************************************/
void Sys_ProtectBootloader(void)
{

}

