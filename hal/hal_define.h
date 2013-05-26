/**
@file       hal_define.h

@brief      HAL definitions for CLEON
*/

#ifndef __HAL_DEFINE_H__
#define __HAL_DEFINE_H__
 
/*----------------------------------------------------------------------------*/
/* IAR compiler specific headers */
#include "intrinsics.h"
#include "msp430.h"

/*----------------------------------------------------------------------------*/
/* Standard C headers */
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

/*----------------------------------------------------------------------------*/
/* User defined headers */
#include "hal_init.h"
#include "hal_wdt.h"
#include "hal_ucs.h"
#include "hal_uart.h"
#include "hal_dma.h"
#include "hal_rtc.h"
#include "hal_gpio.h"
#include "hal_flash.h"
#include "hal_timer.h"
#include "hal_adc.h"
#include "hal_spi.h"
#include "hal_pmm.h"

/*----------------------------------------------------------------------------*/
/* Macros */
#define BV(bit)     (1 << (bit))
#define st(x)       do { x } while (__LINE__ == -1);
#define bin(a,b,c,d,e,f,g,h) ((a<<7)|(b<<6)|(c<<5)|(d<<4)|(e<<3)|(f<<2)|(g<<1)|(h<<0));

#endif