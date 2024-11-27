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


#include "common_types.h"
#include "Cpu.h"
#include "common_memory.h"
#include "Ls_Nm.h"
#if 0
#include "CanIf_Type.h"
#include "CanTrcv.h"
#include "CanIf.h"
#include "Com.h"
#include "wdog.h"
#include "Demm.h"
#include "can.h"
#include "Com.h"
#include "flexcan_hw_access.h"
#include "Demm.h"
#endif
#include "bl_can_transceiver.h"

#ifdef  CAN_AUTOSAR_EN
#include "CanIf_Type.h"
#include "can_drive.h"
#include "CanTrcv.h"
#include "Demm.h"
#include "CanIf.h"
#include "Com.h"
#endif

#define CAN_NM_DEBUG_EN 1


static void Cannm_busoff_task(u8 phy_id);
static void CanNm_CanSetMode(u8 logic_id, nmCanModeType mode);
//static void CanNm_goto_inactive(void);
static void can_nm_phy_busoff_trig(u8 phy_id);
static void can_nm_busoff_recovery(u8 phy_id);
static void can_nm_busoff_recovery_successful(u8 phy_id);
static void can_nm_phy_busoff_pre_trig(u8 phy_id);




/*busoff*/
typedef struct {
    nm_busoff_recovery_state_e state;  //主状态--未busoff，快恢复，慢恢复
    nm_busoff_sub_state_e sub_state;  //子状态，--试，或者回读状态
    //u8 sub_state1;
    u16 timecount;
    u16 try_count;
    //u16 other;
} can_nm_busoff_s;

/**/
typedef struct {
    nm_req_cmd_e cmd;
    nm_main_state_e state;
    nm_sub_state_active_e sub_state;
    nm_flag_shall_sleep_e flag_shall;
    u16 count;
    u8 com_mana;  //com层的管理
    
    can_nm_busoff_s busoff[CAN_ID_TOTAL];
    #ifdef CAN_NM_DEBUG_EN
    u8 test_buf[15];
    #endif
} nm_app_s;

static nm_app_s g_nm_app_s;

//static u8 g_nm_test1[10] ;


static void can_nm_phy_busoff_pre_trig(u8 phy_id) {
    u8 i;
    if (phy_id == CAN_ID_0) {
        #ifdef CAN_ID_0_EN
        //#ifdef NM_BUSOFF_RECOVERY_USE_REINIT
        //for(i=0;i<CAN_ID_0_TX_MAILBOX_ID_MAX;i++)
        {
            //CAN_AbortTransmission(CAN0,CAN_TRANSMIT_SECONDARY);  //mingyea
            FL_CAN_DisableIT_TXOK(CAN);
        }
        #endif
        //#endif
    } else if (phy_id == CAN_ID_1) {
        //#ifdef NM_BUSOFF_RECOVERY_USE_REINIT
        #ifdef CAN_ID_1_EN
        //for(i=0;i<CAN_ID_1_TX_MAILBOX_ID_MAX;i++)
        {
            //CAN_AbortTransmission(CAN1,CAN_TRANSMIT_SECONDARY);
        }
        #endif
        //#endif
    }
    #ifdef CAN_NM_DEBUG_EN
    g_nm_app_s.test_buf[7]++;
    #endif
}

static void can_nm_phy_busoff_trig(u8 phy_id) {
    if (phy_id >= CAN_ID_TOTAL) {
        return;
    }
    g_nm_app_s.busoff[phy_id].timecount = 0u;
    if (g_nm_app_s.busoff[phy_id].state == NM_BUSOFF_RECOVERY_STATE_NULL) { //第一次进来
        if (g_nm_app_s.busoff[phy_id].sub_state == NM_BUSOFF_SUBSTATE_NULL) {
            g_nm_app_s.busoff[phy_id].timecount = 0u;
            g_nm_app_s.busoff[phy_id].try_count = 0u;
            g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_TRY;
            g_nm_app_s.busoff[phy_id].state = NM_BUSOFF_RECOVERY_STATE_QUICK;
            #ifdef CAN_NM_DEBUG_EN
            g_nm_app_s.test_buf[0]++;
            #endif
        }
    } else if (g_nm_app_s.busoff[phy_id].state <= NM_BUSOFF_RECOVERY_STATE_SLOW) { //正在故障时，快慢恢复时
        if (g_nm_app_s.busoff[phy_id].sub_state == NM_BUSOFF_SUBSTATE_NULL) {
            g_nm_app_s.busoff[phy_id].timecount = 0u;
            //g_nm_app_s.busoff[phy_id].try_count=0u;
            g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_TRY;
            //g_nm_app_s.busoff[phy_id].state=1;
            #ifdef CAN_NM_DEBUG_EN
            g_nm_app_s.test_buf[1]++;
            #endif
        }
    }
}

