/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-17 09:14:34
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-10-15 16:46:37
 * @FilePath: \A12 BOOT1\qm\app\main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "main.h"
#include "user_init.h"
#include "logic_delay.h"
#include "logic_time.h"
//#include "lin_app.h"

#include "stack_check.h"

#include "boot_app.h"

#include "bl_timer.h"
#include "bl_can.h"
#include "bl_booting.h"
#include "bl_process_driver.h"
#include "bl_system.h"
#include "bl_logical_block.h"
#include "bl_flash_if.h"
#include "bl_adpt_uds_platform.h"
#include "eeprom_cfg.h"
#include "svd.h"
#include "svd_interrupt.h"
#include "eeprom_cfg.h"
#include "eeprom.h"


u8 g_flash_read_flag2 = 0xff; //没用


void HardFault_Handler(void) {
    NVIC_SystemReset();
}

int main(void) {
    bl_BootingFlag_t flag;
    bl_BootingFlag_t flag1;
    bl_BootingFlag_t flag2;
    
    //__disable_irq();
    
    //__enable_irq();
    
    /*Initialize system*/
    Rte_PreInit();
    
    #if 1
    #ifdef BOOT_STATE_PROG_DEBUG_EN
    adpt_test_init();
    #endif
    #ifdef FLASH_TEST_ERASE_WRITE
    flash_if_test();
    #endif
    boot_app_init(); 
    /* -----------------------------------------------------------------------A12
    先检查重编程是否有效，返回 BOOTING_FLAG_REPROGRAM_VALID
    再检查APP是否有效，返回 BOOTING_FLAG_APPLICATION_VALID
    ----------------------------------------------------------------------- */
    flag = Bootm_CheckBootingRequirement();
    
    if (BOOTING_FLAG_APPLICATION_VALID == flag) {
        #ifdef BOOT_STATE_PROG_DEBUG_EN
        #else
        //#ifdef FALSH_DRIVER_INIT_EN
        //#else
        // boot_app_jump();
        Bootm_GotoApplication();
        //#endif
        #endif
        //flag=0u;
        /*never return unless StayInBoot enable and receive a valid SIB frame*/
    }
    // g_flash_read_flag2 = flag;
    #endif
    
    (void)Rte_PostInit();
    
    #ifdef FALSH_DRIVER_INIT_EN
    // lb_logic_block_init_flashdrv();
    //Dm_WriteDefaultData(1);  //flash app
    //g_test_flash_app_flag = Dm_IsDefaultData(1);
    #endif
    EE_Init(); //需要初始化EE起始地址的标志位，否则写入位置不正确。并不会初始化EE中的数据，具体需要研究
    EE_Flash_Init();
    #if 0
    if (flag2 == BOOTING_FLAG_REPROGRAM_VALID) {
        clear_program_valid_to_app_valid();
        //回复，默认会话
        Adpt_Res_default_ValidProcess();
    }
    #endif
    
    Rte_ProcessReset(flag);     // flag=0(BOOTING_FLAG_NULL) 或者 flag=1(BOOTING_FLAG_REPROGRAM_VALID)
    #ifdef NVM_EEPROM_EN
    nvm_eeprom_init();  //要在不跳转时，初始化EEPROM
    #endif
    FL_SVD_DisableDigitalFilter(SVD);
    SVD_Interrupt_Init(FL_DISABLE);
    for (;;) {
    
        wdog_task();
        //app_task_1ms();
        PROC_SCHEDULE();  // 相當於 Proc_PassiveSchedule(&g_UdsPlatformAdapter)
    }
}

