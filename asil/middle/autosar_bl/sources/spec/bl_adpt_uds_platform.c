/**************************************************************************//**
 *
 *  \copyright  This software is the property of  Technologies. Any
 *              information contained in this doc should not be reproduced,
 *              or used, or disclosed without the written authorization from
 *               Technologies.
 *
 *  \brief      This is the source file of the adapter module based on uds
 *              platform.
 *
 *  \file       bl_adpt_uds_platform.c
 *  \ingroup    communication_adapter_module
 *  \author
 *
 *  \version    5.1.0
 *  \date       27/03/2018
 *
 *  \par        Changelist
 *      Version  | Date       | Authors          | CR# | Descriptions
 *      -------- | ---------- | ---------------- | --- | ------------
 *      01.00.00 | 25/03/2018 | xucong      | N/A | Boot010001

 *
 *****************************************************************************/


#include "bl_common.h"
#include "bl_rte.h"
#include "bl_data.h"
#include "bl_can_tp.h"
#include "bl_can_if.h"
#include "bl_dcm.h"
#include "bl_dcm_cfg.h"
#include "bl_adapter.h"
#include "bl_security.h"
#include "bl_logical_block.h"
#include "bl_memory.h"
#include "bl_memory_cfg.h"
#include "bl_data_cfg.h"

#include "bl_adpt_uds_platform.h"
#include "bl_adpt_uds_platform_cfg.h"
#include "bl_booting_cfg.h"
#include "calibration.h"

#include "common_types.h"
#include "error.h"
#include "bl_system_cfg.h"
#include "crc.h"

#include "Ls_Nm.h"
#include "eeprom_cfg.h"
#include "eeprom.h"
#include "bl_security_cfg.h"
#include "bl_access_udsdemo.h"
#include "bl_logical_block.h"
#include "bl_logical_block_cfg.h"
#include "rsa.h"
#include "x509_crt.h"

/*   定义使用的配置及型号，有且只有一个 */
#define    SWS_BOOT_SW_HIGH    0x30
#define    SWS_BOOT_SW_MID     0x30
#define    SWS_BOOT_SW_LOW     0X31


#define SHA256_ROTL(a,b) (((a>>(32-b))&(0x7fffffff>>(31-b)))|(a<<b))
#define SHA256_SR(a,b) ((a>>b)&(0x7fffffff>>(b-1)))
#define SHA256_Ch(x,y,z) ((x&y)^((~x)&z))
#define SHA256_Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define SHA256_E0(x) (SHA256_ROTL(x,30)^SHA256_ROTL(x,19)^SHA256_ROTL(x,10))
#define SHA256_E1(x) (SHA256_ROTL(x,26)^SHA256_ROTL(x,21)^SHA256_ROTL(x,7))
#define SHA256_O0(x) (SHA256_ROTL(x,25)^SHA256_ROTL(x,14)^SHA256_SR(x,3))
#define SHA256_O1(x) (SHA256_ROTL(x,15)^SHA256_ROTL(x,13)^SHA256_SR(x,10))
/* 证书解析 */
#define    crt_start_addr           32 //6000服务校验参数，证书起始地址
#define    version_start_addr       1056 //6000服务校验参数，版本信息起始地址
#define    file_header_Sign_start_addr       1066 //6000服务校验参数，刷新文件头信息签名值起始地址

__root uint8_t u8ReprogramFlag[16] __attribute__((section(".ReprogramFlag")));
/*boot中定义F189，需要和app中一致，目的是为了在app不可用时，仍然能读取*/
__root const uint8_t system_supplier_ECU_hardware_Version_F150[16]@0x0000BF40 = {'0', '.', '0', '0', '.', '0', '1', 0x00, 0x00, 0x00,};
__root const uint8_t system_name_F197[16]@0x0000BF50 = {'L', 'P', '-', 'S', 'W', 'B', '0', '8',  '0',  '-', 'A', 'A', 0x00, 0x00, 0x00, 0x00,};
__root const uint8_t Manufacturer_ECU_Software_Version_F189[10]@0x0000BF60 = {'0', '.', '0', '0', '.', '0', '5', 0x00, 0x00, 0x00,};
__root const bl_u8_t     Manufacturer_ECU_Hardware_Number_F191[32]@0x0000BF70 = {'F', 'M', '3', '3', 'L', 'G', '0', '2', 'A', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
__root const bl_u8_t     Manufacturer_ECU_Software_Number_F188[32]@0x0000BF90 = {'L', 'P', 'A', '1', '2', '0', 'S', 'W', 'N', '7', 'B', 'P', '0', '1', 'S', 'W', 'B', 0x00, 0x00,};
__root const bl_u8_t     Manufacturer_Spare_Part_Number_F187[32]@0x0000BFB0 = {'5', '7', '1', '0', '1', '0', '0', '-', 'A', 'G', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
/*boot中定义F180，需要和app中一致，目的是为了在app不可用时，仍然能读取*/
__root const bl_u8_t     Primary_Bootloader_Version_F180[16]@0x0000BFD0 = {'0', '.', '0', '0', '.', '0', '3', 0x24, 0x11, 0x18,};
/* SHA256 K值 固定为常量 */
__root const bl_u32_t K[64]@0x0000BB00 = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};
/* LP CA公钥 固定为常量 */
__root const bl_u8_t ca_public_key_encrypted[256]@0x0000BC00 = {
    0x23, 0xa4, 0xfd, 0xe6, 0x16, 0x1c, 0x8f, 0xa7, 0x46, 0xfc, 0x8e, 0x96, 0x30, 0x9f, 0x0e, 0x42,
    0xfd, 0x9f, 0x4f, 0x1c, 0xfe, 0xae, 0xa3, 0x14, 0x08, 0x59, 0x3a, 0x06, 0xc6, 0x02, 0x4b, 0xcf,
    0x91, 0x98, 0xc6, 0x11, 0x7f, 0x1c, 0x77, 0x85, 0x11, 0x44, 0x58, 0x8d, 0x99, 0x8c, 0xe0, 0xb0,
    0x48, 0xc9, 0x00, 0xc5, 0x89, 0x76, 0x71, 0xc7, 0x57, 0x42, 0x77, 0x2a, 0x24, 0x9b, 0x71, 0xd8,
    0x79, 0xdd, 0x48, 0x17, 0x32, 0x0e, 0x77, 0xc5, 0x30, 0xc1, 0x5a, 0x07, 0xe3, 0x6a, 0x2e, 0x1e,
    0x4e, 0x8d, 0xa3, 0xef, 0x1f, 0x95, 0xb3, 0x49, 0x27, 0x77, 0xeb, 0x45, 0x83, 0xf3, 0xdb, 0x00,
    0x77, 0x24, 0x57, 0x36, 0x68, 0x9e, 0x1e, 0xcb, 0x9f, 0xe8, 0xb8, 0x98, 0x70, 0x82, 0xb2, 0x50,
    0xa1, 0xea, 0x11, 0xef, 0x96, 0x04, 0x6a, 0x23, 0xa7, 0xf7, 0xdd, 0x2c, 0x74, 0x1e, 0x74, 0x97,
    0x73, 0x89, 0xba, 0x45, 0xa2, 0x6c, 0x37, 0x54, 0x67, 0x2a, 0x5a, 0x6d, 0xb7, 0x70, 0x0e, 0x0b,
    0x0c, 0xdd, 0x5e, 0x1c, 0x4b, 0x02, 0x75, 0x9c, 0xcf, 0x26, 0x5f, 0x4f, 0xf8, 0x49, 0xfe, 0x08,
    0xca, 0x8a, 0xff, 0x7d, 0x35, 0x46, 0x63, 0xc6, 0xcf, 0x10, 0x68, 0x08, 0x95, 0x76, 0x70, 0x2c,
    0xfe, 0x53, 0xa5, 0x57, 0x2e, 0xe8, 0x71, 0x1d, 0x4d, 0x14, 0xf1, 0x97, 0xaf, 0x9e, 0x62, 0x7e,
    0x27, 0xa8, 0x2b, 0x47, 0x40, 0xf6, 0x2d, 0x87, 0x05, 0x38, 0x46, 0xd9, 0x58, 0x91, 0xc6, 0x96,
    0x8c, 0x8b, 0xcf, 0xff, 0x9e, 0x1a, 0x6a, 0x94, 0x66, 0x55, 0x96, 0xe1, 0xcc, 0xba, 0xd0, 0x61,
    0x04, 0x60, 0xd2, 0x38, 0xed, 0xa0, 0x97, 0x01, 0x89, 0x98, 0xcc, 0x35, 0xe0, 0xc3, 0xb3, 0xf7,
    0x19, 0xbb, 0x19, 0x65, 0xe4, 0x81, 0x77, 0x84, 0x9f, 0x49, 0x8d, 0x2d, 0x3a, 0x96, 0x82, 0x25,
};
#if 0 //自签证书中的公钥
unsigned char rsa_N_hex[256] = {
    0xd9, 0xb3, 0x4d, 0x79, 0xc6, 0xca, 0x23, 0x19, 0x63, 0x1f, 0xcd, 0xaf, 0x61, 0x77, 0xe9, 0x8f,
    0xec, 0xd8, 0xad, 0x08, 0x7e, 0xa5, 0x05, 0xed, 0xa4, 0x70, 0xe4, 0x0f, 0xf1, 0x0e, 0xa6, 0xbd,
    0xcf, 0x91, 0xca, 0xff, 0x58, 0xfd, 0x71, 0xe1, 0xdf, 0x63, 0x88, 0xd1, 0xfa, 0xa1, 0x91, 0xf2,
    0x70, 0x5c, 0x91, 0xb7, 0x41, 0xec, 0x18, 0x38, 0xfe, 0xe6, 0x71, 0xcf, 0x03, 0xb5, 0x7e, 0xea,
    0xb2, 0x02, 0x52, 0xec, 0xeb, 0xb9, 0xf4, 0xf3, 0x73, 0xf1, 0x24, 0x22, 0xa2, 0xb8, 0xfe, 0x05,
    0xe8, 0xda, 0x65, 0x8d, 0x35, 0x2e, 0xeb, 0x7e, 0xc8, 0x28, 0x00, 0x45, 0x4c, 0x3f, 0xaf, 0xc8,
    0x8a, 0xe5, 0xb5, 0xdd, 0xca, 0xa7, 0x25, 0x0d, 0xb1, 0x45, 0x9b, 0x6f, 0x27, 0xc3, 0x62, 0xb8,
    0x49, 0xdd, 0xb8, 0xb5, 0x99, 0xf4, 0x68, 0xa0, 0xca, 0x70, 0x10, 0x7c, 0x54, 0x93, 0x06, 0x7d,
    0x96, 0xfd, 0x91, 0x54, 0x09, 0xd3, 0x07, 0xf5, 0x4d, 0x01, 0x0d, 0x17, 0xe9, 0x70, 0x46, 0xc0,
    0x3e, 0x7c, 0x52, 0x1a, 0xe4, 0xb8, 0x70, 0x85, 0xe8, 0x77, 0x38, 0x34, 0x53, 0xf4, 0x58, 0xaf,
    0x3e, 0x3c, 0x09, 0x3d, 0xda, 0x5b, 0x70, 0xba, 0x0a, 0x3e, 0xfa, 0xb7, 0x80, 0x9a, 0xda, 0x8f,
    0x76, 0x2a, 0x0e, 0x04, 0x2f, 0xac, 0xc3, 0x8c, 0xf9, 0xc0, 0xd6, 0xd5, 0xde, 0x1c, 0x9c, 0x12,
    0x4e, 0x01, 0xba, 0xc0, 0x2d, 0x16, 0x01, 0xba, 0x9e, 0x98, 0xe2, 0xf6, 0xba, 0x6b, 0x4c, 0x0e,
    0x60, 0xa1, 0x25, 0x7c, 0x1c, 0xaa, 0x83, 0xe7, 0xc0, 0xb8, 0x48, 0xea, 0x04, 0xbf, 0x6d, 0x16,
    0x13, 0x2f, 0xb4, 0x08, 0x30, 0x95, 0xea, 0x9e, 0x76, 0x71, 0x08, 0x57, 0xe5, 0x5d, 0x34, 0x16,
    0x95, 0x79, 0x0a, 0x94, 0x37, 0x93, 0x63, 0x8c, 0xcb, 0x5b, 0x3f, 0x0c, 0xaf, 0x19, 0x7e, 0x1b,
};
#endif
/* CA公钥中的E值 */
const unsigned char rsa_E_hex[3] = {0x1, 0x0, 0x1};
/* 自签证书中的颁发者信息www.taiking.cn */
const unsigned char issuer_TK[14] = {0x77, 0x77, 0x77, 0x2E, 0x74, 0x61, 0x69, 0x6B, 0x69, 0x6E, 0x67, 0x2E, 0x63, 0x6E,};
/* 新的哈希计算前，需要重新赋值哈希初始值0x0000BD00~0x0000BD1F */
__root const bl_u32_t init_hash[8]@0x0000BD00 = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

__root const bl_u32_t ca_pub_key_sha256[8]@0x0000BD20 = {
    0x612d16a8, 0xb61cf085,
    0x142494b9, 0x1f5c9dff,
    0x930bce4c, 0xdb5d8dde,
    0x59260662, 0xda66e33a,
};
/* EKEA PROJECT END*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef UDS_DEBUG_EN
typedef struct {
    u16 data[2];
    u16 time[8];
} usd_test_s;
usd_test_s g_usd_test = {0};
#endif


/*****************************************************************************
 *  Verify The Configurations of Macro
 *****************************************************************************/
#if (DCM_BUFFER_SIZE < ADPT_MAXNUM_OF_BLOCKLENGTH)
#error "the buffer of the Dcm module is too small."
#endif

#if (RTE_FUN_GATEWAY == BL_FUN_ON)
#error "the function of gateway is not ok."
#endif
/*****************************************************************************
 *  Internal Macro Definitions
 *****************************************************************************/
/*lint -emacro(778,ADPT_SESSION_RESPONSE0,ADPT_SESSION_RESPONSE1)*/
/*lint -emacro(778,ADPT_SESSION_RESPONSE2,ADPT_SESSION_RESPONSE3)*/
/*lint -emacro(572,ADPT_SESSION_RESPONSE0,ADPT_SESSION_RESPONSE1)*/
/*lint -emacro(572,ADPT_SESSION_RESPONSE2,ADPT_SESSION_RESPONSE3)*/
#define ADPT_P2START_TIMER          (DCM_P2STARTIMEVALUE/DCM_P2STARTIMER_DIV)
#define ADPT_SESSION_RESPONSE0      (u8)(((u16)DCM_P2TIMEVALUE >> 8) & 0xFFu)
#define ADPT_SESSION_RESPONSE1      (u8)(((u16)DCM_P2TIMEVALUE >> 0) & 0xFFu)
#define ADPT_SESSION_RESPONSE2      (u8)(((u16)ADPT_P2START_TIMER >> 8) & 0xFFu)
#define ADPT_SESSION_RESPONSE3      (u8)(((u16)ADPT_P2START_TIMER >> 0) & 0xFFu)

#define ADPT_MEMORY_LEN_FORMAT          (0x04u)
#define ADPT_MEMORY_ADDR_FORMAT         (0x04u)

#define ADPT_MEMORY_LENANDADDR_FORMAT   ((ADPT_MEMORY_LEN_FORMAT << 4) \
    | ADPT_MEMORY_ADDR_FORMAT)

#define ADPT_ERASE_DATA_LENGTH          (0x01u \
    + ADPT_MEMORY_LEN_FORMAT \
    + ADPT_MEMORY_ADDR_FORMAT)
#define ADPT_CHECKSUM_LENGTH            (0x04u)

#define ADPT_DATAFORMAT_IDEN            (0x00u)
#define ADPT_LENFORMAT_IDEN             (0x20u)
#define ADPT_IDEN_LEN_FORMAT            ((ADPT_LENFORMAT_IDEN >> 4) & 0x0Fu)
#define ADPT_REQDOWNLOAD_RESPLENGTH     (0x01u + ADPT_IDEN_LEN_FORMAT)

