/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file to configurate the DCM module.
 *
 *  \file       bl_dcm_cfg.c
 *  \ingroup    communication_protocol_stack_module
 *  \author
 *
 *
 *  \version    5.2.0
 *  \date       19/06/2014
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 08/04/2010 | hewei.zhang      | N/A | Boot010001
 *      01.02.00 | 19/04/2010 | hewei.zhang      | N/A | Boot010003
 *      01.04.00 | 06/05/2010 | hewei.zhang      | N/A | Boot010005
 *      02.00.00 | 18/03/2011 | mingqing.tang    | N/A | Boot020001
 *      02.01.00 | 31/03/2011 | mingqing.tang    | N/A | Boot020002
 *      02.03.00 | 07/04/2011 | mingqing.tang    | N/A | Boot020004
 *      03.00.00 | 12/09/2011 | mingqing.tang    | N/A | Boot030001
 *      03.05.00 | 11/10/2011 | mingqing.tang    | N/A | Boot030006
 *      03.06.00 | 26/10/2011 | mingqing.tang    | N/A | Boot030007
 *      04.00.00 | 02/01/2012 | mingqing.tang    | N/A | Boot040001
 *      04.02.00 | 16/01/2012 | mingqing.tang    | N/A | Boot040003
 *      04.08.00 | 04/04/2012 | mingqing.tang    | N/A | Boot040009
 *      05.00.00 | 19/06/2013 | heli.zhang       | N/A | Boot050001
 *      05.01.00 | 27/03/2014 | mingqing.tang    | N/A | Boot050002
 *      05.02.00 | 19/06/2014 | mingqing.tang    | N/A | BootSpec050001
 *
 *****************************************************************************/
#include "bl_dcm_cfg.h"
#include "bl_adpt_uds_platform.h"

/*****************************************************************************
 *  QAC Suppression
 *****************************************************************************/
/*PRQA S 1257,1531,1533,3120,3211 EOF*/
/*
 * 1257:Suffixed integer constant implicitly converted to smaller integer type
 * on assignment.
 *
 * 1531:The object is referenced in only one translation unit.
 * g_DcmBuffer is a configurations.
 *
 * 1533:The object is only referenced by one function.
 * The g_DcmServiceList is a configurations.
 *
 * 3120:Hard-coded magic integer constant.
 * This is a only configuration.
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
/**************************************************************************//**
 *  \details The dcmbuffer of saving rx and tx data.
 *****************************************************************************/
STATIC bl_Buffer_t gs_Buffer[DCM_BUFFER_SIZE];  //診斷時的多幀最大緩存  見 g_DcmBuffer.buffer 才是真正被調用的

/**************************************************************************//**
 *  \details The subfunction list of 0x10 service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm10SubId[DCM_10SUBID_NUMBER] = {
    {
        //id=1 //num=0  //minsize=0  //maxsize=2
        1, 0, DCM_10SUBID_MINLEN, DCM_10SUBID_MAXLEN,
        //flag=1B1F    //serviceList空  //header=空  //Callback
        DCM_10SUBID01_FLAG, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback1001
    },
    {
        //id=2 //num=0  //minsize=0  //maxsize=2
        2, 0, DCM_10SUBID_MINLEN, DCM_10SUBID_MAXLEN,
        //flag=0x1B16    //serviceList空  //header=空  //Callback
        DCM_10SUBID02_FLAG, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback1002
    },
    {
        //id=3 //num=0  //minsize=0  //maxsize=2
        3, 0, DCM_10SUBID_MINLEN, DCM_10SUBID_MAXLEN,
        //flag=0x1B1D      //serviceList空  //header=空  //Callback
        DCM_10SUBID03_FLAG, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback1003
    },
    #if 0
    {
        //id=3 //num=0  //minsize=0  //maxsize=2
        4, 0, DCM_10SUBID_MINLEN, DCM_10SUBID_MAXLEN,
        //flag=40000031D    //serviceList空  //header=空  //Callback
        DCM_10SUBID04_FLAG, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback1004
    },
    #endif
};

/**************************************************************************//**
 *  \details The subfunction list of 0x11 service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm11SubId[DCM_11SUBID_NUMBER] = {
    {
        //id=1 //num=0  //minsize=0  //maxsize=2
        1, 0, DCM_11SUBID_MINLEN, DCM_11SUBID_MAXLEN,
        //flag=0    //serviceList空  //header=空  //Callback
        DCM_11SUBID_FLAG, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback1101
    },
    {
        //id=1 //num=0  //minsize=0  //maxsize=2
        3, 0, DCM_11SUBID_MINLEN, DCM_11SUBID_MAXLEN,
        //flag=0    //serviceList空  //header=空  //Callback
        DCM_11SUBID_FLAG, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback1103
    }
};

/**************************************************************************//**
 *  \details The subfunction list of 0x27 service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm27SubId[DCM_27SUBID_NUMBER] = {
    {
        //id=0x01  //num=0  //minsize=2   //maxsize=2  //B14
        DCM_SA_SEED_REQ, 0, DCM_27SEED_MINLEN, 2, DCM_27SUBID01_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackGetSeedForLevel1
    },
    {
        //id=0x02  //num=0  //minsize=6   //maxsize=6  //B14
        DCM_SA_KEY_RSP, 0, DCM_27KEY_MINLEN, 6, DCM_27SUBID02_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackVerifyKeyForLevel1
    },
    {
        //id=0x11  //num=0  //minsize=2   //maxsize=2  //1912
        DCM_SA_SEED_2_REQ, 0, DCM_27SEED_MINLEN, 2, DCM_27SUBID11_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackGetSeedForLevel2
    },
    {
        //id=0x12  //num=0  //minsize=6   //maxsize=6  //1912
        DCM_SA_KEY_2_RSP, 0, DCM_27KEY_MINLEN, 6, DCM_27SUBID12_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackVerifyKeyForLevel2
    },
};

/**************************************************************************//**
 *  \details The subfunction list of 0x28 service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm28SubId[DCM_28SUBID_NUMBER] = {
    {
        //id=0x3  //num=0  //minsize=0   //maxsize=0  //flag=0
        3, 0, DCM_28SUBID_MINLEN, 0, DCM_28SUBID03_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback28
    },
    /* 原0x1,A12不支持 */
    {
        //id=0x1  //num=0  //minsize=0   //maxsize=0  //flag=0
        0xFF, 0, DCM_28SUBID_MINLEN, 0, DCM_28SUBID01_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback28
    },
    {
        //id=0x0  //num=0  //minsize=0   //maxsize=0  //flag=0
        0, 0, DCM_28SUBID_MINLEN, 0, DCM_28SUBID00_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback28
    },
};


