/**
@file       sys_power.c

@brief      Power related functions
*/

#ifndef __SYS_POWER_H__
#define __SYS_POWER_H__

#define BOOST_SHDN_N_HIGH()     BOOST_SHDN_N_PORT(OUT)    |=  BV(BOOST_SHDN_N_PIN);
#define BOOST_SHDN_N_LOW()      BOOST_SHDN_N_PORT(OUT)    &= ~BV(BOOST_SHDN_N_PIN);

#define GPS_PWR_SHDN_N_HIGH()   GPS_PWR_SHDN_N_PORT(OUT)  |=  BV(GPS_PWR_SHDN_N_PIN);
#define GPS_PWR_SHDN_N_LOW()    GPS_PWR_SHDN_N_PORT(OUT)  &= ~BV(GPS_PWR_SHDN_N_PIN);

void SYS_PWR_Init(void);

#endif
