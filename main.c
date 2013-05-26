/** 
@file       main.c

@brief      Main entry for the CLEO sensor board firmware
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

void main( void )
{
    //-------------- DO NOT MODIFY THIS INITIALIZATION PART ---------- START -//
    HAL_StopWDT();
    
    HAL_DisableGlobalInterrupt();
    SYS_Init();
    HAL_EnableGlobalInterrupt();
    //-------------- DO NOT MODIFY THIS INITIALIZATION PART ---------- END ---//
 
    while(1){
        HAL_ENTER_LPM4();
        APP_Log();
        APP_USB_ProcessingRcvdFrame();
    }
}
