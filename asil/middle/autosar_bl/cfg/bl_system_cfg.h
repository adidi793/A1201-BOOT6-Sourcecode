/**************************************************************************//**
 *
 *  \copyright  This software is the property of HiRain Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *              HiRain Technologies.
 *
 *  \brief      This is a head file to configurate the data of the security
 *              manager module.
 *
 *  \file       bl_security_funcfg.h
 *  \ingroup    security_manager_module
 *  \author     xin.shang <xin.shang@hirain.com>
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      05.00.00 | 19/06/2013 | xin.shang        | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_TEST_H_
#define _BL_TEST_H_
#include "bl_typedefs.h"


//#define MCU_OSC_IS_4 1


//#define TEST_MEMORY_FLASH 1
//#define TEST_MEMORY_IN 1
//#define UDS_DEBUG_EN 1
//#define FLASH_DRIVER_DEBUG_EN 1
//#define TEST_GPIO_EN 1

//#define MEMORY_DEBUG_EN  1    //MEM 模塊加一些變量
//#define LOGIC_BLOCK_DEBUG_EN  1    // BLOCK ID 模塊加一些變量

//#define BOOT_STATE_PROG_DEBUG_EN  1   //保持在boot prog 會話狀態 仿真

#define FALSH_DRIVER_INIT_EN  1   //在main初始化時，就把 flash ram 初始化


//#define FLASH_DRIVER_IS_BY_RAM  1  //FLASH 驅動，的函數 ，都放在 RAM中，

//#define FLASH_TEST_ERASE_WRITE 1  //測試擦寫

//目前以中斷方式。--不能改
#define CAN_TX_INTERRUPT_EN 1   // CAN發送中断使能；--则要開啟隊列
#define CAN_RX_INTERRUPT_EN 1   // CAN接收中断使能；--则要開啟隊列

//#define BL_EEPROM_EN  1  //

//#define DEFAULT_GOTO_APP_DEFAULT_SEESION_78   //当跳转到APP前，先只发送78,要在app做逻辑

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/

/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