/** \brief Indicate the 31 service is ok.*/
#define ADPT_ROUTINE_CORRECT_RESULT     (0u)  //(0) // GL =  4	//31應答，FE-5HA是 4 和 5
/** \brief Indicate the 31 service is not ok.*/
#define ADPT_ROUTINE_INCORRECT_RESULT   (1u)  //(1) // GL = 5
#define ADPT_ROUTINE_INCORRECT_SIG_RESULT   (2u)  //(1) // GL = 5
#if 0  //目前沒有用到
#define ADPT_ROUTINE_NOT_ALL_MANDATORY_BLOCKS_PRESENT_RESULT   (6)  // 31  FF 01

#define ADPT_ROUTINE_HW_INCOMPATIBILITY_RESULT   (7)  // 31  FF 01
#define ADPT_ROUTINE_SW_INCOMPATIBILITY_RESULT   (8)  // 31  FF 01
#endif

#define ADPT_UDS_SYSTEM_STATUS_MASK     (RTE_SYSFLAG_WRITE_FINGPRINT \
    | ADPT_SYSTEM_STATUS_MASK)  //1c

#define ADPT_UDS_STATUS_MASK            (ADPT_STATUS_ALLOW_TRANS_DATA \
    | ADPT_STATUS_BLOCK_FIRST_DATA \
    | ADPT_STATUS_SEGMENT_FIRST_DATA)  //78000000

#define ADPT_UDS_36_SERVICE_ID              (0x36u)
#define ADPT_UDS_31_SERVICE_ID              (0x31u)
#define ADPT_UDS_3E_SERVICE_ID              (0x3Eu)
#define ADPT_UDS_11_SERVICE_ID              (0x11u)

#define ADPT_UDS_36_SERVICE_INDEX_POS       (0x00u)
#define ADPT_UDS_36_SERVICE_DATA_POS        (0x01u)

#define ADPT_UDS_ERASE_MEMORY_FORMAT_POS    (0)  // 44開始
#define ADPT_UDS_ERASE_MEMORY_ADDRESS_POS   (1)
#define ADPT_UDS_ERASE_MEMORY_SIZE_POS      (5)

#define ADPT_UDS_34_SERVICE_IDEN_POS        (0)
#define ADPT_UDS_34_SERVICE_ADDRANDSIZE_POS (1)
#define ADPT_UDS_34_SERVICE_ADDRESS_POS     (2)
#define ADPT_UDS_34_SERVICE_SIZE_POS        (ADPT_UDS_34_SERVICE_ADDRESS_POS \
    + ADPT_MEMORY_ADDR_FORMAT)   //6

#define ADPT_UDS_34_RESPONSE_FORMAT_POS     (0)  // 如 20是格式
#define ADPT_UDS_34_RESPONSE_SIZE_POS       (1)		// 如 0402是長度


#define ADPT_STATUS_ALLOW_TRANS_DATA        DCM_MAKE_FLAG(0x20000000UL)  //
#define ADPT_STATUS_BLOCK_FIRST_DATA        DCM_MAKE_FLAG(0x10000000UL)
#define ADPT_STATUS_SEGMENT_FIRST_DATA      DCM_MAKE_FLAG(0x08000000UL)

#define ADPT_UDS_MAX_CAN_FRAME_SIZE         (CANTP_MAX_FRAME_SIZE)    //好好查一下該宏 
#define ADPT_UDS_CAN_FRAME_INDEX_0          (0)
#define ADPT_UDS_CAN_FRAME_INDEX_1          (1)
#define ADPT_UDS_CAN_FRAME_INDEX_2          (2)
#define ADPT_UDS_CAN_FRAME_INDEX_3          (3)
#define ADPT_UDS_CAN_FRAME_INDEX_4          (4)
#define ADPT_UDS_CAN_FRAME_INDEX_5          (5)
#define ADPT_UDS_CAN_FRAME_INDEX_6          (6)
#define ADPT_UDS_CAN_FRAME_INDEX_7          (7)
#define ADPT_UDS_CAN_FRAME_INDEX_8          (8)

/*****************************************************************************
 *  Internal Type Definitions
 *****************************************************************************/
/** \brief A alias of struct _tag_AdapterData.*/
typedef struct _tag_AdapterData bl_AdapterData_t;

#if (RTE_FUN_GATEWAY == BL_FUN_ON)
/** \brief A alias of struct _tag_GatewayTransmitList.*/
typedef struct _tag_GatewayTransmitList bl_GatewayTransmitList_t;
/** \brief A alias of struct _tag_GatewayConversion.*/
typedef struct _tag_GatewayConversion bl_GatewayConversion_t;
#endif

/*****************************************************************************
 *  Internal Structure Definitions
 *****************************************************************************/
/** \brief This is data of a adapter.*/
struct _tag_AdapterData {
    struct _tag_SecurityDelay delay;    /**< security access delay*/
    bl_u8_t blockIndex;     /**< the block index of 36 service*/
    bl_Buffer_t serviceId;  /**< the id of processing service*/
    bl_Address_t address;   /**< the address of 36 service*/
    bl_Size_t remainSize;   /**< the remain size of 36 service*/
};

#if(RTE_FUN_GATEWAY == BL_FUN_ON)
/** \brief This is a list of gateway transmission.*/
struct _tag_GatewayTransmitList {
    bl_u16_t handle;
    bl_u16_t number;
    struct _tag_GatewayConversion *data;
};

/**
 * \brief This is info of gateway conversion, it includes services
 *        needed to be transmitted.
 */
struct _tag_GatewayConversion {
    bl_u8_t sid;
    bl_u16_t size;
    bl_Buffer_t *data;
};

#endif
/*****************************************************************************
 *  Internal Function Declarations
 *****************************************************************************/
#if(RTE_FUN_GATEWAY == BL_FUN_ON)
/** \brief Gateway transmits messages.*/
static void _Adpt_UdsGatewayTransmit(bl_u8_t id);
#endif
/** \brief Send 0x78 negative response.*/
static void _Adpt_UdsPendingResponse(bl_u8_t id);
/** \brief Timeout callback of sending 0x78.*/
static void _Adpt_TimeOutCallBack(bl_u8_t id);
/** \brief send a response by comif.*/
static void _Adpt_SendResponse(bl_BufferSize_t size, const bl_Buffer_t *buffer);
/** \brief Set response info of session.*/
static bl_BufferSize_t _Adpt_SetSessionResp(bl_Buffer_t *buffer);
/** \brief Goto the session.*/
static void _Adpt_UdsGotoSession(bl_DownStatus_t sessionMask);
/** \brief Reset the bootloader.*/
static void _Adpt_UdsResetSystem(bl_u8_t sid, bl_u16_t flagId);
/** \brief Clear the download information.*/
static void _Adpt_UdsClearDownInfo(bl_AdapterData_t *adpt);

/*****************************************************************************
 *  Internal Variable Definitions
 *****************************************************************************/
/** \brief Private data variable.*/
STATIC bl_AdapterData_t gs_UdsPrivateData;
bl_Buffer_t block_data_36_service[ADPT_MAXNUM_OF_TRANS_DATA];
bl_u16_t block_used_length;
/* SHA256 */
bl_u32_t hash_buff[8]; //哈希结果
bl_u32_t sum_length = 0; //计算哈希值的分块数据总长度
bl_u32_t all_sha256_data_length; //完整的数据长度
bl_u32_t sha256_block1_length; //flashdriver的长度
bl_u8_t sha256_cook_data[64]; //将原始数据处理后的数据

/* X509 以下值均为相对证书起始地址的偏移量 */
bl_u16_t crt_tbs_addr_offset; //证书主体
bl_u16_t crt_tbs_len; //证书主体长度
bl_u16_t crt_issuer_addr_offset; //证书签发者
bl_u16_t crt_issuer_len; //证书签发者
bl_u16_t pkey_N_addr_offset; //N值
bl_u16_t pkey_E_addr_offset; //E值
bl_u16_t pkey_sig_addr_offset; //签名
bl_u8_t recv_data_6000[1322];


bl_u32_t KEY2712_local;
bl_u32_t KEY2712_recv;
//检查合法性及完整性校验是否失败;检查和激活次级引导加载程序是否失败
bl_u8_t necessary_condition_count = 0; //初始化

/*以两个用于查看程序执行时间*/
bl_u32_t IWDT_CNT_test;
bl_u32_t IWDT_CNT_test1;
#if 0
struct _tag_AdapterData {
    struct _tag_SecurityDelay delay;    /**< security access delay*/
    bl_u8_t blockIndex;     /**< the block index of 36 service*/
    bl_Buffer_t serviceId;  /**< the id of processing service*/
    bl_Address_t address;   /**< the address of 36 service*/
    bl_Size_t remainSize;   /**< the remain size of 36 service*/
};
#endif

#if ADPT_UDS_MAX_CAN_FRAME_SIZE <=8
STATIC const bl_Buffer_t gs_DefaultResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x06u, 0x50u, 0x01u, ADPT_SESSION_RESPONSE0, ADPT_SESSION_RESPONSE1,
    ADPT_SESSION_RESPONSE2, ADPT_SESSION_RESPONSE3, CANTP_FRAME_PADDING_VALUE
};
#else
STATIC const bl_Buffer_t gs_DefaultResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x00, 0x06u, 0x50u, 0x01u, ADPT_SESSION_RESPONSE0, ADPT_SESSION_RESPONSE1,
    ADPT_SESSION_RESPONSE2, ADPT_SESSION_RESPONSE3
};
#endif

#if ADPT_UDS_MAX_CAN_FRAME_SIZE <=8
STATIC const bl_Buffer_t gs_ReprogrammingResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x06u, 0x50u, 0x02u, ADPT_SESSION_RESPONSE0, ADPT_SESSION_RESPONSE1,
    ADPT_SESSION_RESPONSE2, ADPT_SESSION_RESPONSE3, CANTP_FRAME_PADDING_VALUE
};
#else
STATIC const bl_Buffer_t gs_ReprogrammingResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x00, 0x06u, 0x50u, 0x02u, ADPT_SESSION_RESPONSE0, ADPT_SESSION_RESPONSE1,
    ADPT_SESSION_RESPONSE2, ADPT_SESSION_RESPONSE3
};
#endif

#if ADPT_UDS_MAX_CAN_FRAME_SIZE <=8
STATIC const bl_Buffer_t gs_ResetResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x02u, 0x51u, 0x01u, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE
};
#else
STATIC const bl_Buffer_t gs_ResetResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x00, 0x02u, 0x51u, 0x01u, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE
};
#endif

#if ADPT_UDS_MAX_CAN_FRAME_SIZE <=8
STATIC const bl_Buffer_t gs_ResetResponse_sub03[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x02u, 0x51u, 0x03u, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE
};
#else
STATIC const bl_Buffer_t gs_ResetResponse_sub03[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x00, 0x02u, 0x51u, 0x03u, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE,
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE
};
#endif


#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
#if ADPT_UDS_MAX_CAN_FRAME_SIZE <=8
STATIC const bl_Buffer_t gs_StayInBootResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x04u, 0x71u, 0x01u, ADPT_STAY_IN_BOOT_RESPONSE_HID, //F5
    //18							AA
    ADPT_STAY_IN_BOOT_RESPONSE_LID, CANTP_FRAME_PADDING_VALUE,
    //AA 						AA
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE
};
#else
STATIC const bl_Buffer_t gs_StayInBootResponse[ADPT_UDS_MAX_CAN_FRAME_SIZE] = {
    0x0, 0x04u, 0x71u, 0x01u, ADPT_STAY_IN_BOOT_RESPONSE_HID, //F5
    //18							AA
    ADPT_STAY_IN_BOOT_RESPONSE_LID, CANTP_FRAME_PADDING_VALUE,
    //AA 						AA
    CANTP_FRAME_PADDING_VALUE, CANTP_FRAME_PADDING_VALUE
};
#endif
#endif

#if(RTE_FUN_GATEWAY == BL_FUN_ON)
/** \details The data of gateway conversion. */
STATIC const bl_u8_t gs_UdsGatewayData[ADPT_UDS_CONVERSION_DATA_LENGTH] = { //2
    0x3Eu, 0x80u,
}

/** \details A list of gateway transmission. */
STATIC const bl_GatewayTransmitList_t gs_UdsGatewayList = {
    ADPT_UDS_COMOPS_HANDLE,  //0XFFFF
    ADPT_UDS_NUMBER_OF_GATEWAY,  //1
    gs_UdsGateway,
};

/** \details The info of gateway conversion. */
STATIC const bl_GatewayConversion_t gs_UdsGateway[ADPT_UDS_NUMBER_OF_GATEWAY] = { //1
    {
        ADPT_UDS_3E_SERVICE_ID,   // SID= 3E
        ADPT_UDS_CONVERSION_DATA_LENGTH,  //SZIE =2
        gs_UdsGatewayData  // 內容指針 指向 gs_UdsGatewayData 3e 80
    },
};

#endif

/*****************************************************************************
 *  Global Variable Definitions
 *****************************************************************************/
/** \details The Uds platform adapter informations. */
const bl_Adapter_t g_UdsPlatformAdapter = {
    //type=0  index=0 //Init      //Callback
    0x00u, 0x00u, &Adpt_UdsInit, &Adpt_UdsCallback,
    //PeriodCallback             //Timeout
    &Adpt_UdsPeriodicCallback, &Adpt_UdsTimeOutCallBack,
    //Sleep 目前沒有sleep
    #if (RTE_FUN_SYSTEM_SLEEP == BL_FUN_ON)
    &Canif_Sleep,
    #else
    BL_NULL_PTR, /*RTE_FUN_SYSTEM_SLEEP is OFF*/
    #endif
};
/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
//Level-3进入编程会话用：
static bl_u32_t SeedKeyPROGSupplier(bl_u32_t seed) {
    const bl_u32_t key[4U] = {0x4fe87269U, 0x6bc361d8U, 0x9b127d51U, 0x5ba41903U}; /*128bits*/
    bl_u32_t y = ((seed << 24U) & 0xff000000U) + ((seed << 8U) & 0xff0000U) + ((seed >> 8U) & 0xff00U) + ((seed >> 24U) & 0xffU); /* swap byte order */
    bl_u32_t z = 0U, sum = 0U;
    bl_u8_t n = 64U; /*numberof iterations */
    while (n > 0U) { /*encrypt*/
        y += (((z << 4U) ^ (z >> 5U)) + z) ^ (sum + key[sum & 3U]);
        sum += 0x8f750a1dU;
        z += (((y << 4U) ^ (y >> 5U)) + y) ^ (sum + key[(sum >> 11U) & 3U]);
        n--;
    }
    return ((z << 24U) & 0xff000000U) + ((z << 8U) & 0xff0000U) + ((z >> 8U) & 0xff00U) + ((z >> 24U) & 0xffU);
    /*swapbyteorder*/
    //Eg:输入：0x12345678，输出：0x680F2EDE
}
/* -----------------------------------------------------------------------A12
length输入的值有64字节（完整的块，若是数据最后一块，后面要加一块补充块），
length=0时，（完整块后的）补充块
56<=length<64(余数数据余数>=56的第一块,后面还有补充块)
＞64字节的（余数块后的）补充块
原始数据根据长度通过预处理分为完整块+补充块或者余数块+补充块或者仅余数块
----------------------------------------------------------------------- */
void StrSHA256(const bl_u8_t *buffer, bl_u32_t length, bl_u32_t *input_hash) {
    bl_u8_t *ppend;
    bl_u32_t i, W[64], T1, T2, A, B, C, D, E, F, G, H;
    
    if (length <= 64) { //完整块 余数块
        sum_length += length;
    } else {}//补充块
    if (length <= 64) { //补充完整
        for (i = 0; i < length; i++) {
            sha256_cook_data[i + 3 - 2 * (i % 4)] = buffer[i];//将输入的数据按word格式排序
        }
    }
    if (length < 64) { //余数块；length=0时，作为补充块
        sha256_cook_data[i + 3 - 2 * (i % 4)] = 128;
        for (i++; i < 64; i++) { //刚进来时i=length
            sha256_cook_data[i + 3 - 2 * (i % 4)] = 0;
        }
        if (length < 56) {
            *((bl_u32_t *)(sha256_cook_data + 64 - 4)) = sum_length << 3;
            *((bl_u32_t *)(sha256_cook_data + 64 - 8)) = sum_length >> 29;
            sum_length = 0;
        }
    } else if (length > 64) { //补充块
        for (i = 0; i < 64; i++) { //刚进来时i=length
            sha256_cook_data[i + 3 - 2 * (i % 4)] = 0;
        }
        *((uint32_t *)(sha256_cook_data + 64 - 4)) = sum_length << 3;
        *((uint32_t *)(sha256_cook_data + 64 - 8)) = sum_length >> 29;
        sum_length = 0;
    } else {}
    for (i = 0; i < 16; i++) {
        W[i] = ((bl_u32_t *)sha256_cook_data)[i]; //先将数据分割成64字节按word格式
    }
    for (i = 16; i < 64; W[i] = (SHA256_O1(W[i - 2]) + W[i - 7] + SHA256_O0(W[i - 15]) + W[i - 16]), i++); //再将数据新增3*64字节
    A = input_hash[0];
    B = input_hash[1];
    C = input_hash[2];
    D = input_hash[3];
    E = input_hash[4];
    F = input_hash[5];
    G = input_hash[6];
    H = input_hash[7];
    for (i = 0; i < 64; i++) {//对这4*64个大块进行计算
        T1 = H + SHA256_E1(E) + SHA256_Ch(E, F, G) + K[i] + W[i];
        T2 = SHA256_E0(A) + SHA256_Maj(A, B, C);
        H = G;
        G = F;
        F = E;
        E = D + T1;
        D = C;
        C = B;
        B = A;
        A = T1 + T2;
    }
    input_hash[0] += A;
    input_hash[1] += B;
    input_hash[2] += C;
    input_hash[3] += D;
    input_hash[4] += E;
    input_hash[5] += F;
    input_hash[6] += G;
    input_hash[7] += H; //新的哈希初始值
}

