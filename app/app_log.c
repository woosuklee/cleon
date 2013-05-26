/**
@file       app_log.c

@brief      Logging CLEON data
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

// Second and millisecond time tick
extern milliseoncd_time_tick_u uniMillisecondTimeTick;
extern seoncd_time_tick_u      uniSecondTimeTick;

// File system variables
extern FIL   fileObject;      
extern UINT  bw;
char   sFilename[50];

// Flags
extern bool bFLAG_MicroSDInserted;
extern bool bFLAG_DMATransferCompleted;
extern bool bFLAG_IsTimeSynced;
       bool bFLAG_ISLoggingRequested = _CLEAR_;
       bool bFLAG_GPSSignalIsBeingCaptured = _CLEAR_;
       bool bFLAG_CLEONSampleIntervalConuter = _CLEAR_;

// Measuring time taken by each step of CLEON data logging
extern time_measurement_s  stTimeMeasurement[NUMBER_OF_TIME_MEASUREMENT_TEST];       
       
// Counting number of samples stored in a file
unsigned long   ulNumberOfCLEONSamplesInAFileCount = 0;       

// User parameters
unsigned long   ulSampleCount = 0;
unsigned long   ulSampleGap = 0;
unsigned long   ulChunkCount = 0;
unsigned long   ulChunkGap = 0;
       
/*----------------------------------------------------------------------------*/
/**
@brief    Collect CLEON data sample(s)

@return   void

@param    void
*/
void APP_Log(void)
{
///////////////////////// CHECK IF CONDITIONS ARE MET //////////////////////////
#if TIME_MEASUREMENT == _ENABLE_                    
    // Logging current system time tick
    if(ulNumberOfCLEONSamplesInAFileCount < NUMBER_OF_TIME_MEASUREMENT_TEST){
        stTimeMeasurement[ulNumberOfCLEONSamplesInAFileCount].ullTimeAtStartAPPLog = uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick;
    }
#endif                    
    // Check if user button is pressed
    if(bFLAG_ISLoggingRequested == _SET_){
        // Check if time information is synchorinized
        if(bFLAG_IsTimeSynced == _SET_){
            // Check if MicroSD is inserted
            if(bFLAG_MicroSDInserted == _SET_){
                // Check if defined interval period has passed
                if(bFLAG_CLEONSampleIntervalConuter == _SET_){
                    bFLAG_CLEONSampleIntervalConuter = _CLEAR_;

/////////////////////////////// PREPARATION 1/2 ////////////////////////////////
#if APP_LOG_LED_INDICATION == _ENABLE_        
                    // Turn LED2 on
                    SAMPLING_IN_ACTION_LED_ON();     
#endif // APP_LOG_LED_INDICATION            
                    
                    // Start GPS
                    SYS_GPS_StartOperation();

/////////////////////// SENSING HUMIDITY AND TEMPERATURE ///////////////////////
#if APP_LOG_TEMP_AND_HUM_SENSORS == _ENABLE_        
                    // Turn SHT11 on
                    SYS_SENSORS_SHT11_On();
                    uniCLEONSensorData.stSensors.uiTemperature = SYS_SENSORS_SHT11_ReadTemperature();
                    uniCLEONSensorData.stSensors.uiHumidity    = SYS_SENSORS_SHT11_ReadHumidity();
                    // Turn SHT11 off
                    SYS_SENSORS_SHT11_Off();
#endif // APP_LOG_TEMP_AND_HUM_SENSORS        

////////////////////////// SENSING LIGHT SENSORS ///////////////////////////////
#if APP_LOG_LIGHT_SENSORS == _ENABLE_        
                    // Start ADC conversion
                    HAL_ADC_StartADC();   
                    uniCLEONSensorData.stSensors.uiLightPAR    = SYS_SENSORS_S1087_Read(); 
                    uniCLEONSensorData.stSensors.uiLightTSR    = SYS_SENSORS_S108701_Read();
                    // Stop ADC conversion
                    HAL_ADC_StopADC();
#endif  // APP_LOG_LIGHT_SENSORS        

/////////////////////////////// PREPARATION 2/2 ////////////////////////////////
                    // Declare variables for temporary time information
                    unsigned long long ullPreviousTimeTick = 0;
                    unsigned long long ullCurrentTimeTick = 0;
                    
                    // Wait until GPS is fully up and running 
                    SYS_GPS_TimingDealy(100);   // 100ms delay
                    
                    // If this is the first time to log, create a file that captured samples will be written to
                    if(ulNumberOfCLEONSamplesInAFileCount == 0){
                        if(ulSampleCount != 1){
                            // Initailize filename
                            memset(sFilename, 0, sizeof(sFilename));
                            // File name will start with CLEON's current system time
#if (APP_LOG_TEMP_AND_HUM_SENSORS == _ENABLE_) || (APP_LOG_LIGHT_SENSORS == _ENABLE_)                    
                            sprintf(sFilename, "%lld_s16368_if4092000_m2_i2_s8.bin", uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick);
#else
                            sprintf(sFilename, "%lld_s16368_if4092000_m2_i2_s0.bin", uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick);
#endif                            
                            // The created file will remain openned until the number of samples defined in 'APP_LOG_SAMPLES_IN_A_FILE' are captured
                            FS_Open(&fileObject, sFilename, FA_WRITE | FA_CREATE_ALWAYS);
                        }
                    }
                    
                    // Number of GPS samples in a CLEON sample
                    uniCLEONHeader.stCLEONHeader.uniChunkCount.ulOriginalVar = ulChunkCount;
                    
                    // Chunk gap information
                    if(ulChunkCount == 1){
                        uniCLEONHeader.stCLEONHeader.uniChunkGapMS.ulOriginalVar = 0;  
                    }else{
                        uniCLEONHeader.stCLEONHeader.uniChunkGapMS.ulOriginalVar = ulChunkGap;
                    }
                    
                    // Update time tick information
                    uniCLEONHeader.stCLEONHeader.uniMillisecondTimeTick.ullMillisecondTimeTick = uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick;
                                
#if TIME_MEASUREMENT == _ENABLE_                    
                    // Logging current system time tick
                    if(ulNumberOfCLEONSamplesInAFileCount < NUMBER_OF_TIME_MEASUREMENT_TEST){
                        stTimeMeasurement[ulNumberOfCLEONSamplesInAFileCount].ullTimeAtWriteHeaderToSDCard = uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick;
                    }
#endif                                        
                    // Move pointer to the end of file to append
                    FS_Lseek(&fileObject, FS_GetSizeOfFile(&fileObject));
                    // Write captured CLEON header (44 bytes) to the openned file
                    FS_Write(&fileObject, &uniCLEONHeader, sizeof(uniCLEONHeader), &bw);
                    // Flush cached information of a writing file
                    FS_Sync(&fileObject);

///////////////////////////// CAPTURING GPS SIGNAL /////////////////////////////              
                    for(int i = 0 ; i < ulChunkCount ; i++){
                        // Check mutex for GPS DMA transfer 
                        if(bFLAG_GPSSignalIsBeingCaptured == _CLEAR_){
                            // P(bFLAG_GPSSignalIsBeingCaptured)
                            bFLAG_GPSSignalIsBeingCaptured = _SET_;
                            
                            if( i == 0 ){
#if TIME_MEASUREMENT == _ENABLE_                    
                                // Logging current system time tick
                                if(ulNumberOfCLEONSamplesInAFileCount < NUMBER_OF_TIME_MEASUREMENT_TEST){
                                    stTimeMeasurement[ulNumberOfCLEONSamplesInAFileCount].ullTimeAtFirstChunk = uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick;
                                }
#endif                                                                      
                                // First sample will be captured immediately 
                                ullCurrentTimeTick = uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick;
                                ullPreviousTimeTick = ullCurrentTimeTick;
                            }else{
                                while(1){
                                    ullCurrentTimeTick = uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick;
                                    // From second chunk, it will wait until time defined in 'uiChunkGap' has passed
                                    if(((ullPreviousTimeTick/10000) + ulChunkGap) <= (ullCurrentTimeTick/10000)){
                                        ullPreviousTimeTick = ullCurrentTimeTick;
                                        break;
                                    }
                                }
                            }
                                                
                            // Start DMA transfer
                            DMA0CTL |= DMAEN;  
                                
                            // Wait until DMA-transfer-complete-IF is set
                            while(!bFLAG_DMATransferCompleted);
                            bFLAG_DMATransferCompleted = _CLEAR_;
                            
                            // Move pointer to the end of file to append
                            FS_Lseek(&fileObject, FS_GetSizeOfFile(&fileObject));
                            // Write captured CLEON sample to the openned file
                            FS_Write(&fileObject, &uniCLEONGPSData, sizeof(uniCLEONGPSData), &bw);
                            // Flush cached information of a writing file
                            FS_Sync(&fileObject);
                            
                            // V(bFLAG_GPSSignalIsBeingCaptured)
                            bFLAG_GPSSignalIsBeingCaptured = _CLEAR_;
                        }
                    }
                    
///////////////////////////// STORING SENSOR DATA  /////////////////////////////
#if (APP_LOG_TEMP_AND_HUM_SENSORS == _ENABLE_) || (APP_LOG_LIGHT_SENSORS == _ENABLE_)                    
                    // Move pointer to the end of file to append
                    FS_Lseek(&fileObject, FS_GetSizeOfFile(&fileObject));
                    // Write captured CLEON sample to the openned file
                    FS_Write(&fileObject, &uniCLEONSensorData, sizeof(uniCLEONSensorData), &bw);
                    // Flush cached information of a writing file
                    FS_Sync(&fileObject);
#endif
                    
///////////////////////////////// WRAPPING UP //////////////////////////////////                                  
                    // Stop GPS
                    SYS_GPS_StopOperation();
    
                    // Increase the total number of samples written to the openned file
                    if(ulNumberOfCLEONSamplesInAFileCount < (ulSampleCount - 1)){
                        ulNumberOfCLEONSamplesInAFileCount++;
                    }else if(ulNumberOfCLEONSamplesInAFileCount == (ulSampleCount - 1)){
                        ulNumberOfCLEONSamplesInAFileCount = 0;
                        bFLAG_ISLoggingRequested = _CLEAR_;
                    }

                    // If the total number of captured and written samples reach value defined in 'APP_LOG_SAMPLES_IN_A_FILE',
                    // close file and reset counter to zero
                    if(ulNumberOfCLEONSamplesInAFileCount == APP_LOG_SAMPLES_IN_A_FILE){
                        FS_Close(&fileObject);
                        ulNumberOfCLEONSamplesInAFileCount = 0;    
                    }

#if APP_LOG_LED_INDICATION == _ENABLE_        
                    // Turn LED2 off
                    SAMPLING_IN_ACTION_LED_OFF(); 
#endif // APP_LOG_LED_INDICATION    
                }
            }
        }
#if TIME_MEASUREMENT == _ENABLE_                    
        if(ulNumberOfCLEONSamplesInAFileCount == NUMBER_OF_TIME_MEASUREMENT_TEST){
            for(int i = 0 ; i < NUMBER_OF_TIME_MEASUREMENT_TEST; i++){
                /*
                if(i == 0 ){
                    printf("Sample #\n");
                    printf("Time at APP_Log start - Time at user button press\n");
                    printf("Time at start writing header to SD card - Time at APP_Log start\n");
                    printf("Time at the first GPS signal chunk - Time at start writing header to SD card\n\n");
                }
                */
                printf("Sample %d\n", i);
                printf("%lld ms\n", (stTimeMeasurement[i].ullTimeAtStartAPPLog - stTimeMeasurement[i].ullTimeAtUserButtonPressed) / 10000);    
                printf("%lld ms\n", (stTimeMeasurement[i].ullTimeAtWriteHeaderToSDCard - stTimeMeasurement[i].ullTimeAtStartAPPLog) / 10000);    
                printf("%lld ms\n\n", (stTimeMeasurement[i].ullTimeAtFirstChunk - stTimeMeasurement[i].ullTimeAtWriteHeaderToSDCard) / 10000);    
            }
        }
#endif                                        
        
        // If sensing is set to on-demand mode, clear the flag
        // This locates at the end of the fuction 'APP_Log()' to prevent the flag from being set again during the function is in operation
        if(ulSampleCount == 1) bFLAG_ISLoggingRequested = _CLEAR_;
    }
}













