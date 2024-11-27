/*
** ###################################################################
**     Processor:           S32K11x with mm KB SRAM
**     Compiler:            GNU C Compiler
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright 2018 NXP
**     All rights reserved.
**
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x00000200;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x00000200;
STACK_CHECK_S_ADDR = 0x210;

/* If symbol __flash_vector_table__=1 is defined at link time
 * the interrupt vector will not be copied to RAM.
 * Warning: Using the interrupt vector from Flash will not allow
 * INT_SYS_InstallHandler because the section is Read Only.
 */
M_VECTOR_RAM_SIZE = DEFINED(__flash_vector_table__) ? 0x0 : 0x00C0;

/* Specify the memory areas */
MEMORY
{
	/* Flash */
	m_interrupts          (RX)  : ORIGIN = 0x00000000, LENGTH = 0x000000C0


	m_text                (RX)  : ORIGIN = 0x00000100, LENGTH = 0x0007F00
  	m_text1               (RX)  : ORIGIN = 0x0001BFF0, LENGTH = 0x10 /*FOR UDS DATA*/


	/* SRAM_L */
	/*m_custom              (RW)  : ORIGIN = 0x1FFFFC00, LENGTH = 0x00000300*/
	/* SRAM_U */
	m_data                (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00003800
	m_data_2                (RW)  : ORIGIN = 0x20003800, LENGTH = 0x00002800  /*目前 ASIL */
	m_data_5              (RW)  : ORIGIN = 0x20007C00, LENGTH = 0x00000200  /*目前未初始化*/
	m_data_3              (RW)  : ORIGIN = (0x20003800-STACK_CHECK_S_ADDR), LENGTH = 0x10
}

/* Define output sections */
SECTIONS
{

	/* The startup code goes first into internal flash */
	.interrupts :
	{
		__VECTOR_TABLE = .;
		__interrupts_start__ = .;
		. = ALIGN(4);
		KEEP(*(.isr_vector))     /* Startup code */
		__interrupts_end__ = .;
		. = ALIGN(4);
	} > m_interrupts


	/* The program code and other data goes into internal flash */
	.text :
	{
		. = ALIGN(4);
		*(.text)                 /* .text sections (code) */
		*(.text*)                /* .text* sections (code) */
		*(.rodata)               /* .rodata sections (constants, strings, etc.) */
		*(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
		*(.init)                 /* section used in crti.o files */
		*(.fini)                 /* section used in crti.o files */
		*(.eh_frame)             /* section used in crtbegin.o files */
		. = ALIGN(4);
	} > m_text

	/* Section used by the libgcc.a library for fvp4 */
	.ARM :
	{
		__exidx_start = .;
		*(.ARM.exidx*)
		__exidx_end = .;
	} > m_text

	__etext = .;    /* Define a global symbol at end of code. */
	__DATA_ROM = .; /* Symbol is used by startup for data initialization. */
	 /* The program code and other data goes into internal flash */
	.ROM_APP_VALID   :
	{
		. = ALIGN(4);
		*(.ROM_APP_VALID)                 /* .text sections (code) */
		*(.ROM_APP_VALID*)                /* .text* sections (code) */
		. = ALIGN(4);
	} > m_text1
	__DATA_UDS_ROM = .;  /* Symbol is used by startup for data initialization. */

	.interrupts_ram :
	{
		. = ALIGN(4);
		__VECTOR_RAM__ = .;
		__RAM_START = .;
		__interrupts_ram_start__ = .; /* Create a global symbol at data start. */
		*(.m_interrupts_ram)          /* This is a user defined section. */
		. += M_VECTOR_RAM_SIZE;
		. = ALIGN(4);
		__interrupts_ram_end__ = .;   /* Define a global symbol at data end. */
	} > m_data

	__VECTOR_RAM = DEFINED(__flash_vector_table__) ? ORIGIN(m_interrupts) : __VECTOR_RAM__ ;
	__RAM_VECTOR_TABLE_SIZE = DEFINED(__flash_vector_table__) ? 0x0 : (__interrupts_ram_end__ - __interrupts_ram_start__) ;

	.data : AT(__DATA_ROM)
	{
		. = ALIGN(4);
		__DATA_RAM = .;
		__data_start__ = .;      /* Create a global symbol at data start. */
		*(.data)                 /* .data sections */
		*(.data*)                /* .data* sections */
		. = ALIGN(4);
		__data_end__ = .;        /* Define a global symbol at data end. */
	} > m_data

	__DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
	__CODE_ROM = __DATA_END; /* Symbol is used by code initialization. */

	.code : AT(__CODE_ROM)
	{
		. = ALIGN(4);
		__CODE_RAM = .;
		__code_start__ = .;      /* Create a global symbol at code start. */
		__code_ram_start__ = .;
		*(.code_ram)             /* Custom section for storing code in RAM */
		. = ALIGN(4);
		__code_end__ = .;        /* Define a global symbol at code end. */
		__code_ram_end__ = .;
	} > m_data

	__CODE_END = __CODE_ROM + (__code_end__ - __code_start__);
	__CUSTOM_ROM = __CODE_END;

	/* Custom Section Block that can be used to place data at absolute address. */
	/* Use __attribute__((section (".customSection"))) to place data here. */
	/* Use this section only when MTB (Micro Trace Buffer) is not used, because MTB uses the same RAM area, as described in S32K Reference Manual. */
	

	/* Uninitialized data section. */
	.bss :
	{
		/* This is used by the startup in order to initialize the .bss section. */
		. = ALIGN(4);
		__BSS_START = .;
		__bss_start__ = .;
		*(.bss)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
		__BSS_END = .;
	} > m_data

	 /* Put heap section after the program data */
	.heap :
	{
		. = ALIGN(8);
		__end__ = .;
		__heap_start__ = .;
		PROVIDE(end = .);
		PROVIDE(_end = .);
		PROVIDE(__end = .);
		__HeapBase = .;
		. += HEAP_SIZE;
		__HeapLimit = .;
		__heap_limit = .;
		__heap_end__ = .;
	} > m_data

	/* Initializes stack on the end of block */
	__StackTop   = ORIGIN(m_data) + LENGTH(m_data);
	__StackLimit = __StackTop - STACK_SIZE;
	__StackLimit_check = __StackTop - STACK_SIZE -0x10;
	PROVIDE(__stack = __StackTop);
	//__RAM_END = __StackTop;

	.stack __StackLimit :
	{
		. = ALIGN(8);
		__stack_start__ = .;
		. += STACK_SIZE;
		__stack_end__ = .;
	} > m_data



	__DATA2_ROM = __CUSTOM_ROM;
	.RAM_ASIL ORIGIN(m_data_2) : AT(__DATA2_ROM)
	{
		. = ALIGN(4);
		__RAM_ASILStart = .;
		__RAM_ASIL_Start__ = .;
		KEEP(*(.RAM_ASIL))  /* Keep section even if not referenced. */
		__RAM_ASIL_end__ = .;
		. = ALIGN(4);
		__scst_data_start__ = .;
		*(.m0_scst_ram_data)
		. = ALIGN(4);
		*(.m0_scst_ram_data_target0)
		. = ALIGN(4);
		*(.m0_scst_ram_data_target1)
		. = ALIGN(4);
		*(.m0_scst_ram_test_code)
		*(.m0_scst_test_shell_data)
		__scst_data_end__ = .;
	} >m_data_2

		/* Initializes stack on the end of block */
	__DATA2_RAM_END   = ORIGIN(m_data_2) + LENGTH(m_data_2);
	__DATA2_ROM_END = __DATA2_ROM + __scst_data_end__ - __RAM_ASIL_Start__;

	__STACK_CHECK_ROM = __DATA2_ROM_END;
	__STACK_CHECK_RAM = ORIGIN(m_data_3) + LENGTH(m_data_3);
	.STACK_CHECK_ADDR  ORIGIN(m_data_3) : AT(__STACK_CHECK_ROM)
	{
		__STACK_CHECK_ADDRStart = .;
		__STACK_CHECK_ADDR_start__ = .;
		KEEP(*(.STACK_CHECK_ADDR))  /* Keep section even if not referenced. */
		__STACK_CHECK_ADDR_end__ = .;
	} > m_data_3
	__STACK_CHECK_ROM_END = __STACK_CHECK_ROM + (__STACK_CHECK_ADDR_end__ - __STACK_CHECK_ADDR_start__);

	__DATA5_ROM = __STACK_CHECK_ROM_END;
	.RAM_FLAG_NOLINT ORIGIN(m_data_5) : AT(__DATA5_ROM)
	{
		. = ALIGN(4);
		__RAM_FLAG_NOLINTstart = .;
		__RAM_FLAG_NOLINT_Start__ = .;
		KEEP(*(.RAM_FLAG_NOLINT))  /* Keep section even if not referenced. */
		__RAM_FLAG_NOLINT_end__ = .;
		. = ALIGN(4);
	} >m_data_5

	/* Initializes stack on the end of block */
	__DATA5_RAM_END   = ORIGIN(m_data_5) + LENGTH(m_data_5);
	__DATA5_ROM_END = __DATA5_ROM + __RAM_FLAG_NOLINT_end__ - __RAM_FLAG_NOLINTstart;

	__RAM_END = __DATA2_RAM_END;
	.ARM.attributes 0 : { *(.ARM.attributes) }

	ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
	ASSERT((0x20003800-STACK_CHECK_S_ADDR) == __StackLimit_check, "region stack check addr is error")
}

