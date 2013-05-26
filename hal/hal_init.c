/**
@file       hal_init.c

@brief      Initializing HAL
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing hardware

@return   void 

@param    void
*/
void HAL_Init(void)
{ 
    HAL_PMM_Init();    
    HAL_GPIO_Init();
    HAL_UCS_Init();
    HAL_UART_Init();
    HAL_RTC_Init(); 
    HAL_FLASH_Init();
    HAL_DMA_Init();
    HAL_ADC_Init();
    HAL_TIMER_Init();    
    HAL_SPI_Init();
        
#if (DEBUG_MODE) && (DUMP_SYSTEM_INFO_VIA_USB)
    // Welcome screen
    SYS_USB_Printf("=======================================================\r\n");
    SYS_USB_Printf("CLEON                                                  \r\n");
    SYS_USB_Printf("=======================================================\r\n");    
    
    // Clock setting information
    SYS_USB_Printf("CLCOK SETTING                                          \r\n");       
    SYS_USB_Printf("    MCLK  = 12MHz\r\n");
    SYS_USB_Printf("    ACLK  = 32768Hz\r\n");
    SYS_USB_Printf("    SMCLK = 12MHz\r\n");
    SYS_USB_Printf("\r\n");
#endif           
}
