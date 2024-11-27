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

#ifndef _LS_NM_H_
#define _LS_NM_H_

#include "common_types.h"
//#include "Std_Types.h"
//#include "ComStack_Types.h"
#include "Cpu.h"
#include "can.h"
#include "Ls_Nm_Cfg.h"


/*請求命令*/
typedef enum
{
    NM_REQ_CMD_NULL = 0,
    NM_REQ_CMD_GOTO_INACTIVE,
    NM_REQ_CMD_GOTO_ACTIVE,
    NM_REQ_CMD_POWER_ON,	/*第一次上電*/
}nm_req_cmd_e;


/*主狀態*/
typedef enum
{
    NM_MAIN_STATE_INACTIVE = 0,
    NM_MAIN_STATE_ACTIVE,
}nm_main_state_e;


/*子狀態*/
typedef enum
{
    NM_SUB_STATE_INACTIVE_IDLE = 0,
    NM_SUB_STATE_ACTIVE_NORMAL ,
    NM_SUB_STATE_ACTIVE_BUSOFF,
}nm_sub_state_active_e;


/*標誌--可預睡眠*/
typedef enum
{
    NM_FLAG_SHALL_SLEEP = 0,
    NM_FLAG_SHALL_WAKE,
}nm_flag_shall_sleep_e;








#if 0
typedef enum
{	/*網絡管理喚醒源*/
	NM_ACTIVE_SOURCE_MASK_KL15  	= 0x01u,
	NM_ACTIVE_SOURCE_MASK_DIAGNOSE	= 0x02u,
	NM_ACTIVE_SOURCE_MASK_TIMER		= 0x04u,
	NM_ACTIVE_SOURCE_MASK_4  		= 0x08u,
	NM_ACTIVE_SOURCE_MASK_5  		= 0x10u,
	NM_ACTIVE_SOURCE_MASK_6  		= 0x20u,
	NM_ACTIVE_SOURCE_MASK_7  		= 0x40u,
	NM_ACTIVE_SOURCE_MASK_8  		= 0x80u,
	NM_ACTIVE_SOURCE_MASK_NULL 		= 0x00u,
}nmActiveSourceType;


typedef enum
{
	NM_WAKEUP_SOURCE_WAKEUP 		= 0u,
	NM_WAKEUP_SOURCE_KL15 			= 1u,
	NM_WAKEUP_SOURCE_1 				= 2u,
	NM_WAKEUP_SOURCE_2 				= 3u,
	NM_WAKEUP_SOURCE_3 				= 4u,
	NM_WAKEUP_SOURCE_4 				= 5u,
	NM_WAKEUP_SOURCE_5 				= 6u,
	NM_WAKEUP_SOURCE_6 				= 7u,
	NM_WAKEUP_SOURCE_RKE			= 10u,
	NM_WAKEUP_SOURCE_KEY			= 11u,
}nmWakeUpSourceType;
#endif


typedef enum
{
    NM_DLL_ERROR_ACTIVE = 0,
    NM_DLL_ERROR_PASSIVE,
    NM_DLL_BUS_OFF_PRE, //预进入busoff
    NM_DLL_BUS_OFF,  //真正进入busoff
}nm_erro_source_e;



typedef enum
{
    NM_MOTE_STARTED = 0,
    NM_MOTE_SLEEP 	= 1,
    NM_MOTE_STOP 	= 2,
}nmCanModeType;

typedef enum
{
    NM_REC_SEND = 0,
    NM_REC_ONLY = 1,
    NM_REC_STOP = 2,
}nmIlReqType;


//快慢恢复状态
typedef enum
{
    NM_BUSOFF_RECOVERY_STATE_NULL = 0u,
    NM_BUSOFF_RECOVERY_STATE_QUICK,
    NM_BUSOFF_RECOVERY_STATE_SLOW
}nm_busoff_recovery_state_e;


//子状态
typedef enum
{
    NM_BUSOFF_SUBSTATE_NULL = 0u,
    NM_BUSOFF_SUBSTATE_TRY,
    NM_BUSOFF_SUBSTATE_WAIT
}nm_busoff_sub_state_e;


/***********************************************************************/
/*對外接口宏定義********************************************************/
extern void CanNm_Init(void);
extern void CanNm_Req(nm_req_cmd_e cmd);
extern bool CanNm_GetSleepState(void);
extern void CanNm_MainFunction(void);
#if 0
extern void CanNm_SetActiveSourceMask(nmActiveSourceType source);
extern void CanNm_ClrActiveSourceMask(nmActiveSourceType source);
extern void CanNm_SetWakeUpSource(nmWakeUpSourceType source);
#endif

/*END對外接口***********************************************************/
/***********************************************************************/
/*CanIf接口宏定義*******************************************************/
extern void CanNm_WakeUp(u8 controller);
extern u8 CanNm_PhysErrorInd(u8 phy_id,nm_erro_source_e erro);
extern void CanNm_RxIndication(PduIdType canNmPduId,Can_IdType canNmRxId,const PduInfoType* pCanNmRxPduPtr);
extern void CanNm_TxConfirmation(PduIdType canNmPduId,u8 phy_id);
extern void CanNm_TxConfirmation_com(PduIdType canNmPduId,u8 phy_id);

void CanNm_goto_inactive(void);

/*END接口宏定義*********************************************************/

bool CanNm_is_inactive(void);
void CanNm_get_wakeup_flag(void);

bool CanNm_is_busoff(void);

void CanNm_IlReq(nmIlReqType req);
u8 ls_nm_get_com_manage_mode(void);

#if 0
#define NM_CPU_SR_ALLOC()  //CPU_SR_ALLOC()
#define NM_ENTER_CRITICAL() INT_SYS_DisableIRQGlobal()
#define NM_EXIT_CRITICAL()  INT_SYS_EnableIRQGlobal()
#else
#define NM_CPU_SR_ALLOC()  //CPU_SR_ALLOC()
#define NM_ENTER_CRITICAL() //ENTER_CRITICAL()
#define NM_EXIT_CRITICAL()  //EXIT_CRITICAL()
#endif

#define NM_BUSOFF_RECOVERY_USE_REINIT 1     //以can初始化方式，实现busoff

/*END接口宏定义*********************************************************/

#endif