//5ms周期
static void can_nm_busoff_recovery(u8 phy_id) {
    u8 l_logic_id = CAN_ID_NULL;
    CAN_Type *const l_flexcanBasePtr[] = CAN_BASE_PTRS;
    #ifdef CAN_AUTOSAR_EN
    l_logic_id = can_drive_get_logic_id(phy_id);
    #else
    l_logic_id = CAN_ID_0;
    #endif
    if ((phy_id >= CAN_ID_TOTAL) || (l_logic_id == CAN_ID_NULL)) {
        return;
    }
    if (g_nm_app_s.busoff[phy_id].state == NM_BUSOFF_RECOVERY_STATE_QUICK) {
        if (g_nm_app_s.busoff[phy_id].sub_state == NM_BUSOFF_SUBSTATE_TRY) {
            g_nm_app_s.busoff[phy_id].timecount++;
            if (g_nm_app_s.busoff[phy_id].timecount >= NM_MAIN_BUSOFF_FAST_H) {
                g_nm_app_s.busoff[phy_id].timecount = 0u;
                #ifdef CAN_NM_DEBUG_EN
                g_nm_app_s.test_buf[2]++;
                #endif
                //g_nm_app_s.busoff.sub_state=1u;
                #ifdef NM_BUSOFF_RECOVERY_USE_REINIT
                g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_NORMAL;
                //wdog_task();
                //__disable_irq();
                LS_NM_TEST_IO_ON();
                #ifdef CAN_AUTOSAR_EN
                CanTrcv_Init();
                //__disable_irq();
                (void)Can_InitController(l_logic_id, (void *)0);
                can_phy_drive_init((can_id_e)phy_id);
                Can_SetControllerMode(l_logic_id, CAN_T_START);
                CanNm_IlReq(NM_REC_SEND);
                //__enable_irq();
                #else
                CanTrcv_Init();
                can_de_init();
                can_init();
                #endif
                LS_NM_TEST_IO_OFF();
                //__enable_irq();
                #else
                can_driver_busoff_config(phy_id, false);
                //while((l_flexcanBasePtr[phy_id]->ESR1&CAN_ESR1_FLTCONF_MASK)>=CAN_ESR1_FLTCONF(0x2));
                //can_driver_busoff_config(phy_id, true);
                #endif
                #if 0
                g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_WAIT;
                #ifdef CAN_NM_DEBUG_EN
                g_nm_app_s.test_buf[2]++;
                #endif
                #else
                g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_TRY;
                g_nm_app_s.busoff[phy_id].try_count++;
                #ifdef CAN_NM_DEBUG_EN
                g_nm_app_s.test_buf[4]++;
                #endif
                if (g_nm_app_s.busoff[phy_id].try_count >= NM_MAIN_BUSOFF_FAST_COUNT) {
                    g_nm_app_s.busoff[phy_id].try_count = 0u;
                    //g_nm_app_s.busoff.sub_state=NM_BUSOFF_SUBSTATE_NULL;
                    g_nm_app_s.busoff[phy_id].state = NM_BUSOFF_RECOVERY_STATE_SLOW;
                    #ifndef CAN_DRIVE_CLOSE_DEM
                    #ifdef CAN_AUTOSAR_EN
                    dem_set_test_result(DTC_CAN_BUS_OFF_ID, DTC_TEST_STATE_FAILED);
                    #endif
                    #endif
                    #ifdef CAN_NM_DEBUG_EN
                    g_nm_app_s.test_buf[5]++;
                    #endif
                }
                #endif
            }
        }
        #if 0
        if (g_nm_app_s.busoff[phy_id].sub_state == NM_BUSOFF_SUBSTATE_WAIT) {
            g_nm_app_s.busoff[phy_id].timecount++;
            if (g_nm_app_s.busoff[phy_id].timecount >= NM_MAIN_BUSOFF_FAST_L) {
                g_nm_app_s.busoff[phy_id].timecount = 0u;
                #ifndef NM_BUSOFF_RECOVERY_USE_REINIT
                //g_nm_app_s.busoff.sub_state=NM_BUSOFF_SUBSTATE_TRY;
                if ((l_flexcanBasePtr[phy_id]->ESR1 & CAN_ESR1_FLTCONF_MASK) < CAN_ESR1_FLTCONF(0x2))
                
                {
                    #if 1
                    g_nm_app_s.busoff[phy_id].timecount = 0;
                    g_nm_app_s.busoff[phy_id].try_count = 0;
                    g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_NULL;
                    g_nm_app_s.busoff[phy_id].state = NM_BUSOFF_RECOVERY_STATE_NULL;
                    #endif
                    can_driver_busoff_config(phy_id, true);
                    #ifdef CAN_NM_DEBUG_EN
                    g_nm_app_s.test_buf[3]++;
                    #endif
                } else
                #endif
                {
                    #ifdef NM_BUSOFF_RECOVERY_USE_REINIT
                    #else
                    //while((l_flexcanBasePtr[phy_id]->ESR1&CAN_ESR1_FLTCONF_MASK)>=CAN_ESR1_FLTCONF(0x2));
                    //can_driver_busoff_config(phy_id, true);
                    #endif
                    g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_TRY;
                    g_nm_app_s.busoff[phy_id].try_count++;
                    #ifdef CAN_NM_DEBUG_EN
                    g_nm_app_s.test_buf[4]++;
                    #endif
                    if (g_nm_app_s.busoff[phy_id].try_count >= NM_MAIN_BUSOFF_FAST_COUNT) {
                        g_nm_app_s.busoff[phy_id].try_count = 0u;
                        //g_nm_app_s.busoff.sub_state=NM_BUSOFF_SUBSTATE_NULL;
                        g_nm_app_s.busoff[phy_id].state = NM_BUSOFF_RECOVERY_STATE_SLOW;
                        #ifndef CAN_DRIVE_CLOSE_DEM
                        #ifdef CAN_AUTOSAR_EN
                        dem_set_test_result(DTC_CAN_BUS_OFF_ID, DTC_TEST_STATE_FAILED);
                        #endif
                        #endif
                        #ifdef CAN_NM_DEBUG_EN
                        g_nm_app_s.test_buf[5]++;
                        #endif
                    }
                }
            }
        }
        #endif
    } else if (g_nm_app_s.busoff[phy_id].state == NM_BUSOFF_RECOVERY_STATE_SLOW) {
        if (g_nm_app_s.busoff[phy_id].sub_state == NM_BUSOFF_SUBSTATE_TRY) {
            g_nm_app_s.busoff[phy_id].timecount++;
            if (g_nm_app_s.busoff[phy_id].timecount >= NM_MAIN_BUSOFF_SLOW_H) {
                g_nm_app_s.busoff[phy_id].timecount = 0u;
                //g_nm_app_s.busoff[phy_id].sub_state=1u;
                #ifdef NM_BUSOFF_RECOVERY_USE_REINIT
                g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_NORMAL;
                //wdog_task();
                //__disable_irq();
                LS_NM_TEST_IO_ON();
                #ifdef CAN_AUTOSAR_EN
                CanTrcv_Init();
                //__disable_irq();
                (void)Can_InitController(l_logic_id, (void *)0);
                can_phy_drive_init((can_id_e)phy_id);
                Can_SetControllerMode(l_logic_id, CAN_T_START);
                CanNm_IlReq(NM_REC_SEND);
                //__enable_irq();
                #else
                CanTrcv_Init();
                can_de_init();
                can_init();
                #endif
                LS_NM_TEST_IO_OFF();
                //__enable_irq();
                #else
                can_driver_busoff_config(phy_id, false);
                //while((l_flexcanBasePtr[phy_id]->ESR1&CAN_ESR1_FLTCONF_MASK)>=CAN_ESR1_FLTCONF(0x2));
                //can_driver_busoff_config(phy_id, true);
                #endif
                g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_WAIT;
                #ifdef CAN_NM_DEBUG_EN
                g_nm_app_s.test_buf[6]++;
                #endif
            }
        } else if (g_nm_app_s.busoff[phy_id].sub_state == NM_BUSOFF_SUBSTATE_WAIT) {
            g_nm_app_s.busoff[phy_id].timecount++;
            if (g_nm_app_s.busoff[phy_id].timecount >= NM_MAIN_BUSOFF_SLOW_L) {
                g_nm_app_s.busoff[phy_id].timecount = 0u;
                #ifndef NM_BUSOFF_RECOVERY_USE_REINIT
                //g_nm_app_s.busoff.sub_state=NM_BUSOFF_SUBSTATE_TRY;
                if ((l_flexcanBasePtr[phy_id]->ESR1 & CAN_ESR1_FLTCONF_MASK) < CAN_ESR1_FLTCONF(0x2)) {
                    #if 0
                    g_nm_app_s.busoff.timecount = 0;
                    g_nm_app_s.busoff.try_count = 0;
                    g_nm_app_s.busoff.sub_state = NM_BUSOFF_SUBSTATE_NULL;
                    g_nm_app_s.busoff.state = NM_BUSOFF_RECOVERY_STATE_NULL;
                    #endif
                    #ifdef CAN_NM_DEBUG_EN
                    g_nm_app_s.test_buf[8]++;
                    #endif
                } else
                #endif
                {
                    #ifdef NM_BUSOFF_RECOVERY_USE_REINIT
                    #else
                    can_driver_busoff_config(phy_id, true);
                    #endif
                    g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_TRY;
                    g_nm_app_s.busoff[phy_id].try_count++;
                    #ifdef CAN_NM_DEBUG_EN
                    g_nm_app_s.test_buf[9]++;
                    #endif
                }
            }
        }
    }
    //#endif
}

