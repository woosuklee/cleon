/**
@file       sys_mcu.c

@brief      MCU related functions
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing microcontroller

@return   void 

@param    void
*/
void SYS_MCU_Init(void)
{ 
    HAL_Init();
}


