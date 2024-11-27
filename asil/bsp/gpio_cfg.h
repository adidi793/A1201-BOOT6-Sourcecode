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

#ifndef GPIO_CFG_H__
#define GPIO_CFG_H__



#include  "common_types.h"
#include  "common_cfg.h"
#include "Cpu.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/


#define CAN0_TX_PIN_ID GPIO_PIN7
#define CAN0_TX_PIN_MASK GPIO_PIN7_MASK
#define CAN0_TX_GPIO GPIOA
#define CAN0_TX_FUNC_SEL  GPIO_FUN1


#define CAN0_RX_PIN_ID GPIO_PIN6
#define CAN0_RX_PIN_MASK GPIO_PIN6_MASK
#define CAN0_RX_GPIO GPIOA
#define CAN0_RX_FUNC_SEL  GPIO_FUN1


#define CAN0_STB_PIN_ID GPIO_PIN8
#define CAN0_STB_PIN_MASK GPIO_PIN8_MASK
#define CAN0_STB_GPIO GPIOA
#define CAN0_STB_FUNC_SEL  GPIO_FUN0


#define TEST2_ON()       //FL_GPIO_SetOutputPin(TEST2_GPIO,TEST2_PIN_MASK)
#define TEST2_OFF()          //FL_GPIO_ResetOutputPin(TEST2_GPIO,TEST2_PIN_MASK)
#define TEST2_TOGGLE()		//FL_GPIO_ToggleOutputPin(TEST2_GPIO,TEST2_PIN_MASK);

#define TEST1_ON()         //FL_GPIO_SetOutputPin(TEST1_GPIO,TEST1_PIN_MASK)
#define TEST1_OFF()        //FL_GPIO_ResetOutputPin(TEST1_GPIO,TEST1_PIN_MASK)
#define TEST1_TOGGLE()		//FL_GPIO_ToggleOutputPin(TEST1_GPIO,TEST1_PIN_MASK);


#define CAN0_STB_ON()         FL_GPIO_ResetOutputPin(CAN0_STB_GPIO,CAN0_STB_PIN_MASK)
#define CAN0_STB_OFF()        FL_GPIO_SetOutputPin(CAN0_STB_GPIO,CAN0_STB_PIN_MASK)


#define CAN0_STB_INIT_VALUE GPIO_LEVEL_HIGH
#define CAN0_STB_EN_VALUE GPIO_LEVEL_LOW

#define LIN_SLP_INIT_VALUE GPIO_LEVEL_LOW
#define LIN_SLP_EN_VALUE GPIO_LEVEL_HIGH

//void TEST1_TOGGLE(void);
//void TEST2_TOGGLE(void);

#define TEST_1_PIN_TOG() //PINS_DRV_TogglePins(TEST_1_GPIO,TEST_1_PIN_MASK)
#define TEST_2_PIN_TOG() //PINS_DRV_TogglePins(TEST_2_GPIO,TEST_2_PIN_MASK)

#define LED_BLUE_PIN_TOG()// PINS_DRV_TogglePins(LED_BLUE_GPIO,LED_BLUE_PIN_MASK)
#define LED_GREEN_PIN_TOG() //PINS_DRV_TogglePins(LED_GREEN_GPIO,LED_GREEN_PIN_MASK)



void gpio_cfg_init(void);
//void gpio_goto_sleep(void);
//void gpio_goto_wakeup(void);
//void gpio_cfg_re_init(void);






#endif   /* __CLK_CFG_H__ */
