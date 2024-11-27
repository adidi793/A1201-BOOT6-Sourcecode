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


#ifndef __TYPES_H__
#define __TYPES_H__

/*lint -e36 */ /* 不認識的字符*/
/*lint -e19 */ /* 不認識的字符*/
/*lint -e808 */ /* 不認識的字符*/
/*lint -e10 */ /* 不認識的字符*/
/*lint -e761 */ /* 不認識的字符*/
/*lint -e830 */ /* 不認識的字符*/
/*lint -e751 */ /* not referenced*/
/*lint -e750 */ /*not referenced*/
/*lint -e749 */ /*not referenced*/
/************************************各變量類型***********************************/

#include <stdbool.h>
#if 1   /*如果變量的定義使用IAR自帶的h文件中定義*/

/*#include <stdint.h>*/                     /*!< standard types definitions                      */
#include <stdint.h>
#else

/*#include "stdint_copy.h"    */                /*!< standard types definitions                      */

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;
typedef   signed long long    int64_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
/*STM32芯片和 STM8芯片 調試發現 SHORT 都是16位  */
typedef unsigned short    uint16_t;
/*STM32芯片調試發現 INT 和 LONG 都是32位 ;
	而在STM8時 INT 是16位  */
typedef unsigned long     uint32_t;   
typedef unsigned long long    uint64_t;

typedef float    float32_t;
typedef double    float64_t;
typedef long double    float128_t;


#define     __VC    volatile const         /*!< defines 'read only' permissions     */
#define     __I     volatile const         /*!< defines 'read only' permissions     */
#define     __O     volatile               /*!< defines 'write only' permissions     */
#define     __IO    volatile               /*!< defines 'read / write' permissions   */

#endif

#define     __VC    volatile const         /*!< defines 'read only' permissions     */
#define     __I     volatile const         /*!< defines 'read only' permissions     */
#define     __O     volatile               /*!< defines 'write only' permissions     */
#define     __IO    volatile               /*!< defines 'read / write' permissions   */

/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int64_t sc64;  /*!< Read Only */
typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

typedef uint16_t size_n_t;

#if 0
typedef float32_t    f32;
typedef float64_t    f64;
typedef float128_t    f128;

typedef const float32_t    fc32;
typedef const  float64_t    fc64;
typedef const  float128_t    fc128;

typedef __IO float32_t    vf32;
typedef __IO  float64_t   vf64;
typedef __IO  float128_t    vf128;


typedef __I float32_t    vfc32;
typedef __I  float64_t   vfc64;
typedef __I  float128_t    vfc128;
#endif


typedef enum 
{
		RESET = 0, 
		SET = !RESET
/*} FlagStatus, ITStatus,BitStatus, BitAction;*/
} FlagStatus, ITStatus,BitStatus;
#if 0
typedef enum 
{
		DISABLE = 0,
		ENABLE = !DISABLE
} FunctionalState;
#endif

/*lint --e(961) */ /*函數的使用優先選擇函數宏 Function-like macro defined */
//#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//#define IS_FUNCTIONALSTATE_OK(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
#if 0
typedef enum 
{
		ERROR = 0, 
		SUCCESS = !ERROR
} ErrorStatus;
#endif
#if 0
typedef enum
{
	FALSE = 0u,      /* 假 */
	TRUE  = !FALSE   /* 真 */
}bool;  /* 布爾運算 */
#else
//#define FALSE false
//#define TRUE true
typedef enum
{
	FALSE_M = 0u,      /* 假 */
	TRUE_M  = !FALSE_M   /* 真 */
}bool_m_t;  /* 布爾運算 */

#endif

//typedef FunctionalState functional_status_e;
typedef ITStatus it_status_e;


/*操作的結果*/
typedef enum 
{
	FAILED = 0u,      /* 失敗 */
	RIGHT = !FAILED  /* 成功 */
}operate_status_e;



typedef  bool bool_t;   /*  */


//#define  NULL   0
#define  NULL_POINT   0
#ifndef NULL_PTR
#define  NULL_PTR   (void*)0
#endif


/*********符號類型的值範圍************/

#define U8_MAX     (255)
#define S8_MAX     (127)
#define S8_MIN     (-128)
#define U16_MAX    (65535u)
#define S16_MAX    (32767)
#define S16_MIN    (-32768)
#define U32_MAX    (4294967295uL)
#define S32_MAX    (2147483647)
#define S32_MIN    (-2147483648uL)

