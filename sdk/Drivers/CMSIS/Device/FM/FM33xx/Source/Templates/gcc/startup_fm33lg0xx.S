/* ---------------------------------------------------------------------------------------*/
/*  @file:    startup_S32K118.s                                                           */
/*  @purpose: GNU Compiler Collection Startup File                                        */
/*            S32K118                                                                     */
/*  @version: 1.0                                                                         */
/*  @date:    2018-1-22                                                                   */
/*  @build:   b170107                                                                     */
/* ---------------------------------------------------------------------------------------*/
/*                                                                                        */
/* Copyright 2018 NXP                                                                     */
/* All rights reserved.                                                                   */
/*                                                                                        */
/* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR                          */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES              */
/* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.                */
/* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,                    */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                     */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR                     */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)                     */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,                    */
/* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING                  */
/* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF                         */
/* THE POSSIBILITY OF SUCH DAMAGE.                                                        */
/*****************************************************************************/
/* Version: GNU Compiler Collection                                          */
/*****************************************************************************/
    .syntax unified
    .arch armv6-m

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                            /* Top of Stack */
    .long   Reset_Handler                         /* Reset Handler */
    .long   NMI_Handler                        /* Non Maskable Interrupt */
    .long   HardFault_Handler                  /* Cortex-M0 SV Hard Fault Interrupt */
    .long   0
    .long   0
    .long   0
    .long   0
    .long   0
    .long   0
    .long   0
    .long   SVC_Handler                        /* Cortex-M0 SV Call Interrupt */
    .long   0
    .long   0
    .long   PendSV_Handler                     /* Cortex-M0 Pend SV Interrupt */
    .long   SysTick_Handler                    /* Cortex-M0 System Tick Interrupt */


  .long  WWDT_IRQHandler           /* 0:  WWDT         */
  .long  SVD_IRQHandler            /* 1:  SVD          */
  .long  RTC_IRQHandler            /* 2:  RTC          */
  .long  FLASH_IRQHandler          /* 3:  FLASH        */
  .long  FDET_IRQHandler           /* 4:  FDET         */
  .long  ADC_IRQHandler            /* 5:  ADC          */
  .long  DAC_IRQHandler            /* 6:  DAC          */
  .long  SPI0_IRQHandler           /* 7:  SPI0         */
  .long  SPI1_IRQHandler           /* 8:  SPI1         */
  .long  SPI2_IRQHandler           /* 9:  SPI2         */
  .long  UART0_IRQHandler          /* 10: UART0        */
  .long  UART1_IRQHandler          /* 11: UART1        */
  .long  UART3_IRQHandler          /* 12: UART3        */
  .long  UART4_IRQHandler          /* 13: UART4        */
  .long  UART5_IRQHandler          /* 14: UART5        */
  .long  U7816_IRQHandler          /* 15: U7816        */
  .long  LPUARTx_IRQHandler        /* 16: LPUARTx      */
  .long  I2C_IRQHandler            /* 17: I2C          */
  .long  CCL_IRQHandler            /* 18: CCL          */
  .long  AES_IRQHandler            /* 19: AES          */
  .long  LPTIM_IRQHandler          /* 20: LPTIM        */
  .long  DMA_IRQHandler            /* 21: DMA          */
  .long  WKUPx_IRQHandler          /* 22: WKUP         */
  .long  LUT_IRQHandler            /* 23: LUT          */
  .long  BSTIM_IRQHandler          /* 24: BSTIM        */
  .long  COMPx_IRQHandler          /* 25: COMPx        */
  .long  GPTIM0_1_IRQHandler       /* 26: GPTIM0(1)    */
  .long  GPTIM2_IRQHandler         /* 27: GPTIM2       */
  .long  ATIM_IRQHandler           /* 28: ATIM         */
  .long  VREF_IRQHandler           /* 39: VREF         */
  .long  GPIO_IRQHandler           /* 30: GPIO         */
  .long  CAN_IRQHandler            /* 31: CAN          */

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF     /* 8 bytes backdoor comparison key           */
    .long 0xFFFFFFFF     /*                                           */
    .long 0xFFFFFFFF     /* 4 bytes program flash protection bytes    */
    .long 0xFFFF7FFE     /* FDPROT:FEPROT:FOPT:FSEC(0xFE = unsecured) */

    .text
    .thumb

