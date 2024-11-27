/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief
	* @function List:
	******************************************************************************
	* @attention
	*
	*
	* <h2><center>&copy; COPYRIGHT 2021 </center></h2>
	******************************************************************************
	* @History:
	* @Author:
	* @Data:
	* @Version:
*/


//#include "includes.h"
#include "error.h"
#include "boot_app.h"
#include "can.h"
#include "nvm.h"
//#include "lin_commontl_api.h"
//#include "lin_slave_diag.h"
#include "common_cfg.h"
#include "bl_can_if_funcfg.h"
#include "bl_adpt_uds_platform.h"


//跳轉相關
u32 jump_adress;
typedef void (*pFunction)(void)  ;
pFunction jump_to_app;

#if 0 //最后的 app valid flag,应该在boot区成功时，要写入，記得debug時，要開啟
#pragma CONST_SEG MEMORY_MAP_ROM_APP_FLAG_END
const u8 AppValid[16] = {0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5}; //APP_VALID_FLAG_ADDR
#pragma CONST_SEG DEFAULT
#else

#if 0  //尾去掉
#ifdef __ICCARM__
MEMORY_MAP_FLAG_NO_OPTIME	const u8 AppValid[16] @APP_VALID_FLAG_ADDR = {0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5};
#else
MEMORY_MAP_FLAG_NO_OPTIME volatile const MEMORY_MAP_ROM_APP_VALID u8 AppValid[16]   = {0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5, 0xaa, 0xaa, 0x04, 0xe5};
#endif
#endif

#endif


static void Boot_SendEcuResetRespones(void);

static u8 Boot_IsValidFlag(u8 flagId);
static void Boot_SetFlag(u8 flagId);

static u8 Boot_MemCpy(u8 *des, const u8 *src, u32 size);

__IO static u32 g_read_valid;

/* BEGIN_FUNCTION_HDR
************************************************************************************************
* Function Name : Boot_ReprogramCallBack
*
* Description   : Set the reprogramming flag and reset the ECU.
*
* Inputs        : none
*
* Outputs       : none
*
* Limitations   : Only for Bootloader
*
************************************************************************************************
END_FUNCTION_HDR */
void Boot_ReprogramCallBack(void) {
    /*set reprogram flag*/
    Boot_SetFlag(FLAG_REPROGRAM_ID);
    /*reset ECU*/
    ResetMCUHandle();
}

void Boot_Reprogram_only_writeram(void) {
    /*set reprogram flag*/
    Boot_SetFlag(FLAG_REPROGRAM_ID);
    /*reset ECU*/
    //ResetMCUHandle();
}
/* BEGIN_FUNCTION_HDR
************************************************************************************************
* Function Name : Boot_ResetMCUCallBack
*
* Description   : Reset the ECU.
*
* Inputs        : none
*
* Outputs       : none
*
* Limitations   : Only for Bootloader
*
************************************************************************************************
END_FUNCTION_HDR */
void Boot_ResetMCUCallBack(void) {
    /*set reprogram flag*/
    Boot_SetFlag(FLAG_RESET_ID);
    /*reset ECU*/
    ResetMCUHandle();
}

/* BEGIN_FUNCTION_HDR
************************************************************************************************
* Function Name : Boot_SetFlag
*
* Description   : Set the reprogramming flag.
*
* Inputs        : none
*
* Outputs       : none
*
* Limitations   : Only for Bootloader
*
************************************************************************************************
END_FUNCTION_HDR */
static void Boot_SetFlag(u8 flagId) {
    u32 addr = 0xFFFFFFFFUL;
    u32  data = 0xFFFFFFFFUL;
    u8 l_buf[4] = {FLAG_REPROGRAM_u8_DATA0, FLAG_REPROGRAM_u8_DATA1, FLAG_REPROGRAM_u8_DATA2, FLAG_REPROGRAM_u8_DATA3};
    
    switch (flagId) {
        case FLAG_REPROGRAM_ID:
            //addr = AppValid[0];   // AppValid1
            //addr = AppValid_in[0];   // AppValid_in
            addr = FLAG_REPROGRAM_ADDR;
            //data = FLAG_REPROGRAM_DATA;
            break;
        default :
            break;
    }
    
    if (addr != 0xFFFFFFFFUL) {
        #if 0
        (void)FlagWriteHandle(addr, (u32)4, &l_buf);
        #else
        (void)ram_write_bytes(addr, (u8 *)&l_buf[0], FLAG_REPROGRAM_LEN);
        #endif
    }
}


