/*
 * @Author: adidi793 3331521600@qq.com
 * @Date: 2024-12-04 16:09:07
 * @LastEditors: adidi793 3331521600@qq.com
 * @LastEditTime: 2024-12-04 16:14:09
 * @FilePath: \A1201 BOOT6 Sourcecode\qm\module\rng.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __RNG_H
#define __RNG_H


/* Includes -------------------------------------------------------------------------------------------*/
#include "fm33lg0xx_fl.h"

FL_ErrorStatus Test_RNG(void);
FL_ErrorStatus RNG_Init(void);
FL_ErrorStatus Test_CRC32_MPEG2(void);
static uint32_t Calc_Crc32_MPEG2(const uint8_t *data, uint32_t len);

#endif /* __RNG_H */

/************************ (C) COPYRIGHT FMSH *****END OF FILE****/