void SHA256_preporcess(const bl_u8_t *buffer, bl_u32_t length, bl_u32_t max_length, bl_u32_t *input_hash) {
    bl_u16_t block_cnt, i;
    bl_u8_t  block_remainder;
    static bl_u32_t hash_length_sum = 0;
    block_cnt = length / 64;
    block_remainder = length % 64;
    hash_length_sum += length;
    
    if (length > 0) {
        if (block_cnt) {
            for (i = 0; i < block_cnt; i++) {
                StrSHA256((const bl_u8_t *)(&buffer[64 * i]), 64, input_hash);
            }
            if (hash_length_sum >= max_length) { //余数块和补充块计算后就是最终哈希值
                hash_length_sum = 0;
                if (block_remainder >= 56) {
                    for (i = 0; i < 2; i++) { //这里的循环1. 余数块；2.（余数块后的）补充块，此时输入的数据无效
                        StrSHA256((const bl_u8_t *)(&buffer[64 * (block_cnt + i)]), block_remainder * (i + 1), input_hash);
                    }
                } else { //这里分两种情况：1. 余数块；2. （完整块后的）补充块，此时输入的数据无效
                    StrSHA256((const bl_u8_t *)(&buffer[64 * i]), block_remainder, input_hash);
                }
            }
        } else {
            if (hash_length_sum >= max_length) { //余数块和补充块计算后就是最终哈希值
                hash_length_sum = 0;
                if (block_remainder >= 56) {
                    for (i = 0; i < 2; i++) {
                        StrSHA256((const uint8_t *)buffer, block_remainder * (i + 1), input_hash);
                    }
                } else {
                    StrSHA256((const uint8_t *)buffer, block_remainder, input_hash);
                }
            }
        }
    }
}

static int x509_crt_get_id_pubkey(const unsigned char *buf, bl_u16_t buflen) {
    mbedtls_x509_crt cert; //分配证书结构体变量
    int ret;
    
    mbedtls_x509_crt_init(&cert);
    
    /* parse certificate derectly */
    ret = mbedtls_x509_crt_parse(&cert, buf, buflen); //parse解析 &cert传入证书首地址  buf代表证书首地址 buflen代表证书长度
    if (ret != 0) {
    } else {
        // if (pkey && pkeylen) {
        // 	*pkeylen = cert.pk.pk_len - 1;
        // 	memcpy(pkey, (unsigned char*)(cert.pk.pk_ctx + 1), cert.pk.pk_len);
        // }
        
        // if (id && idlen) {
        //     *idlen = cert.serial.len;
        //     memcpy(id, cert.serial.p, cert.serial.len);
        // }
    }
    mbedtls_x509_crt_free(&cert);
    return ret;
}

/**************************************************************************//**
 *
 *  \details Initialize the uds module. This function mainly initialize
 *           CanTp, dcm and security access delay info.
 *
 *  \return If Initialization is successful return BL_ERR_OK,
 *          otherwise return BL_ERR_NOT_OK.
 *
 *  \retval BL_ERR_OK       - Initialize successfully.
 *  \retval BL_ERR_NOT_OK   - Initialize unsuccessfully.
 *
 *  \note This function is called by the process driver module.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_Return_t Adpt_UdsInit(void) {
    bl_Return_t ret;
    
    //ret = Canif_Init(); //己在Rte_PreInit初始化過了
    Cantp_Init();
    Dcm_Init();
    CanNm_Init();
    
    gs_UdsPrivateData.serviceId = 0;
    
    Adpt_InitDelayInfo(&gs_UdsPrivateData.delay);
    
    _Adpt_UdsGotoSession(DCM_STATUS_SESSION_DEFAULT);
    
    return ret;
}

/**************************************************************************//**
 *
 *  \details Periodic callback of function Rte_PeriodicSchedule. This function
 *           needs to be called periodically, and mainly handles timer of dcm,
 *           and CanTp module.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
void Adpt_UdsPeriodicCallback(void) {
    #ifdef UDS_DEBUG_EN
    g_usd_test.time[0]++;
    if (g_usd_test.time[0] >= 1000) { //果然是250us;後來改回1ms了
        g_usd_test.time[0] = 0;
        g_usd_test.time[1]++;
    }
    #endif
    Adpt_DelayTimeCallback(&gs_UdsPrivateData.delay);
    Dcm_TimerFunction();   // gs_DcmS3TimerStartFlag and  gs_DcmS3Timer
    Cantp_PeriodFunction();
    Dcm_MainFunction();
}

/**************************************************************************//**
 *
 *  \details Callback of the adapter, it should be always running. This
 *           function will be called by process driver module, and performs
 *           tx confirmation and rx indication.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
void Adpt_UdsCallback(void) {
    #ifdef UDS_DEBUG_EN
    g_usd_test.time[2]++;
    if (g_usd_test.time[2] >= 1000) {
        g_usd_test.time[2] = 0;
        g_usd_test.time[3]++;
    }
    #endif
    Canif_TxMainFunction();
    Canif_RxMainFunction();
    Canif_BusoffMainFunction();
}

/**************************************************************************//**
 *
 *  \details Callback of default session.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *   在數組 gs_Dcm10SubIdl 中
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback1001(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    bl_u8_t l_buf[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    bl_u8_t i;
    (void)size;
    
    /*programming session.*/
    ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SESSION_PROGRAMMING);
    if (BL_ERR_OK == ret) {
        #if 1
        if (Dcm_IsSuspendedResponse() != BL_ERR_OK) { //gs_SupPosRespFlag==0
            #ifdef DEFAULT_GOTO_APP_DEFAULT_SEESION_78
            //先修改ram标志，使app能判断
            (void)Mem_Write(MEM_MEMORY_ID_RAM, DM_DATA_3_LOCAL_ADDR, DM_DATA_3_SIZE, DM_DATA_3_DDP); //
            _Adpt_UdsPendingResponse(0x10);
            #else
            for (i = 0; i < 8; i++) {
                l_buf[i] = gs_DefaultResponse[i];
            }
            for (i = 8; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
                l_buf[i] = CANTP_FRAME_PADDING_VALUE;
            }
            _Adpt_SendResponse((bl_BufferSize_t)sizeof(l_buf), l_buf);
            #endif
        }
        g_RteInfo.SystemReset();
        #else
        
        if (Dcm_IsSuspendedResponse() != BL_ERR_OK) { //gs_SupPosRespFlag==0
            _Adpt_SendResponse((bl_BufferSize_t)sizeof(gs_DefaultResponse),
                gs_DefaultResponse);
        } else {
            Dcm_StopS3Timer();
            *respSize = _Adpt_SetSessionResp(buffer);
        }
        if (Lbm_IsAllValid() == BL_ERR_OK) {
            Dcm_StopS3Timer();
            _Adpt_UdsGotoSession(DCM_STATUS_SESSION_DEFAULT);
            *respSize = _Adpt_SetSessionResp(buffer);
        } else {
            //(void)Dm_WriteDefaultData(BOOTM_REPROGRAM_FLAG_DID);
            g_RteInfo.SystemReset(); /// 需要注释  待调试确认是否要复位?????
        }
        #endif
    } else {
    
        #if 1
        //
        //#else
        /*extended session.*/
        ret = Rte_IsValidStatus(&g_DownContext,
                DCM_STATUS_SESSION_EXTENDED);
        if (BL_ERR_OK == ret) {
            Dcm_StopS3Timer();
            
            _Adpt_UdsGotoSession(DCM_STATUS_SESSION_DEFAULT);
        }
        
        *respSize = _Adpt_SetSessionResp(buffer);
        #endif
    }
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of programming session.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback1002(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_CONDITIONSNOTCORRECT;
    bl_Return_t ret;
    
    (void)size;
    
    ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SESSION_DEFAULT);
    if (ret != BL_ERR_OK) { /*the default can not go to programming.*/
        /*write flag.*/
        if (Lbm_IsAllValid() == BL_ERR_OK) {
            Dcm_StartS3Timer();
            
            Lbm_ClearBlock(&g_DownContext);  //会知道当前是 flash app 还是 flash drv, 清除为 LBM_STATUS_INVALID
            
            Rte_ClearAddressInfo(&g_DownContext);  // dct->segIndex
            
            _Adpt_UdsGotoSession(DCM_STATUS_SESSION_PROGRAMMING);
            
            *respSize = _Adpt_SetSessionResp(buffer);
            resCode = DCM_E_POSITIVERESPONSE;
        } else {
            if (gs_SupPosRespFlag == 1) {
                u8ReprogramFlag[8] = 1; //A12 在复位后响应抑制位可查询
            }
            (void)Dm_WriteDefaultData(BOOTM_REPROGRAM_FLAG_DID); //BOOTM_REPROGRAM_FLAG_DID
            #ifdef BOOT_STATE_PROG_DEBUG_EN
            #else
            g_RteInfo.SystemReset();
            
            #endif
        }
    }
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of extended session.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback1003(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_CONDITIONSNOTCORRECT;
    bl_Return_t ret;
    
    (void)size;
    
    #if 0  //任何會話，直接進入03
    /*programming session.*/
    ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SESSION_PROGRAMMING);
    
    ///ret = BL_ERR_NOT_OK; ///// ???
    if (ret != BL_ERR_OK) /*the programming can not go to extended.*/
    #endif
    {
        Dcm_StartS3Timer();
        
        _Adpt_UdsGotoSession(DCM_STATUS_SESSION_EXTENDED);
        
        *respSize = _Adpt_SetSessionResp(buffer);
        
        resCode = DCM_E_POSITIVERESPONSE;
    }
    
    return resCode;
}
#if 0
/**************************************************************************//**
 *
 *  \details Callback of extended session.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback1004(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_CONDITIONSNOTCORRECT;
    bl_Return_t ret;
    
    (void)size;
    
    /*programming session.*/
    ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SESSION_PROGRAMMING);
    
    ///ret = BL_ERR_NOT_OK; ///// ???
    if (ret != BL_ERR_OK) { /*the programming can not go to extended.*/
        Dcm_StartS3Timer();
        
        _Adpt_UdsGotoSession(DCM_STATUS_SESSION_EXT_USER);
        
        *respSize = _Adpt_SetSessionResp(buffer);
        
        resCode = DCM_E_POSITIVERESPONSE;
    }
    
    return resCode;
}
#endif

/**************************************************************************//**
 *
 *  \details Callback of reset.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *  gs_Dcm11SubId 數組中
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback1101(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_u8_t i;
    bl_u8_t l_buf[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    (void)respSize;
    (void)buffer;
    (void)size;
    
    if (Dcm_IsSuspendedResponse() != BL_ERR_OK) { //gs_SupPosRespFlag==0
        for (i = 0; i < 8; i++) {
            l_buf[i] = gs_ResetResponse[i];
        }
        for (i = 8; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
            l_buf[i] = CANTP_FRAME_PADDING_VALUE;
        }
        _Adpt_SendResponse((bl_BufferSize_t)sizeof(l_buf), l_buf);
    }
    
    g_RteInfo.SystemReset();
    /*never return*/
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of reset.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *  gs_Dcm11SubId 數組中
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback1103(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_u8_t i;
    bl_u8_t l_buf[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    (void)respSize;
    (void)buffer;
    (void)size;
    
    if (Dcm_IsSuspendedResponse() != BL_ERR_OK) { //gs_SupPosRespFlag==0
        for (i = 0; i < 8; i++) {
            l_buf[i] = gs_ResetResponse_sub03[i];
        }
        for (i = 8; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
            l_buf[i] = CANTP_FRAME_PADDING_VALUE;
        }
        _Adpt_SendResponse((bl_BufferSize_t)sizeof(l_buf), l_buf);
    }
    
    g_RteInfo.SystemReset();
    /*never return*/
    return resCode;
}

/*  22 F198 */
/**************************************************************************//**
 *
 *  \details Callback of reading fingerprint.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallBackReadFingerprint(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    //bl_Return_t ret = BL_ERR_OK;
    (void)size;
    /*read fingerprint.*/
    #ifdef BL_EEPROM_EN
    if (0 == IntEeprom_ReadBytes(EEPROM_START_ADDRESS, buffer, RTE_FINGERPRINT_SIZE))
    #else
    if (0)
    #endif
    {
        resCode = DCM_E_CONDITIONSNOTCORRECT;
    } else {
        *respSize = RTE_FINGERPRINT_SIZE;
    }
    return resCode;
}

/*  22 F187 */
bl_ResponseCode_t Adpt_UdsCallBackReadPartNumber(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    bl_u8_t *port_number;
    bl_u8_t i;
    
    port_number = (bl_u8_t *)&Manufacturer_Spare_Part_Number_F187[0];
    for (i = 0; i < RTE_PART_NUMBER_SIZE; i ++) {
        buffer[i] = port_number[i];
    }
    
    *respSize = RTE_PART_NUMBER_SIZE;
    return resCode;
}

/*   */
void blAdptWriteBootSwId(void) {
    /* 版本升级0->1 */
    bl_u8_t cboot_sw_id[16] = {'G', 'E', 'E', 'L', 'Y', 'F', 'E', '-', '5', 'B', 'T', '0', '0', '1', '0', '2'};
    #ifdef BL_EEPROM_EN
    (void)IntEeprom_WriteBytes(EEPROM_F180_START_ADDRESS, cboot_sw_id, RTE_BOOT_SW_ID_SIZE);
    #endif
}

/*  22 F180 */
bl_ResponseCode_t Adpt_UdsCallBackReadBootSW_ID(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    bl_Buffer_t *data;
    data = (bl_Buffer_t *)0x0001BC90;
    if (0xff == data[0]) { //擦除app，可从boot中读取boot版本号
        data = (bl_Buffer_t *)0x0000BFD0;
    }
    for (i = 0; i < RTE_BOOT_SW_ID_SIZE; i ++) {
        buffer[i] = data[i];
    }
    
    *respSize = RTE_BOOT_SW_ID_SIZE;
    
    return resCode;
}

