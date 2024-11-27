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


#include "clk.h"
#include "mf_config.h"


//static clk_work_mode_e g_clk_work_mode;
static clk_clocks_freq_s g_clk_info;

extern void SystemInit_hsi(void);
/**
	* @brief 初始化
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void clk_init(void)
{
     ///  FL_Init();    
	MF_Clock_Init();
    
	#ifdef CLK_HSE_EN
	    /* XTHF 8M倍频至48M */
	    SelXTHFToPLL(FL_CMU_PLL_PSC_DIV8, 48 - 1);
	#else
		SystemInit_hsi();
	#endif
	//SystemCoreClockUpdate();
}


/*!
* @brief Setup the microcontroller system. DeInitialize the System.
*
* @param[in] none
* @return none
*/
void SystemDeInit (void)
{
    #if 0
    CKGEN_SetSysclkSrc(SYSCLK_SRC_INTERNAL_OSC);    ///<set internal 8M RC as sysclk first
    SPM_EnablePLL(DISABLE);     ///<Disable PLL next
    SPM_EnableXOSC(DISABLE);    ///<Disable XOSC final
    CKGEN_SetPLLReference(PLL_REF_INTERAL_OSC);
    //SPM_EnableLVD(DISABLE);  //disable LVD if need
    #endif
}



/**
  * @brief
  * @param    Source 锁相环输入参考时钟选择
This parameter can be one of the following values:
  *           @arg @ref FL_CMU_PLL_CLK_SOURCE_RCHF
  *           @arg @ref FL_CMU_PLL_CLK_SOURCE_XTHF
  * @param    PLL_REFPSC 参考时钟分频，目标分频值1M
This parameter can be one of the following values:
  *           @arg @ref FL_CMU_PLL_PSC_DIV1
  *           @arg @ref FL_CMU_PLL_PSC_DIV2
  *           @arg @ref FL_CMU_PLL_PSC_DIV4
  *           @arg @ref FL_CMU_PLL_PSC_DIV8
  *           @arg @ref FL_CMU_PLL_PSC_DIV12
  *           @arg @ref FL_CMU_PLL_PSC_DIV16
  *           @arg @ref FL_CMU_PLL_PSC_DIV24
  *           @arg @ref FL_CMU_PLL_PSC_DIV32
  * @param    PLL_DB PLL倍频比，输出时钟频率为1M*（clock+1）
This parameter can be Between parameters of the following values:
  *           @arg @ref

  * @param    PLL_OSEL PLL输出选择
This parameter can be one of the following values:
  *           @arg @ref FL_CMU_PLL_OUTPUT_X1
  *           @arg @ref FL_CMU_PLL_OUTPUT_X2
  * @retval   None
  */
void CMU_PLL_ConfigDomain_SYSTEM(uint32_t Source, uint32_t PLL_REFPSC, uint32_t PLL_DB, uint32_t PLL_OSEL)
{
    MODIFY_REG(CMU->PLLCR, CMU_PLLCR_DB_Msk | CMU_PLLCR_REFPRSC_Msk | CMU_PLLCR_OSEL_Msk | CMU_PLLCR_INSEL_Msk,
               (PLL_DB << CMU_PLLCR_DB_Pos) | PLL_REFPSC | PLL_OSEL | Source);
}

/**
  * @brief  RCHF初始化配置
  * @param  clock: RCHF振荡频率值
  * @retval void
  */
void RCHFInit(uint32_t clock)
{
    switch(clock)
    {
        case FL_CMU_RCHF_FREQUENCY_8MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;

        case FL_CMU_RCHF_FREQUENCY_16MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF16M_TRIM);
            break;

        case FL_CMU_RCHF_FREQUENCY_24MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF24M_TRIM);
            break;

        case FL_CMU_RCHF_FREQUENCY_32MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF32M_TRIM);
            break;

        default:
            FL_CMU_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;
    }

    FL_CMU_RCHF_SetFrequency(clock);
}

/**
  * @brief  选择内部RCHF作为锁相环的参考时钟
  *
  * @note   锁相环的参考时钟需要预分频至1M，再进行PLL倍频
  * @param  rchf  RCHF参考时钟选择    This parameter can be one of the following values:
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_8MHZ
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_16MHZ
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_24MHZ
  *           @arg @ref FL_CMU_RCHF_FREQUENCY_32MHZ
  * @param  clock PLL倍频输出频率
  * @note   输出时钟频率为1M*（clock+1），最高输出频率可达64M
  * @retval none
  */

