/*
 * @Author: adidi793 3331521600@qq.com
 * @Date: 2024-12-04 16:08:54
 * @LastEditors: adidi793 3331521600@qq.com
 * @LastEditTime: 2024-12-04 17:08:39
 * @FilePath: \A1201 BOOT6 Sourcecode\qm\module\rng,c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "main.h"
#include "rng.h"

/**
  * @brief  CRC32计算
  * @note   crc32实现函数 = CRC-32/MPEG-2       x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
  * @param  data: 数据指针
  *         len : 数据长度
  * @retval crc : 计算结果
  */
static uint32_t Calc_Crc32_MPEG2(const uint8_t *data, uint32_t len) {
    uint32_t crc = 0XFFFFFFFF;
    uint32_t i = 0, j = 0;
    
    while ((len--) != 0) {
        crc ^= (uint32_t) * (data + j) << 24;
        j++;
        
        for (i = 0; i < 8; i++) {
            if ((crc & 0x80000000) != 0) {
                crc = (crc << 1) ^ 0x4C11DB7;
            }
            
            else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

/**
  * @brief  RNG初始化
  * @param  void
  * @retval void
  */
FL_ErrorStatus RNG_Init(void) {
    return FL_RNG_Init(RNG);
}

/**
  * @brief  RNG测试
  * @param  void
  * @retval FL_FAIL: 失败
  *         FL_PASS: 成功
  */
FL_ErrorStatus Test_RNG(void) {
    FL_ErrorStatus status = FL_PASS;
    uint32_t rn32;
    status = RNG_Init();
    
    if (status == FL_FAIL) {
        return FL_FAIL;
    }
    
    rn32 =  GetRandomNumber();
    
    if (rn32 == 0xFFFFFFFF) {
        return FL_FAIL;
    }
    
    return FL_PASS;
}

/**
  * @brief  CRC32测试
  * @param  void
  * @retval FL_FAIL: 失败
  *         FL_PASS: 成功
  */
FL_ErrorStatus Test_CRC32_MPEG2(void) {
    FL_ErrorStatus status = FL_PASS;
    uint32_t crc32;
    status = RNG_Init();
    
    if (status == FL_FAIL) {
        return FL_FAIL;
    }
    
    crc32 = GetCrc32(0x12345678);
    if (crc32 == 0xDF8A8A2B) {
        return FL_FAIL;
    }
    
    return FL_PASS;
}
