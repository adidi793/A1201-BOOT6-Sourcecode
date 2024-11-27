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
/*!
** @file Cpu.h
** @version 01.00
** @brief
**
*/
/*!
**  @addtogroup Cpu_module Cpu module documentation
**  @{
*/

#ifndef Cpu_H
#define Cpu_H


/* MODULE Cpu. */
#include "common_types.h"
#include "fm33lg0xx.h"

#include "fm33lg0xx_fl.h"
#if defined(USE_FULL_ASSERT)
#include "fm33_assert.h"
#endif /* USE_FULL_ASSERT */

/*Include shared modules, which are used for whole project*/
#if 0
#include "device_registers.h"

#include "interrupt_manager.h"
#include "clock.h"
#include "osif.h"
#include "lin_driver.h"
#include "lin_common_api.h"
#include "edma_driver.h"
#include "lptmr_driver.h"
/* Including needed modules to compile this module/procedure */
#include "lin_cfg.h"
#include "lin1.h"
#include "lin2.h"
//#include "clockMan1.h"
//6#include "pin_mux.h"
//#include "dmaController1.h"
#include "lpTmr1.h"
#endif





#ifdef __cplusplus
extern "C" {
#endif

/* TBD Cpu configuration will be declared here. */

#define CPU_CCR_ALLOC()		//CPU_CCR_ALLOC()
#define ENTER_CRITICAL() 	//__disable_irq()	//{ENTER_CRITICAL();}
#define EXIT_CRITICAL()  	//__enable_irq()	//{EXIT_CRITICAL();}


#ifdef __cplusplus
}
#endif

/* END Cpu. */

#endif
/* Cpu_H */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
