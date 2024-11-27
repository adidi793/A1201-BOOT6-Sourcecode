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

#ifndef CLK_H___
#define CLK_H___
/* MODULE clockMan1. */


/* Include inherited beans */
#include "common_types.h"
#include "clk_cfg.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/



//自己手動加的
typedef enum
{
	CLK_SYS_SRC_NULL=0,
	CLK_SYS_SRC_OSC=1,
	CLK_SYS_SRC_SIRC=2,
	CLK_SYS_SRC_FIRC=3,
	CLK_SYS_SRC_SPLL=6
}clk_sys_source_e;

typedef struct
{
	u8 error;
	//clk_work_mode_e mode;
	//clk_sys_source_e sys_src;
	u32 osc_freq;
	u32 firc_freq;
	u32 sirc_freq;
	u32 vco_freq;     /*!< returns SYSCLK clock frequency expressed in Hz */
	u32 spll_freq;     /*!< returns SYSCLK clock frequency expressed in Hz */
	u32 core_freq;     /*!< returns SYSCLK clock frequency expressed in Hz */
	u32 sysclk_freq;  /*!< returns SYSCLK clock frequency expressed in Hz */
	u32 bus_freq;    /*!< returns HCLK clock frequency expressed in Hz */
	u32 flash_freq;   /*!< returns PCLK1 clock frequency expressed in Hz */
	u32 spll1_freq;   /*!< returns PCLK2 clock frequency expressed in Hz */
	u32 spll2_freq;   /*!< returns PCLK2 clock frequency expressed in Hz */
	u32 firc1_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 firc2_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 sirc1_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 sirc2_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 sosc1_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 sosc2_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 rtc_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 lpo_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
	u32 rtc_out_freq;  /*!< returns ADCCLK clock frequency expressed in Hz */
}clk_clocks_freq_s;


void clk_init(void);
void clk_get_clock_frequence(clk_clocks_freq_s* clk_freq);
void clk_update_clock_frequence(void);
void SystemDeInit (void);

void FDET_IRQHandler(void);
void CMU_PLL_ConfigDomain_SYSTEM(uint32_t Source, uint32_t PLL_REFPSC, uint32_t PLL_DB, uint32_t PLL_OSEL);

void SelRCHFToPLL(uint32_t rchf, uint32_t clock);
void SelXTHFToPLL(uint32_t div, uint32_t clock);

void RCHFInit(uint32_t clock);
void XTHFInit(void);

#endif
/* ifndef CLK_H___ */