static void can_nm_busoff_recovery_successful(u8 phy_id) {
    if (phy_id >= CAN_ID_TOTAL) {
        return;
    }
    if (g_nm_app_s.busoff[phy_id].state != NM_BUSOFF_RECOVERY_STATE_NULL) {
        g_nm_app_s.busoff[phy_id].timecount = 0;
        g_nm_app_s.busoff[phy_id].try_count = 0;
        g_nm_app_s.busoff[phy_id].sub_state = NM_BUSOFF_SUBSTATE_NULL;
        g_nm_app_s.busoff[phy_id].state = NM_BUSOFF_RECOVERY_STATE_NULL;
        #ifdef CAN_NM_DEBUG_EN
        g_nm_app_s.test_buf[10]++;
        #endif
    }
}

/*****************************************************************************
 *  Function Definitions
 *****************************************************************************/
static void CanNm_CanSetMode(u8 logic_id, nmCanModeType mode) {
    if (mode == NM_MOTE_STARTED) {
        #ifdef CAN_AUTOSAR_EN
        (void)CanIf_SetControllerMode(logic_id, CANIF_CS_STARTED); //  CAN_LOGIC_ID_0
        #endif
    } else if (mode == NM_MOTE_SLEEP) {
        #ifdef CAN_AUTOSAR_EN
        (void)CanIf_SetControllerMode(logic_id, CANIF_CS_SLEEP);
        #endif
    } else if (mode == NM_MOTE_STOP) {
        #ifdef CAN_AUTOSAR_EN
        (void)CanIf_SetControllerMode(logic_id, CANIF_CS_STOPPED);
        #endif
    }
}

