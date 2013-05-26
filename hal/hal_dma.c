/**
@file       hal_dma.c

@brief      DMA configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

// CLEON data structure
extern cleon_header_u      uniCLEONHeader;
extern cleon_gps_data_u    uniCLEONGPSData;
extern cleon_sensor_data_u uniCLEONSensorData;

// Flags
bool bFLAG_DMATransferCompleted = _CLEAR_;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing DMA

@return   void 

@param    void
*/
void HAL_DMA_Init(void)
{ 
    // External DMA trigger is mapped to P2.1
    MAX2769_EXT_DMA_TRIG_PORT(SEL) |=  BV(MAX2769_EXT_DMA_TRIG_PIN);
    MAX2769_EXT_DMA_TRIG_PORT(DIR) &= ~BV(MAX2769_EXT_DMA_TRIG_PIN);
  
    // Port mapping for P2.1
    PMAPKEYID = PMAPKEY;
    // P2.1 is mapped to external DMA trigger
    P2MAP1    = 0x02;
    
    // Set external input as DMA trigger
    DMACTL0   = DMA0TSEL_31;
    
    // Read-modify-write disable
    DMACTL4   =  DMARMWDIS;
    DMA0CTL  &= ~DMAIFG;
    
    // DMA destination address will be incremental while DMA source address won't be changed
    DMA0CTL = DMADSTINCR_3;
    
    // Enabling DMA transfer till the size of 'SIZE_OF_GPS_DATA_CHUNK_IN_WORDS' reached
    DMA0SZ = SIZE_OF_GPS_DATA_CHUNK_IN_BYTE / 2;
    
    // Set DMA source address to base address of P5IN (0x0240)
    // Because P5IN and P6IN are adjuscent registers with 1 byte offset, 
    // P5IN and P6IN will be transferred at a time if accessed with length of word (16 bit)
    __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) 0x0240);
    
    // DMA destination address setting : address of 'GPSData' structure
    __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &uniCLEONGPSData);
    
    // Enabling DMA interrupt 
    DMA0CTL |= DMAIE;    
}

//------------------------------------------------------------------------------
// DMA Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=DMA_VECTOR
__interrupt void DMA_VECTOR_ISR(void)
{
    switch(__even_in_range(DMAIV,16))
    {
        case 0: break;
        case 2:     
            bFLAG_DMATransferCompleted = _SET_;
            break;
        case 4: break;                          
        case 6: break;                          
        case 8: break;                          
        case 10: break;                         
        case 12: break;                         
        case 14: break;                         
        case 16: break;                         
        default: break;
    }
}