#define RID_CheckProgramCond	  (0x0203u)   //0203 是不是檢查獨立性？A12项目中是检查编程完整性
#define RID_CheckProgramCheckSum  (0x0202u)   //原來checksum 是02 02
#define DID_WR_Fingerprint	(0xF198u)   //原來寫入F198表示金手指
#define DID_R_Fingerprint	(0xF198u)
#define DID_WR_ProgramDate	(0xF199u)
#define DID_R_ProgramDate	(0xF199u)

/**************************************************************************//**
 *  \details The data identifier list of 0x31 service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm31Did[DCM_31DID_NUMBER] = {
    {
        //id=0x0203          //num=0  //minsize=0   //maxsize=4   //flag=0x4000 1B12
        RID_CheckProgramCond, 0, DCM_31CONDITION_MINLEN, 4, DCM_31CONDITION_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackCheckProgramCond
    },
    {
        //id=ff00 擦除 num=0   //minsize=13   //maxsize=13   //flag=0x40001B12
        0xFF00u, 0, DCM_31ERASE_MINLEN, 13, DCM_31ERASE_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackEraseMemory
    },
    {
        //id=0202 			//num=0     //minsize=8   //maxsize=8   //flag=0x40001B12
        RID_CheckProgramCheckSum, 0, DCM_31CHECKSUM_MINLEN, 8, DCM_31CHECKSUM_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackCheckSum
    },
    {
        //id=FF01 			//num=0     //minsize=8   //maxsize=8   //flag=0x40001B12
        0xFF01u, 0, 4, 4, DCM_31COMPAT_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackCheckCompatibility  //編程兼容性
    },
    #if(RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
    {
        //id=f518 			//num=0     //minsize=8   //maxsize=8   //flag=0x40000212
        0xF518u, 0, DCM_31STAYINBOOT_MINLEN, 4, DCM_31STAYINBOOT_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackStayInBoot
    },
    #endif
    {
        //id= 			//num=0     //minsize=0   //maxsize=8   //flag=0x40001B12
        0x6000u, 0, DCM_31COMPAT_MINLEN, 1326u, DCM_31_6000_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackCheckLegitimacy
    },
    {
        //id= 			//num=0     //minsize=0   //maxsize=8   //flag=0x40001B12
        0x6001u, 0, 4, 4, DCM_31_6001_FLAG,
        //serviceList空  //header=空  //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallbackCheckVersion
    },
};

/**************************************************************************//**
 *  \details The subfunction list of 0x31 service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm31SubId[DCM_31SUBID_NUMBER] = {
    {
        //id=1 		//num=5或4     //minsize=0   //maxsize=0   //flag=0
        1, DCM_31DID_NUMBER, DCM_31SUBID_MINLEN, 0u, DCM_31SUBID_FLAG,
        //serviceList  //header  //Callback
        gs_Dcm31Did, &g_DcmDidHeader, BL_NULL_PTR
    },
};

/**************************************************************************//**
 *  \details The subfunction list of 0x3E service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm3ESubId[DCM_3ESUBID_NUMBER] = {
    {
        0, 0, 0, 0, 0, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback3E
    },
};

/**************************************************************************//**
 *  \details The subfunction list of 0x85 service.
 *****************************************************************************/
