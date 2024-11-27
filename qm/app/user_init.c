#include "user_init.h"

void ClockInit(uint32_t clock)
{
    FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE);
    switch (clock)
    {
        case FL_CMU_RCHF_FREQUENCY_8MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF8M_TRIM);
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
            FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV1);
            break;
        
        case FL_CMU_RCHF_FREQUENCY_16MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF16M_TRIM);
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
            FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV2);
            break;
        
        case FL_CMU_RCHF_FREQUENCY_24MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF24M_TRIM);
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
            FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV4);
            break;
        
        case FL_CMU_RCHF_FREQUENCY_32MHZ:
            FL_CMU_RCHF_WriteTrimValue(RCHF32M_TRIM);
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
            FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV4);
            break;
        
        default:
            FL_CMU_RCHF_WriteTrimValue(RCHF8M_TRIM);
            FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
            FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV1);
            break;
    }
    
    FL_CMU_RCHF_SetFrequency(clock);
    FL_CMU_SetSystemClockSource(FL_CMU_SYSTEM_CLK_SOURCE_RCHF);
}

void FoutInit(void)
{
    FL_GPIO_InitTypeDef init = {0};
    
    init.pin = FL_GPIO_PIN_11;
    init.mode = FL_GPIO_MODE_DIGITAL;
    init.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    init.pull = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &init);
    
    FL_GPIO_SetFOUT0(GPIO, FL_GPIO_FOUT0_SELECT_AHBCLK_DIV64);
}

#ifndef MFANG

#ifdef __CC_ARM
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{ 
    int handle; 
}; 
FILE __stdout;
#endif

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
    x = x; 
} 

#if 0
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
    FL_UART_WriteTXBuff(UART0, (uint8_t)ch);
    while(FL_UART_IsActiveFlag_TXBuffEmpty(UART0) != FL_SET);
    return ch;
}
#endif

void DebugUartInit(void)
{
     FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    FL_UART_InitTypeDef UART_InitStruct = {0};   

    //PA13:UART0-RX   PA14:UART0-TX
    GPIO_InitStruct.pin        = FL_GPIO_PIN_13|FL_GPIO_PIN_14;
    GPIO_InitStruct.mode       = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;   
    GPIO_InitStruct.pull       = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UART_InitStruct.clockSrc = FL_CMU_UART0_CLK_SOURCE_APBCLK;
	
    UART_InitStruct.baudRate            = 115200;							        //波特率
    UART_InitStruct.dataWidth           = FL_UART_DATA_WIDTH_8B;			      	//数据位数
    UART_InitStruct.stopBits            = FL_UART_STOP_BIT_WIDTH_1B;				//停止位
    UART_InitStruct.parity              = FL_UART_PARITY_EVEN;				        //奇偶校验
    UART_InitStruct.transferDirection   = FL_UART_DIRECTION_TX_RX;	                //接收-发送使能
    FL_UART_Init(UART0, &UART_InitStruct);	
}

void LedInit(void)
{
    uint8_t count = 5;
    
    FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
   
    FL_GPIO_ResetOutputPin(LED0_GPIO, LED0_PIN);
    
    GPIO_InitStruct.pin = LED0_PIN;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    FL_GPIO_Init(LED0_GPIO, &GPIO_InitStruct);
    
    while (count--)
    {
        LED0_ON();
        FL_DelayMs(100);
		LED0_OFF();
		FL_DelayMs(100);
    }
}

#endif 

void UserInit(void)
{
#ifndef MFANG
    LedInit();
    DebugUartInit();
#endif
}
