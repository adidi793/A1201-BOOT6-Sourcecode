/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the timer module.
 *
 *  \file       bl_timer.c
 *  \ingroup    driver_module
 *  \author     
 *
 *  \version    1.0.0
 *  \date       14/05/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | -------------
 *      01.00.00 | 14/05/2014 | ning.chen        | N/A | BootDrv010001
 *****************************************************************************/
#include "bl_system.h"
#include "bl_timer.h"

#include "Cpu.h"
#include "tim.h"
#include "common_types.h"
//#include "ac780x_timer.h"



#if 1
#define TIM_APP_ALLOC()  //CPU_SR_ALLOC()
#define TIM_APP_ENTER_CRITICAL() INT_SYS_DisableIRQGlobal()
#define TIM_APP_EXIT_CRITICAL()  INT_SYS_EnableIRQGlobal()
#else
#define TIM_APP_ALLOC()  //CPU_SR_ALLOC()
#define TIM_APP_ENTER_CRITICAL() INT_SYS_DisableIRQGlobal()
#define TIM_APP_EXIT_CRITICAL()  INT_SYS_EnableIRQGlobal()
#endif

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 303,1251,1257,1532,2100,3442,3453 EOF*/
/*
 * MISRA-C:2004 11.3(303):Cast a pointer to an integral type.
 * To address a register must cast an integral type to a pointer.
 *
 * 1251:Suffixed integer constant causes implicit conversion of other
 * (integral) operand.
 *
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1532:The function is only referenced in one translation unit.
 * all functions in this file are configurations in the Security Module.
 *
 * 2100:Integral promotion : unsigned char promoted to signed int.
 *
 * 3442:Operator other than & or = applied to a volatile object.
 *
 * MISRA-C:2004 19.7(3453):A function could probably be used instead of this
 * function-like macro.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/

static u8 g_time_tick = 0;
static u32 g_time_tick_seed = 0;

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
/**************************************************************************//**
 *
 *  \details Initialize timer module.
 *
 *  \return If the timer is initialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the timer is initialized successfully.
 *  \retval BL_ERR_NOT_OK   - the timer is not initialized successfully.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
void Tim_Init(void)
{
    g_time_tick = 0;
    // systick_init();  //
    // app_tim_init();  /
    tim_init();
    BSTIM16_Init();
}

void BSTIM16_Init(void)
{
  FL_BSTIM16_InitTypeDef    TimerBase_InitStruct;

  TimerBase_InitStruct.prescaler = 47; //fCK_CNT=fCK_PSC/(PSC[15:0]+1) 4800预分频值，100us*65535
  TimerBase_InitStruct.autoReload = 60000; //48000是1ms
  TimerBase_InitStruct.autoReloadState = FL_ENABLE;
  TimerBase_InitStruct.clockSource = FL_CMU_BSTIM16_CLK_SOURCE_APBCLK;

  FL_BSTIM16_Init(BSTIM16, &TimerBase_InitStruct);


//   FL_BSTIM16_ClearFlag_Update(BSTIM16);    /* 清除计数器中断标志位 */
//   FL_BSTIM16_EnableIT_Update(BSTIM16);     /* 开启计数器中断 */


//   NVIC_DisableIRQ(BSTIM_IRQn);
//   NVIC_SetPriority(BSTIM_IRQn, 2);        /* 中断优先级配置 */
//   NVIC_EnableIRQ(BSTIM_IRQn);


  FL_BSTIM16_Enable(BSTIM16);         /* 使能定时器 */

}


void Tim_tick_count(void)
{
    g_time_tick++;
    g_time_tick_seed++;
}

bl_u32_t Tim_seedtick_Get(void)
{
  return g_time_tick_seed;
}
/**************************************************************************//**
 *
 *  \details Check whether the timer is time out.
 *
 *  \return If the timer is time out returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the timer is time out.
 *  \retval BL_ERR_NOT_OK   - the timer is not time out.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
bl_Return_t Tim_IsTimeout(void)
{
    //bl_Return_t ret = BL_ERR_OK;
    #if 1
    bl_Return_t ret = BL_ERR_NOT_OK;

    if(g_time_tick >=1)
    {
        //DISABLE_INTERRUPTS();
        /* write 1 to clear flag */
        g_time_tick=0;
        //ENABLE_INTERRUPTS();
        ret = BL_ERR_OK;
    }
    #endif
    return ret;
}

/**************************************************************************//**
 *
 *  \details Deinitialize timer module.
 *
 *  \return If the timer is deinitialized successfully returns BL_ERR_OK,
 *          otherwise returns BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK - the timer is deinitialized successfully.
 *  \retval BL_ERR_NOT_OK   - the timer is not deinitialized successfully.
 *
 *  \since V2.0.0
 *
 *****************************************************************************/
#define S32_SysTick_CSR_CLKSOURCE_MASK           0x4u
#define S32_SysTick_CSR_ENABLE_MASK              0x1u
#define S32_SysTick_CSR_TICKINT_MASK             0x2u

void Tim_Deinit(void)
{
    //NVIC_DisableIRQ(TIMER_CHANNEL0_IRQn);
    //NVIC_DisableIRQ(TIMER_CHANNEL1_IRQn);
    //NVIC_DisableIRQ(TIMER_CHANNEL2_IRQn);
    //NVIC_DisableIRQ(TIMER_CHANNEL3_IRQn);
    (*((u32 *)(SysTick_BASE)))   &= ~(S32_SysTick_CSR_CLKSOURCE_MASK|S32_SysTick_CSR_ENABLE_MASK|S32_SysTick_CSR_TICKINT_MASK) ;
    tim_de_init();
    return ;
}