void CanNm_IlReq(nmIlReqType req) {
    g_nm_app_s.com_mana = req;
    if (req == NM_REC_SEND) {
        #ifdef CAN_AUTOSAR_EN
        (void)Com_TxStart();
        (void)Com_RxStart();
        //CanNm_StartBusOffMgt();
        #endif
    } else if (req == NM_REC_ONLY) {
        #ifdef CAN_AUTOSAR_EN
        (void)Com_RxStart();
        (void)Com_TxStop();
        //CanNm_StartBusOffMgt();
        #endif
    } else if (req == NM_REC_STOP) {
        #ifdef CAN_AUTOSAR_EN
        (void)Com_TxStop();
        (void)Com_RxStop();
        //CanNm_StopBusOffMgt();
        #endif
    }
}


void CanNm_Init(void) {
    CanNm_WakeUp(0);
    //CanNm_goto_inactive();  //暂时关闭
    //g_nm_app_s.cmd = NM_REQ_CMD_POWER_ON;	/*是第一次上電*/
    
    common_memory_clear((u8 *)&g_nm_app_s, sizeof(g_nm_app_s));
    g_nm_app_s.cmd = NM_REQ_CMD_NULL;	/*是第一次上電*/
    
    g_nm_app_s.sub_state = NM_SUB_STATE_INACTIVE_IDLE;
    g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
}



