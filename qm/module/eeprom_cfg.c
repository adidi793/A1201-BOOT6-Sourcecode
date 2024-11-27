/*
 * @Author: lin-boyi linboyi@163.com
 * @Date: 2024-09-14 11:59:36
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-10-15 13:50:48
 * @FilePath: \A1201 BOOT5\qm\module\eeprom_cfg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "eeprom_cfg.h"
#include "eeprom.h"


__root uint8_t u8EE_Flash_Ram[4096] __attribute__((section(".EE_Flash_Ram")));
void EE_Flash_Init(void)
{
    uint16_t i;
    uint32_t data;

    for(i = 0; i < NumbOfVar; i ++)
    {
        if(EE_ReadVariable(i , &data) == 1)
        {
            u8EE_Flash_Ram[i * 4] = 0;
            u8EE_Flash_Ram[i * 4 + 1] = 0;
            u8EE_Flash_Ram[i * 4 + 2] = 0;
            u8EE_Flash_Ram[i * 4 + 3] = 0;
        }
        else
        {
            u8EE_Flash_Ram[i * 4] = (uint8_t)data;
            u8EE_Flash_Ram[i * 4 + 1] = (uint8_t)data >> 8;
            u8EE_Flash_Ram[i * 4 + 2] = (uint8_t)data >> 16;
            u8EE_Flash_Ram[i * 4 + 3] = (uint8_t)data >> 24;
        }
    }
}


void EE_Flash_Task(void)
{
    static uint16_t ee_flash_addr = 0;
    uint32_t data;
    uint8_t buf[4];
    
    if(ee_flash_addr >= NumbOfVar)
    {
        ee_flash_addr = 0;
    }
    if(EE_ReadVariable(ee_flash_addr , &data) == 1)
    {
        data = 0xffffffffu;
    }
    buf[0] = (uint8_t)data;
    buf[1] = (uint8_t)(data >> 8);
    buf[2] = (uint8_t)(data >> 16);
    buf[3] = (uint8_t)(data >> 24);

    if((u8EE_Flash_Ram[ee_flash_addr * 4] != buf[0]) || (u8EE_Flash_Ram[ee_flash_addr * 4 + 1] != buf[1]) \
    || (u8EE_Flash_Ram[ee_flash_addr * 4 + 2] != buf[2]) || (u8EE_Flash_Ram[ee_flash_addr * 4 + 3] != buf[3]))
    {
        data = u8EE_Flash_Ram[ee_flash_addr * 4];
        data <<= 8;
        data |= u8EE_Flash_Ram[ee_flash_addr * 4 + 1];
        data <<= 8;
        data |= u8EE_Flash_Ram[ee_flash_addr * 4 + 2];
        data <<= 8;
        data |= u8EE_Flash_Ram[ee_flash_addr * 4 + 3];
        EE_WriteVariable(ee_flash_addr, data);
    }
    ee_flash_addr ++;

}


uint8_t EE_FlashRamRead(uint16_t index)
{
    return (u8EE_Flash_Ram[index]);
}
