/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief  偽隨機數
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

#include "common_types.h"
//#include "error.h"
#include "crc.h"

#include "random_pseudo.h"
//#include "adc_manage.h"

#define  RAND_SEED_INIT_VAL                                1u   /* See Note #1a.                                        */

#define  RAND_LCG_PARAM_M                         0x7FFFFFFFu   /* See Note #1b2B.                                      */
#define  RAND_LCG_PARAM_A                         1103515245u   /* See Note #1b1A2.                                     */
#define  RAND_LCG_PARAM_B                              12345u   /* See Note #1b1A3.   */

u32 random_seed_cur=0;		//当前随机数值
u32 random_seed_pre=0;		//上次随机数值
u32 random_input1_count=0;

static void random_set_seed(u32 seed);
static u32 random_seed(u32 seed);
static u32 random_generate_seed(void);

/**
 * @brief  初始化
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others 注意，不能初始值为一个固定值，应考虙每个单片机的不同；在定时器初始化之后
 */
void random_init(void)
{
	u32 l_random;
	#if 0
	random_set_seed(RAND_SEED_INIT_VAL);	//第一次
	#else
	l_random = 0;   //adc_get_voltage_value(ADC_CHANNEL_SBAT);	//定时器0寄存器的计数值
	random_set_seed(l_random);	//第一次
	#endif
}



static void random_set_seed(u32 seed)
{
	random_seed_cur = seed;
}


/**
 * @brief  算法
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
static u32 random_seed(u32 seed)
{
	u32 l_random;
	l_random = ( ( (u32)RAND_LCG_PARAM_A * seed) + (u32)RAND_LCG_PARAM_B)  %  ((u32)RAND_LCG_PARAM_M + 1u);
	return l_random;
}


/**
 * @brief  产生随机数
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
static u32 random_generate_seed(void)
{
	u32 l_random;
	l_random = random_seed_cur;
	random_seed_cur = random_seed(l_random  + random_input1_count);
	return random_seed_cur;
}



/**
 * @brief  启动一次随机数的产生；
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others  防止连续两次不重复
 */
void random_start(void)
{
	random_generate_seed();
	if( random_seed_cur == random_seed_pre)
	 {
	 		random_seed_cur++;
	 }
	 else
	 {

	 }
	random_seed_pre = random_seed_cur;
}



/**
 * @brief  返回当前随机数
 * @param  input:
 * @param  output:
 * @return   none:
 * @calls
 * @calls by
 * @others
 */
void random_get(u8 *random_data,u8 random_len)
{
	//u8  i;
	u8 l_len = random_len;
	if(l_len > SEED_LEN_MAX)
	{
		l_len =SEED_LEN_MAX;
	}
	random_data[0] = (u8)(random_seed_cur >> 24);
	random_data[1] = (u8)(random_seed_cur >> 16);
	random_data[2] = (u8)(random_seed_cur >> 8);
	random_data[3] = (u8)random_seed_cur;
}







void random_input_count(void)
{
	random_input1_count++;
}