void CanNm_goto_inactive(void) {
    u8 i;
    u8 l_phy_id = CAN_ID_0;
    //g_nm_test1[0]++;
    g_nm_app_s.cmd = NM_REQ_CMD_NULL;
    g_nm_app_s.state = NM_MAIN_STATE_INACTIVE;
    g_nm_app_s.sub_state = NM_SUB_STATE_INACTIVE_IDLE;
    g_nm_app_s.flag_shall = NM_FLAG_SHALL_SLEEP;
    g_nm_app_s.count = 0;
    
    #if 0
    CanTrcv_Init();
    CanNm_CanSetMode(CAN_LOGIC_ID_0, NM_MOTE_STARTED);
    CanNm_IlReq(NM_REC_ONLY);
    #else
    for (i = 0u; i < CAN_LOGIC_ID_MAX; i++) {
        #ifdef CAN_AUTOSAR_EN
        l_phy_id = can_drive_get_phy_id(i);
        #endif
        #ifdef CAN_ID_0_EN
        if (l_phy_id == CAN_ID_0) {
            CanNm_CanSetMode(i, NM_MOTE_SLEEP);
        }
        #endif
        #ifdef CAN_ID_1_EN
        if (l_phy_id == CAN_ID_1) {
            CanNm_CanSetMode(i, NM_MOTE_SLEEP);
        }
        #endif
        //CanNm_CanSetMode(i,NM_MOTE_SLEEP);
    }
    CanNm_IlReq(NM_REC_ONLY);
    #ifdef CAN_AUTOSAR_EN
    CanTrcv_Disable();
    #endif
    
    #endif
    
    #if 0
    CanNm_CanSetMode(CAN_LOGIC_ID_0, NM_MOTE_SLEEP);
    #endif
    
    
}



void CanNm_MainFunction(void) {
    #if 1
    bool ign_state;
    NM_CPU_SR_ALLOC();
    
    NM_ENTER_CRITICAL();
    
    
    if (g_nm_app_s.state == NM_MAIN_STATE_INACTIVE) {
        if (g_nm_app_s.cmd == NM_REQ_CMD_GOTO_ACTIVE) {
            g_nm_app_s.cmd = NM_REQ_CMD_NULL;
            
            CanNm_WakeUp(1);
        }
        #if 0
        /*上 電時 的判斷*/
        else if (g_nm_app_s.cmd == NM_REQ_CMD_POWER_ON) {
            (void)ReceiveMessage(MSG_R_IGN_SW, &ign_state);
            if (ign_state == TRUE) {
                g_nm_app_s.cmd = NM_REQ_CMD_NULL;
                CanNm_WakeUp(1);
            }
        }
        #endif
    } else if (g_nm_app_s.state == NM_MAIN_STATE_ACTIVE) {
    
        if (g_nm_app_s.cmd == NM_REQ_CMD_GOTO_ACTIVE) {
            g_nm_app_s.cmd = NM_REQ_CMD_NULL;
            g_nm_app_s.flag_shall = NM_FLAG_SHALL_WAKE;
        }
        
        switch (g_nm_app_s.sub_state) {
        
            case NM_SUB_STATE_ACTIVE_NORMAL:
            
                #if 0
                if (g_nm_app_s.flag_shall == NM_FLAG_SHALL_SLEEP) {
                    CanNm_goto_inactive();
                } else if (g_nm_app_s.cmd == NM_REQ_CMD_GOTO_INACTIVE) {
                    g_nm_app_s.cmd = NM_REQ_CMD_NULL;
                    CanNm_goto_inactive();
                }
                #endif
                //恢复要1s吗
                
                
                break;
                
            case NM_SUB_STATE_ACTIVE_BUSOFF:
            
                if (g_nm_app_s.cmd == NM_REQ_CMD_GOTO_INACTIVE) {
                    g_nm_app_s.cmd = NM_REQ_CMD_NULL;
                    g_nm_app_s.flag_shall = NM_FLAG_SHALL_SLEEP;
                }
                #ifdef CAN_ID_0_EN
                can_nm_busoff_recovery(CAN_ID_0);
                #endif
                #ifdef CAN_ID_1_EN
                can_nm_busoff_recovery(CAN_ID_1);
                #endif
                break;
                
            default:
            
                break;
                
        }
        
    } else {
    
    }
    NM_EXIT_CRITICAL();
    #else
    can_nm_busoff_recovery(CAN_ID_0);
    can_nm_busoff_recovery(CAN_ID_1);
    #endif
}


