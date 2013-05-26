/**
@file       sys_flash.c

@brief      FLASH related functions
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing external FLASH

@return   void 

@param    void
*/
void SYS_EXT_FLASH_Init(void)
{ 
    SYS_EXT_FLASH_InitIO();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing external FLASH interface pins

@return   void 

@param    void
*/
void SYS_EXT_FLASH_InitIO(void)
{
    // Release CS initially
    EXT_FLASH_CS_PORT(OUT)   |= BV(EXT_FLASH_CS_PIN);
    EXT_FLASH_CS_PORT(DIR)   |= BV(EXT_FLASH_CS_PIN);
    
    EXT_FLASH_WP_PORT(OUT)   |= BV(EXT_FLASH_WP_PIN);
    EXT_FLASH_WP_PORT(DIR)   |= BV(EXT_FLASH_WP_PIN);
    
    EXT_FLASH_HOLD_PORT(OUT) |= BV(EXT_FLASH_HOLD_PIN);
    EXT_FLASH_HOLD_PORT(DIR) |= BV(EXT_FLASH_HOLD_PIN);
    
    // Set as SPI pins
    EXT_FLASH_MOSI_PORT(SEL) |= BV(EXT_FLASH_MOSI_PIN);
    EXT_FLASH_MISO_PORT(SEL) |= BV(EXT_FLASH_MISO_PIN);
    EXT_FLASH_CLK_PORT(SEL)  |= BV(EXT_FLASH_CLK_PIN);
}
