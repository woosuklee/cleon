/**
@file       sys_define.h

@brief      System definitions for CLEON
*/

#ifndef __SYS_DEFINE_H__
#define __SYS_DEFINE_H__

/*----------------------------------------------------------------------------*/
/* User defined headers */
#include "sys_mcu.h"
#include "sys_usb.h"
#include "sys_gps.h"
#include "sys_power.h"
#include "sys_flash.h"
#include "sys_sensors.h"
#include "sys_mmc.h"
#include "sys_init.h"

/*----------------------------------------------------------------------------*/
/* General definition */
#define _TRUE_      1
#define _FALSE_     0

#define _SUCCESS_   1
#define _FAIL_      0

#define _ENABLE_    1
#define _DISABLE_   0

#define _SET_       1
#define _CLEAR_     0

#define _HIGH_      1
#define _LOW_       0

#define _ON_        1
#define _OFF_       0

#define _ERROR_     1
#define _OK_        0

#define _MASTER_    1
#define _SLAVE_     0

#define _MSB_       1
#define _LSB_       0

#define _MCLK_      2
#define _SMCLK_     1
#define _ACLK_      0

#endif