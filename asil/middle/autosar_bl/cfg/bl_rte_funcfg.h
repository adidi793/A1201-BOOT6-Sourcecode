/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a functional head file of the runtime environment
 *              module.
 *
 *  \file       bl_rte_funcfg.h
 *  \ingroup    rte_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 17/03/2010 | hewei.zhang      | N/A | Boot010001
 *      01.01.00 | 20/04/2010 | hewei.zhang      | N/A | Boot010002
 *      01.03.00 | 28/05/2010 | hewei.zhang      | N/A | Boot010004
 *      01.04.00 | 26/06/2010 | hewei.zhang      | N/A | Boot010005
 *      01.08.00 | 24/08/2010 | hewei.zhang      | N/A | Boot010009
 *      01.11.00 | 07/12/2010 | hewei.zhang      | N/A | Boot010012
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
#ifndef _BL_RTE_FUNCFG_H_
#define _BL_RTE_FUNCFG_H_

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/** \brief */
#define RTE_SYSTEM_SECURITY_CLASS   BL_SECURITY_CLASS_DDD
/** \brief */
#define RTE_FUN_STAY_IN_BOOT        BL_FUN_ON
/** \brief */
#define RTE_FUN_STAY_IN_BOOT_RESET  BL_FUN_ON
/** \brief */
#define RTE_FUN_WATCHDOG            BL_FUN_ON     //記得真正運行--要開回去
/** \brief */
#define RTE_FUN_COPY_WDG_TO_RAM     BL_FUN_OFF      //這宏開啟，-表示 Wdg_Feed 函數己真正 復制到RAM中
/** \brief */
#define RTE_FUN_PRE_INIT_WDG        BL_FUN_ON    //當前是關閉的 ，在post時再開啟  --為了了兼頋，在跳到app時，的wdog重新初始化；所以該宏不能開
/** \brief */
#define RTE_FUN_GATEWAY             BL_FUN_OFF
/** \brief */
#define RTE_FUN_GATEWAY_CONVERSION  BL_FUN_OFF
/** \brief */
#define RTE_FUN_SYSTEM_SLEEP                    BL_FUN_OFF
/** \brief */
#define RTE_FUN_ADDITIONAL_SLEEP_CONDITION      BL_FUN_OFF
/** \brief */
#define RTE_FUN_FILE_HEADER_PROCESS             BL_FUN_OFF
/** \brief */
#define RTE_FUN_INTEGRALITY_VALUE_IN_HEADER     BL_FUN_OFF
/** \brief */
#define RTE_FUN_COMPATIBILITY_VALUE_IN_HEADER   BL_FUN_OFF
/** \brief */
#define RTE_FUN_FINGERPRINT                     BL_FUN_ON///BL_FUN_OFF

/** \brief */
#define RTE_SYSTEM_VERSION                  (0x00050100UL)
/** \brief */
#define RTE_SYSTEM_SCHEDULE_PERIOD          (1u) //1ms执行一次
//以下4項 只在FE-5HA有，原D50沒有
#define RTE_FINGERPRINT_SIZE               (9UL)
#define RTE_TESTER_SERIAL_NUMBER_SIZE      (16UL)
#define RTE_PGM_DATE_SIZE                  (4UL)
#define RTE_ECU_HW_NUMBER_SIZE             (10UL)   
#define RTE_BOOT_SW_ID_SIZE                (10UL) 
#define RTE_PORT_NUMBER_SIZE               (12UL)  
#define RTE_SUPPLIER_CODE_SIZE             (6UL)  
#define RTE_ECU_SW_NUMBER_SIZE             (10UL)  
#define RTE_PARTS_ASSEMBLY_NUMBER_SIZE     (7UL)
#define RTE_Diagnosis_Specification_Version_SIZE     (4UL) //FF00
#define RTE_Manufacturer_ECU_Software_Number_SIZE     (19UL) //F188
#define RTE_Manufacturer_ECU_Software_Version_SIZE     (10UL) //F189
#define RTE_Manufacturer_ECU_Hardware_Number_SIZE     (29UL) //F191
#define RTE_System_Supplier_ECU_Hardware_Version_SIZE     (10UL) //F150
#define RTE_Calibration_Software_Version_SIZE     (10UL) //F182
#define RTE_System_Name_SIZE     (16UL) //F197
#define RTE_BootApp_Identify_SIZE     (1UL) //F1EF
#define RTE_Secure_Flash_SIZE     (1UL) //F1ED



#define RTE_PART_NUMBER_SIZE               (19UL) //F187
#define RTE_PARTS_SOFT_CODE_SIZE           (9UL)
#define RTE_WATCHDOG_FEED_INTERFACE_ADDR    (FLAG_RAM_WDOG_ADDR)
#define RTE_WATCHDOG_FEED_INTERFACE_SIZE    (8U)   //注意 Wdg_Feed 函數留給8字節長度

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

