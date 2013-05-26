/**
@file       hal_adc.h

@brief      ADC configuration
*/

#ifndef __HAL_ADC_H__
#define __HAL_ADC_H__

#define HAL_ADC_StartADC()      ADC12CTL0 |=  ADC12SC	                        
#define HAL_ADC_StopADC()       ADC12CTL0 &= ~ADC12SC	      

void HAL_ADC_Init(void);

#endif
