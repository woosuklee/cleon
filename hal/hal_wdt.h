/**
@file       hal_wdt.h

@brief      Watchdog timer configuration
*/

#ifndef __HAL_WDT_H__
#define __HAL_WDT_H__

#define HAL_StopWDT()   WDTCTL = WDTPW + WDTHOLD

#endif
