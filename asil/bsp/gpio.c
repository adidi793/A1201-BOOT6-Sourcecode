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
#include  "gpio.h"




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
void gpio_init(void)
{
	gpio_cfg_init();
	//gpio_exit_init();
}


/*PCLINT_ERROS*/ /*lint --e(961) --e(9026)*/ /* 961 不要過份依賴運算優重出級  */
#define IS_GPIO_ALL_ID(GPIO_ID) (((GPIO_ID) < Port_id_MAX)  )



 /*end line*/

