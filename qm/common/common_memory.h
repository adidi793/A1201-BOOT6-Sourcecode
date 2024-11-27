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
#ifndef __COMMON_MEMORY_H__
#define __COMMON_MEMORY_H__

/*****************************************************************************
* I N C L U D E   F I L E S
*****************************************************************************/
#include "common_types.h"

/*****************************************************************************
* D E F I N E S / M A C R O S
*****************************************************************************/

/*****************************************************************************
* T Y P E   D E F I N I T I O N S
*****************************************************************************/
typedef enum
{
	MEMORY_RESULT_DIFFERENT = 0U,
	MEMORY_RESULT_SAME = 1U
}memory_result_t;

/*****************************************************************************
* G L O B A L   V A R I A B L E S
* only configuration table allowed here,variables are not allowed!
*****************************************************************************/

/*****************************************************************************
* F U N C T I O N   P R O T O T Y P E
*****************************************************************************/
void common_memory_copys(u8* dst,const u8* src,u16 length);
void common_memory_copys_u16(u16* dst,const u16* src,u16 length);
memory_result_t common_memory_compare_u16(u16 *dst , u16 *src, u16 length);
memory_result_t common_memory_compare(u8 *dst , u8 *src, u16 length);

void common_memory_clear(u8 *dst , u16 length);
void common_memory_fill(u8 *dst , u8 value, u16 length);

u8 common_get_str_length(const char *chr);
bool_t common_string_compare(const char *ps1, const char *ps2);

#endif/*__MEMORY_STRING_H__*/

/* [] END OF FILE */