void CanNm_Req(nm_req_cmd_e cmd) {
    NM_CPU_SR_ALLOC();
    NM_ENTER_CRITICAL();
    g_nm_app_s.cmd = cmd;
    NM_EXIT_CRITICAL();
}


static void Cannm_busoff_task(u8 phy_id) {





}


u8 CanNm_PhysErrorInd(u8 phy_id, nm_erro_source_e erro) {
    u8 retValue ;
    u8 i;
    NM_CPU_SR_ALLOC();
    
    //g_nm_test1[1]++;
    if (erro == NM_DLL_BUS_OFF) {
        //g_nm_test1[2]++;
        
        NM_ENTER_CRITICAL();
        //g_nm_app_s.busoff[phy_id].timecount=0u;  //注意，要清零
        #if 1
        if (g_nm_app_s.sub_state != NM_SUB_STATE_ACTIVE_BUSOFF) {
            g_nm_app_s.busoff[phy_id].timecount = 0u; //注意，要清零
            g_nm_app_s.count = 0;
            g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
            g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_BUSOFF;
            can_nm_phy_busoff_trig(phy_id);
        }
        #else
        can_nm_phy_busoff_trig(phy_id);
        #endif
        NM_EXIT_CRITICAL();
        
    } else if (erro == NM_DLL_ERROR_PASSIVE) {
        //g_nm_test1[3]++;
    } else if (erro == NM_DLL_ERROR_ACTIVE) {
        //g_nm_test1[4]++;
    } else if (erro == NM_DLL_BUS_OFF_PRE) {
        can_nm_phy_busoff_pre_trig(phy_id);
    }
    
    return retValue;
    
}

static u8 g_flag_wakeup = 0;


void CanNm_get_wakeup_flag(void) {
    #if 0
    if (g_flag_wakeup == 1) {
        g_flag_wakeup = 0;
        
        CanNm_CanSetMode(CAN_LOGIC_ID_0, NM_MOTE_STOP);
        CanTrcv_Disable();
        
    }
    #else
    g_flag_wakeup = 1;
    #endif
}


