/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a head file to configurate the data of the data manager
 *              module.
 *
 *  \file       bl_data_cfg.h
 *  \ingroup    data_manager_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      04.00.00 | 06/03/2013 | xin.shang        | N/A | Boot040001
 *      04.02.00 | 11/03/2013 | xin.shang        | N/A | Boot040003
 *      04.03.00 | 11/03/2013 | xin.shang        | N/A | Boot040004
 *      04.11.00 | 13/03/2013 | xin.shang        | N/A | Boot040012
 *      04.13.00 | 15/03/2013 | xin.shang        | N/A | Boot040014
 *      04.14.00 | 20/03/2013 | xin.shang        | N/A | Boot040015
 *      04.21.00 | 21/03/2013 | xin.shang        | N/A | Boot040022
 *      04.31.00 | 04/04/2013 | xin.shang        | N/A | Boot040032
 *      05.00.00 | 19/06/2013 | xin.shang        | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#ifndef _BL_DATA_CFG_H_
#define _BL_DATA_CFG_H_
#include "bl_data.h"
#include "bl_rte_funcfg.h"
#include "boot_app_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 3210,3447 EOF*/
/*
 * 3210:The global identifier is declared but is not used.
 * The functions are used in the bl_security_cfg.c file.
 *
 * MISRA-C:2004 8.8(3447):The identifier has external linkage but this
 * declaration is not in a header file.
 * The function Declarations are in the header file.
 *
 */

/*****************************************************************************
 *  Macro Definitions
 *****************************************************************************/
/*the memory configurations*/
#define DM_NUMBER_OF_MANAGED_DATA   (2u)
#define DM_NMBr_OF_MANAGED_Data_ID  (5u)

#define DM_DATA_SIZE_SUM     (16u)
extern const bl_Buffer_t gs_defaultDataList[DM_DATA_SIZE_SUM] ;

/**
 * 和bl_booting_cfg.h中的宏 相關
 * BOOTM_REPROGRAM_FLAG_DID
 * BOOTM_RESET_FLAG_DID
 */

/*reprogram requirement flag*/
#define DM_DATA_0_TYPE              DM_DATA_TYPE_READ_AND_WRITE
#define DM_DATA_0_MEMID             (0)	//ram 在 memory 中 的心確是 id=1
#define DM_DATA_0_LOCAL_ADDR        (FLAG_REPROGRAM_ADDR)		//RAM中的 3FF8就是重編程的標志所在  //0x20006ffC
#define DM_DATA_0_SIZE              (4UL)
#define DM_DATA_0_DDP               (&gs_defaultDataList[0])   //0xA5u,0xA5u,0xE5u,0x34u

/*application valid flag*/
#define DM_DATA_1_TYPE              DM_DATA_TYPE_READ_AND_WRITE
#define DM_DATA_1_MEMID             (1)   //flash 在 memory 中 的心確是 id=1
#define DM_DATA_1_LOCAL_ADDR        (APP_VALID_FLAG_ADDR)	// stm32k144  //0x0007ff00
#define DM_DATA_1_SIZE              (4UL)
#define DM_DATA_1_DDP               (&gs_defaultDataList[4])  //0xAAu,0xAAu,0x04u,0xE5u

/*application valid flag1*/
#define DM_DATA_2_TYPE              DM_DATA_TYPE_READ_AND_WRITE
#define DM_DATA_2_MEMID             (1u)
#define DM_DATA_2_LOCAL_ADDR        (APP_VALID_FLAG1_ADDR) // 0xFF9FF0   0xFF9C00UL  //(0xFFBDF0UL) // (0x00FFBFF0UL) //(0x0003BFF0UL) 0xFFBFFF -app flagµÄµØÖ·
#define DM_DATA_2_SIZE              (4UL)  /* App valid flag 4->8bytes */
#define DM_DATA_2_DDP               (&gs_defaultDataList[8])


/*app reprogram requirement flag*/
#define DM_DATA_3_TYPE              DM_DATA_TYPE_READ_AND_WRITE
#define DM_DATA_3_MEMID             (0u)
#define DM_DATA_3_LOCAL_ADDR        (FLAG_JUMP_TO_BOOT_RESPONSE_ADDR) //(0x00001200UL) // 0x00003FF8UL)  --±àÒë±êÖ¾
#define DM_DATA_3_SIZE              (4UL)
#define DM_DATA_3_DDP               (&gs_defaultDataList[12])
#if 0  //可以刪除
/*program fingerprinter data*/
#define DM_DATA_2_TYPE              DM_DATA_TYPE_READ_AND_WRITE
#define DM_DATA_2_MEMID             (2u)
#define DM_DATA_2_LOCAL_ADDR        (0x00000BE0UL) //EEPROM
#define DM_DATA_2_SIZE              (16UL)
#define DM_DATA_2_DDP               //(&gs_defaultDataList[20])

/*program attemp counter 编程失败重试计数器,  program valid counter 编程成功计数器, program PGM DATE  编程指纹计数器4 BCD */
#define DM_DATA_3_TYPE              DM_DATA_TYPE_READ_AND_WRITE
#define DM_DATA_3_MEMID             (3u)
#define DM_DATA_3_LOCAL_ADDR        (0x00000BF0UL) // EEPROM
#define DM_DATA_3_SIZE              (8UL)
#define DM_DATA_3_DDP               //(&gs_defaultDataList[24])

#endif
#define DM_MAX_DATA_SIZE            DM_DATA_1_SIZE //A12这里有疑问，为什么不是DATA0的长度

/*****************************************************************************
 *  Type Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  External Global Variable Declarations
 *****************************************************************************/
/** \brief The data informations list.*/
extern const bl_DataInfoList_t g_DataInfoList;
/*****************************************************************************
 *  External Function Prototype Declarations
 *****************************************************************************/

#endif

