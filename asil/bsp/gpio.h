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

#ifndef __GPIO_H
#define __GPIO_H

#include  "common_types.h"
#include  "gpio_cfg.h"

/*lint -e749 */ /* 枚舉 not referenced*/
/*lint -e750 */ /* 宏 not referenced*/
/*lint -e751 */ /* 變量 not referenced*/
/*lint -e752 */ /* 函數 not referenced*/
/*lint -e753 */ /* 枚舉 not referenced*/
/*lint -e754 */ /* 結構體成員 not referenced*/

typedef u32  gpio_io_t;
typedef u16 gpio_id_t;

#if 1
/*定義對某一管腳讀時的數據; 電平*/
typedef enum
{
	GPIO_LEVEL_LOW = 0u,
	GPIO_LEVEL_HIGH=1u,
}gpio_output_level_e;
#endif

/*方向*/
typedef enum
{
	DDR_INPUT = 0u,
	DDR_OUTPUT =1,
}gpio_dir_e;

#if 0
#define GPIOA  PTA
#define GPIOB  PTB
#define GPIOC  PTC
#define GPIOD  PTD
#define GPIOE  PTE
#endif

/*GPIO PORT 的id值*/
typedef enum
{
	Port_id_00 = 0u,
	Port_id_01,
	Port_id_02,
	Port_id_03,
	Port_id_04,
	Port_id_05,
	Port_id_06,
	Port_id_07,
	Port_id_08,
	Port_id_09,
	Port_id_10,
	Port_id_11,
	Port_id_12,
	Port_id_13,
	Port_id_14,
	Port_id_15
}gpio_port_id_e;


/*GPIO PORT 的id值在 STM32 芯片中表示的序號*/
typedef enum
{
	Port_id_A = Port_id_00,
	Port_id_B,
	Port_id_C,
	Port_id_D,
	Port_id_E,
}gpio_port_letter_e;
#define Port_id_MAX 5u

/*PIN的id值*/
typedef enum
{
	Pin_id_00 = 0u,
	Pin_id_01,
	Pin_id_02,
	Pin_id_03,
	Pin_id_04,
	Pin_id_05,
	Pin_id_06,
	Pin_id_07,
	Pin_id_08,
	Pin_id_09,
	Pin_id_10,
	Pin_id_11,
	Pin_id_12,
	Pin_id_13,
	Pin_id_14,
	Pin_id_15,
	Pin_id_16,
	Pin_id_17,
	Pin_id_18,
	Pin_id_19,
	Pin_id_20,
	Pin_id_21,
	Pin_id_22,
	Pin_id_23,
	Pin_id_24,
	Pin_id_25,
	Pin_id_26,
	Pin_id_27,
	Pin_id_28,
	Pin_id_29,
	Pin_id_30,
	Pin_id_31,
}gpio_pin_id_e;



/*PIN的id值*/
typedef enum
{
	Pin_00_mask = ((gpio_io_t)0x00000001),
	Pin_01_mask = ((gpio_io_t)0x00000002),
	Pin_02_mask = ((gpio_io_t)0x00000004),
	Pin_03_mask = ((gpio_io_t)0x00000008),
	Pin_04_mask = ((gpio_io_t)0x00000010),
	Pin_05_mask = ((gpio_io_t)0x00000020),
	Pin_06_mask = ((gpio_io_t)0x00000040),
	Pin_07_mask = ((gpio_io_t)0x00000080),
	Pin_08_mask = ((gpio_io_t)0x00000100),
	Pin_09_mask = ((gpio_io_t)0x00000200),
	Pin_10_mask = ((gpio_io_t)0x00000400),
	Pin_11_mask = ((gpio_io_t)0x00000800),
	Pin_12_mask = ((gpio_io_t)0x00001000),
	Pin_13_mask = ((gpio_io_t)0x00002000),
	Pin_14_mask = ((gpio_io_t)0x00004000),
	Pin_15_mask = ((gpio_io_t)0x00008000),
	Pin_16_mask = ((gpio_io_t)0x00010000),
	Pin_17_mask = ((gpio_io_t)0x00020000),
	Pin_18_mask = ((gpio_io_t)0x00040000),
	Pin_19_mask = ((gpio_io_t)0x00080000),
	Pin_20_mask = ((gpio_io_t)0x00100000),
	Pin_21_mask = ((gpio_io_t)0x00200000),
	Pin_22_mask = ((gpio_io_t)0x00400000),
	Pin_23_mask = ((gpio_io_t)0x00800000),
	Pin_24_mask = ((gpio_io_t)0x01000000),
	Pin_25_mask = ((gpio_io_t)0x02000000),
	Pin_26_mask = ((gpio_io_t)0x04000000),
	Pin_27_mask = ((gpio_io_t)0x08000000),
	Pin_28_mask = ((gpio_io_t)0x10000000),
	Pin_29_mask = ((gpio_io_t)0x20000000),
	Pin_30_mask = ((gpio_io_t)0x40000000),
	Pin_31_mask = ((gpio_io_t)0x80000000),
	Pin_All = ((gpio_io_t)0xFFFFFFFF)
}gpio_pin_id_mask_e;


/*lint -e(849) */ /*枚舉中定義了相同值的成員*/

typedef enum
{
	GPIO_OPERATE_FALSE = 0u,
	GPIO_OPERATE_TRUE = !GPIO_OPERATE_FALSE
}gpio_operate_flag_e;


#define GPIO_READ_ERRO  0XFFu


typedef enum{
    GPIO_PIN0_MASK = (uint32_t)0x1,
    GPIO_PIN1_MASK = (uint32_t)0x2,
    GPIO_PIN2_MASK = (uint32_t)0x4,
    GPIO_PIN3_MASK = (uint32_t)0x8,
    GPIO_PIN4_MASK = (uint32_t)0x10,
    GPIO_PIN5_MASK = (uint32_t)0x20,
    GPIO_PIN6_MASK = (uint32_t)0x40,
    GPIO_PIN7_MASK = (uint32_t)0x80,
    GPIO_PIN8_MASK = (uint32_t)0x100,
    GPIO_PIN9_MASK = (uint32_t)0x200,
    GPIO_PIN10_MASK = (uint32_t)0x400,
    GPIO_PIN11_MASK = (uint32_t)0x800,
    GPIO_PIN12_MASK = (uint32_t)0x1000,
    GPIO_PIN13_MASK = (uint32_t)0x2000,
    GPIO_PIN14_MASK = (uint32_t)0x4000,
    GPIO_PIN15_MASK = (uint32_t)0x8000,
    GPIO_PIN_ALL_MASK = (uint32_t)0xFFFF,
}GPIO_PinMaskType;

typedef enum{
    GPIO_PIN0 = 0,
    GPIO_PIN1,
    GPIO_PIN2,
    GPIO_PIN3,
    GPIO_PIN4,
    GPIO_PIN5,
    GPIO_PIN6,
    GPIO_PIN7,
    GPIO_PIN8,
    GPIO_PIN9,
    GPIO_PIN10,
    GPIO_PIN11,
    GPIO_PIN12,
    GPIO_PIN13,
    GPIO_PIN14,
    GPIO_PIN15,
    GPIO_PIN_MAX_ONE_GROUP,
}GPIO_PinType;
 /*!< GPIO pin check, should be 0~15 */
#define IS_GPIO_PIN(PINx) ((PINx) <= GPIO_PIN15)



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





void gpio_init(void);


#include "gpio_cfg.h"


#endif  /* __GPIO_H */
