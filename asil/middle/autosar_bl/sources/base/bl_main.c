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

u8 g_flash_read_flag2 =0xff;


int bl_main(void)
{
    u8 i;
    bl_BootingFlag_t flag;

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
    flag = Bootm_CheckBootingRequirement();
    //flag = BOOTING_FLAG_REPROGRAM_VALID;
    if (BOOTING_FLAG_APPLICATION_VALID == flag)
    {
        if(lb_logic_block_read_app_valid1()!=0)
        {
            #ifdef BOOT_STATE_PROG_DEBUG_EN
            #else
                //#ifdef FALSH_DRIVER_INIT_EN
                //#else
                //boot_app_jump();
                Bootm_GotoApplication();
                //#endif
            #endif
            flag=0u;
        }
        else
        {
            flag=0u;
        }
        /*never return unless StayInBoot enable and receive a valid SIB frame*/
    }
    g_flash_read_flag2 = flag;
    #endif
    (void)Rte_PostInit();

    #ifdef FALSH_DRIVER_INIT_EN
    lb_logic_block_init_flashdrv();
    //Dm_WriteDefaultData(1);  //flash app
    //g_test_flash_app_flag = Dm_IsDefaultData(1);
    #endif

    Rte_ProcessReset(flag);     // 跳到這里 flag=0; 或者  BOOTING_FLAG_REPROGRAM_VALID
    #ifdef NVM_EEPROM_EN
    nvm_eeprom_init();  //要在不跳转时，初始化EEPROM
    #endif

    for( ;; )
    {
        wdog_task();
        //app_task_1ms();
        PROC_SCHEDULE();  // 相當於 Proc_PassiveSchedule(&g_UdsPlatformAdapter)
    }
}

