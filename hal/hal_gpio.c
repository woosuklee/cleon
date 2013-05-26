/**
@file       hal_gpio.c

@brief      GPIO configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

// File system variables
extern FATFS Fatfs;     
extern FIL   fileObject;
extern char  sFilename[50];

// Flags
extern bool bFLAG_CLEONSampleIntervalConuter;
extern bool bFLAG_MicroSDInserted;
extern bool bFLAG_USBConnected;
extern bool bFLAG_ISLoggingRequested;
extern bool bFLAG_IsTimeSynced;;

// Second and millisecond time tick
extern milliseoncd_time_tick_u uniMillisecondTimeTick;
extern seoncd_time_tick_u      uniSecondTimeTick;

// User parameters
extern unsigned long   ulSampleCount;
extern unsigned long   ulSampleGap;
extern unsigned long   ulChunkCount;
extern unsigned long   ulChunkGap;

// Counting number of seconds
extern unsigned char ucRTCNumberOfSecondCounter;

// Counting number of samples stored in a file
extern unsigned long ulNumberOfCLEONSamplesInAFileCount;

// Measuring time taken by each step of CLEON data logging
extern time_measurement_s  stTimeMeasurement[NUMBER_OF_TIME_MEASUREMENT_TEST];


/*----------------------------------------------------------------------------*/
/**
@brief    Initializing GPIO

@return   void 

@param    void
*/
void HAL_GPIO_Init(void)
{ 
    // User swtich
    USER_SW_PORT(DIR) &= ~BV(USER_SW_PIN);
    USER_SW_PORT(IES) |=  BV(USER_SW_PIN);
    USER_SW_PORT(IE)  |=  BV(USER_SW_PIN);
    
    // LEDs
    LED1_PORT(OUT)    |=  BV(LED1_PIN);
    LED1_PORT(DIR)    |=  BV(LED1_PIN);
    LED2_PORT(OUT)    |=  BV(LED2_PIN);
    LED2_PORT(DIR)    |=  BV(LED2_PIN);
    LED3_PORT(OUT)    |=  BV(LED3_PIN);
    LED3_PORT(DIR)    |=  BV(LED3_PIN);
    LED4_PORT(OUT)    |=  BV(LED4_PIN);
    LED4_PORT(DIR)    |=  BV(LED4_PIN);
    LED5_PORT(OUT)    |=  BV(LED5_PIN);
    LED5_PORT(DIR)    |=  BV(LED5_PIN);
    LED6_PORT(OUT)    |=  BV(LED6_PIN);
    LED6_PORT(DIR)    |=  BV(LED6_PIN);
}

//------------------------------------------------------------------------------
// PORT1 Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_VECTOR_ISR (void)
{
    switch(__even_in_range(P1IV,16))
    {
        case 0: break;                
        case 2: 
            if(bFLAG_IsTimeSynced == _SET_){
                if(bFLAG_ISLoggingRequested == _CLEAR_){
                    // This flag is set only when time has been synchronized
                    bFLAG_ISLoggingRequested = _SET_;
#if TIME_MEASUREMENT == _ENABLE_                    
                    // Logging current system time tick
                    if(ulNumberOfCLEONSamplesInAFileCount < NUMBER_OF_TIME_MEASUREMENT_TEST){
                        stTimeMeasurement[ulNumberOfCLEONSamplesInAFileCount].ullTimeAtUserButtonPressed = uniSecondTimeTick.ullSecondTimeTick + uniMillisecondTimeTick.ullMillisecondTimeTick;
                    }
#endif                    
                    if(ulSampleCount == 1){
                        bFLAG_CLEONSampleIntervalConuter = _SET_;
                        __low_power_mode_off_on_exit();
                    }
                }
            }
            break;                                     
        case 4: break;                
        case 6: 
            if(SYS_MMC_IsMicroSDInserted() == _TRUE_){
                // Mount FatFs moudle
                FS_Mount(0, &Fatfs);
                // Create directory
                FS_MakeDir("CLEON");
                // Change directory
                FS_ChangeDir("/CLEON");
                
                if(ulSampleCount == 1){
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
                // Micro SD is inserted
                bFLAG_MicroSDInserted = _SET_;
                // Turn LED1 off if Micro SD is inserted
                MICRO_SD_ERROR_LED_OFF();
                // Change interrupt edge to low-to-high
                MICROSD_STATUS_PORT(IES) &= ~BV(MICROSD_STATUS_PIN);
            }else{
                // Micro SD is missing
                bFLAG_MicroSDInserted = _CLEAR_;
                // Disable logging
                bFLAG_ISLoggingRequested = _CLEAR_;
                // Set number of seconds counter to zero
                ucRTCNumberOfSecondCounter = 0;
                // Move pointer to the end of file to append
                FS_Lseek(&fileObject, FS_GetSizeOfFile(&fileObject));
                // Flush cached information of a writing file
                FS_Sync(&fileObject);
                // Close file
                FS_Close(&fileObject);
                // Unmount file system
                FS_Mount(0, NULL);
                // Set counter value to zero to start over the logging process
                ulNumberOfCLEONSamplesInAFileCount = 0;
                // Turn LED1 on if Micro SD is not inserted
                MICRO_SD_ERROR_LED_ON();
                // Change interrupt edge to high-to-low
                MICROSD_STATUS_PORT(IES) |=  BV(MICROSD_STATUS_PIN);
            }
            break;                
        case 8: 
            if(SYS_USB_IsUSBConnected() == _TRUE_){
                // USB is connected
                bFLAG_USBConnected = _SET_;
                // Change interrupt edge to low-to-high
                USB_STATUS_PORT(IES) |=  BV(USB_STATUS_PIN);
                // Exit low power mode
                __low_power_mode_off_on_exit();
            }else{
                // USB is disconnected
                bFLAG_USBConnected = _CLEAR_;
                // Change interrupt edge to high-to-low
                USB_STATUS_PORT(IES) &= ~BV(USB_STATUS_PIN);
            }
            break;                
        case 10: break;                
        case 12: break;                
        case 14: break;                
        case 16: break;                
        default: break;
    }
}
