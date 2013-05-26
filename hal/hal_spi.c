/**
@file       hal_spi.c

@brief      SPI configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing SPI

@return   void 

@param    void
*/
void HAL_SPI_Init(void)
{ 
    //Initialize UCB1 for SPI Master operation
    UCB1CTL1 |= UCSWRST;                                    //Put state machine in reset
    UCB1CTL0  = UCCKPL | UCMSB | UCMST | UCMODE_0 | UCSYNC; //3-pin, 8-bit SPI master

    UCB1CTL1 = UCSWRST | UCSSEL_2;                          //Use SMCLK, keep RESET
    UCB1BR0  = 31;                                          //Initial SPI clock must be <400kHz
    UCB1BR1  = 0;                                           //f_UCxCLK = 12MHz/31 = 387kHz
    UCB1CTL1 &= ~UCSWRST;                                   //Release USCI state machine
    UCB1IFG  &= ~UCRXIFG;
}

