/**
@file       hal_flash.h

@brief      FLASH configuration
*/

#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

#define ERASE_INTERNAL_FLASH_ON_RESET           _DISABLE_

#define MSP430_INTERNAL_FLASH_BANK0_ADDRESS     0x008000
#define MSP430_INTERNAL_FLASH_BANK1_ADDRESS     0x018000
#define MSP430_INTERNAL_FLASH_BANK2_ADDRESS     0x028000
#define MSP430_INTERNAL_FLASH_BANK3_ADDRESS     0x038000

void HAL_FLASH_Init(void);
void HAL_FLASH_Write(unsigned int uiSize, unsigned char *ptrucSource, unsigned char *ptrucDestination);
void HAL_FLASH_EraseSegment(unsigned long *ptrulDestination);
void HAL_FLASH_EraseBank(unsigned long *ptrulDestination);

#endif
