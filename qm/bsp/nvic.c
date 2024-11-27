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

/* Includes ------------------------------------------------------------------*/

#include "nvic.h"
#include "Cpu.h"
#include "boot_app.h"


/** @addtogroup RCC_ClockConfig
	* @{
	*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/


void nvic_set_vector_table(uint32_t NVIC_VectTab, uint32_t Offset)
{
	SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}


/**
	* @brief  初始化
	* @param input none:
	* @param input none:
	* @param  output:
	* @retval None
 * @calls
 * @calls by
 * @others
	*/
void nvic_init(void)
{
	//NVIC_SetVectorTable(FLASH_BASE,BOOT_APP_VECTOR_OFFSET);
	nvic_set_vector_table(0x00000000u,0x00); //
	/* Configure one bit for preemption priority */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
