/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-06-17 09:14:33
 * @LastEditors: lin-boyi linboyi@163.com
 * @LastEditTime: 2024-07-08 15:20:41
 * @FilePath: \A12 BOOT1\asil\middle\autosar_bl\sources\cfg\bl_adapter_cfg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file to configurate the adapter module.
 *
 *  \file       bl_adapter_cfg.c
 *  \ingroup    communication_adapter_module
 *  \author     
 *
 *  \version    5.1.0
 *  \date       27/03/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      03.00.00 | 12/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.02.00 | 27/09/2011 | mingqing.tang    | N/A | Boot030003
 *      03.03.00 | 29/09/2011 | mingqing.tang    | N/A | Boot030004
 *      03.05.00 | 12/10/2011 | mingqing.tang    | N/A | Boot030006
 *      04.00.00 | 10/07/2012 | mingqing.tang    | N/A | Boot040001
 *      04.02.00 | 15/07/2012 | mingqing.tang    | N/A | Boot040003
 *      04.04.00 | 18/07/2012 | mingqing.tang    | N/A | Boot040005
 *      04.06.00 | 31/07/2012 | mingqing.tang    | N/A | Boot040007
 *      04.10.00 | 07/08/2012 | mingqing.tang    | N/A | Boot040011
 *      04.11.00 | 15/08/2012 | mingqing.tang    | N/A | Boot040012
 *      05.00.00 | 19/06/2013 | heli.zhang       | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *
 *****************************************************************************/
#include "bl_adapter_cfg.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1531,3211 EOF*/
/*
 * 1531:The object is referenced in only one translation unit.
 * g_SecurityDelayInfo is a configurations.
 *
 * 3211:The global identifier is defined but not used.
 * The g_SecurityDelayInfo is used in the bl_adapter.c file.
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
 *  External Variable Definitions
*****************************************************************************/
/** \details The delay info of security access.*/
const bl_SecurityDelay_t g_SecurityDelayInfo =
{
    ADPT_SECURITY_DELAY_TIME,  //delayTime 10S
    ADPT_SECURITY_ATTEMPT_COUNT,
#if (ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
    ADPT_SECURITY_DELAY_FLAG_ID,
#endif
#if (ADPT_FUN_ADVANCED_ATTEMPT_DELAY ==BL_FUN_ON)
    ADPT_SECURITY_FAILED_COUNT_ID
#endif
};

