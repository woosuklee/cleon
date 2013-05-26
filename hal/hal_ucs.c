/**
@file       hal_ucs.c

@brief      Universal clock system configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing universal clock system

@return   void 

@param    void
*/
void HAL_UCS_Init()
{ 
    unsigned int    uiXT1Timeout = 50000;
    unsigned int    uiXT2Timeout = 50000;
    
    while (BAKCTL & LOCKBAK) BAKCTL &= ~LOCKBAK;
    
    // XT1 (32768Hz)
    UCSCTL6 |= XT1DRIVE1 + XT1DRIVE0;           
    
    UCSCTL6 &= ~XT1OFF;                         // Set XT1 On
    UCSCTL6 &= ~(XCAP1 + XCAP0);                // Internal load cap
    
    __delay_cycles(50000);                      // Delay until XT2 is stabilized

    do                                          // Loop until XT1 stabilizes
    {
        UCSCTL7 &= ~XT1LFOFFG;                  // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    }while ((UCSCTL7 & XT1LFOFFG) && --uiXT1Timeout);   // Check XT2 fault flag
    
    // XT2 (12000000Hz)
    XT2_PORT(SEL) |= BV(XT2OUT_PIN);            // Set P7.2/XT2IN and P7.3/XT2OUT for XT pins
    XT2_PORT(SEL) |= BV(XT2IN_PIN);
    
    UCSCTL6 |= XT2DRIVE0;                       // Drive strength set to range of 8MHz to 16MHz
    UCSCTL6 &= ~XT2OFF;                         // Set XT2 On
    
    __delay_cycles(50000);                      // Delay until XT2 is stabilized
    
    do                                          // Loop until XT2 stabilizes
    {
        UCSCTL7 &= ~XT2OFFG;                    // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                      // Clear fault flags
    }while ((UCSCTL7 & XT2OFFG) && --uiXT2Timeout);   // Check XT2 fault flag
    
    // Clock selection
    UCSCTL4 |= SELA_0 + SELS_5 + SELM_5;        // Select SMCLK, ACLK source and DCO source
}