#if 0
/*適合STM32F4 DSP IB*/
#define INT32_MIN  S32_MIN
#define INT32_MAX  S32_MAX
#define UINT32_MAX  U32_MAX
#define INT16_MIN  S16_MIN
#define INT16_MAX  S16_MAX
#define UINT16_MAX  U16_MAX
#define INT8_MIN  S8_MIN
#define INT8_MAX  S8_MAX
#define UINT8_MAX  U8_MAX
#endif


typedef union
{
	uint16_t w;
	struct{
	uint8_t b0; /*0 for CW 为HIGH_BYTE, FOR PICC 为LOW_BYTE */
	uint8_t b1; /* 0 for CW 为LIGH_BYTE, FOR PICC 为HIGH_BYTE */
	}b;
	uint8_t byt[2];
}u16_swap_t;





typedef union
{
	uint32_t dw;
	struct{
	uint8_t b0;/*0  for CW 为HIGH_BYTE, FOR PICC 为LOW_BYTE */
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
	}b;
	uint16_t wd[2];
	uint8_t byt[4];
}u32_swap_t;

typedef union
{
	uint32_t dw;
	uint16_t a[2];/* 0 for CW 为HIGH_WORD, FOR PICC 为LOW_WORD */
	uint8_t  b[4];/* 0 for CW 为HIGH_BYTE, FOR PICC 为LOW_BYTE */
}u32_swap1_t;


typedef union
{
	uint32_t dw[2];
	struct{
	u32_swap_t dw_l;
	u32_swap_t dw_h;
	}lb;
}u64_swap_t;



#ifndef TRUE                /* conditional check */
    #define TRUE      1
#endif

#ifndef FALSE               /* conditional check */
    #define FALSE     0
#endif


#if 0



/*************各變量類型的簡寫**********************/

/*********有符號類型************/
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef const int8_t sc8;    /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int32_t sc32;  /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;    /*!< Read Only */


/*********無符號類型************/
typedef uint8_t  u8 ;       /* USARTdiv 的小數部分  */
typedef uint16_t u16 ;      /* ENABLE or DISABLE  接收緩衝區DMA使能 */  
typedef uint32_t  u32;
typedef uint64_t  u64;


typedef const uint8_t uc8;       /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint32_t uc32;     /*!< Read Only */

typedef __IO uint8_t  vu8;
typedef __IO uint16_t vu16;
typedef __IO uint32_t  vu32;

typedef __I u32 vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */


typedef void (*tIntFunc)(void);


/*每一類型的長度(單位是位數)*/
#define  INT_08_NBR_BITS      8u
#define  INT_16_NBR_BITS      16u
#define  INT_32_NBR_BITS      32u
#define  INT_64_NBR_BITS      64u

/*每一類型的長度(單位是字節)*/
#define  INT_08_NBR_BYTES      1u
#define  CPU_INT_16_NBR_BYTES      2u
#define  CPU_INT_32_NBR_BYTES      4u
#define  INT_64_NBR_BYTES      8u

/************************************以上是各變量類型***********************************/




/************************************各枚舉標誌定義***********************************/

typedef enum
{
	FALSE = 0u,      /* 假 */
	TRUE  = !FALSE   /* 真 */
}bool;  /* 布爾運算 */


typedef enum 
{
	RESET = 0u,    /* 復位 */
	SET = !RESET   /* 設置 */
}set_status_e,it_status_e,bit_status_e,bit_action_e;





/*判斷的結果*/
typedef enum 
{
	ERROR = 0u,       /* 錯誤 */
	RIGHT = !ERROR     /* 正確 */
}judge_status_e,erro_status_e;






typedef enum 
{
	UNBUSY = 0u,    /* 不在忙 */
	BUSY = !UNBUSY  /* 忙 */
} busy_status_e;


typedef enum 
{
	UNDONE = 0u,     /* 未完成 */
	DONE = !UNDONE    /* 完成 */
} done_status_e; 

#if 0
	
/*電平值 */
typedef enum
{ 
	GPIOP_LOW = 0u,   /* 低電平 */
	GPIOP_HIGH = !GPIOP_LOW   /* 高電平 */
}gpio_pin_value_e;

#endif

#if 0

