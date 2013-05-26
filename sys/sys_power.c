/**
@file       sys_power.c

@brief      Power related functions
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing power

@return   void 

@param    void
*/
void SYS_PWR_Init(void)
{   
    // Boost-up regulator
    BOOST_SHDN_N_PORT(OUT) &= ~BV(BOOST_SHDN_N_PIN);    
    BOOST_SHDN_N_PORT(DIR) |=  BV(BOOST_SHDN_N_PIN);
    
    // GPS power
    GPS_PWR_SHDN_N_PORT(OUT) &= ~BV(GPS_PWR_SHDN_N_PIN);    
    GPS_PWR_SHDN_N_PORT(DIR) |=  BV(GPS_PWR_SHDN_N_PIN);
}
