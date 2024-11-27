/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-17 09:14:33
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-07-02 16:03:37
 * @FilePath: \A12 BOOT1\asil\middle\autosar_bl\sources\cfg\bl_booting_cfg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is a source file to configurate the booting manager
 *              module.
 *
 *  \file       bl_booting_cfg.c
 *  \ingroup    booting_manager_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 15/03/2010 | hewei.zhang      | N/A | Boot010001
 *      01.02.00 | 16/06/2010 | hewei.zhang      | N/A | Boot010003
 *      01.03.00 | 18/08/2010 | hewei.zhang      | N/A | Boot010004
 *      02.00.00 | 16/05/2011 | mingqing.tang    | N/A | Boot020001
 *      02.05.00 | 25/05/2011 | mingqing.tang    | N/A | Boot020006
 *      03.00.00 | 15/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.02.00 | 20/10/2011 | mingqing.tang    | N/A | Boot030003
 *      05.00.00 | 19/06/2013 | xin.shang        | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_booting_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1533,3211 EOF*/
/*
 * 1533:The object is referenced in only one translation unit.
 * g_BootingCheckerList is a configurations.
 *
 * 3211:The global identifier is defined but not used.
 * The g_BootingCheckerList is used in the bl_booting.c file.
 *
 */

/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/
/**************************************************************************//**
 *  \details The booting checkers.
 *****************************************************************************/
STATIC const bl_BootingChecker_t
gs_BootingChecker[BOOTM_NUMBER_OF_BOOTING_CHECKER] = {
    &Bootm_ReprogramFlagChecker, //"&"符号是可选的
    &Bootm_AppValidFlagChecker, 
};

/******************************************************************************
 *  \details The list of booting checkers.
 *
 *  \warning Do not change the name of this variable.
 *****************************************************************************/
const bl_BootingCheckerList_t g_BootingCheckerList = {
    BOOTM_NUMBER_OF_BOOTING_CHECKER, gs_BootingChecker,	//為什麼 第一參數是2個 原來有兩個變量需要判斷；
};
/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/