void SelRCHFToPLL(uint32_t rchf, uint32_t clock)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    uint32_t counter = 0;
    uint32_t readystatus = 0;
    uint32_t div = FL_CMU_PLL_PSC_DIV8;

    if(clock > 63) { return; }

    RCHFInit(rchf);

    FL_CMU_ClearFlag_SYSCLKSELError();                  //清除时钟选择错误标志
    FL_CMU_EnableIT_SYSCLKSELError();                   //使能时钟选择错误中断

    InterruptConfigStruct.preemptPriority = 0x00;
    FL_NVIC_Init(&InterruptConfigStruct, FDET_IRQn);    //系统时钟选择错误中断

    switch(rchf)
    {
        case FL_CMU_RCHF_FREQUENCY_16MHZ:
            div = FL_CMU_PLL_PSC_DIV16;
            break;

        case FL_CMU_RCHF_FREQUENCY_24MHZ:
            div = FL_CMU_PLL_PSC_DIV24;
            break;

        case FL_CMU_RCHF_FREQUENCY_32MHZ:
            div = FL_CMU_PLL_PSC_DIV32;
            break;

        default:
            break;
    }

    if(clock <= 23)
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
    }

    else
        if((clock > 23) && (clock <= 47))
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
        }

        else
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);
        }

    CMU_PLL_ConfigDomain_SYSTEM(FL_CMU_PLL_CLK_SOURCE_RCHF, div, clock, FL_CMU_PLL_OUTPUT_X1);

    FL_CMU_PLL_Enable();
    do
    {
        readystatus = FL_CMU_IsActiveFlag_PLLReady();
        counter++;
    } while((readystatus != 0x1U) && (counter != PLL_TIMEOUT));

    FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV8);   //CDIF接口访问的最高频率建议不超过8M

    FL_CMU_SetSystemClockSource(FL_CMU_SYSTEM_CLK_SOURCE_PLL);  //系统时钟源选择PLL
    FL_CMU_SetAHBPrescaler(FL_CMU_AHBCLK_PSC_DIV1);
    FL_CMU_SetAPBPrescaler(FL_CMU_APBCLK_PSC_DIV1);
    SystemCoreClockUpdate();
}

/**
  * @brief  XTHF初始化配置
  * @param  void
  * @retval void
  */
void XTHFInit(void)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;
    //PC2.3配置成模拟功能，外接XTHF

    GPIO_InitStruct.pin        = FL_GPIO_PIN_2 | FL_GPIO_PIN_3;
    GPIO_InitStruct.mode       = FL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull       = FL_DISABLE;
    GPIO_InitStruct.remapPin   = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;     
    (void)FL_GPIO_Init(GPIOC, &GPIO_InitStruct);


    FL_CMU_XTHF_WriteDriverStrength(0x0A);  //振荡强度
    //使能XTHF
    FL_CMU_XTHF_Enable();
    FL_DelayMs(2);
    
    //FL_CMU_ClearFlag_XTHFFail();
    //FL_CMU_EnableIT_XTHFFail();   //开启XTHF停振检测中断
    //InterruptConfigStruct.preemptPriority = 0x00;
    //FL_NVIC_Init(&InterruptConfigStruct, FDET_IRQn);   
    
    SystemCoreClockUpdate();
    
}

/**
  * @brief  选择外部XTHF作为锁相环的参考时钟
  *
  * @note   锁相环的参考时钟需要预分频至1M，再进行PLL倍频
  * @param  div  对XTHF参考时钟的分频，目标分频到1M
  *
  * @param  clock PLL倍频输出频率
  * @note   输出时钟频率为1M*（clock+1），最高输出频率可达64M
  * @retval none
  */

void SelXTHFToPLL(uint32_t div, uint32_t clock)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;
    uint32_t counter = 0;
    uint32_t readystatus = 0;

    if(clock > 63) { return; }

    XTHFInit();

    FL_CMU_ClearFlag_SYSCLKSELError();                  //清除时钟选择错误标志
    FL_CMU_EnableIT_SYSCLKSELError();                   //使能时钟选择错误中断

    InterruptConfigStruct.preemptPriority = 0x00;
    FL_NVIC_Init(&InterruptConfigStruct, FDET_IRQn);    //系统时钟选择错误中断

    if(clock <= 23)
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
    }

    else
        if((clock > 23) && (clock <= 47))
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
        }

        else
        {
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);
        }

    CMU_PLL_ConfigDomain_SYSTEM(FL_CMU_PLL_CLK_SOURCE_XTHF, div, clock, FL_CMU_PLL_OUTPUT_X1);

    FL_CMU_PLL_Enable();
    do
    {
        readystatus = FL_CMU_IsActiveFlag_PLLReady();
        counter++;
    } while((readystatus != 0x1U) && (counter != PLL_TIMEOUT));
    FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV8);   //CDIF接口访问的最高频率建议不超过8M

    FL_CMU_SetSystemClockSource(FL_CMU_SYSTEM_CLK_SOURCE_PLL);
    FL_CMU_SetAHBPrescaler(FL_CMU_AHBCLK_PSC_DIV1);
    FL_CMU_SetAPBPrescaler(FL_CMU_APBCLK_PSC_DIV1);
    SystemCoreClockUpdate();
}

