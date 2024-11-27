#ifndef __EEPROM_CFG_H
#define __EEPROM_CFG_H


/* Includes ------------------------------------------------------------------*/
#include "mf_config.h"
#include "fm33lg0xx_fl.h"

/* -----------------------------------------------------------------------A12
VIN码 A12项目为17个字节
----------------------------------------------------------------------- */
#define SIZE_OF_VIN_EE_FLASH_RAM    20u
#define VIN_BYTE0_EE_FLASH_RAM_ADDRESS  0u
#define VIN_BYTE1_EE_FLASH_RAM_ADDRESS  1u
#define VIN_BYTE2_EE_FLASH_RAM_ADDRESS  2u
#define VIN_BYTE3_EE_FLASH_RAM_ADDRESS  3u
#define VIN_BYTE4_EE_FLASH_RAM_ADDRESS  4u
#define VIN_BYTE5_EE_FLASH_RAM_ADDRESS  5u
#define VIN_BYTE6_EE_FLASH_RAM_ADDRESS  6u
#define VIN_BYTE7_EE_FLASH_RAM_ADDRESS  7u
#define VIN_BYTE8_EE_FLASH_RAM_ADDRESS  8u
#define VIN_BYTE9_EE_FLASH_RAM_ADDRESS  9u
#define VIN_BYTE10_EE_FLASH_RAM_ADDRESS 10u
#define VIN_BYTE11_EE_FLASH_RAM_ADDRESS 11u
#define VIN_BYTE12_EE_FLASH_RAM_ADDRESS 12u
#define VIN_BYTE13_EE_FLASH_RAM_ADDRESS 13u
#define VIN_BYTE14_EE_FLASH_RAM_ADDRESS 14u
#define VIN_BYTE15_EE_FLASH_RAM_ADDRESS 15u
#define VIN_BYTE16_EE_FLASH_RAM_ADDRESS 16u
#define VIN_RESERVED1_EE_FLASH_RAM_ADDRESS 17u
#define VIN_RESERVED2_EE_FLASH_RAM_ADDRESS 18u
#define VIN_RESERVED3_EE_FLASH_RAM_ADDRESS 19u

#define VIN_BYTE0_3_EE_FLASH_ADDRESS   0u
#define VIN_BYTE4_7_EE_FLASH_ADDRESS   1u
#define VIN_BYTE8_11_EE_FLASH_ADDRESS  2u
#define VIN_BYTE12_15_EE_FLASH_ADDRESS 3u
#define VIN_BYTE16_19_EE_FLASH_ADDRESS 4u
/* -----------------------------------------------------------------------A12
金手指 A12项目为16个字节
----------------------------------------------------------------------- */
#define SIZE_OF_APPLICATION_SOFTWARE_FINGERPRINT_EE_FLASH_RAM   12u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE0_EE_FLASH_RAM_ADDRESS  20u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE1_EE_FLASH_RAM_ADDRESS  21u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE2_EE_FLASH_RAM_ADDRESS  22u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE3_EE_FLASH_RAM_ADDRESS  23u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE4_EE_FLASH_RAM_ADDRESS  24u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE5_EE_FLASH_RAM_ADDRESS  25u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE6_EE_FLASH_RAM_ADDRESS  26u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE7_EE_FLASH_RAM_ADDRESS  27u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE8_EE_FLASH_RAM_ADDRESS  28u
#define APPLICATION_SOFTWARE_FINGERPRINT_RESERVED1_EE_FLASH_RAM_ADDRESS  29u
#define APPLICATION_SOFTWARE_FINGERPRINT_RESERVED2_EE_FLASH_RAM_ADDRESS  30u
#define APPLICATION_SOFTWARE_FINGERPRINT_RESERVED3_EE_FLASH_RAM_ADDRESS  31u

#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE0_3_EE_FLASH_ADDRESS  5u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE4_7_EE_FLASH_ADDRESS  6u
#define APPLICATION_SOFTWARE_FINGERPRINT_BYTE7_11_EE_FLASH_ADDRESS 7u

#define APPLICATION_SOFTWARE_FINGERPRINT_EE_FLASH_SIZE  3