void CanNm_WakeUp(u8 controller) {
    u8 i;
    bool l_ign_logic_state;
    u8 l_temp;
    u8 l_phy_id = CAN_ID_0;
    #if 1
    NM_CPU_SR_ALLOC();
    
    
    //(void)ReceiveMessage(MSG_R_IGN_LOGIC_SW,&l_ign_logic_state);
    
    
    //if(ign_state == TRUE)
    //if(l_ign_logic_state == TRUE)
    #if 1
    if (TRUE)
    #else
    if (g_nm_app_s.busoff[CAN_ID_0].state == NM_BUSOFF_RECOVERY_STATE_NULL)
    #endif
    {
        //g_flag_wakeup = 1;
        (void)controller;
        //g_nm_test1[5]++;
        NM_ENTER_CRITICAL();
        #ifdef CAN_AUTOSAR_EN
        CanTrcv_Init();
        #endif
        for (i = 0u; i < CAN_LOGIC_ID_MAX; i++) {
            #ifdef CAN_AUTOSAR_EN
            l_phy_id = can_drive_get_phy_id(i);
            #endif
            #ifdef CAN_ID_0_EN
            if (l_phy_id == CAN_ID_0) {
                CanNm_CanSetMode(i, NM_MOTE_STARTED);
                #ifdef CAN_AUTOSAR_EN
                l_temp = Can_Controller_enable(i, TRUE); //一定要重新开启
                #endif
            }
            #endif
            #ifdef CAN_ID_1_EN
            if (l_phy_id == CAN_ID_1) {
                CanNm_CanSetMode(i, NM_MOTE_STARTED);
                #ifdef CAN_AUTOSAR_EN
                l_temp = Can_Controller_enable(i, TRUE); //一定要重新开启
                #endif
            }
            #endif
            //CanNm_CanSetMode(i,NM_MOTE_STARTED);
        }
        CanNm_IlReq(NM_REC_SEND);
        //CanNm_IlReq(NM_REC_ONLY);
        g_nm_app_s.count = 0;
        g_nm_app_s.flag_shall = NM_FLAG_SHALL_WAKE;
        g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
        g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_NORMAL;
        NM_EXIT_CRITICAL();
    } else {
        NM_ENTER_CRITICAL();
        g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
        g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_BUSOFF;
        NM_EXIT_CRITICAL();
    }
    
    if (g_flag_wakeup == 1) {
        g_flag_wakeup = 0xff;
    }
    
    #endif
    
}

void CanNm_RxIndication(PduIdType canNmPduId, Can_IdType canNmRxId, const PduInfoType *pCanNmRxPduPtr) {
}
void CanNm_TxConfirmation_com(PduIdType canNmPduId, u8 phy_id) {
    bool l_ign_logic_state;
    NM_CPU_SR_ALLOC();
    (void)canNmPduId;
    
    //(void)ReceiveMessage(MSG_R_IGN_LOGIC_SW,&l_ign_logic_state);
    
    #ifndef CAN_DRIVE_CLOSE_DEM
    #ifdef CAN_AUTOSAR_EN
    dem_set_test_result(DTC_CAN_BUS_OFF_ID, DTC_TEST_STATE_PASSED);
    #endif
    #endif
    
    #if 0
    //if(l_ign_logic_state == TRUE)
    if (0) {
        //MemberNm.IndState.dllStatus = DLL_ERROR_ACTIVE;
        //g_nm_test1[6]++;
        NM_ENTER_CRITICAL();
        g_nm_app_s.count = 0;
        g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
        g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_NORMAL;
        NM_EXIT_CRITICAL();
    } else
    #endif
    {
        //g_nm_test1[8]++;
        NM_ENTER_CRITICAL();
        if (g_nm_app_s.sub_state == NM_SUB_STATE_ACTIVE_BUSOFF) {
            can_nm_busoff_recovery_successful(phy_id);
            g_nm_app_s.count = 0;
            g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
            g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_NORMAL;
            //
        }
        NM_EXIT_CRITICAL();
    }
    
    
}

void CanNm_TxConfirmation(PduIdType canNmPduId, u8 phy_id) {
    bool l_ign_logic_state;
    NM_CPU_SR_ALLOC();
    (void)canNmPduId;
    
    //(void)ReceiveMessage(MSG_R_IGN_LOGIC_SW,&l_ign_logic_state);
    
    #ifndef CAN_DRIVE_CLOSE_DEM
    #ifdef CAN_AUTOSAR_EN
    dem_set_test_result(DTC_CAN_BUS_OFF_ID, DTC_TEST_STATE_PASSED);
    #endif
    #endif
    
    #if 0
    //if(l_ign_logic_state == TRUE)
    if (0) {
        //MemberNm.IndState.dllStatus = DLL_ERROR_ACTIVE;
        //g_nm_test1[6]++;
        NM_ENTER_CRITICAL();
        g_nm_app_s.count = 0;
        g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
        g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_NORMAL;
        NM_EXIT_CRITICAL();
    } else
    #endif
    #if 0
    {
        //g_nm_test1[8]++;
        NM_ENTER_CRITICAL();
        if (g_nm_app_s.sub_state == NM_SUB_STATE_ACTIVE_BUSOFF) {
            can_nm_busoff_recovery_successful(phy_id);
            g_nm_app_s.count = 0;
            g_nm_app_s.state = NM_MAIN_STATE_ACTIVE;
            g_nm_app_s.sub_state = NM_SUB_STATE_ACTIVE_NORMAL;
            //
        }
        NM_EXIT_CRITICAL();
    }
    #endif
}