static u8 Boot_MemCpy(u8 *des, const u8 *src, u32 size) { /*PRQA S 3219*/
    u32 i;
    
    for (i = 0; i < size; i++) {
        des[i] = src[i];
    }
    
    return 0;
    
}


/**
* @brief  初始化
* @param  input:
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
void  boot_app_init(void) {
    u8 i;  /*PRQA S 3205*/
    u8 l_buf[8];
    u32 l_temp;
    u32 tempP2Star;
    #if 0
    boot_app_clear_flag();
    (void)in_flash_read_words(&g_boot_flag_info, in_flash_get_boot_log_s_addr(), IN_FLASH_LOG_LEN_BOOT_WORKS);
    if (g_boot_flag_info == BOOT_APP_FLAG_IS_BOOT_SUCCESS) {
        boot_app_jump_to_app();
    }
    #endif
    #if 1
    //g_read_valid = AppValid[0] ;
    ram_read_bytes(FLAG_JUMP_TO_BOOT_RESPONSE_ADDR, (u8 *)&l_temp, FLAG_JUMP_TO_BOOT_RESPONSE_LEN);
    #ifdef LIN_ENABLE
    switch (l_temp) {
        case FLAG_JUMP_TO_APP_RESPONSE_11_01:
            #if 1
            //l_buf[0] = LIN_SALVE_DIAG_NAD;
            l_buf[0] = 0x02;
            l_buf[1] = 0x51;
            l_buf[2] = 0x01;
            //l_buf[4] = 0xff;
            //l_buf[5] = 0xff;
            //l_buf[6] = 0xff;
            //l_buf[7] = 0xff;
            //ld_put_raw(&l_buf[0]);
            #else
            l_buf[0] = CANIF_PHY_TX_CANID;
            for (i = 0; i < 7; i++) {
                l_buf[i + 1] = gs_ResetResponse[i];
            }
            #endif
            #ifdef CAN_ID_0_EN
            #else
            ld_send_message(l_buf[0], &l_buf[1]);
            #endif
            break;
        case FLAG_JUMP_TO_APP_RESPONSE_11_03:
            #if 1
            //l_buf[0] = LIN_SALVE_DIAG_NAD;
            l_buf[0] = 0x02;
            l_buf[1] = 0x51;
            l_buf[2] = 0x03;
            //l_buf[4] = 0xff;
            //l_buf[5] = 0xff;
            //l_buf[6] = 0xff;
            //l_buf[7] = 0xff;
            //ld_put_raw(&l_buf[0]);
            #else
            l_buf[0] = CANIF_PHY_TX_CANID;
            for (i = 0; i < 7; i++) {
                l_buf[i + 1] = gs_ResetResponse_sub03[i];
            }
            #endif
            #ifdef CAN_ID_0_EN
            #else
            ld_send_message(l_buf[0], &l_buf[1]);
            #endif
            break;
        case FLAG_JUMP_TO_APP_RESPONSE_10_02:
            #if 0
            l_buf[0] = LIN_SALVE_DIAG_NAD;
            for (i = 0; i < 7; i++) {
                l_buf[i + 1] = gs_ReprogrammingResponse[i];
            }
            ld_put_raw(LI0, &l_buf[0]);
            #endif
            break;
        case FLAG_JUMP_TO_APP_RESPONSE_10_01:
            #if 0
            //l_buf[0] = LIN_SALVE_DIAG_NAD;
            l_buf[0] = 0x06;
            l_buf[1] = 0x50;
            l_buf[2] = 0x01;
            l_buf[3] = (u8)(50 >> 8);
            l_buf[4] = (u8)(50);
            tempP2Star = 5000 / 10;
            l_buf[5] = (u8)(tempP2Star >> 8);
            l_buf[6] = (u8)(tempP2Star);
            //ld_put_raw(&l_buf[0]);
            ld_send_message(l_buf[0], &l_buf[1]);
            #endif
            break;
        default:
            break;
    }
    //flash_drv_ram_read();
    l_temp = FLAG_JUMP_TO_BOOT_RESPONSE_INIT;
    ram_write_bytes(FLAG_JUMP_TO_BOOT_RESPONSE_ADDR, (u8 *)&l_temp, FLAG_JUMP_TO_BOOT_RESPONSE_LEN);
    #endif
    #endif
}


