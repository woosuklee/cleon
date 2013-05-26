/**
@file       hal_flash.c

@brief      FLASH configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing FLASH

@return   void 

@param    void
*/
void HAL_FLASH_Init(void)
{ 
#if ERASE_INTERNAL_FLASH_ON_RESET == _ENABLE_  
    HAL_FLASH_EraseBank((unsigned long *)MSP430_INTERNAL_FLASH_BANK1_ADDRESS);
    HAL_FLASH_EraseBank((unsigned long *)MSP430_INTERNAL_FLASH_BANK2_ADDRESS);
    HAL_FLASH_EraseBank((unsigned long *)MSP430_INTERNAL_FLASH_BANK3_ADDRESS);
#endif    
}

/*----------------------------------------------------------------------------*/
/**
@brief    Write data to internal FLASH

@return   void 

@param    uiSize - size of source data
@param    *ptrulSource - base address of source data
@param    *ptrulDestination - base address of FLASH destination
*/
void HAL_FLASH_Write(unsigned int uiSize, unsigned char *ptrucSource, unsigned char *ptrucDestination)
{
    FCTL3 = FWKEY;                              // Clear Lock bit
    FCTL1 = FWKEY + WRT;                        // Enable byte/word write mode
    
    while (uiSize > 0) {
        while (FCTL3 & BUSY);                   // test busy
        *ptrucDestination++ = *ptrucSource++;   // Write to Flash
        uiSize--;
    }

    FCTL1 = FWKEY;                              // Clear Erase bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}

/*----------------------------------------------------------------------------*/
/**
@brief    Erase FLASH segement

@return   void 

@param    *ptrulDestination - base address of FLAHS segment
*/
void HAL_FLASH_EraseSegment(unsigned long *ptrulDestination)
{
    FCTL3 = FWKEY;                              // Clear Lock bit
    FCTL1 = FWKEY + ERASE;                      // Set Erase bit
    *ptrulDestination = 0;                      // Dummy write to erase Flash seg
    while (FCTL3 & BUSY);                       // test busy
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}

/*----------------------------------------------------------------------------*/
/**
@brief    Erase FLASH bank

@return   void 

@param    *ptrulDestination - base address of FLAHS bank
*/
void HAL_FLASH_EraseBank(unsigned long *ptrulDestination)
{
    FCTL3 = FWKEY;                              // Clear Lock bit
    FCTL1 = FWKEY + MERAS;                      // Set Erase bit
    *ptrulDestination = 0;                      // Dummy write to erase Flash seg
    while (FCTL3 & BUSY);                       // test busy
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}