STATIC const bl_DcmService_t gs_Dcm85SubId[DCM_85SUBID_NUMBER] = {
    {
        1, 0, 0, 0, 0, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback85
    },
    {
        2, 0, 0, 0, 0, BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallback85
    },
};


/* -----------------------------------------------------------------------A12
2E服务执行前需要先跳转到APP中初始化EE，将0x1C000~0x1C003中的数据设置为0
----------------------------------------------------------------------- */
STATIC const bl_DcmService_t gs_Dcm2ESubId[DCM_2ESUBID_NUMBER] = {
    {
        //id=F198 		//num=0     //minsize=19   //maxsize=19   //flag=0
        DID_WR_Fingerprint, 0, DCM_2EFINGERPRINT_MINLEN, DCM_2ESERVICE_MAXLEN, DCM_2EDID_FLAG,
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackWrite_Tester_Serial_Number
    },
    
    {
        //id=F199
        DID_WR_ProgramDate, 0, DCM_2EPROGRAMDATE_MINLEN, DCM_2ESERVICE_MAXLEN, DCM_2EDID_FLAG,
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackWriteProgrammingDate
    },
};

STATIC const bl_DcmService_t gs_Dcm22SubId[DCM_22SUBID_NUMBER] = {
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF187U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadPartNumber
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xFFFFU, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadBootSW_ID
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF18AU, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadSupplierCode
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF193U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadECU_HardwareVersionNumber
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF195U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadECU_SoftwareVersionNumber
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xFFFFU, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadSoftCode
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF197U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_System_Name
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF150U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_System_Supplier_ECU_Hardware_Version
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0x0200U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Programming_Counter
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0x0201U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Programming_Attempts_Counter
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF180U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadBootSW_ID
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF186U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadActiveSession
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xFF00U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Diagnosis_Specification_Version
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF188U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Manufacturer_ECU_Software_Number
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF189U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Manufacturer_ECU_Software_Version
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF191U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Manufacturer_ECU_Hardware_Number
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF198U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Tester_Serial_Number
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF199U, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackReadProgrammingDate
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF1EDU, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Secure_Flash
    },
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xF1EFU, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_BootApp_Identify
    },
    /* 原F182，boot要求不能支持 */
    {
        // id    //num=0  //minSize=2 //maxSize=19 //flag = DCM_22DID_FLAG
        0xFFFFU, 0, 2, 19, DCM_22DID_FLAG,
        //serviceList //header   //Callback
        BL_NULL_PTR, BL_NULL_PTR, &Adpt_UdsCallBackRead_Calibration_Software_Version
    },
};

/**************************************************************************//**
 *  \details The all service list of dcm mudule.
 *****************************************************************************/

