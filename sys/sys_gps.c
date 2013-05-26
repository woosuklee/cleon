/**
@file       sys_gps.c

@brief      GPS related functions
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
extern bool bFLAG_DMATransferCompleted;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing GPS

@return   void 

@param    void
*/
void SYS_GPS_Init(void)
{
    // Initializing GPS control pins    
    SYS_GPS_InitControlIO();
    
    // Stop GPS operation before initialization
    SYS_GPS_StopOperation();
    
    // Program the GPS with pre-configured mode
    SYS_GPS_InitProgramIO();
    
    // Initializing GPS data pins
    SYS_GPS_InitDataIO();
    
#if GPS_TEST_ON_INITIALIZATION == _ENABLE_    
    // Check if GPS is functional by sampling a chunk of GPS signal and take a look inside
    for(int i = 1 ; i < MAX_NUMBER_OF_GPS_TEST_COUNT; i++){
        // If fail, allow more time to settle (100ms, 200ms, 300ms ...)
        if(SYS_GPS_TestGPS(100 * i) == _SUCCESS_){
            // Return if it passes GPS test
            return;
        }
    }
    
    // If it fails the test, turn LED on
    GPS_ERROR_LED_ON();
#endif    
}

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing GPS data pins

@return   void 

@param    void
*/
void SYS_GPS_InitDataIO(void)
{
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_1_PIN);      
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_2_PIN);      
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_3_PIN);      
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_4_PIN);      
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_5_PIN);      
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_6_PIN);      
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_7_PIN);      
    MAX2769_Q0_PORT(OUT) &= ~BV(MAX2769_Q0_8_PIN);  
    
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_1_PIN);      
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_2_PIN);      
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_3_PIN);      
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_4_PIN);      
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_5_PIN);      
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_6_PIN);      
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_7_PIN);      
    MAX2769_Q1_PORT(OUT) &= ~BV(MAX2769_Q1_8_PIN);      
    
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_1_PIN);      
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_2_PIN);      
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_3_PIN);      
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_4_PIN);      
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_5_PIN);      
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_6_PIN);      
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_7_PIN);      
    MAX2769_I0_PORT(OUT) &= ~BV(MAX2769_I0_8_PIN);  
    
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_1_PIN);      
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_2_PIN);      
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_3_PIN);      
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_4_PIN);      
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_5_PIN);      
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_6_PIN);      
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_7_PIN);      
    MAX2769_I1_PORT(OUT) &= ~BV(MAX2769_I1_8_PIN);
    
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_1_PIN);      
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_2_PIN);      
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_3_PIN);      
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_4_PIN);      
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_5_PIN);      
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_6_PIN);      
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_7_PIN);      
    MAX2769_Q0_PORT(DIR) &= ~BV(MAX2769_Q0_8_PIN);  
    
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_1_PIN);      
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_2_PIN);      
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_3_PIN);      
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_4_PIN);      
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_5_PIN);      
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_6_PIN);      
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_7_PIN);      
    MAX2769_Q1_PORT(DIR) &= ~BV(MAX2769_Q1_8_PIN);      
    
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_1_PIN);      
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_2_PIN);      
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_3_PIN);      
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_4_PIN);      
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_5_PIN);      
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_6_PIN);      
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_7_PIN);      
    MAX2769_I0_PORT(DIR) &= ~BV(MAX2769_I0_8_PIN);  
    
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_1_PIN);      
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_2_PIN);      
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_3_PIN);      
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_4_PIN);      
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_5_PIN);      
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_6_PIN);      
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_7_PIN);      
    MAX2769_I1_PORT(DIR) &= ~BV(MAX2769_I1_8_PIN);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing GPS control pins

@return   void 

@param    void
*/
void SYS_GPS_InitControlIO(void)
{                
    // MAX2769 is in operation
    MAX2769_SHDN_N_PORT(OUT)    |=  BV(MAX2769_SHDN_N_PIN);    
    MAX2769_SHDN_N_PORT(DIR)    |=  BV(MAX2769_SHDN_N_PIN);    
    
    // MAX2769 is in operation
    MAX2769_IDLE_N_PORT(OUT)    |=  BV(MAX2769_IDLE_N_PIN);            
    MAX2769_IDLE_N_PORT(DIR)    |=  BV(MAX2769_IDLE_N_PIN);    
}