/* -----------------------------------------------------------------------A12
F198 Tester Serial Number 外部工具序列号
----------------------------------------------------------------------- */
#define SIZE_OF_TESTER_SERIAL_NUMBER_EE_FLASH_RAM    16u
#define TESTER_SERIAL_NUMBER_BYTE0_EE_FLASH_RAM_ADDRESS   100u
#define TESTER_SERIAL_NUMBER_BYTE1_EE_FLASH_RAM_ADDRESS   101u
#define TESTER_SERIAL_NUMBER_BYTE2_EE_FLASH_RAM_ADDRESS   102u
#define TESTER_SERIAL_NUMBER_BYTE3_EE_FLASH_RAM_ADDRESS   103u
#define TESTER_SERIAL_NUMBER_BYTE4_EE_FLASH_RAM_ADDRESS   104u
#define TESTER_SERIAL_NUMBER_BYTE5_EE_FLASH_RAM_ADDRESS   105u
#define TESTER_SERIAL_NUMBER_BYTE6_EE_FLASH_RAM_ADDRESS   106u
#define TESTER_SERIAL_NUMBER_BYTE7_EE_FLASH_RAM_ADDRESS   107u
#define TESTER_SERIAL_NUMBER_BYTE8_EE_FLASH_RAM_ADDRESS   108u
#define TESTER_SERIAL_NUMBER_BYTE9_EE_FLASH_RAM_ADDRESS   109u
#define TESTER_SERIAL_NUMBER_BYTE10_EE_FLASH_RAM_ADDRESS   110u
#define TESTER_SERIAL_NUMBER_BYTE11_EE_FLASH_RAM_ADDRESS   111u
#define TESTER_SERIAL_NUMBER_BYTE12_EE_FLASH_RAM_ADDRESS   112u
#define TESTER_SERIAL_NUMBER_BYTE13_EE_FLASH_RAM_ADDRESS   113u
#define TESTER_SERIAL_NUMBER_BYTE14_EE_FLASH_RAM_ADDRESS   114u
#define TESTER_SERIAL_NUMBER_BYTE15_EE_FLASH_RAM_ADDRESS   115u

//4字节格式排序
#define TESTER_SERIAL_NUMBER_EE_FLASH_ADDRESS_0   25u
#define TESTER_SERIAL_NUMBER_EE_FLASH_ADDRESS_1   26u
#define TESTER_SERIAL_NUMBER_EE_FLASH_ADDRESS_2   27u
#define TESTER_SERIAL_NUMBER_EE_FLASH_ADDRESS_3   28u
#define TESTER_SERIAL_NUMBER_EE_FLASH_SIZE  4
/* -----------------------------------------------------------------------A12
F199 Programming Date 重编程日期
----------------------------------------------------------------------- */
#define SIZE_OF_Programming_Date_EE_FLASH_RAM    4u
#define Programming_Date_BYTE0_EE_FLASH_RAM_ADDRESS   116u
#define Programming_Date_BYTE1_EE_FLASH_RAM_ADDRESS   117u
#define Programming_Date_BYTE2_EE_FLASH_RAM_ADDRESS   118u
#define Programming_Date_BYTE3_EE_FLASH_RAM_ADDRESS   119u
//4字节格式排序
#define Programming_Date_EE_FLASH_ADDRESS_0   29u
#define Programming_Date_EE_FLASH_SIZE  1

/* -----------------------------------------------------------------------A12
Programming Counter  编程次数
Programming Attempts Counter 尝试编程次数
服务名_字节排序_EE_FLASH_RAM
----------------------------------------------------------------------- */
#define SIZE_OF_Programming_Counter_EE_FLASH_RAM    4u
#define Programming_Attempts_Counter_BYTE0_EE_FLASH_RAM_ADDRESS  120u
#define Programming_RESERVED1_BYTE1_EE_FLASH_RAM_ADDRESS         121u
#define Programming_Success_Counter_BYTE2_EE_FLASH_RAM_ADDRESS   122u
#define Programming_Success_Counter_BYTE3_EE_FLASH_RAM_ADDRESS   123u
//4字节格式排序
#define Programming_Counter_EE_FLASH_ADDRESS_0   30u
#define Programming_Counter_EE_FLASH_SIZE  1

/* -----------------------------------------------------------------------A12
安全刷写失败原因
----------------------------------------------------------------------- */
#define SIZE_OF_Security_flash_failure_EE_FLASH_RAM    4u
#define Security_flash_failure_BYTE0_EE_FLASH_RAM_ADDRESS  124u
#define Security_flash_failure_BYTE1_EE_FLASH_RAM_ADDRESS  125u
#define Security_flash_failure_BYTE2_EE_FLASH_RAM_ADDRESS  126u
#define Security_flash_failure_BYTE3_EE_FLASH_RAM_ADDRESS  127u
//4字节格式排序
#define Security_flash_failure_EE_FLASH_ADDRESS_0   31u
#define Security_flash_failure_EE_FLASH_SIZE  1

/* -----------------------------------------------------------------------A12
错误访问计数器 attemptcount of security access
----------------------------------------------------------------------- */
#define SIZE_OF_security_access_attemptcount_EE_FLASH_RAM    4u
#define security_access_attemptcount_BYTE0_EE_FLASH_RAM_ADDRESS  128u
#define security_access_attemptcount_BYTE1_EE_FLASH_RAM_ADDRESS  129u
#define security_access_attemptcount_BYTE2_EE_FLASH_RAM_ADDRESS  130u
#define security_access_attemptcount_BYTE3_EE_FLASH_RAM_ADDRESS  131u
//4字节格式排序
#define security_access_attemptcount_EE_FLASH_ADDRESS_0   32u
#define security_access_attemptcount_EE_FLASH_SIZE  1

void EE_Flash_Task(void);
void EE_Flash_Init(void);
uint8_t EE_FlashRamRead(uint16_t index);
#endif