/* -----------------------------------------------------------------------A12
flag=0表示会话和安全都支持
0x1=默认，0x2编程，0x4扩展
0x08功能寻址, 0x10物理寻址
0x100=lock1, 0x200=unlock1,
0x800=lock2, 0x1000=unlock2,
----------------------------------------------------------------------- */
//這是總的DCM UDS 服務表
STATIC const bl_DcmService_t gs_DcmService[DCM_SERVICE_NUMBER] = {
    {
        //id=0x0010  //num=3    //minSize=2
        0x0010u, DCM_10SUBID_NUMBER, DCM_10SERVICE_MINLEN,
        //maxsize=2             //flag=0         //serviceList
        DCM_10SERVICE_MAXLEN, DCM_10SERVICE_FLAG, gs_Dcm10SubId,
        //header
        &g_DcmSubIdHeader, BL_NULL_PTR
    },
    {
        //id=0x0011  //num=1    //minSize=2
        0x0011u, DCM_11SUBID_NUMBER, DCM_11SERVICE_MINLEN,
        //maxsize=2             //flag=4000 1B1E         //serviceList
        DCM_11SERVICE_MAXLEN, DCM_11SERVICE_FLAG, gs_Dcm11SubId, \
        //header
        &g_DcmSubIdHeader, BL_NULL_PTR
    },
    {
        //id=0x0027  //num=2    //minSize=2
        0x0027u, DCM_27SUBID_NUMBER, DCM_27SERVICE_MINLEN,
        //maxsize=0             //flag=4000 1B16         //serviceList
        DCM_27SERVICE_MAXLEN, DCM_27SERVICE_FLAG, gs_Dcm27SubId,
        //header
        &g_DcmSubIdHeader, BL_NULL_PTR
    },
    {
        //id=0x0028  //num=3    //minSize=3
        0x0028u, DCM_28SUBID_NUMBER, DCM_28SERVICE_MINLEN,
        //maxsize=3             //flag=4000031C         //serviceList
        DCM_28SERVICE_MAXLEN, DCM_28SERVICE_FLAG, gs_Dcm28SubId,
        //header
        &g_DcmSubIdHeader, BL_NULL_PTR
    },
    {
        //id=0x0031  //num=1    //minSize=4
        0x0031u, DCM_31SUBID_NUMBER, DCM_31SERVICE_MINLEN,
        //maxsize=0             //flag=0         //serviceList
        DCM_31SERVICE_MAXLEN, DCM_31SERVICE_FLAG, gs_Dcm31SubId,
        //header
        &g_DcmSubIdHeader, BL_NULL_PTR
    },
    {
        //id=0x0034  //num=0    //minSize=11
        0x0034u, DCM_34SUBID_NUMBER, DCM_34SERVICE_MINLEN,
        //maxsize=11             //flag=40001012         //serviceList
        DCM_34SERVICE_MAXLEN, DCM_34SERVICE_FLAG, BL_NULL_PTR,
        //header
        BL_NULL_PTR, &Adpt_UdsCallback34
    },
    {
        //id=0x0036  //num=0    //minSize=3
        0x0036u, DCM_36SUBID_NUMBER, DCM_36SERVICE_MINLEN,
        //maxsize=0             //flag=40001012         //serviceList
        DCM_36SERVICE_MAXLEN, DCM_36SERVICE_FLAG, BL_NULL_PTR,
        //header
        BL_NULL_PTR, &Adpt_UdsCallback36
    },
    {
        //id=0x0037  //num=0    //minSize=1
        0x0037u, DCM_37SUBID_NUMBER, DCM_37SERVICE_MINLEN,
        //maxsize=1             //flag=40001012         //serviceList
        DCM_37SERVICE_MAXLEN, DCM_37SERVICE_FLAG, BL_NULL_PTR,
        //header
        BL_NULL_PTR, &Adpt_UdsCallback37
    },
    {
        //id=0x003E  //num=1    //minSize=2
        0x003Eu, DCM_3ESUBID_NUMBER, DCM_3ESERVICE_MINLEN,
        //maxsize=2             //flag=4000 1B1f         //serviceList
        DCM_3ESERVICE_MAXLEN, DCM_3ESERVICE_FLAG, gs_Dcm3ESubId,
        //header
        &g_DcmSubIdHeader, BL_NULL_PTR
    },
    {
        //id=0x0085  //num=2    //minSize=2
        0x0085u, DCM_85SUBID_NUMBER, DCM_85SERVICE_MINLEN,
        //maxsize=2             //flag=4000031C         //serviceList
        DCM_85SERVICE_MAXLEN, DCM_85SERVICE_FLAG, gs_Dcm85SubId,
        //header
        &g_DcmSubIdHeader, BL_NULL_PTR
    },
    {
        //id=0x002e  //num=2    //minSize=7
        0x002Eu, DCM_2ESUBID_NUMBER, DCM_2ESERVICE_MINLEN,
        //maxsize=19             //flag=4000 1216         //serviceList
        DCM_2ESERVICE_MAXLEN, DCM_2ESERVICE_FLAG, gs_Dcm2ESubId,
        //header
        &g_DcmDidHeader, BL_NULL_PTR
    },
    {
        //id=0x0022  //num=12    //minSize=3
        0x0022u, DCM_22SUBID_NUMBER, DCM_22SERVICE_MINLEN,
        //maxsize=3             //flag=4000 1B1f         //serviceList
        DCM_22SERVICE_MAXLEN, DCM_22SERVICE_FLAG, gs_Dcm22SubId,
        //header
        &g_DcmDidHeader, BL_NULL_PTR
    },
    //g_DcmSubIdHeader
};

/*****************************************************************************
 *  External Variable Definitions
*****************************************************************************/
/**************************************************************************//**
 *  \details The dcmbuffer info, including recvSize, tranSize and so on.
 *****************************************************************************/
bl_DcmBuffer_t g_DcmBuffer = {
    //status                  //index //recvSize  //tranSize //totalSize  *b=實體uffer
    DCM_STATUS_BUFFER_UNLOCK, 0, 0, 0, DCM_BUFFER_SIZE, gs_Buffer,
};

const bl_DcmService_t g_DcmServiceList = {
    0,   //id
    DCM_SERVICE_NUMBER,  //number
    0,  //minSize
    0,	//maxSize
    0,   //flag
    gs_DcmService,  //serviceList  -- 結構體 _tag_DcmService
    &g_DcmServiceHeader,  //header  -- 結構體 _tag_DcmServiceHeader
    BL_NULL_PTR
};

