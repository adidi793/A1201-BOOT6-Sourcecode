/**
	******************************************************************************
	* @file
	* @author
	* @version
	* @date
	* @brief   主要用於批量處理數據，如初始化，復制，清除等等
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

/*****************************************************************************
* I N C L U D E   F I L E S
*****************************************************************************/
#include "common_memory.h"
#include "Cpu.h"

/*****************************************************************************
* D E F I N E S / M A C R O S
*****************************************************************************/

/*****************************************************************************
* T Y P E   D E F I N I T I O N S
*****************************************************************************/

/*****************************************************************************
* S T A T I C   V A R I A B L E S
*****************************************************************************/

/*****************************************************************************
* G L O B A L   V A R I A B L E S
*****************************************************************************/

/*****************************************************************************
* C O N S T A N T S
*****************************************************************************/

/*****************************************************************************
* F U N C T I O N   P R O T O T Y P E
*****************************************************************************/

/*****************************************************************************
|Prototype			: void common_memory_copys(u8* dst,const u8* src,u16 length)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: None
|Description		: u8类型的数据拷贝
*****************************************************************************/
void common_memory_copys(u8 *dst, const u8 *src, u16 length) {
    u16 i = 0;
    while (length--) {
        dst[i] = src[i];
        i++;
    }
} /* End of function common_memory_copys*/

/*****************************************************************************
|Prototype			: void common_memory_copys_u16(u16* dst,const u16* src,u16 length)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: None
|Description		: u16类型的数据拷贝
*****************************************************************************/
void common_memory_copys_u16(u16 *dst, const u16 *src, u16 length) {
    u16 i = 0;
    while (length--) {
        dst[i] = src[i];
        i++;
    }
} /* End of function common_memory_copys_u16*/

/*****************************************************************************
|Prototype			: void common_memory_clear(u8 *dst , u16 length)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: None
|Description		: u8类型的数据清零
*****************************************************************************/
void common_memory_clear(u8 *dst, u16 length) {
    u16 i = 0;
    while (length--) {
        dst[i] = 0u;
        i++;
    };
} /* End of function common_memory_clear*/

/*****************************************************************************
|Prototype			: u8 common_get_str_length(const char *chr)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: None
|Description		: 获取str的长度
*****************************************************************************/
u8 common_get_str_length(const char *chr) {
    u8 len = 0;
    /*求字符串s1的长度  */
    /*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
    for (len = 0u ; * (chr + len) != '\0' ; len++) { /*lint !e960 */
        ;
    }
    return len;
} /* End of function common_get_str_length*/

/*****************************************************************************
|Prototype			: bool_t common_string_compare(const char *ps1, const char *ps2)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: 相同返回1；不同返回0
|Description		: 比较字符串的內容是否相同；
*****************************************************************************/
/*lint -efunc(818,common_string_compare) */ /*（建議）16.7 函數的指針參數 如果不是用來修改的話，建議聲明指向 const 類型*/
bool_t common_string_compare(const char *ps1, const char *ps2) {
    u8 i;
    u8 n1 = common_get_str_length(ps1), n2 = common_get_str_length(ps2);
    /*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
    bool_t l_result = TRUE ;  /*PRQA S 4420*/
    
    /*字符长度不同，一定內容不同*/
    if (n1 != n2) {
        /*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
        l_result = FALSE;  /*PRQA S 4420*/
        /* return FALSE; */
    } else {
        for (i = 0u; i < n1; i++) {
            /*PCLINT_ERROS_931*/	/*lint --e(931) */ /* 	931: Both sides have side effects [MISRA 2004 Rule 1.2] 兩邊等式都有副作用 */
            if (*ps1++ != *ps2++) {
                /* return FALSE; */
                /*PCLINT_ERROS_960*/	/*lint --e(960)*/  /*過虙10.1 類型的強制隱制轉換 */
                l_result = FALSE;  /*PRQA S 4420*/
                break;
            }
        }
    }
    return l_result;
} /* End of function common_string_compare*/

/*****************************************************************************
|Prototype			: void common_memory_fill(u8 *dst , u8 value, u16 length)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: None
|Description		: 将一串数填充为相同的值
*****************************************************************************/
void common_memory_fill(u8 *dst, u8 value, u16 length) {
    u16 i;
    
    for (i = 0; i < length; i++) {
        dst[i] = value;
    }
} /* End of function common_memory_fill*/

/*****************************************************************************
|Prototype			: void common_memory_copys_u16(u16* dst,const u16* src,u16 length)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: MEMORY_RESULT_DIFFERENT/ MEMORY_RESULT_SAME
|Description		: u8类型的两个数组比较
*****************************************************************************/
memory_result_t common_memory_compare(u8 *dst, u8 *src, u16 length) {
    u16 i;
    
    for (i = 0; i < length; i++) {
        if (dst[i] != src[i]) {
            return MEMORY_RESULT_DIFFERENT;
        }
    }
    
    return MEMORY_RESULT_SAME;
} /* End of function common_memory_compare*/

/*****************************************************************************
|Prototype			: void common_memory_copys_u16(u16* dst,const u16* src,u16 length)
|Called by			: None
|Preconditions		: None
|Input parameters	: None
|Output parameters	: None
|Return value		: MEMORY_RESULT_DIFFERENT/ MEMORY_RESULT_SAME
|Description		: u16类型的两个数组比较
*****************************************************************************/
memory_result_t common_memory_compare_u16(u16 *dst, u16 *src, u16 length) {
    u16 i;
    
    for (i = 0; i < length; i++) {
        if (dst[i] != src[i]) {
            return MEMORY_RESULT_DIFFERENT;
        }
    }
    
    return MEMORY_RESULT_SAME;
} /* End of function common_memory_compare_u16*/

/* [] END OF FILE */