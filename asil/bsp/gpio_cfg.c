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

#include "gpio.h"
#include "nvic.h"
#include "wdog.h"
//#include "power_manager_app.h"


/**
* GPIO_ADC_Init
*
* @param[in]  none
* @return none
*
* @brief  init led control gpio
*
*/
void gpio_can_init(void)
{
    FL_GPIO_InitTypeDef      GPIO_InitStruct = {0};
    /* PC2.3配置成模拟功能，外接XTHF */
    /*-----------------------------------GPIO初始化---------------------------------------*/
    GPIO_InitStruct.pin          = CAN0_TX_PIN_MASK;  /* 48PIN开发板引脚定义 */
    GPIO_InitStruct.mode         = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.pull         = FL_ENABLE;
    GPIO_InitStruct.outputType   = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.remapPin     = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(CAN0_TX_GPIO, &GPIO_InitStruct); /* 48PIN开发板引脚定义 */

    GPIO_InitStruct.pin          = CAN0_RX_PIN_MASK;   /* 48PIN开发板引脚定义 */
    GPIO_InitStruct.mode         = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.pull         = FL_ENABLE;
    GPIO_InitStruct.outputType   = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.remapPin     = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(CAN0_RX_GPIO, &GPIO_InitStruct); /* 48PIN开发板引脚定义 */

    /* STB引脚初始化 */
    GPIO_InitStruct.pin          = CAN0_STB_PIN_MASK;  /* 48PIN开发板引脚定义 */
    GPIO_InitStruct.mode         = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.pull         = FL_ENABLE;
    GPIO_InitStruct.outputType   = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.remapPin     = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(CAN0_STB_GPIO, &GPIO_InitStruct);

    CAN0_STB_ON();
}





/**
 * @brief
 * @param  input:
 * @param  output:
* @return   none:
 * @calls
 * @calls by
 * @others
 */
void gpio_cfg_init(void)
{
    gpio_can_init();
}



void gpio_cfg_re_init(void)
{
	gpio_cfg_init();
}


 /*end line*/