/* Reset Handler */

    .thumb_func
    .align 2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    cpsid   i               /* Mask interrupts */

    /* Init the rest of the registers */
    ldr     r1,=0
    ldr     r2,=0
    ldr     r3,=0
    ldr     r4,=0
    ldr     r5,=0
    ldr     r6,=0
    ldr     r7,=0
    mov     r8,r7
    mov     r9,r7
    mov     r10,r7
    mov     r11,r7
    mov     r12,r7

#ifdef START_FROM_FLASH

    /* Init ECC RAM */
    #ldr r6, =0x200030c0
    #ldr r7, =0x200030d0
    ldr r1, =__RAM_START
    ldr r2, =__RAM_END


    subs    r2, r1
    subs    r2, #1
    ble .LC5

    movs    r0, 0
    movs    r3, #4
.LC4:
    str r0, [r1]
.LC41:
    add	r1, r1, r3
    subs r2, 4
    bge .LC4
	blt .LC5
.LC42:
	/*a<0x200030c0 a>0x200030d0 ¼ÌÐøLC41*/
	ldr r5, =0x20005710
	movs r4,r1
	subs r4,r5
	bgt .LC4
	ldr r5, =0x20005700
	movs r4,r1
	subs r4,r5
	blt .LC4
    bge .LC41
.LC5:
#else
	bl ram_init
#endif
    
    /* Initialize the stack pointer */
    ldr     r0,=__StackTop
    mov     r13,r0

#ifndef __NO_SYSTEM_INIT
    /* Call the system init routine */
    ldr     r0,=SystemInit
    blx     r0
#endif

    /* Init .data and .bss sections */
    ldr     r0,=init_data_bss
    blx     r0
    cpsie   i               /* Unmask interrupts */
    bl      main
JumpToSelf:
    b       JumpToSelf

    .pool
    .size Reset_Handler, . - Reset_Handler

    .align  1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    b       DefaultISR
    .size DefaultISR, . - DefaultISR

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler    NMI_Handler
    def_irq_handler    HardFault_Handler
    def_irq_handler    SVC_Handler
    def_irq_handler    PendSV_Handler
    def_irq_handler    SysTick_Handler
    def_irq_handler    WWDT_IRQHandler
    def_irq_handler    SVD_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    FLASH_IRQHandler
    def_irq_handler    FDET_IRQHandler
    def_irq_handler    ADC_IRQHandler
    def_irq_handler    DAC_IRQHandler
    def_irq_handler    SPI0_IRQHandler
    def_irq_handler    SPI1_IRQHandler
    def_irq_handler    SPI2_IRQHandler
    def_irq_handler    UART0_IRQHandler
    def_irq_handler    UART1_IRQHandler
    def_irq_handler    UART3_IRQHandler
    def_irq_handler    UART4_IRQHandler
    def_irq_handler    UART5_IRQHandler
    def_irq_handler    U7816_IRQHandler
    def_irq_handler    LPUARTx_IRQHandler
    def_irq_handler    I2C_IRQHandler
    def_irq_handler    CCL_IRQHandler
    def_irq_handler    AES_IRQHandler
    def_irq_handler    LPTIM_IRQHandler
    def_irq_handler    DMA_IRQHandler
    def_irq_handler    WKUPx_IRQHandler
    def_irq_handler    LUT_IRQHandler
    def_irq_handler    BSTIM_IRQHandler
    def_irq_handler    COMPx_IRQHandler
    def_irq_handler    GPTIM0_1_IRQHandler
    def_irq_handler    GPTIM2_IRQHandler
    def_irq_handler    ATIM_IRQHandler
    def_irq_handler    VREF_IRQHandler
    def_irq_handler    GPIO_IRQHandler
    def_irq_handler    CAN_IRQHandler

    .end