/*----------------------------------------------------------------------------*/
/**
@brief      Initializing GPS control pins

@return     void 

@param      void
*/
void SYS_GPS_InitProgramIO(void)
{
    // CLEON uses default device state 2 (PGM:1, CS:0, SCLK:0, SDATA:1)
    // - IF: 4092000 Hz
    // - Sampling rate: 16368 samples/sec
    // - CMOS level I0 and I1
  
    // PGM pin is set to high
    MAX2769_PGM_EN_N_PORT(OUT)  |=  BV(MAX2769_PGM_EN_N_PIN);    
    MAX2769_PGM_EN_N_PORT(DIR)  |=  BV(MAX2769_PGM_EN_N_PIN);    
    
    // CS pin is set to low
    MAX2769_PGM_CS_N_PORT(OUT)  &= ~BV(MAX2769_PGM_CS_N_PIN);    
    MAX2769_PGM_CS_N_PORT(DIR)  |=  BV(MAX2769_PGM_CS_N_PIN);    
    
    // SCLK pin is set to low
    MAX2769_PGM_SCLK_PORT(OUT)  &= ~BV(MAX2769_PGM_SCLK_PIN);        
    MAX2769_PGM_SCLK_PORT(DIR)  |=  BV(MAX2769_PGM_SCLK_PIN);    
    
    // SDATA pin is set to high
    MAX2769_PGM_SDATA_PORT(OUT) |=  BV(MAX2769_PGM_SDATA_PIN);      
    MAX2769_PGM_SDATA_PORT(DIR) |=  BV(MAX2769_PGM_SDATA_PIN);  
}

/*----------------------------------------------------------------------------*/
/**
@brief    Starting GPS operation

@return   void 

@param    void
*/
void SYS_GPS_StartOperation(void)
{
    // LDO for GPS is turned on
    GPS_PWR_SHDN_N_HIGH();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Starting GPS operation

@return   void 

@param    void
*/
void SYS_GPS_StopOperation(void)
{
    // LDO for GPS is in shutdown mode
    GPS_PWR_SHDN_N_LOW();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Test if GPS is initalized properly

@return   void 

@param    uiDelay - time for GPS to settle down (in milliseconds)
*/
bool SYS_GPS_TestGPS(unsigned int uiDelay)
{
    unsigned int uiTestCountFor0xFF = 0;
    unsigned int uiTestCountFor0x00 = 0;
    
    // Start GPS operation
    SYS_GPS_StartOperation();
    
    // Allow GPS time to settle
    SYS_GPS_TimingDealy(uiDelay);
    
    // Start DMA transfer
    DMA0CTL |= DMAEN;  
    
    // Wait until DMA transfer completed
    while((DMA0CTL & DMAIFG) != DMAIFG);
    DMA0CTL &= ~DMAIFG;
 
    // Count the number of '0xFF' and '0x00'
    for(int i = 0 ; i < SIZE_OF_GPS_DATA_CHUNK_IN_BYTE ; i++ ){
        if(uniCLEONGPSData.ucSignal[i] == 0xFF){
            uiTestCountFor0xFF++;
        }else if(uniCLEONGPSData.ucSignal[i] == 0x00){
            uiTestCountFor0x00++;
        }
    }
    
    // Stop GPS operation
    SYS_GPS_StopOperation();
    
    // If number of '0xFF' or '0x00' exceeds half or total sampled signal, regards GPS chip is not functional
    // (number of '0xFF' or '0x00' cannot exceed half of captured GPS signal due to signal's physical nature)
    if((uiTestCountFor0xFF > (SIZE_OF_GPS_DATA_CHUNK_IN_BYTE/2))||(uiTestCountFor0x00 > (SIZE_OF_GPS_DATA_CHUNK_IN_BYTE/2))){
        return _FAIL_;
    }else{
        return _SUCCESS_;
    }
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    