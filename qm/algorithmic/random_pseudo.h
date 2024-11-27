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
#ifndef _RANDOM_PSEUDO_H_
#define _RANDOM_PSEUDO_H_

#include "common_types.h"


#define SEED_LEN_MAX 4u

//#define ADC_CHANNEL_SBAT  0


void random_init(void);

void random_start(void);

void random_get(u8 *random_data,u8 random_len);


void random_input_count(void);


#endif