#if 0
void CanNm_SetActiveSourceMask(nmActiveSourceType source) {
    #if 0
    if (NM_GET_ACTIVE_SOURCE(NM_ACTIVE_SOURCE_MASK_NULL)) {
        if (source == NM_ACTIVE_SOURCE_MASK_DIAGNOSE) {
            MemberNm.DcmWakeUpIf = TRUE;
        }
    }
    NM_SET_ACTIVE_SOURCE(source);
    
    if (source == NM_ACTIVE_SOURCE_MASK_KL15) {
        MemberNm.GatewayRequest = NM_STATE_GATEWAY;
    }
    #endif
}

void CanNm_ClrActiveSourceMask(nmActiveSourceType source) {
    #if 0
    NM_CLR_ACTIVE_SOURCE(source);
    
    if (source == NM_ACTIVE_SOURCE_MASK_KL15) {
        MemberNm.GatewayRequest = 0;
    }
    #endif
}

void CanNm_SetWakeUpSource(nmWakeUpSourceType source) {
    #if 0
    switch (source) {
        case NM_WAKEUP_SOURCE_WAKEUP:
            NM_SET_WAKE_SOURCE(NM_WAKEUP_SOURCE_WAKEUP);
            break;
        case NM_WAKEUP_SOURCE_KL15:
            NM_SET_WAKE_SOURCE(NM_WAKEUP_SOURCE_KL15);
            break;
        default:
            NM_SET_WAKE_SOURCE(source);
            break;
    }
    #endif
    
}
#endif





bool CanNm_is_inactive(void) {
    bool l_result = FALSE;
    
    NM_CPU_SR_ALLOC();
    
    NM_ENTER_CRITICAL();
    
    if (g_nm_app_s.state == NM_MAIN_STATE_INACTIVE) {
        l_result = TRUE;
    }
    
    NM_EXIT_CRITICAL();
    
    return l_result;
    
}



bool CanNm_is_busoff(void) {
    bool l_result = FALSE;
    
    NM_CPU_SR_ALLOC();
    
    NM_ENTER_CRITICAL();
    #if 0
    if ((g_nm_app_s.state == NM_MAIN_STATE_ACTIVE)  &&  \
        (g_nm_app_s.sub_state == NM_SUB_STATE_ACTIVE_BUSOFF)  &&  \
        (g_ls_nm_busoff.state == 1)
    ) {
        l_result = TRUE;
    }
    #else
    if ((g_nm_app_s.state == NM_MAIN_STATE_ACTIVE)   \
        && (g_nm_app_s.sub_state != NM_SUB_STATE_ACTIVE_BUSOFF)   \
        && (g_nm_app_s.busoff[CAN_ID_0].state == NM_BUSOFF_RECOVERY_STATE_NULL)
        && (g_nm_app_s.busoff[CAN_ID_1].state == NM_BUSOFF_RECOVERY_STATE_NULL)
    ) {
        l_result = FALSE;
    } else {
        l_result = TRUE;
    }
    #endif
    
    NM_EXIT_CRITICAL();
    
    return l_result;
    
}



/*****************************************************************************
|Prototype         :  ls_nm_get_com_manage_mode
|Called by         :
|Preconditions     : -
|Input parameters  : None
|Output parameters : None
|Return value      : None
|Description       : 读取对COM层的管理模式
*****************************************************************************/
u8 ls_nm_get_com_manage_mode(void) {
    return g_nm_app_s.com_mana;
}
