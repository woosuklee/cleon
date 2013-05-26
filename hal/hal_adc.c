/**
@file       hal_adc.c

@brief      ADC configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing ADC

@return   void 

@param    void
*/
void HAL_ADC_Init(void)
{ 
    ADC12CTL0 |=  ADC12SHT0_15 + ADC12SHT1_15 + ADC12MSC + ADC12ON;   
    ADC12CTL1 |=  ADC12SHP;

    ADC12MCTL0 = (ADC12INCH_15 + ADC12SREF_0); 
    ADC12MCTL1 = (ADC12INCH_14 + ADC12SREF_0 + ADC12EOS); 

    ADC12CTL1 |= ADC12CONSEQ_1;  
    ADC12CTL1 |= ADC12CSTARTADD_0;
    
    ADC12CTL0 |=  ADC12ON;
    
    // Initially ADC is disabled
    ADC12CTL0 |=  ADC12ENC;	
}