/**
* @brief
* @param  input:
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
void boot_app_task(void) {
    g_read_valid++;
    
}


#if 0
#ifdef __ICCARM__
static void __set_MSP(uint32_t topOfMainStack) {
    __asm("msr msp, r0");
}
static __inline void __set_PSP(uint32_t topOfProcStack) {
    __asm("msr psp, r0");
}
/**
	\brief   Set Control Register
	\details Writes the given value to the Control Register.
	\param [in]    control  Control Register value to set
 */
static __inline void __set_CONTROL(uint32_t control) {
    __asm volatile("MSR control, %0" : : "r"(control) : "memory");
}

#endif

#ifdef __GNUC__

static void __set_MSP(uint32_t topOfMainStack) {
    __asm volatile("MSR msp, %0\n" : : "r"(topOfMainStack) : "sp");
}
static void __set_PSP(uint32_t topOfMainStack) {
    __asm volatile("MSR psp, %0\n" : : "r"(topOfMainStack) : "sp");
}
/**
	\brief   Set Control Register
	\details Writes the given value to the Control Register.
	\param [in]    control  Control Register value to set
 */
static void __set_CONTROL(uint32_t control) {
    __asm volatile("MSR control, %0" : : "r"(control) : "memory");
}


#endif

#endif


/**
* @brief  初始化
* @param  input:
* @param  output:
* @return   none:
* @calls
* @calls by
* @others
*/
void boot_app_jump_to_app(void) {

    /*一上電就跳轉到應用程序區*/
    
    /*檢查棧頂地址是否合法*/
    if (((* (__IO u32 *)APP_ADRRESS_VECTOR) & 0x2ffe0000u)  == 0x20000000u) {
        /*用戶代碼區第二個字為程序開始地址(復位地址)  即Reset_handler  */
        jump_adress = * (__IO u32 *)(APP_ADRRESS_VECTOR + 4u);		/*所謂的PC 是0x8000004  ,SP 是 0x8000000*/
        jump_to_app = (pFunction)jump_adress;
        
        /*考虙這語句是否可以去掉---後來測試發現不影響--是獨立的*/
        // SCB->VTOR = 0x08000000u; /* Vector Table Relocation in Internal FLASH. */
        
        //和RTOS相關的，這兩句，研究了好多天，價格不菲
        //__set_PSP(*(__IO u32 *)APP_ADRRESS_VECTOR);
        //__set_CONTROL(0);
        /*初始化APP堆棧指針---用戶代碼區的第一個字用於存放棧頂地址*/
        __set_MSP((* (__IO u32 *)APP_ADRRESS_VECTOR));    		/*__set_MSP 是設置椎棧指針*/
        
        #if 0
        bsp_int_disable_all_int();
        #if OS_TYPE == OS_TYPES_NULL
        bsp_int_disable_primask();
        bsp_int_disable_faultmask();
        #endif
        #endif
        
        jump_to_app();
        
    }
    
    
    
}
