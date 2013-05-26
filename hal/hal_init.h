/**
@file       hal_init.h

@brief      Initializing HAL
*/

#ifndef __HAL_INIT_H__
#define __HAL_INIT_H__

#define HAL_DisableGlobalInterrupt()    __disable_interrupt()
#define HAL_EnableGlobalInterrupt()     __enable_interrupt()

void HAL_Init(void);

#endif