/*總的錯誤提示 */
typedef enum
{ 
	ERR_OK = (u8)0,      /* OK */
	ERR_SPEED = (u8)1,  /* This device does not work in the active speed mode. */
	ERR_RANGE = (u8)2,  /* Parameter out of range. */
	ERR_VALUE = (u8)3,    /* Parameter of incorrect value. */
	ERR_OVERFLOW = (u8)4,                /* Timer overflow. */
	ERR_MATH  = (u8)5  ,                 /* Overflow during evaluation. */
	ERR_ENABLED  = (u8)6,                 /* Device is enabled. */
	ERR_DISABLED = (u8)7,                /* Device is disabled. */
	ERR_BUSY    = (u8)8 ,                 /* Device is busy. */
	ERR_NOTAVAIL = (u8)9,                 /* Requested value or method not available. */
	ERR_RXEMPTY = (u8)10,                /* No data in receiver. */
	ERR_TXFULL  = (u8)11,                /* Transmitter is full. */
	ERR_BUSOFF = (u8)12,                /* Bus not available. */
	ERR_OVERRUN  = (u8)13,              /* Overrun error is detected. */
	ERR_FRAMING = (u8)14,                /* Framing error is detected. */
	ERR_PARITY = (u8)15,                 /* Parity error is detected. */
	ERR_NOISE  = (u8)16,                 /* Noise error is detected. */
	ERR_IDLE   = (u8)17,                 /* Idle error is detected. */
	ERR_FAULT   = (u8)18,                /* Fault error is detected. */
	ERR_BREAK  = (u8)19,                 /* Break char is received during communication. */
	ERR_CRC    = (u8)20,                 /* CRC error is detected. */
	ERR_ARBITR  = (u8)21 ,               /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
	ERR_PROTECT   = (u8)22   ,           /* Protection error is detected. */
	ERR_UNDERFLOW  = (u8)23  ,           /* Underflow error is detected. */
	ERR_UNDERRUN   = (u8)24  ,           /* Underrun error is detected. */
	ERR_COMMON    = (u8)25   ,           /* Common error of a device. */
	ERR_LINSYNC   = (u8)26   ,           /* LIN synchronization error is detected. */
	ERR_FAILED   = (u8)27    ,           /* Requested functionality or process failed. */
	ERR_QFULL    = (u8)28    ,           /* Queue is full. */ 
	ERR_MAX = (u8)29 ,
	ERR_NULL = (u8)0xff	   /* 無 */
}erro_flag_e;

#else

#define    ERR_OK  (u8)0      /* OK */
#define    ERR_SPEED  (u8)1  /* This device does not work in the active speed mode. */
#define    ERR_RANGE  (u8)2  /* Parameter out of range. */
#define    ERR_VALUE  (u8)3    /* Parameter of incorrect value. */
#define    ERR_OVERFLOW  (u8)4                /* Timer overflow. */
#define    ERR_MATH   (u8)5                   /* Overflow during evaluation. */
#define    ERR_ENABLED   (u8)6                 /* Device is enabled. */
#define    ERR_DISABLED  (u8)7                /* Device is disabled. */
#define    ERR_BUSY     (u8)8                  /* Device is busy. */
#define    ERR_NOTAVAIL  (u8)9                 /* Requested value or method not available. */
#define    ERR_RXEMPTY  (u8)10                /* No data in receiver. */
#define    ERR_TXFULL   (u8)11                /* Transmitter is full. */
#define    ERR_BUSOFF  (u8)12                /* Bus not available. */
#define    ERR_OVERRUN   (u8)13               /* Overrun error is detected. */
#define    ERR_FRAMING  (u8)14                /* Framing error is detected. */
#define    ERR_PARITY  (u8)15                 /* Parity error is detected. */
#define    ERR_NOISE   (u8)16                 /* Noise error is detected. */
#define    ERR_IDLE    (u8)17                 /* Idle error is detected. */
#define    ERR_FAULT    (u8)18                /* Fault error is detected. */
#define    ERR_BREAK   (u8)19                 /* Break char is received during communication. */
#define    ERR_CRC    (u8)20                 /* CRC error is detected. */
#define    ERR_ARBITR   (u8)21                /* A node losts arbitration. This error occurs if two nodes start transmission at the same time. */
#define    ERR_PROTECT    (u8)22              /* Protection error is detected. */
#define    ERR_UNDERFLOW   (u8)23             /* Underflow error is detected. */
#define    ERR_UNDERRUN    (u8)24             /* Underrun error is detected. */
#define    ERR_COMMON     (u8)25              /* Common error of a device. */
#define    ERR_LINSYNC    (u8)26              /* LIN synchronization error is detected. */
#define    ERR_FAILED    (u8)27               /* Requested functionality or process failed. */
#define    ERR_QFULL     (u8)28               /* Queue is full. */ 
#define    ERR_MAX  (u8)29 
#define    ERR_NULL  (u8)0xff	   /* 無 */


#endif



/************************************以上是枚舉標誌定義***********************************/



/************************************基本運算的公用函數宏***********************************/


/*************基本運算和參數命名無關*******************/

#define clear_reg(reg)        ((reg) = (0x0))    /* 清零 */
#define write_reg(reg, val)   ((reg) = (val))     /* 寫入 */
#define read_reg(reg)         ((reg))               /* 讀取 */