/*  22 F18A */
bl_ResponseCode_t Adpt_UdsCallBackReadSupplierCode(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_f18a;
    
    data_f18a = (uint8_t *)0x0001BC10;
    for (i = 0; i < RTE_SUPPLIER_CODE_SIZE; i ++) {
        buffer[i] = data_f18a[i];
    }
    
    
    *respSize = RTE_SUPPLIER_CODE_SIZE;
    
    return resCode;
}

/*  22 F193 */
bl_ResponseCode_t Adpt_UdsCallBackReadECU_HardwareVersionNumber(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_f191;
    
    data_f191 = (uint8_t *)0x0001BC20;
    for (i = 0; i < RTE_ECU_HW_NUMBER_SIZE; i ++) {
        buffer[i] = data_f191[i];
    }
    
    
    *respSize = RTE_ECU_HW_NUMBER_SIZE;
    
    return resCode;
}


/*  22 F195 */
bl_ResponseCode_t Adpt_UdsCallBackReadECU_SoftwareVersionNumber(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_f195;
    
    
    data_f195 = (uint8_t *)0x0001BC30;
    for (i = 0; i < RTE_ECU_SW_NUMBER_SIZE; i ++) {
        buffer[i] = data_f195[i];
    }
    
    /*
     buffer[0]=0x75;
     buffer[1]=0x39;
     buffer[2]=0x17;
     buffer[3]=0x0b;
     buffer[4]=0x0a;
     buffer[5]=0x00;
    */
    *respSize = RTE_ECU_SW_NUMBER_SIZE;
    
    return resCode;
}

/*  22 F198 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Tester_Serial_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint32_t data_F198;
    
    for (i = 0; i < TESTER_SERIAL_NUMBER_EE_FLASH_SIZE; i ++) {
        if (EE_ReadVariable(TESTER_SERIAL_NUMBER_EE_FLASH_ADDRESS_0 + i, &data_F198) == 1) {
            buffer[i * 4] = 0xffu;
            buffer[i * 4 + 1] = 0xffu;
            buffer[i * 4 + 2] = 0xffu;
            buffer[i * 4 + 3] = 0xffu;
        } else {
            buffer[i * 4] = (uint8_t)data_F198;
            buffer[i * 4 + 1] = (uint8_t)(data_F198 >> 8);
            buffer[i * 4 + 2] = (uint8_t)(data_F198 >> 16);
            buffer[i * 4 + 3] = (uint8_t)(data_F198 >> 24);
        }
    }
    *respSize = RTE_TESTER_SERIAL_NUMBER_SIZE;
    return resCode;
}

/*  22 F194 */
bl_ResponseCode_t Adpt_UdsCallBackReadSoftCode(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    bl_u8_t i;
    bl_u8_t *data_f194;
    data_f194 = (bl_u8_t *)&Manufacturer_Spare_Part_Number_F187[0];
    for (i = 0; i < RTE_PARTS_SOFT_CODE_SIZE; i ++) {
        buffer[i] = data_f194[i];
    }
    
    *respSize = RTE_PARTS_SOFT_CODE_SIZE;
    
    return resCode;
}

/*  22 F186 */
bl_ResponseCode_t Adpt_UdsCallBackReadActiveSession(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    (void)size;
    buffer[0] = 0x0;
    ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SESSION_DEFAULT);
    if (BL_ERR_OK == ret) {
        buffer[0] = 0x01;
    } else {
        ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SESSION_PROGRAMMING);
        if (BL_ERR_OK == ret) {
            buffer[0] = 0x02;
        } else {
            ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SESSION_EXTENDED);
            if (BL_ERR_OK == ret) {
                buffer[0] = 0x03;
            }
        }
    }
    *respSize = 1;
    return resCode;
}

/*  22 F195 */
bl_ResponseCode_t Adpt_UdsCallBackReadEcuSoftVersion(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    #ifdef BL_EEPROM_EN
    if (0 == IntEeprom_ReadBytes(EEPROM_F180_START_ADDRESS + 0x1AU, buffer, RTE_PGM_DATE_SIZE))
    #else
    if (0)
    #endif
    {
        resCode = DCM_E_CONDITIONSNOTCORRECT;
    } else {
        #if 0
        if ((buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0 && buffer[3] == 0 &&
                buffer[4] == 0 && buffer[5] == 0 && buffer[6] == 0 && buffer[7] == 0 &&
                buffer[8] == 0 && buffer[9] == 0 && buffer[10] == 0 && buffer[11] == 0 &&
                buffer[12] == 0 && buffer[13] == 0 && buffer[14] == 0 && buffer[15] == 0) || (
                
                buffer[0] == 0xff && buffer[1] == 0xff && buffer[2] == 0xff && buffer[3] == 0xff &&
                buffer[4] == 0xff && buffer[5] == 0xff && buffer[6] == 0xff && buffer[7] == 0xff &&
                buffer[8] == 0xff && buffer[9] == 0xff && buffer[10] == 0xff && buffer[11] == 0xff &&
                buffer[12] == 0xff && buffer[13] == 0xff && buffer[14] == 0xff && buffer[15] == 0xff)) {
                
            buffer[0] = buffer[1] = buffer[2] = buffer[3] = 0x20;
            buffer[4] = buffer[5] = buffer[6] = buffer[7] = 0x20;
            buffer[8] = buffer[9] = buffer[10] = buffer[11] = 0x20;
            buffer[12] = buffer[13] = buffer[14] = buffer[15] = 0x20;
        }
        #endif
        *respSize = 16;
    }
    return resCode;
}

/*  22 F199 */
bl_ResponseCode_t Adpt_UdsCallBackReadProgrammingDate(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    #ifdef BL_EEPROM_EN
    if (0 == IntEeprom_ReadBytes(EEPROM_START_ADDRESS + 0x14, buffer, RTE_PGM_DATE_SIZE))
    #else
    uint8_t i;
    uint32_t data_F199;
    
    for (i = 0; i < Programming_Date_EE_FLASH_SIZE; i ++) {
        if (EE_ReadVariable(Programming_Date_EE_FLASH_ADDRESS_0 + i, &data_F199) == 1) {
            buffer[i * 4] = 0xffu;
            buffer[i * 4 + 1] = 0xffu;
            buffer[i * 4 + 2] = 0xffu;
            buffer[i * 4 + 3] = 0xffu;
        } else {
            buffer[i * 4] = (uint8_t)data_F199;
            buffer[i * 4 + 1] = (uint8_t)(data_F199 >> 8);
            buffer[i * 4 + 2] = (uint8_t)(data_F199 >> 16);
            buffer[i * 4 + 3] = (uint8_t)(data_F199 >> 24);
        }
    }
    #endif
        *respSize = 4;
    return resCode;
}

/*  22 0200 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Programming_Counter(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint32_t data_0200;
    for (i = 0; i < Programming_Counter_EE_FLASH_SIZE; i ++) { //4字节数据长度
        if (EE_ReadVariable(Programming_Counter_EE_FLASH_ADDRESS_0 + i, &data_0200) == 1) {
            buffer[i * 4] = 0xffu;
            buffer[i * 4 + 1] = 0xffu;
            buffer[i * 4 + 2] = 0xffu;
            buffer[i * 4 + 3] = 0xffu;
        } else {
            buffer[i * 4] = (uint8_t)(data_0200 >> 24);
            buffer[i * 4 + 1] = (uint8_t)(data_0200 >> 16);
        }
    }
    *respSize = 2;
    
    return resCode;
}

/*  22 0201 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Programming_Attempts_Counter(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint32_t data_0201;
    for (i = 0; i < Programming_Counter_EE_FLASH_SIZE; i ++) { //4字节数据长度
        if (EE_ReadVariable(Programming_Counter_EE_FLASH_ADDRESS_0 + i, &data_0201) == 1) {
            buffer[i * 4] = 0xffu;
            buffer[i * 4 + 1] = 0xffu;
            buffer[i * 4 + 2] = 0xffu;
            buffer[i * 4 + 3] = 0xffu;
        } else {
            buffer[i * 4] = (uint8_t)data_0201;
        }
    }
    *respSize = 1;
    
    return resCode;
}

/*  22 FF00 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Diagnosis_Specification_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_FF00;
    
    data_FF00 = (uint8_t *)0x0001BCA0;
    for (i = 0; i < RTE_Diagnosis_Specification_Version_SIZE; i ++) {
        buffer[i] = data_FF00[i];
    }
    *respSize = RTE_Diagnosis_Specification_Version_SIZE;
    return resCode;
}

/*  22 F188 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Manufacturer_ECU_Software_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_F188;
    
    data_F188 = (uint8_t *)&Manufacturer_ECU_Software_Number_F188[0];
    for (i = 0; i < RTE_Manufacturer_ECU_Software_Number_SIZE; i ++) {
        buffer[i] = data_F188[i];
    }
    *respSize = RTE_Manufacturer_ECU_Software_Number_SIZE;
    return resCode;
}

/*  22 F189 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Manufacturer_ECU_Software_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_F189;
    
    data_F189 = (uint8_t *)0x0001BC70;
    if (0xff == data_F189[0]) { //擦除app，可从boot中读取boot版本号
        data_F189 = (bl_Buffer_t *)0x0000BF60;
    }
    for (i = 0; i < RTE_Manufacturer_ECU_Software_Version_SIZE; i ++) {
        buffer[i] = data_F189[i];
    }
    *respSize = RTE_Manufacturer_ECU_Software_Version_SIZE;
    return resCode;
}

/*  22 F191 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Manufacturer_ECU_Hardware_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_F191;
    
    data_F191 = (uint8_t *)&Manufacturer_ECU_Hardware_Number_F191[0];
    for (i = 0; i < RTE_Manufacturer_ECU_Hardware_Number_SIZE; i ++) {
        buffer[i] = data_F191[i];
    }
    *respSize = RTE_Manufacturer_ECU_Hardware_Number_SIZE;
    return resCode;
}

/*  22 F150 */
bl_ResponseCode_t Adpt_UdsCallBackRead_System_Supplier_ECU_Hardware_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_F150;
    
    data_F150 = (uint8_t *)0x0000BF40;
    for (i = 0; i < RTE_System_Supplier_ECU_Hardware_Version_SIZE; i ++) {
        buffer[i] = data_F150[i];
    }
    *respSize = RTE_System_Supplier_ECU_Hardware_Version_SIZE;
    return resCode;
}

/*  22 F182 */
bl_ResponseCode_t Adpt_UdsCallBackRead_Calibration_Software_Version(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_F182;
    
    data_F182 = (uint8_t *)0x0001BCB0;
    for (i = 0; i < RTE_System_Supplier_ECU_Hardware_Version_SIZE; i ++) {
        buffer[i] = data_F182[i];
    }
    *respSize = RTE_System_Supplier_ECU_Hardware_Version_SIZE;
    return resCode;
}

/*  22 F197 */
bl_ResponseCode_t Adpt_UdsCallBackRead_System_Name(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    uint8_t i;
    uint8_t *data_F197;
    
    data_F197 = (uint8_t *)0x0000BF50;
    for (i = 0; i < RTE_System_Name_SIZE; i ++) {
        buffer[i] = data_F197[i];
    }
    *respSize = RTE_System_Name_SIZE;
    return resCode;
}

/*  22 F1ED */
bl_ResponseCode_t Adpt_UdsCallBackRead_Secure_Flash(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    
    buffer[0] = 0x01; //Boot程序中
    *respSize = RTE_Secure_Flash_SIZE;
    return resCode;
}

/*  22 F1EF */
bl_ResponseCode_t Adpt_UdsCallBackRead_BootApp_Identify(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    (void)size;
    
    buffer[0] = 0x01; //Boot程序中
    *respSize = RTE_BootApp_Identify_SIZE;
    return resCode;
}

bl_u8_t g_cur_request_level = 0; //當前請求的等級

