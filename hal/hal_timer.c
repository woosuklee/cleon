/**
@file       hal_timer.c

@brief      timer configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

// Second and millisecond time tick
extern milliseoncd_time_tick_u uniMillisecondTimeTick;
extern seoncd_time_tick_u      uniSecondTimeTick;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing timer

@return   void 

@param    void
*/
void HAL_TIMER_Init()
{ 
    HAL_TIMER_TimerA1Init();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing timerA1

@return   void 

@param    void
*/
void HAL_TIMER_TimerA1Init()
{ 
    TA1CCTL0 = CCIE;                            // CCR0 toggle mode
    TA1CCR0  = 12000;                           // Set it to 12000 to generate 1ms tick with 12MHz clock source
    TA1CTL   = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR
}

//------------------------------------------------------------------------------
// TIMER1_A0 Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_VECTOR_ISR (void)
{   
     uniMillisecondTimeTick.ullMillisecondTimeTick += 10000;
}