#define set_8bits_mask(reg, mask)     ((reg) |= (u8)(mask))    /* 將mask中的值為1的那幾位置1 */
#define clear_8bits_mask(reg, mask)   ((reg) &= (u8)~(mask))   /* 將mask中的值為0的那幾位清0 */
#define get_8bits_mask(reg, mask)    ((reg) & (u8)(mask))     /* 讀取mask中的值為1的那幾位的情況 */
#define change_8Bits_mask(reg, mask)   ((reg) ^= (u8)(mask))     /* 將mask中值為1的那幾位最反 */
#define modify_8bits_reg(reg, clearmask, setmask)  write_reg((reg), (((read_reg(reg)) & (~(u8)(clearmask))) | (u8)(setmask)))
/*#define set_8bits_mask_val(reg, mask, val)   ( ((val) == 0) ? clear_8bits_mask(reg, mask) : (set_8bits_mask(reg, mask)))*/
#define set_8bits_bit(reg, bit)     ((reg) |= (u8)((u8)0x01)<<(bit))    /*reg 第bit位置一*/
#define clear_8bits_bit(reg, bit)     ((reg) &= ~(u8)((u8)0x01)<<(bit))    /*reg 第bit位清0*/

#define set_16bits_mask(reg, mask)     ((reg) |= (u16)(mask))    /* 將mask中的值為1的那幾位置1 */
#define clear_16bits_mask(reg, mask)   ((reg) &= (u16)~(mask))   /* 將mask中的值為0的那幾位清0 */
#define get_16bits_mask(reg, mask)    ((reg) & (u16)(mask))     /* 讀取mask中的值為1的那幾位的情況 */
#define change_16Bits_mask(reg, mask)   ((reg) ^= (u16)(mask))      /* 將mask中值為1的那幾位最反 */
#define modify_16bits_reg(reg, clearmask, setmask)  write_reg((reg), (((read_reg(reg)) & (~(u16)(clearmask))) | (u16)(setmask)))
/*#define set_16bits_mask_val(reg, mask, val)   ( ((val) == 0) ? clear_16bits_mask(reg, mask) : (set_16bits_mask(reg, mask)))*/
#define set_16bits_bit(reg, bit)     ((reg) |= (u16)((u16)0x01)<<(bit))    /*reg 第bit位置一*/
#define clear_16bits_bit(reg, bit)     ((reg) &= ~(u16)((u16)0x01)<<(bit))    /*reg 第bit位清0*/

#define set_32bits_mask(reg, mask)     ((reg) |= (u32)(mask))    /* 將mask中的值為1的那幾位置1 */
#define clear_32bits_mask(reg, mask)   ((reg) &= (u32)~(mask))   /* 將mask中的值為0的那幾位清0 */
#define get_32bits_mask(reg, mask)    ((reg) & (u32)(mask))     /* 讀取mask中的值為1的那幾位的情況 */
#define change_32Bits_mask(reg, mask)   ((reg) ^= (u32)(mask))    /* 將mask中值為1的那幾位最反 */
#define modify_32bits_reg(reg, clearmask, setmask)  write_reg((reg), (((read_reg(reg)) & (~(u32)(clearmask))) | (u32)(setmask)))
/*#define set_32bits_mask_val(reg, mask, val)   ( ((val) == 0) ? clear_32bits_mask(reg, mask) : (set_32bits_mask(reg, mask)))*/
#define set_32bits_bit(reg, bit)     ((reg) |= (u32)((u32)0x01)<<(bit))    /*reg 第bit位置一*/
#define clear_32bits_bit(reg, bit)     ((reg) &= ~(u32)((u32)0x01)<<(bit))    /*reg 第bit位清0*/


#define bits8_to_bits16(var,l,h)  ((var) = ((u16)(l)) | (((u16)(h)) << 8))   /*兩個8位轉成16位*/
#define bits16_to_bits8(l,h,val) ((l)= (u8)(val), (h) = (u8)((val) >> 8))  /*16位轉成兩個高低8位;h為高 l為低*/
#define bits16_to_bits32(var,l,h)  ((var) = ((u32)(l)) | (((u32)(h)) << 16))   /*兩個16位轉成32位*/
#define bits32_to_bits16(l,h,val) ((l) = (u16)(val), (h) = (u16)((val) >> 8))  /*16位轉成兩個高低8位;h為高 l為低*/


#define output(P, V) ((P) = (V))  /*v的值賦給p*/
#define input(P) (P)            





#if 0



/** @addtogroup Exported_macro
	* @{
	*/

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))



#endif



#endif


#endif   /* __CPU_TYPES_H__ */