/**************************************************************************//**
 *
 *  \details Callback of getting seed.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallbackGetSeedForLevel1(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    #if(ADPT_FUN_DELAY_WITH_CONTINUOUS_SEED == BL_FUN_ON)
    bl_u16_t remainCount = 0;
    #endif
    
    (void)size;
    
    do {
        /*judge wheter security access is available.*/
        ret = Adpt_IsAccessible(&gs_UdsPrivateData.delay);
        if (ret != BL_ERR_OK) {
            resCode = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
            break;
        }
        
        /*请求的安全访问已解锁, 种子值设为全0*/
        ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SECURITY_UNLOCK_1);
        if (BL_ERR_OK == ret) {
            Bl_MemSet(buffer, 0, (bl_Size_t)SECM_ACCESS_SEED_SIZE);
            break;
        }
        
        /*种子已请求，正在等待密钥，*/
        ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SECURITY_WAITKEY_1);
        if (BL_ERR_OK == ret) {
            #if(ADPT_FUN_DELAY_WITH_CONTINUOUS_SEED == BL_FUN_ON)
            /*if always request seed, decrease attemptcount.*/
            remainCount = Adpt_DecreaseAttemptCount(&gs_UdsPrivateData.delay);
            if (remainCount == 0x03u) {
                #if(ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
                /*set security access delay flag.*/
                (void)Dm_WriteDefaultData(gs_UdsPrivateData.delay.delayFlagId);
                #endif
                
                Rte_SetMutexDownStatus(&g_DownContext,
                    DCM_STATUS_SECURITY_LOCK_1,
                    DCM_SECURITYACCESS_1_MASK);
                    
                // resCode = DCM_E_EXCEEDNUMBEROFATTEMPTS; //期望种子相同时，则前四次请求种子,ECU肯定响应，第5次请求种子ECU响应NR37（NRC36是密钥错误，与种子无关）
                break;
            }
            #endif
            /*return the last seed.*/
            Secm_GetSeed(buffer);
            
            break;
        }
        
        /*request a new seed.*/
        Secm_MakeNewSeed();
        /*Copy the seed to buffer*/
        Secm_GetSeed(buffer);
        /*set wait key status.*/
        Rte_SetDownStatus(&g_DownContext, DCM_STATUS_SECURITY_WAITKEY_1);
        
        g_cur_request_level = DCM_SA_SEED_REQ; //level 實際值
        
    } while (0); /*lint !e717*/
    
    *respSize = SECM_ACCESS_SEED_SIZE;
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of verifying key.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
/*lint -e{818}*/
bl_ResponseCode_t Adpt_UdsCallbackVerifyKeyForLevel1(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_u16_t remainCount;
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    // bl_u32_t calKey;
    // bl_u32_t tranKey;
    bl_u32_t attemptcount_ee_buff;
    
    do {
        ret = Rte_IsValidStatus(&g_DownContext,	DCM_STATUS_SECURITY_WAITKEY_1);
        if (ret != BL_ERR_OK) {
            resCode = DCM_E_REQUESTSEQUENCEERROR;
            break;
        }
        
        /*verify key.*/
        ret = Secm_VerifyKey((bl_Size_t)size, buffer); //A12安全算法 0xEF877C70
        // ret = BL_ERR_OK; //调试先通过
        /*
          calKey = _Acc_UdsDemoCalculateKey();   //這是相當於本地算出來的
          tranKey = BL_BE32_TO_MCU(buffer);  // tranKey = *key  --這是相當於 診斷上收回來的數據；BL_BE32_TO_MCU
          if(calKey==tranKey)  ret=BL_ERR_OK;
          else                 ret=BL_ERR_NOT_OK;
        */
        if (ret != BL_ERR_OK) {
            remainCount = Adpt_DecreaseAttemptCount(&gs_UdsPrivateData.delay);
            if (0x03u == remainCount) {
                #if(ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
                /*set security access delay flag.*/
                (void)Dm_WriteDefaultData(gs_UdsPrivateData.delay.delayFlagId);
                #endif
                resCode = DCM_E_EXCEEDNUMBEROFATTEMPTS;
            } else {
                resCode = DCM_E_INVALIDKEY;
            }
            Rte_SetMutexDownStatus(&g_DownContext,
                DCM_STATUS_SECURITY_LOCK_1,
                DCM_SECURITYACCESS_1_MASK);
            break;
        }
        gs_UdsPrivateData.delay.attemptCount = 0;
        attemptcount_ee_buff = (attemptcount_ee_buff & 0xFFFF0000) | gs_UdsPrivateData.delay.attemptCount;
        EE_WriteVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0, attemptcount_ee_buff);
        *respSize = 0x00u;
        Rte_SetMutexDownStatus(&g_DownContext,
            DCM_STATUS_SECURITY_UNLOCK_1,
            DCM_SECURITYACCESS_1_MASK);
    } while (0); /*lint !e717*/
    
    return resCode;
}
/**************************************************************************//**
 *
 *  \details Callback of getting seed.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallbackGetSeedForLevel2(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    #if(ADPT_FUN_DELAY_WITH_CONTINUOUS_SEED == BL_FUN_ON)
    bl_u16_t remainCount = 0;
    #endif
    
    (void)size;
    
    do {
        /*judge wheter security access is available.*/
        ret = Adpt_IsAccessible(&gs_UdsPrivateData.delay);
        if (ret != BL_ERR_OK) {
            resCode = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;
            break;
        }
        
        /*请求的安全访问已解锁, 种子值设为全0*/
        ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SECURITY_UNLOCK_2);
        if (BL_ERR_OK == ret) {
            Bl_MemSet(buffer, 0, (bl_Size_t)SECM_ACCESS_SEED_SIZE);
            break;
        }
        
        /*种子已请求，正在等待密钥，*/
        ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SECURITY_WAITKEY_2);
        if (BL_ERR_OK == ret) {
            #if(ADPT_FUN_DELAY_WITH_CONTINUOUS_SEED == BL_FUN_ON)
            /*if always request seed, decrease attemptcount.*/
            remainCount = Adpt_DecreaseAttemptCount(&gs_UdsPrivateData.delay);
            if (remainCount == 0x03u) {
                #if(ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
                /*set security access delay flag.*/
                (void)Dm_WriteDefaultData(gs_UdsPrivateData.delay.delayFlagId);
                #endif
                
                Rte_SetMutexDownStatus(&g_DownContext,
                    DCM_STATUS_SECURITY_LOCK_2,
                    DCM_SECURITYACCESS_2_MASK);
                    
                // resCode = DCM_E_EXCEEDNUMBEROFATTEMPTS; //期望种子相同时，则前四次请求种子,ECU肯定响应，第5次请求种子ECU响应NR37（NRC36是密钥错误，与种子无关）
                break;
            }
            #endif
            /*连续请求种子返回相同的密钥*/
            Secm_GetSeed(buffer);
            
            break;
        }
        
        /*request a new seed.*/
        Secm_MakeNewSeed();
        /*Copy the seed to buffer*/
        Secm_GetSeed(buffer);
        /*set wait key status.*/
        Rte_SetDownStatus(&g_DownContext, DCM_STATUS_SECURITY_WAITKEY_2);
        
        g_cur_request_level = DCM_SA_SEED_2_REQ; //level 實際值
        
    } while (0); /*lint !e717*/
    
    *respSize = SECM_ACCESS_SEED_SIZE;
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of verifying key.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
/*lint -e{818}*/
bl_ResponseCode_t Adpt_UdsCallbackVerifyKeyForLevel2(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_u16_t remainCount;
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    bl_u8_t MagicSeed[4] = {0};
    bl_u32_t raw_seed;
    bl_u32_t attemptcount_ee_buff;
    
    do {
        ret = Rte_IsValidStatus(&g_DownContext, DCM_STATUS_SECURITY_WAITKEY_2);
        if (ret != BL_ERR_OK) {
            resCode = DCM_E_REQUESTSEQUENCEERROR;
            break;
        }
        
        /*verify key.*/
        Acc_UdsDemoGetSeed(MagicSeed);
        raw_seed = MagicSeed[0];
        raw_seed <<= 8;
        raw_seed |= MagicSeed[1];
        raw_seed <<= 8;
        raw_seed |= MagicSeed[2];
        raw_seed <<= 8;
        raw_seed |= MagicSeed[3];
        KEY2712_local = SeedKeyPROGSupplier(raw_seed);
        KEY2712_recv = BL_BE32_TO_MCU(buffer);
        if (KEY2712_local == KEY2712_recv) {
            ret = BL_ERR_OK;
        } else {
            ret = BL_ERR_NOT_OK;
        }
        // ret = Secm_VerifyKey((bl_Size_t)size, buffer); 27 01的密钥处理，在这里不适用
        // ret = BL_ERR_OK; //调试先通过
        if (ret != BL_ERR_OK) {
            remainCount = Adpt_DecreaseAttemptCount(&gs_UdsPrivateData.delay);  //相當於  等 attemptCount  操作次數
            if (0x03u == remainCount) {
                #if(ADPT_FUN_SAVE_DELAY_FLAG == BL_FUN_ON)
                /*set security access delay flag.*/
                (void)Dm_WriteDefaultData(gs_UdsPrivateData.delay.delayFlagId);
                #endif
                resCode = DCM_E_EXCEEDNUMBEROFATTEMPTS;
            } else {
                resCode = DCM_E_INVALIDKEY;
            }
            Rte_SetMutexDownStatus(&g_DownContext,
                DCM_STATUS_SECURITY_LOCK_2,
                DCM_SECURITYACCESS_2_MASK);
            break;
        }
        gs_UdsPrivateData.delay.attemptCount = 0;
        attemptcount_ee_buff = (attemptcount_ee_buff & 0xFFFF0000) | gs_UdsPrivateData.delay.attemptCount;
        EE_WriteVariable(security_access_attemptcount_EE_FLASH_ADDRESS_0, attemptcount_ee_buff);
        *respSize = 0x00u;
        Rte_SetMutexDownStatus(&g_DownContext,
            DCM_STATUS_SECURITY_UNLOCK_2,
            DCM_SECURITYACCESS_2_MASK);
    } while (0); /*lint !e717*/
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of enable or disable communication.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval DCM_E_POSITIVERESPONSE   - Positive response code.
 *  \retval DCM_E_REQUESTOUTOFRANGE - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
/*lint -e{818}*/
bl_ResponseCode_t Adpt_UdsCallback28(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    
    (void)size;
    /*if(ADPT_COMMCONTROLTYPE_DATA != buffer[0])*/
    if ((ADPT_COMMCONTROLTYPE_DATA != buffer[0]) && (0x3 != buffer[0])) { // buffer[0]！=ADPT_COMMCONTROLTYPE_DATA是01 且不等於03
        resCode = DCM_E_REQUESTOUTOFRANGE;  //回復 31
    } else {
        *respSize = 0x00u;
    }
    
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of writting fingerprint.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
/*lint -e{818}*/
bl_ResponseCode_t Adpt_UdsCallBackWriteFingerprint(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_GENERALPROGRAMMINGFAILURE;
    //    bl_Return_t ret;
    (void)size;
    uint32_t data;
    uint8_t i;
    /*write fingerprint data.*/
    #if (LBM_FUN_SAVE_FINGERPRINT == BL_FUN_ON)
    #ifdef BL_EEPROM_EN
    (void)IntEeprom_WriteBytes(EEPROM_START_ADDRESS, buffer, RTE_FINGERPRINT_SIZE);
    #endif
    for (i = 0; i < APPLICATION_SOFTWARE_FINGERPRINT_EE_FLASH_SIZE; i ++) {
        data = buffer[i * 4 + 3];
        data <<= 8;
        data |= buffer[i * 4 + 2];
        data <<= 8;
        data |= buffer[i * 4 + 1];
        data <<= 8;
        data |= buffer[i * 4];
        EE_WriteVariable(APPLICATION_SOFTWARE_FINGERPRINT_BYTE0_3_EE_FLASH_ADDRESS + i, data);
    }
    
    #endif
    Rte_SetDownStatus(&g_DownContext, RTE_SYSFLAG_WRITE_FINGPRINT);
    *respSize = 0u;
    resCode = DCM_E_POSITIVERESPONSE;
    return resCode;
}

bl_ResponseCode_t Adpt_UdsCallBackWrite_Tester_Serial_Number(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_GENERALPROGRAMMINGFAILURE;
    //    bl_Return_t ret;
    (void)size;
    uint32_t data;
    uint8_t i;
    IWDT_CNT_test = 0;
    IWDT_CNT_test1 = 0;
    
    do {
        if (necessary_condition_count != 2) { //6001未通过
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        FL_BSTIM16_GenerateUpdateEvent(BSTIM16);
        /*IWDT_CNT_test验证BSTIM16更新后是否清0*/
        IWDT_CNT_test = FL_BSTIM16_ReadCounter(BSTIM16);
        /*write fingerprint data.*/
        #if (LBM_FUN_SAVE_FINGERPRINT == BL_FUN_ON)
        #ifdef BL_EEPROM_EN
        (void)IntEeprom_WriteBytes(EEPROM_START_ADDRESS, buffer, RTE_TESTER_SERIAL_NUMBER_SIZE);
        #endif
        for (i = 0; i < TESTER_SERIAL_NUMBER_EE_FLASH_SIZE; i ++) {
            data = buffer[i * 4 + 3];
            data <<= 8;
            data |= buffer[i * 4 + 2];
            data <<= 8;
            data |= buffer[i * 4 + 1];
            data <<= 8;
            data |= buffer[i * 4];
            EE_WriteVariable(TESTER_SERIAL_NUMBER_EE_FLASH_ADDRESS_0 + i, data);
        }
        //可以在这里加上一个读EE的判断语句
        /*写EE16字节，13us*/
        IWDT_CNT_test1 = FL_BSTIM16_ReadCounter(BSTIM16);
        #endif
        Rte_SetDownStatus(&g_DownContext, RTE_SYSFLAG_WRITE_FINGPRINT); //ID = 0
        *respSize = 0u;
        resCode = DCM_E_POSITIVERESPONSE;
    } while (0);
    if (resCode == DCM_E_POSITIVERESPONSE) {
        necessary_condition_count = 3; //F198通过
    }
    
    return resCode;
}

bl_ResponseCode_t Adpt_UdsCallBackWriteProgrammingDate(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_GENERALPROGRAMMINGFAILURE;
    //    bl_Return_t ret;
    (void)size;
    uint32_t data;
    uint8_t i;
    IWDT_CNT_test = 0;
    IWDT_CNT_test1 = 0;
    
    do {
        if (necessary_condition_count != 3) { //F198未通过
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        FL_BSTIM16_GenerateUpdateEvent(BSTIM16);
        IWDT_CNT_test = FL_BSTIM16_ReadCounter(BSTIM16);
        /*write PGM date TO RAM  */
        #if (LBM_FUN_SAVE_PROGRAMMING_DATE == BL_FUN_ON)
        #ifdef BL_EEPROM_EN
        (void)IntEeprom_WriteBytes(EEPROM_START_ADDRESS + 0x14, buffer, RTE_PGM_DATE_SIZE);
        //(void)EEPROM_SetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR+EEPROM_START_ADDRESS+0x14), ((bl_u16_t)(buffer[0]<<8))|buffer[0]);
        //(void)EEPROM_SetWord((EEPROM_TAddress)(EEPROM_DATA_BASE_ADDR+EEPROM_START_ADDRESS+0x16), ((bl_u16_t)(buffer[2]<<8))|buffer[3]);
        #endif
        for (i = 0; i < Programming_Date_EE_FLASH_SIZE; i ++) {
            data = buffer[i * 4 + 3];
            data <<= 8;
            data |= buffer[i * 4 + 2];
            data <<= 8;
            data |= buffer[i * 4 + 1];
            data <<= 8;
            data |= buffer[i * 4];
            EE_WriteVariable(Programming_Date_EE_FLASH_ADDRESS_0 + i, data);
        }
        /*写EE16字节，3us*/
        IWDT_CNT_test1 = FL_BSTIM16_ReadCounter(BSTIM16);
        #endif
        Rte_SetDownStatus(&g_DownContext, RTE_SYSFLAG_WRITE_OTHER);
        *respSize = 0u;
        resCode = DCM_E_POSITIVERESPONSE;
    } while (0);
    if (resCode == DCM_E_POSITIVERESPONSE) {
        necessary_condition_count = 4; //F199通过
    }
    return resCode;
}


/**************************************************************************//**
 *
 *  \details Callback of checking programming contiditions.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallbackCheckProgramCond(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    uint8_t sha256_result[32];
    uint8_t i;
    bl_u32_t data;
    (void)size;
    bl_DownContext_t raw_dct = g_DownContext;
    bl_Buffer_t *value;
    ret = BL_ERR_OK;
    do {
        if (necessary_condition_count != 8) { //36下载app未通过
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        gs_UdsPrivateData.serviceId = ADPT_UDS_31_SERVICE_ID;
        _Adpt_TimeOutCallBack(ADPT_UDS_31_SERVICE_ID);
        for (i = 0; i < 8; i++) {
            sha256_result[i * 4 + 0] = (bl_u8_t)((hash_buff[i] & 0xff000000u) >> 24); //高字节
            sha256_result[i * 4 + 1] = (bl_u8_t)((hash_buff[i] & 0x00ff0000u) >> 16);
            sha256_result[i * 4 + 2] = (bl_u8_t)((hash_buff[i] & 0x0000ff00u) >> 8);
            sha256_result[i * 4 + 3] = (bl_u8_t)((hash_buff[i] & 0x000000ffu));       //低字节
        }
        for (i = 0; i < 32; i++) {
            if (sha256_result[i] != recv_data_6000[i]) {
                ret = BL_ERR_USER_CODE_01;
                break;
            }
        }
        if (BL_ERR_OK == ret) {
            raw_dct.segment.segList[0].size -= 4;
            value = (bl_Buffer_t *)(raw_dct.segment.segList[0].address + raw_dct.segment.segList[0].size);
            ret = Adpt_VerifyData(&g_DownContext, (bl_Size_t)4, value);
            if (ret != BL_ERR_OK) {
                ret = BL_ERR_USER_CODE_02;
            }
        }
        //ret = BL_ERR_OK; //240826 客户沟通，暂时先过
        if (BL_ERR_OK == ret) {
            buffer[0] = 0x4; //例程成功
            g_LogicalBlockList.lbList[0].status[0] = (LBM_STATUS_UPDATED);
            //app 烧写成功
            Dm_WriteDefaultData(BOOTM_RESET_FLAG_DID);
            necessary_condition_count = 9; //0203通过
            if (BL_ERR_OK == ret) {
            
            } else {
                buffer[0] = 0x6; //例程失败
            }
        } else {
            if (BL_ERR_USER_CODE_01 == ret) {
                buffer[0] = 0x5;
            } else if (BL_ERR_USER_CODE_02 == ret) {
                buffer[0] = 0x6;
            } else {
            }
            Rte_ClearDownStatus(&g_DownContext, ADPT_UDS_STATUS_MASK);
            for (i = 0; i < Security_flash_failure_EE_FLASH_SIZE; i ++) {
                data = 2;
                EE_WriteVariable(Security_flash_failure_EE_FLASH_ADDRESS_0 + i, data);
            }
        }
        Rte_ClearAddressInfo(&g_DownContext);   //dct->segIndex = 0;
        Rte_ClearDownStatus(&g_DownContext, ADPT_SYSTEM_STATUS_MASK);  // & ~0x0c
        *respSize = 0x01u;
    } while (0);
    
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of erasing memory.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallbackEraseMemory(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_Size_t eraseSize;
    bl_Address_t address;
    bl_ResponseCode_t resCode = DCM_E_CONDITIONSNOTCORRECT;
    bl_Return_t ret;
    bl_u8_t i;
    (void)size;
    //刷写过程中，下载驱动后，跳过3101FF00xx，请求34服务，ECU肯定响应，期望响应NRC22
    do {
        if (necessary_condition_count != 6) { //0202未通过
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        /* -----------------------------------------------------------------------A12
        检查地址和长度格式标识符是否为0x44
        address=0x8100，eraseSize=0x013F00
        ----------------------------------------------------------------------- */
        if ((bl_Buffer_t)ADPT_MEMORY_LENANDADDR_FORMAT == buffer[ADPT_UDS_ERASE_MEMORY_FORMAT_POS]) {
            address = BL_BE32_TO_MCU(&buffer[ADPT_UDS_ERASE_MEMORY_ADDRESS_POS]);
            eraseSize = BL_BE32_TO_MCU(&buffer[ADPT_UDS_ERASE_MEMORY_SIZE_POS]);
            
            gs_UdsPrivateData.serviceId = ADPT_UDS_31_SERVICE_ID;
            _Adpt_TimeOutCallBack(ADPT_UDS_31_SERVICE_ID);       //请求未决定
            /* -----------------------------------------------------------------------A12
            检查地址和长度格式标识符是否为0x44
            ----------------------------------------------------------------------- */
            ret = Adpt_EraseLB(&g_DownContext, address, eraseSize);
            if (BL_ERR_OK == ret) {
                buffer[0] = 0x4; //例程成功
                necessary_condition_count = 7; //FF00通过
            } else if (ret == BL_ERR_USER_CODE_01) {
                resCode = DCM_E_REQUESTOUTOFRANGE; //错误的APP地址，期望ECU响应NRC31；
                break;
            } else {
                //刷写过程中，请求3101FF00xx（错误的APP地址），ECU响应7101FF0005，期望ECU响应NRC31；
                Rte_ClearDownStatus(&g_DownContext, ADPT_UDS_STATUS_MASK);
                // buffer[0] = 0x5; //例程失败
                resCode = DCM_E_REQUESTOUTOFRANGE;
                break;
            }
            *respSize = 0x01u;
            resCode = DCM_E_POSITIVERESPONSE;
        }
    } while (0);
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of checksum.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallbackCheckSum(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    
    do {
        if (necessary_condition_count != 5) { //36下载驱动未通过
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        gs_UdsPrivateData.serviceId = ADPT_UDS_31_SERVICE_ID;
        _Adpt_TimeOutCallBack(ADPT_UDS_31_SERVICE_ID);
        /* -----------------------------------------------------------------------A12
        N60和BYD用的flashdriver都一样的，因为从34 01 和34 02的数据看，所以校验和肯定都一样
        hex文件中，flashdriver的大小为0x5AC
        ----------------------------------------------------------------------- */
        ret = Adpt_VerifyData(&g_DownContext, (bl_Size_t)size, buffer); //这里需要改动，因为接口变了。
        // ret = BL_ERR_NOT_OK;
        if (BL_ERR_OK == ret) {
            buffer[0] = 0x4;  //正确
            necessary_condition_count = 6; //0202通过
        } else {
            //刷写过程中，31010202校验不通过，请求3101FF00xx，ECU响应7101FF0005，期望响应NRC22
            if (BL_ERR_USER_CODE_01 == ret) {
                buffer[0] = 0x5;
            } else if (BL_ERR_USER_CODE_02 == ret) {
                buffer[0] = 0x6;
            } else {
            }
        }
        Rte_ClearDownStatus(&g_DownContext, ADPT_UDS_STATUS_MASK); //g_DownContext.status[1]& 0xC7FF FFFF
        *respSize = 0x01u;
    } while (0);
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of checking compatibility.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallbackCheckCompatibility(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    bl_u8_t i;
    bl_u32_t data;
    (void)size;
    
    do {
        if (necessary_condition_count != 9) { //36下载app未通过
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        ret = Adpt_CheckCompatibility();
        if (BL_ERR_OK == ret) {
            buffer[0] = 0x4;
            necessary_condition_count = 0; //FF01通过
            //每次刷写成功后都将22F1ED数据清0
            for (i = 0; i < Security_flash_failure_EE_FLASH_SIZE; i ++) {
                data = 0;
                EE_WriteVariable(Security_flash_failure_EE_FLASH_ADDRESS_0 + i, data);
            }
        } else {
            buffer[0] = 0x5;
        }
        _Adpt_UdsClearDownInfo(&gs_UdsPrivateData);
        *respSize = 0x01u;
    } while (0);
    return resCode;
}

bl_u32_t buff_sha256[8];
void decrypt_public_key(const bl_u8_t *encrypted_key, bl_u8_t *decrypted_key) {
    for (int i = 0; i < 256; i++) {
        decrypted_key[i] = encrypted_key[i] ^ 0x9A;
    }
}
//6000 校验文件合法性
bl_ResponseCode_t Adpt_UdsCallbackCheckLegitimacy(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_u16_t ret;
    bl_u16_t i;
    bl_u32_t data;
    bl_u16_t len;
    bl_u8_t u8_buff_sha256[32];
    bl_u8_t ca_pub_key_decrypted[256];
    bl_u32_t ca_pub_key_decrypted_sha256[8];
    (void)size;
    IWDT_CNT_test = 0;
    IWDT_CNT_test1 = 0;
    ret = 0;
    gs_UdsPrivateData.serviceId = ADPT_UDS_31_SERVICE_ID;
    _Adpt_TimeOutCallBack(ADPT_UDS_31_SERVICE_ID);
    for (i = 0; i < 1322; i++) {
        recv_data_6000[i] = buffer[i];
    }
    /* SM2 with SM3 certificate */
    len = 1256;
    //FL_IWDT_ReloadCounter(IWDT);
    // FL_BSTIM16_Enable(BSTIM16);         /* 使能定时器 */
    FL_BSTIM16_GenerateUpdateEvent(BSTIM16);
    
    /* 证书解析时间，400us */
    x509_crt_get_id_pubkey((const bl_u8_t *)recv_data_6000 + crt_start_addr, len); //这里的key就只有128字节
    
    for (i = 0; i < 8; i++) {
        buff_sha256[i] = init_hash[i];
    }
    all_sha256_data_length = crt_tbs_len; //crt_tbs_len 在上一步证书解析时更新
    /* 计算证书主体的摘要（哈希值） */
    SHA256_preporcess((const bl_u8_t *)recv_data_6000 + crt_start_addr + crt_tbs_addr_offset, crt_tbs_len, all_sha256_data_length, &buff_sha256[0]);
    for (i = 0; i < 8; i++) { //32位格式哈希转为8位格式，用以后续的验签
        u8_buff_sha256[i * 4 + 0] = (bl_u8_t)((buff_sha256[i] & 0xff000000u) >> 24); //高字节
        u8_buff_sha256[i * 4 + 1] = (bl_u8_t)((buff_sha256[i] & 0x00ff0000u) >> 16);
        u8_buff_sha256[i * 4 + 2] = (bl_u8_t)((buff_sha256[i] & 0x0000ff00u) >> 8);
        u8_buff_sha256[i * 4 + 3] = (bl_u8_t)((buff_sha256[i] & 0x000000ffu));       //低字节
    }
    /*如果要看执行时间，需要将TimerBase_InitStruct.prescaler = 4799，以100us单位*/
    IWDT_CNT_test = FL_BSTIM16_ReadCounter(BSTIM16);
    /* 判断是否是自签证书 */
    for (i = 0; i < crt_issuer_len; i++) { //crt_issuer_len可以为0
        if ((recv_data_6000 + crt_start_addr + crt_issuer_addr_offset)[i] == issuer_TK[i]) {
            ret++;
        } else {
            ret = 0;
            break;
        }
    }
    if (ret != crt_issuer_len) {
        /* 非易失存储器中的CA公钥，验签， 401.8ms */
        decrypt_public_key(&ca_public_key_encrypted[0], ca_pub_key_decrypted);
        for (i = 0; i < 8; i++) {
            ca_pub_key_decrypted_sha256[i] = init_hash[i];
        }
        SHA256_preporcess((const bl_u8_t *)ca_pub_key_decrypted, 256, 256, &ca_pub_key_decrypted_sha256[0]);
        for (i = 0; i < 8; i++) {
            if (ca_pub_key_decrypted_sha256[i] != ca_pub_key_sha256[i]) {
                ret = 1;
                break;
            }
        }
        ret = mbedtls_rsa_self_test((const bl_u8_t *)ca_pub_key_decrypted, \
                (const bl_u8_t *)recv_data_6000 + crt_start_addr + pkey_E_addr_offset, \
                (const bl_u8_t *)recv_data_6000 + crt_start_addr + pkey_sig_addr_offset, \
                (const bl_u8_t *)&u8_buff_sha256[0]);
    } else {
        /* TK自签证书公钥验签， 401.8ms */
        ret = mbedtls_rsa_self_test((const bl_u8_t *)recv_data_6000 + crt_start_addr + pkey_N_addr_offset, \
                (const bl_u8_t *)recv_data_6000 + crt_start_addr + pkey_E_addr_offset, \
                (const bl_u8_t *)recv_data_6000 + crt_start_addr + pkey_sig_addr_offset, \
                (const bl_u8_t *)&u8_buff_sha256[0]);
    }
    if ((ret) != 0) {
        ret = 1; //ret = 1定义为验签证书校验失败
        goto cleanup;
    }
    
    
    /* “模块数据摘要值”“模块数据验签证书”“版本信息”的摘要（哈希值） */
    for (i = 0; i < 8; i++) {
        buff_sha256[i] = init_hash[i];
    }
    all_sha256_data_length = 32 + 1024 + 10;
    SHA256_preporcess((const bl_u8_t *)recv_data_6000, 1066, all_sha256_data_length, &buff_sha256[0]);
    for (i = 0; i < 8; i++) {
        u8_buff_sha256[i * 4 + 0] = (bl_u8_t)((buff_sha256[i] & 0xff000000u) >> 24); //高字节
        u8_buff_sha256[i * 4 + 1] = (bl_u8_t)((buff_sha256[i] & 0x00ff0000u) >> 16);
        u8_buff_sha256[i * 4 + 2] = (bl_u8_t)((buff_sha256[i] & 0x0000ff00u) >> 8);
        u8_buff_sha256[i * 4 + 3] = (bl_u8_t)((buff_sha256[i] & 0x000000ffu));       //低字节
    }
    /* 证书中的公钥验签，401.8ms */
    ret = mbedtls_rsa_self_test((const bl_u8_t *)recv_data_6000 + crt_start_addr + pkey_N_addr_offset, \
            (const bl_u8_t *)rsa_E_hex, \
            (const bl_u8_t *)recv_data_6000 + all_sha256_data_length, \
            (const bl_u8_t *)&u8_buff_sha256[0]);
    if ((ret) != 0) {
        ret = 2; //ret = 2信息签名值校验失败
        goto cleanup;
    }
    /* 完成校验总时间813.5 ms */
    /*如果要看执行时间，需要将TimerBase_InitStruct.prescaler = 4799，以100us单位*/
    IWDT_CNT_test1 = FL_BSTIM16_ReadCounter(BSTIM16);
    
cleanup:
    if (0 == ret) {
        buffer[0] = 0x4;
        necessary_condition_count = 1; //6000校验成功后，necessary_condition_count=1
    } else {
        if (1 == ret) {
            buffer[0] = 0x5;
        } else if (2 == ret) {
            buffer[0] = 0x6;
        } else {
        }
        for (i = 0; i < Security_flash_failure_EE_FLASH_SIZE; i ++) {
            data = 1;
            EE_WriteVariable(Security_flash_failure_EE_FLASH_ADDRESS_0 + i, data);
        }
    }
    
    *respSize = 0x01u;
    return resCode;
}

//6001 版本校验
bl_ResponseCode_t Adpt_UdsCallbackCheckVersion(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    bl_u8_t i;
    bl_u8_t *data_F189;
    
    (void)size;
    // ret = BL_ERR_NOT_OK;
    do {
        if (necessary_condition_count != 1) { //若经过6000，necessary_condition_count=1
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        
        data_F189 = (bl_u8_t *)0x0001BC70;
        if (0xff == data_F189[0]) {
            data_F189 = (bl_Buffer_t *)0x0000BF60;
        }
        for (i = 0; i < RTE_Manufacturer_ECU_Software_Version_SIZE; i++) {
            if (recv_data_6000[version_start_addr + i] < data_F189[i]) {
                ret = BL_ERR_NOT_OK;
                break;
            } else if (recv_data_6000[version_start_addr + i] > data_F189[i]) { //32条修改
                ret = BL_ERR_OK;
                break;
            } else {
            }
        }
        if (i == RTE_Manufacturer_ECU_Software_Version_SIZE) {
            ret = BL_ERR_OK;
        }
        
        //ret = BL_ERR_OK; //240826 客户沟通，暂时先过
        if (BL_ERR_OK == ret) {
            buffer[0] = 0x4;
            necessary_condition_count = 2; //6001通过
        } else {
            buffer[0] = 0x5;
        }
        *respSize = 0x01u; //如果是否定响应，不需要返回相应长度
    } while (0);
    return resCode;
}

#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details Callback of stayinboot.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallbackStayInBoot(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    
    (void)buffer;
    (void)size;
    
    *respSize = 0x00u;
    
    return resCode;
}
#endif

/**************************************************************************//**
 *
 *  \details Callback of requesting download.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback34(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_Size_t reqSize;
    bl_Address_t addr;
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    bl_u8_t i;
    (void)size;
    tFlashOptInfo *l_pstFlashOptInfo = (void *)0;
    l_pstFlashOptInfo = (tFlashOptInfo *)flashDriverStartAdd;
    do {
        if (g_DownContext.lbId == 0) {
        
            if ((l_pstFlashOptInfo->magic != FLASH_DRIVER_VERSION_MAGIC) || (l_pstFlashOptInfo->version != FLASH_DRIVER_VERSION_VERSION)) {
                resCode = DCM_E_REQUESTOUTOFRANGE;
                break;
            }
        }
        ret = Rte_IsValidStatus(&g_DownContext, ADPT_STATUS_ALLOW_TRANS_DATA);
        if (BL_ERR_OK == ret) {
            resCode = DCM_E_REQUESTSEQUENCEERROR;
            break;
        }
        /* -----------------------------------------------------------------------A12
        发送过来的数据中buffer[0]（数据格式标识符）是否为为0x00，buffer[1]是否为0x44
        ----------------------------------------------------------------------- */
        if ((ADPT_DATAFORMAT_IDEN != buffer[ADPT_UDS_34_SERVICE_IDEN_POS])
            || (ADPT_MEMORY_LENANDADDR_FORMAT
                != buffer[ADPT_UDS_34_SERVICE_ADDRANDSIZE_POS])) {
            resCode = DCM_E_CONDITIONSNOTCORRECT;
            break;
        }
        /* -----------------------------------------------------------------------A12
        下载falshdriver时，addr=0x20007000; reqSize=0x05AC
        ----------------------------------------------------------------------- */
        addr = BL_BE32_TO_MCU(&buffer[ADPT_UDS_34_SERVICE_ADDRESS_POS]);
        reqSize = BL_BE32_TO_MCU(&buffer[ADPT_UDS_34_SERVICE_SIZE_POS]);  /*PRQA S 1840*/
        ret = Adpt_ActivateDownload(&g_DownContext, addr, reqSize); //g_DownContext.ID会更新
        //刷写过程中，请求34服务下载错误的驱动地址，，ECU响应NRC10，期望响应NRC31
        if (ret != BL_ERR_OK) {
            resCode = DCM_E_REQUESTOUTOFRANGE;
            break;
        }
        
        if (g_DownContext.lbId == 0) { //0代表app
            //刷写过程中，下载驱动后，跳过3101FF00xx，请求34服务，ECU肯定响应，期望响应NRC22
            ret = Rte_IsValidStatus(&g_DownContext, RTE_SYSFLAG_ERASE_BLOCK); //app区域内存已经擦除了，条件成立
            if (ret != BL_ERR_OK) {
                resCode = DCM_E_CONDITIONSNOTCORRECT;
                break;
            }
            all_sha256_data_length = 0;
            all_sha256_data_length += sha256_block1_length;
            all_sha256_data_length += reqSize;
        } else if (g_DownContext.lbId == 1) { //1代表flash driver
            if (necessary_condition_count != 4) { //F199未通过 FLASH DRIVER 下载请求条件不通过
                resCode = DCM_E_CONDITIONSNOTCORRECT;
                break;
            }
            all_sha256_data_length = 0xFFFFFFFF;
            sha256_block1_length = 0;
            sha256_block1_length += reqSize;
            for (i = 0; i < 8; i++) {
                hash_buff[i] = init_hash[i];
            }
        } else {
        }
        
        buffer[ADPT_UDS_34_RESPONSE_FORMAT_POS] = ADPT_LENFORMAT_IDEN; //响应的buffer[0]数据设置为0x20
        
        #if(1u == ADPT_IDEN_LEN_FORMAT)
        /*get maxnum of block length.*/
        buffer[ADPT_UDS_34_RESPONSE_SIZE_POS] = ADPT_MAXNUM_OF_BLOCKLENGTH;
        #elif (2u == ADPT_IDEN_LEN_FORMAT)
        BL_MCU_TO_BE16(&buffer[ADPT_UDS_34_RESPONSE_SIZE_POS], ADPT_MAXNUM_OF_BLOCKLENGTH); //响应的buffer[1]和[2]数据设置为0x0402
        #else
#error "ADPT_IDEN_LEN_FORMAT is error."
        #endif
        
        gs_UdsPrivateData.address = addr;
        gs_UdsPrivateData.remainSize = reqSize;
        gs_UdsPrivateData.blockIndex = 0x01u;
        
        *respSize = ADPT_REQDOWNLOAD_RESPLENGTH; //*respSize = 0x3
        
        Rte_SetDownStatus(&g_DownContext, ADPT_STATUS_ALLOW_TRANS_DATA);
    } while (0); /*lint !e717*/
    
    if (resCode != DCM_E_POSITIVERESPONSE) {
        _Adpt_UdsClearDownInfo(&gs_UdsPrivateData);
    }
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of transferring data.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
/*lint -e{818}*/
#if 1 //未实现36服务请求长度小于反馈长度
bl_ResponseCode_t Adpt_UdsCallback36(bl_BufferSize_t size,
    bl_Buffer_t *buffer,	//這裡的數據，己經去掉了sid 或者 sudid did等
    bl_BufferSize_t *respSize) {
    bl_u8_t expendIndex;
    bl_Size_t dataSize;
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    
    /* -----------------------------------------------------------------------A12
    size已经去掉了SID,SUBID或DID等，dataSize去掉了SN
    0x401 - 0x1
    0x1AD - 0x1
    ----------------------------------------------------------------------- */
    dataSize = (bl_Size_t)((bl_Size_t)size - 1u); /*overflow checking is not required.*/
    
    do {
        /* -----------------------------------------------------------------------A12
        在34服务中已经设置 g_DownContext.status[1]为0x20000000UL
        ----------------------------------------------------------------------- */
        ret = Rte_IsValidStatus(&g_DownContext, ADPT_STATUS_ALLOW_TRANS_DATA);
        if (ret != BL_ERR_OK) {
            resCode = DCM_E_REQUESTSEQUENCEERROR;
            break;
        }
        
        expendIndex = gs_UdsPrivateData.blockIndex; //在34服务中已经设置gs_UdsPrivateData.blockIndex=0x1 第二个数据块在前面的基础上加1
        
        ret = Rte_IsValidStatus(&g_DownContext, ADPT_STATUS_SEGMENT_FIRST_DATA);
        if (BL_ERR_NOT_OK == ret) { //
            if (buffer[ADPT_UDS_36_SERVICE_INDEX_POS] != expendIndex) { //
                resCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;  //顺序
                break;
            }
            /*set first 0x36 service of a download segment.*/
            Rte_SetDownStatus(&g_DownContext,
                ADPT_STATUS_SEGMENT_FIRST_DATA); //status[1]，首次进入36服务时，期望的数据块序列编号和收到的数据一致
        }
        
        #if(ADPT_FUN_FIRST_DATA_PREPROCESS == BL_FUN_ON)
        ret = Rte_IsValidStatus(&g_DownContext, ADPT_STATUS_BLOCK_FIRST_DATA);
        if (BL_ERR_NOT_OK == ret) {
        
            /*set first 0x36 service of a logical block.*/
            Rte_SetDownStatus(&g_DownContext, ADPT_STATUS_BLOCK_FIRST_DATA);
            
            App_PreprocessData();
        }
        #endif
        
        /*check whether the SN is valid.*/
        if (buffer[ADPT_UDS_36_SERVICE_INDEX_POS] != expendIndex) {
            if (buffer[ADPT_UDS_36_SERVICE_INDEX_POS] != (expendIndex - 1u)) {
                resCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
            } else {
                /*repeat request*/
                //刷写过程中，进行36传输数据时，重复发送同一个块序号，第二个3601xx，ECU响应76，期望响应NRC73，不允许刷写成功
                resCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
            }
            break;
        }
        
        /*Check the size of received data.*/
        if (gs_UdsPrivateData.remainSize > ADPT_MAXNUM_OF_TRANS_DATA) { //falshdriver remainSize=0x05AC > 0x400
            /*It is not the last 0x36 service.*/
            if (ADPT_MAXNUM_OF_TRANS_DATA != dataSize) {
                resCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
                break;
            }
        } else if (gs_UdsPrivateData.remainSize != dataSize) { //falshdriver的第二部分 remainSize和dataSize都是0x1AC
            resCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            break;
        } else {
            /*last service and data size is ok.*/
        }
        
        /*send 0x78.*/
        gs_UdsPrivateData.serviceId = ADPT_UDS_36_SERVICE_ID;
        _Adpt_TimeOutCallBack(ADPT_UDS_36_SERVICE_ID);
        #ifdef UDS_DEBUG_EN
        g_usd_test.data[0]++;
        #endif
        /* -----------------------------------------------------------------------A12
        
        ----------------------------------------------------------------------- */
        ret = Adpt_DownloadData(&g_DownContext,
                gs_UdsPrivateData.address,
                dataSize,
                &buffer[ADPT_UDS_36_SERVICE_DATA_POS]);
        SHA256_preporcess(&buffer[ADPT_UDS_36_SERVICE_DATA_POS], dataSize, all_sha256_data_length, hash_buff);
        if (BL_ERR_OK != ret) {
            resCode = DCM_E_GENERALPROGRAMMINGFAILURE;
            break;
        } else {
            *respSize = 0x01u;
            gs_UdsPrivateData.blockIndex++;
            gs_UdsPrivateData.address += dataSize;
            gs_UdsPrivateData.remainSize -= dataSize; //0x5AC-0x400
        }
    } while (0); /*lint !e717*/
    
    if (resCode != DCM_E_POSITIVERESPONSE) {
        _Adpt_UdsClearDownInfo(&gs_UdsPrivateData);
    } else {
        if (g_DownContext.lbId == 0) {
            necessary_condition_count = 8; //36下载app通过
        } else if (g_DownContext.lbId == 1) {
            necessary_condition_count = 5; //36下载驱动通过
        }
    }
    return resCode;
}
#endif
#if 0 //36服务任意长度已实现
bl_ResponseCode_t Adpt_UdsCallback36(bl_BufferSize_t size,
    bl_Buffer_t *buffer,	//這裡的數據，己經去掉了sid 或者 sudid did等
    bl_BufferSize_t *respSize) {
    bl_u8_t expendIndex;
    bl_Size_t dataSize;
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    bl_u16_t i;
    /* -----------------------------------------------------------------------A12
    size已经去掉了SID,SUBID或DID等，dataSize去掉了SN
    0x401 - 0x1
    0x1AD - 0x1
    ----------------------------------------------------------------------- */
    dataSize = (bl_Size_t)((bl_Size_t)size - 1u); /*overflow checking is not required.*/
    
    do {
        /* -----------------------------------------------------------------------A12
        在34服务中已经设置 g_DownContext.status[1]为0x20000000UL
        ----------------------------------------------------------------------- */
        ret = Rte_IsValidStatus(&g_DownContext, ADPT_STATUS_ALLOW_TRANS_DATA);
        if (ret != BL_ERR_OK) {
            resCode = DCM_E_REQUESTSEQUENCEERROR;
            break;
        }
        //数据块顺序编号允许诊断 ECU 检测和管理传输错误，首个编号应从 1 开始，每次递增1，当数值达到 255 时，下一次则应从 0 开始。
        expendIndex = gs_UdsPrivateData.blockIndex; //在34服务中已经设置gs_UdsPrivateData.blockIndex=0x1 第二个数据块在前面的基础上加1
        
        ret = Rte_IsValidStatus(&g_DownContext, ADPT_STATUS_SEGMENT_FIRST_DATA);
        if (BL_ERR_NOT_OK == ret) { //
            if (buffer[ADPT_UDS_36_SERVICE_INDEX_POS] != expendIndex) {
                resCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;  //顺序
                break;
            }
            /*set first 0x36 service of a download segment.*/
            Rte_SetDownStatus(&g_DownContext, ADPT_STATUS_SEGMENT_FIRST_DATA); //status[1]新增状态0x08000000UL
        }
        
        #if(ADPT_FUN_FIRST_DATA_PREPROCESS == BL_FUN_ON)
        ret = Rte_IsValidStatus(&g_DownContext, ADPT_STATUS_BLOCK_FIRST_DATA);
        if (BL_ERR_NOT_OK == ret) {
        
            /*set first 0x36 service of a logical block.*/
            Rte_SetDownStatus(&g_DownContext, ADPT_STATUS_BLOCK_FIRST_DATA);
            
            App_PreprocessData();
        }
        #endif
        
        /*check whether the SN is valid.*/
        //刷写过程中，进行36传输数据时，重复发送同一个块序号，第二个3601xx，ECU响应76，期望响应NRC73，不允许刷写成功
        if (buffer[ADPT_UDS_36_SERVICE_INDEX_POS] != expendIndex) { //当前块的序号与预期序号不一致
            if (buffer[ADPT_UDS_36_SERVICE_INDEX_POS] != (expendIndex - 1u)) { //当前序号和前一个序号差1，说明是重复请求
                resCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;
            } else {
                /*repeat request*/
                resCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER; //原本是DCM_E_POSITIVERESPONSE
            }
            break;
        }
        if (dataSize > ADPT_MAXNUM_OF_TRANS_DATA) {
            resCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            break;
        } else {
            if (block_used_length == 0) { //数据块未使用
                for (i = 0; i < dataSize; i++) {
                    block_data_36_service[i] = buffer[i + ADPT_UDS_36_SERVICE_DATA_POS];
                }
                block_used_length = dataSize;
            } else if (block_used_length < ADPT_MAXNUM_OF_TRANS_DATA) { //数据块部分填充
                if (block_used_length + dataSize < ADPT_MAXNUM_OF_TRANS_DATA) {
                    for (i = 0; i < dataSize; i++) {
                        block_data_36_service[i + block_used_length] = buffer[i + ADPT_UDS_36_SERVICE_DATA_POS];
                    }
                } else { //数据块填充完成
                    //待下载数据若正好填充数据块，刷写后重置数据块；若填充数据块后还有剩余数据，则刷写后填充剩余数据
                    for (i = 0; i < ADPT_MAXNUM_OF_TRANS_DATA - block_used_length; i++) {
                        block_data_36_service[i + block_used_length] = buffer[i + ADPT_UDS_36_SERVICE_DATA_POS];
                    }
                }
                block_used_length += dataSize;
            } else { //判断数据块使用状态时不会出现，正好填充或是溢出的情况
            }
        }
        
        /*Check the size of received data.*/ //以下判断数据长度的逻辑并不适用零跑A12
        // if (gs_UdsPrivateData.remainSize > ADPT_MAXNUM_OF_TRANS_DATA) { //falshdriver remainSize=0x05AC > 0x400
        //     /*It is not the last 0x36 service.*/
        //     if (ADPT_MAXNUM_OF_TRANS_DATA != dataSize) {
        //         resCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        //         break;
        //     }
        // } else if (gs_UdsPrivateData.remainSize != dataSize) { //falshdriver的第二部分 remainSize和dataSize都是0x1AC
        //     resCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        //     break;
        // } else {
        //     /*last service and data size is ok.*/
        // }
        
        /*send 0x78.*/
        
        gs_UdsPrivateData.serviceId = ADPT_UDS_36_SERVICE_ID;
        _Adpt_TimeOutCallBack(ADPT_UDS_36_SERVICE_ID);
        #ifdef UDS_DEBUG_EN
        g_usd_test.data[0]++;
        #endif
        //若是最后一块或者块填充完成
        if ((block_used_length == gs_UdsPrivateData.remainSize) || (block_used_length >= ADPT_MAXNUM_OF_TRANS_DATA)) {
            ret = Adpt_DownloadData(&g_DownContext,
                    gs_UdsPrivateData.address,
                    block_used_length,
                    block_data_36_service);
            SHA256_preporcess(block_data_36_service, block_used_length, all_sha256_data_length, hash_buff);
            if (BL_ERR_OK != ret) {
                resCode = DCM_E_GENERALPROGRAMMINGFAILURE;
                break;
            } else {
                *respSize = 0x01u;
                gs_UdsPrivateData.address += block_used_length; //原先是gs_UdsPrivateData.address += dataSize;
                gs_UdsPrivateData.remainSize -= block_used_length; //原先是gs_UdsPrivateData.remainSize -= dataSize
            }
            Bl_MemSet(block_data_36_service, 0, (bl_Size_t)ADPT_MAXNUM_OF_TRANS_DATA);
            if (block_used_length == gs_UdsPrivateData.remainSize) {
                block_used_length = 0;
            } else {
                if (block_used_length > ADPT_MAXNUM_OF_TRANS_DATA) {
                    block_used_length -= ADPT_MAXNUM_OF_TRANS_DATA;
                    for (i = 0; i < block_used_length; i++) {
                        block_data_36_service[i] = buffer[i + ADPT_UDS_36_SERVICE_DATA_POS + dataSize - block_used_length];
                    }
                }
                if (block_used_length == ADPT_MAXNUM_OF_TRANS_DATA) {
                    block_used_length = 0;
                }
            }
        }
        gs_UdsPrivateData.blockIndex++; //作为36服务的数据块顺序编号，而不是实际刷写进flash的块顺序
    } while (0); /*lint !e717*/
    
    if (resCode != DCM_E_POSITIVERESPONSE) {
        _Adpt_UdsClearDownInfo(&gs_UdsPrivateData);
    }
    
    return resCode;
}
#endif
/**************************************************************************//**
 *
 *  \details Callback of requesting transfer exit.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback37(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    bl_Return_t ret;
    
    (void)buffer;
    (void)size;
    
    ret = Rte_IsValidStatus(&g_DownContext, (ADPT_STATUS_SEGMENT_FIRST_DATA | ADPT_STATUS_ALLOW_TRANS_DATA));
    if (BL_ERR_OK != ret) {
        _Adpt_UdsClearDownInfo(&gs_UdsPrivateData);
        
        resCode = DCM_E_REQUESTSEQUENCEERROR;
    } else {
        Rte_ClearDownStatus(&g_DownContext, (ADPT_STATUS_SEGMENT_FIRST_DATA | ADPT_STATUS_ALLOW_TRANS_DATA));
        *respSize = 0x00u;
    }
    #ifdef UDS_DEBUG_EN
    g_usd_test.data[1]++;
    #endif
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of keeping session.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval 0x00   - Positive response code.
 *  \retval others - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback3E(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    
    (void)size;
    (void)buffer;
    
    #if(RTE_FUN_GATEWAY == BL_FUN_ON)
    _Adpt_UdsGatewayTransmit(ADPT_UDS_3E_SERVICE_ID);
    #endif
    
    *respSize = 0x00u;
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Callback of switching on of off dtc.
 *
 *  \param[in]     size     - the data size, not include id, subid and did.
 *  \param[in/out] buffer   - the pointer of data.
 *  \param[out]    respSize - the size of response data.
 *
 *  \return Return response code.
 *
 *  \retval DCM_E_POSITIVERESPONSE   - Positive response code.
 *  \retval DCM_E_SUBFUNCTIONNOTSUPPORTED - Negative response code.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
bl_ResponseCode_t Adpt_UdsCallback85(bl_BufferSize_t size,
    bl_Buffer_t *buffer,
    bl_BufferSize_t *respSize) {
    bl_ResponseCode_t resCode = DCM_E_POSITIVERESPONSE;
    
    (void)size;
    (void)buffer;
    
    *respSize = 0x00u;
    
    return resCode;
}

/**************************************************************************//**
 *
 *  \details Timeout callback of sending 0x78.
 *
 *  \param[in]  id  - service id.
 *
 *  \since  V3.0.0
 *  是靠 Rte_ReportEvent(RTE_EVENT_TYPE_MEMOPS_TIMEOUT);  //要發送 78負響應 調用 的
 *****************************************************************************/
void Adpt_UdsTimeOutCallBack(void) {
    #ifdef UDS_DEBUG_EN
    g_usd_test.time[4]++;
    if (g_usd_test.time[4] >= 1000) {
        g_usd_test.time[4] = 0;
        g_usd_test.time[5]++;
    }
    #endif
    _Adpt_TimeOutCallBack(gs_UdsPrivateData.serviceId);
    
    return ;
}

//
void Adpt_Res_default_ValidProcess(void) {
    bl_u8_t i;
    bl_u8_t l_buf[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    Dcm_StartS3Timer();
    
    Rte_SetMutexDownStatus(&g_DownContext,
        DCM_STATUS_SESSION_DEFAULT,
        DCM_SESSION_MASK);
        
    for (i = 0; i < 8; i++) {
        l_buf[i] = gs_DefaultResponse[i];
    }
    for (i = 8; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
        l_buf[i] = CANTP_FRAME_PADDING_VALUE;
    }
    _Adpt_SendResponse((bl_BufferSize_t)sizeof(l_buf), l_buf);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details When the reprogram flag is valid, the RTE module call this
 *           function.
 *
 *  \since  V3.0.0
 *  好像只被用到一次；如果從app 跳轉到 boot 則要響應 0x06u,0x50u,0x02u,
 *****************************************************************************/
void Adpt_ReprogramValidProcess(void) {
    bl_u8_t i;
    bl_u8_t l_buf[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    Dcm_StartS3Timer();
    
    Rte_SetMutexDownStatus(&g_DownContext,
        DCM_STATUS_SESSION_PROGRAMMING,
        DCM_SESSION_MASK);
    if (u8ReprogramFlag[8] != 1) {
        for (i = 0; i < 8; i++) {
            l_buf[i] = gs_ReprogrammingResponse[i];
        }
        for (i = 8; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
            l_buf[i] = CANTP_FRAME_PADDING_VALUE;
        }
        _Adpt_SendResponse((bl_BufferSize_t)sizeof(l_buf), l_buf);
    }
    u8ReprogramFlag[8] = 0;
    return ;
}

/**************************************************************************//**
 *
 *  \details When the reprogram flag is valid, the RTE module call this
 *           function.
 *
 *  \since  V3.0.0
 *  好像只被用到一次；如果從app 跳轉到 boot 則要響應 0x06u,0x50u,0x02u,
 *****************************************************************************/
void Adpt_ReprogramValid_nores_Process(void) {
    #ifdef SERVICE_36_BLOCK_REPEATE
    g_36_block_repeat_count = 0;
    #endif
    
    Dcm_StartS3Timer();
    
    Rte_SetMutexDownStatus(&g_DownContext,
        DCM_STATUS_SESSION_PROGRAMMING,
        DCM_SESSION_MASK);
    #if 0
    _Adpt_SendResponse((bl_BufferSize_t)sizeof(gs_ReprogrammingResponse),
        gs_ReprogrammingResponse);
    #endif
    return ;
}
/**************************************************************************//**
 *
 *  \details When the reset flag is valid, the RTE module call this function.
 *
 *  \since  V3.0.0
 *  0x02u,0x51u,0x01u  好像目前沒有用到
 *****************************************************************************/
void Adpt_SystemResetProcess(void) {
    u8 i;
    u8 l_buf[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    for (i = 0; i < 8; i++) {
        l_buf[i] = gs_ResetResponse[i];
    }
    for (i = 8; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
        l_buf[i] = CANTP_FRAME_PADDING_VALUE;
    }
    _Adpt_SendResponse((bl_BufferSize_t)sizeof(gs_ResetResponse), gs_ResetResponse);
    
    return ;
}

#if (RTE_FUN_STAY_IN_BOOT == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details When the application is valid and the stay in boot is enabled,
 *           the RTE module call this function.
 *
 *  \since  V3.0.0
 *  0x04u,0x71u,0x01u F5 18
 *****************************************************************************/
void Adpt_AppValidProcess(void) {
    bl_u8_t i;
    bl_u8_t l_buf[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    for (i = 0; i < 8; i++) {
        l_buf[i] = gs_StayInBootResponse[i];
    }
    for (i = 8; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
        l_buf[i] = CANTP_FRAME_PADDING_VALUE;
    }
    _Adpt_SendResponse((bl_BufferSize_t)sizeof(l_buf), l_buf);
    
    return ;
}
#endif

#if(RTE_FUN_GATEWAY == BL_FUN_ON)
/**************************************************************************//**
 *
 *  \details Gateway transmits messages by id.
 *
 *  \param[in]  id  - service id.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Adpt_UdsGatewayTransmit(bl_u8_t id) {
    bl_u8_t i;
    bl_u16_t handle;
    bl_u16_t number;
    const bl_GatewayConversion_t *gateway;
    
    number = gs_UdsGatewayList.number;
    gateway = gs_UdsGatewayList.data;
    handle = gs_UdsGatewayList.handle;
    
    for (i = 0u; i < number; i++) {
        if (id == gateway[i].sid) {
            (void)Canif_Write(handle, gateway[i].size, gateway[i].data);
            break;
        }
    }
    
    return ;
}
#endif

/**************************************************************************//**
 *
 *  \details Send 0x78 negative response.
 *
 *  \param[in]  id  - service id.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static void _Adpt_UdsPendingResponse(bl_u8_t id) {
    u8 i;
    #if ADPT_UDS_MAX_CAN_FRAME_SIZE <=8
    
    bl_Buffer_t buffer[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    
    buffer[ADPT_UDS_CAN_FRAME_INDEX_0] = DCM_NEGATIVE_RESPONSE_SIZE;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_1] = DCM_NEGATIVE_RESPONSE_CODE;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_2] = id;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_3] = DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING;
    #if(CANTP_FUN_FRAME_PADDING == BL_FUN_ON)
    for (i = ADPT_UDS_CAN_FRAME_INDEX_4; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
        buffer[i] = CANTP_FRAME_PADDING_VALUE;
    }
    #endif
    #else
    
    bl_Buffer_t buffer[ADPT_UDS_MAX_CAN_FRAME_SIZE];
    
    buffer[ADPT_UDS_CAN_FRAME_INDEX_0] = 0x00;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_1] = DCM_NEGATIVE_RESPONSE_SIZE;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_2] = DCM_NEGATIVE_RESPONSE_CODE;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_3] = id;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_4] =
        DCM_E_REQUESTCORRECTLYRECEIVED_RESPONSEPENDING;
    #if(CANTP_FUN_FRAME_PADDING == BL_FUN_ON)
    for (i = ADPT_UDS_CAN_FRAME_INDEX_5; i < ADPT_UDS_MAX_CAN_FRAME_SIZE; i++) {
        buffer[i] = CANTP_FRAME_PADDING_VALUE;
    }
    #endif
    
    #endif
    
    _Adpt_SendResponse((bl_BufferSize_t)sizeof(buffer), buffer);
    
    return ;
}

/**************************************************************************//**
 *
 *  \details Send a response by ComOps in the g_RteInfo. When the transmitter
 *           is activated, This function will wait for some time configurated
 *           by ADPT_UDS_TX_TIMEOUT. If the response is not send within this
 *           time, this function will still return.
 *
 *  \param[in]  size    - the size of buffer.
 *  \param[in]  buffer  - the data will be send.
 *
 *  \since  V3.0.0
 *
 *****************************************************************************/
static void _Adpt_SendResponse(bl_BufferSize_t size, const bl_Buffer_t *buffer) {
    #if ADPT_UDS_MAX_CAN_FRAME_SIZE <=8
    bl_u32_t timeout = ADPT_UDS_TX_TIMEOUT;
    #else
    bl_u32_t timeout = ADPT_UDS_CANFD_TX_TIMEOUT;
    #endif
    
    /*whether return value is necessary*/
    /* -----------------------------------------------------------------------A12
    发送报文 03 7F 31 78 AA AA AA AA
    ----------------------------------------------------------------------- */
    (void)Canif_Write(ADPT_UDS_TX_HANDLE, size, buffer);
    
    BL_WAITFOR_TIMEOUT(timeout, Canif_IsWritten(ADPT_UDS_TX_HANDLE) == BL_ERR_OK)
    return ;
}
/**************************************************************************//**
 *
 *  \details Timeout callback of sending 0x78.
 *
 *  \param[in]  id  - service id.
 *
 *  \since  V2.0.0
 *  負響應  78
 *****************************************************************************/
static void _Adpt_TimeOutCallBack(bl_u8_t id) {
    _Adpt_UdsPendingResponse(id);
    
    #if(RTE_FUN_GATEWAY == BL_FUN_ON)
    _Adpt_UdsGatewayTransmit(ADPT_UDS_3E_SERVICE_ID);
    #endif
}

/**************************************************************************//**
 *
 *  \details Set response info of session.
 *
 *  \param[out]  buffer  - the pointer of response data.
 *
 *  \return Return the size of response data.
 *
 *  \since  V4.0.0
 *  會話的應答時間參數
 *****************************************************************************/
static bl_BufferSize_t _Adpt_SetSessionResp(bl_Buffer_t *buffer) {
    bl_BufferSize_t size;
    
    size = ADPT_SESSION_RESPONSE_LEN;
    
    buffer[ADPT_UDS_CAN_FRAME_INDEX_0] = ADPT_SESSION_RESPONSE0;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_1] = ADPT_SESSION_RESPONSE1;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_2] = ADPT_SESSION_RESPONSE2;
    buffer[ADPT_UDS_CAN_FRAME_INDEX_3] = ADPT_SESSION_RESPONSE3;
    
    return size;
}

/**************************************************************************//**
 *
 *  \details Go to the session
 *
 *  \param[in] sessionMask - the session will be gone.
 *
 *  \since  V5.0.0
 *
 *****************************************************************************/
static void _Adpt_UdsGotoSession(bl_DownStatus_t sessionMask) {
    _Adpt_UdsClearDownInfo(&gs_UdsPrivateData);
    /*
    adpt->address = 0;
    adpt->remainSize = 0;
    adpt->blockIndex = 0;
    再清 g_DownContext.state[0-1]
    state[0]一般指#define RTE_SYSFLAG_SECURITY_ACCESS     (0x00000001UL)
        #define RTE_SYSFLAG_CHECK_SIGNATURE     (0x00000002UL)
        #define RTE_SYSFLAG_ERASE_BLOCK         (0x00000004UL)
        #define RTE_SYSFLAG_WRITE_DATA          (0x00000008UL)
        #define RTE_SYSFLAG_WRITE_FINGPRINT     (0x00000010UL)  //寫入金手指，具體是什麼？
        #define RTE_SYSFLAG_WRITE_OTHER         (0x00000020UL)
    state[1]一般指 RTE_USER_FLAG_GROUP_1
        下面包含 ALLOW_TRANS_DATA ；BLOCK_FIRST_DATA ； SEGMENT_FIRST_DATA ; SEGMENT_FIRST_DATA
        所有 DCM診斷相關
    */
    
    #if (DCM_FUN_SECURITY_ACCESS == BL_FUN_ON)
    /* -----------------------------------------------------------------------A12
    安全访问 lock the level 1
    status[1] = 0x0000 0100
    ----------------------------------------------------------------------- */
    Rte_SetMutexDownStatus(&g_DownContext,
        DCM_STATUS_SECURITY_LOCK_1,  //40000100
        DCM_SECURITYACCESS_1_MASK);  //40000700  相當於清除其余兩個 再置 DCM_STATUS_SECURITY_LOCK_1
    Rte_SetMutexDownStatus(&g_DownContext,
        DCM_STATUS_SECURITY_LOCK_2,
        DCM_SECURITYACCESS_2_MASK);
    #else
    /*unlock the level 1.*/
    Rte_SetMutexDownStatus(&g_DownContext,
        DCM_STATUS_SECURITY_UNLOCK_1,  //40000200
        DCM_SECURITYACCESS_1_MASK);  //40000700  相當於清除其余兩個 再置 DCM_SECURITYACCESS_1_MASK
    #endif
    /* -----------------------------------------------------------------------A12
    会话控制 默认会话
    status[1] = 0x0000 0101
    ----------------------------------------------------------------------- */
    Rte_SetMutexDownStatus(&g_DownContext,
        sessionMask,    //DCM_STATUS_SESSION_DEFAULT = 0x40000001
        DCM_SESSION_MASK);   //40000007 相當於清除其余兩個  再置sessionMask
    return ;
}

/**************************************************************************//**
 *
 *  \details Clear the download information.
 *
 *  \since  V5.1.0
 *
 *****************************************************************************/
static void _Adpt_UdsClearDownInfo(bl_AdapterData_t *adpt) {
    adpt->address = 0;
    adpt->remainSize = 0;
    adpt->blockIndex = 0;
    block_used_length = 0;
    Bl_MemSet(block_data_36_service, 0, (bl_Size_t)ADPT_MAXNUM_OF_TRANS_DATA);
    /* -----------------------------------------------------------------------A12
    ADPT_UDS_SYSTEM_STATUS_MASK = 1c  status[0] = 0
    ----------------------------------------------------------------------- */
    Rte_ClearDownStatus(&g_DownContext, ADPT_UDS_SYSTEM_STATUS_MASK);
    /*
     (RTE_SYSFLAG_ERASE_BLOCK \  4
    	| RTE_SYSFLAG_WRITE_DATA)  //8
    	| RTE_SYSFLAG_WRITE_FINGPRINT 10
     */
    /* -----------------------------------------------------------------------A12
    ADPT_UDS_STATUS_MASK = 78000000  status[1] = 0
    ----------------------------------------------------------------------- */
    Rte_ClearDownStatus(&g_DownContext, ADPT_UDS_STATUS_MASK);
    /*
    	(ADPT_STATUS_ALLOW_TRANS_DATA \
    	| ADPT_STATUS_BLOCK_FIRST_DATA \
    	| ADPT_STATUS_SEGMENT_FIRST_DATA)  //78000000
     */
    return ;
}

/**************************************************************************//**
 *
 *  \details Reset the system.
 *
 *  \since  V5.1.0
 *  好像沒有用到
 *****************************************************************************/
static void _Adpt_UdsResetSystem(bl_u8_t sid, bl_u16_t flagId) {
    bl_Return_t ret;
    
    ret = Dcm_IsSuspendedResponse(); //gs_SupPosRespFlag==0
    if (ret != BL_ERR_OK) {
        _Adpt_TimeOutCallBack(sid);
        /*write flag.*/
        (void)Dm_WriteDefaultData(flagId);
    }
    
    g_RteInfo.SystemReset();
}








void  adpt_test_init(void) {
    (void)Dm_WriteDefaultData(BOOTM_REPROGRAM_FLAG_DID);
}
