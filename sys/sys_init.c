/**
@file       sys_init.c

@brief      Initializing the system
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

__no_init cleon_header_u      uniCLEONHeader;
__no_init cleon_gps_data_u    uniCLEONGPSData;
__no_init cleon_sensor_data_u uniCLEONSensorData;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing CLEON

@return   void 

@param    void
*/
void SYS_Init(void)
{ 
    SYS_MCU_Init();
    SYS_PWR_Init();
    SYS_EXT_FLASH_Init();       
    SYS_MMC_Init();
    SYS_SENSORS_Init();
    SYS_USB_Init();
    
    SYS_InitCLEONDataStructure();
  
    SYS_GPS_Init();    
}

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing CLEON data structure

@return   void 

@param    void
*/
void SYS_InitCLEONDataStructure(void)
{ 
    for( int i = 0 ; i < SIZE_OF_HEADER_IN_BYTE; i++){
        uniCLEONHeader.ucSingleBytes[i] = 0;
    }
    
    for( int i = 0 ; i < SIZE_OF_GPS_SIGANL_IN_BYTE; i++){
        uniCLEONGPSData.ucSingleByte[i] = 0;
    }
    
    for( int i = 0 ; i < SIZE_OF_SENSOR_DATA_IN_BYTE; i++){
        uniCLEONSensorData.ucSingleByte[i] = 0;
    }
    
    uniCLEONHeader.stCLEONHeader.uniSignature.ucSingleByte[0] = 'L';
    uniCLEONHeader.stCLEONHeader.uniSignature.ucSingleByte[1] = 'E';
    uniCLEONHeader.stCLEONHeader.uniSignature.ucSingleByte[2] = 'A';
    uniCLEONHeader.stCLEONHeader.uniSignature.ucSingleByte[3] = 'P';
   
    uniCLEONHeader.stCLEONHeader.uniVersion.ucSingleByte[0] = 0x01;
    uniCLEONHeader.stCLEONHeader.uniVersion.ucSingleByte[1] = 0x00;
    uniCLEONHeader.stCLEONHeader.uniVersion.ucSingleByte[2] = 0x01;
    uniCLEONHeader.stCLEONHeader.uniVersion.ucSingleByte[3] = 0x00;
 
    uniCLEONHeader.stCLEONHeader.stReceiverSetting.uniSamplesPerMS.ulOriginalVar = 16368;
    uniCLEONHeader.stCLEONHeader.stReceiverSetting.uniIntermediateFreq.dOriginalVar = 4092000.0;

    uniCLEONHeader.stCLEONHeader.uniChunkCount.ulOriginalVar = 1;
    uniCLEONHeader.stCLEONHeader.uniChunkSizeMS.ulOriginalVar = 2;
    uniCLEONHeader.stCLEONHeader.uniChunkGapMS.ulOriginalVar = 50;
    uniCLEONHeader.stCLEONHeader.uniGPSSignalDataFormat.ulOriginalVar = 1;
}



