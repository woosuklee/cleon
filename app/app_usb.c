/**
@file       app_usb.c

@brief      Communication via USB
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

// RTC time
extern rtc_time_u  uniRTCTime;

// Second and millisecond time tick
extern milliseoncd_time_tick_u uniMillisecondTimeTick;
extern seoncd_time_tick_u      uniSecondTimeTick;

// USB reception frame
extern usb_frame_u uniUSBRecvFrame;

// Counting number of samples stored in a file
extern unsigned long ulNumberOfCLEONSamplesInAFileCount;

// Flags
extern bool bFLAG_USBConnected;
extern bool bFLAG_USBFrameReceived;
extern bool bFLAG_IsTimeSynced;
extern bool bFLAG_RTCTimeTickOfSecond;

// File system variables
extern FIL   fileObject;      
extern UINT  bw;
extern char  sFilename[50];

// User parameters
extern unsigned long   ulSampleCount;
extern unsigned long   ulSampleGap;
extern unsigned long   ulChunkCount;
extern unsigned long   ulChunkGap;

/*----------------------------------------------------------------------------*/
/**
@brief          Processing USB frame
                - For every received USB frame, there will be an ack 
                - Ack frame will contain received command as data in data field

@return		void

@param		void
*/
void APP_USB_ProcessingRcvdFrame()
{
///////////////////////// CHECK IF CONDITIONS ARE MET //////////////////////////
    // Check if USB frame has been received
    if(bFLAG_USBFrameReceived == _SET_){
        bFLAG_USBFrameReceived = _CLEAR_;

////////////////////////////// IDENTIFY COMMAND ////////////////////////////////  
        // Identify command contained in the USB frame
        switch(uniUSBRecvFrame.stUSBFrame.ucCommand){
          
///////////////// COMMAND : USB_FRAME_COMMAND_CONNECT //////////////////////////
            case USB_FRAME_COMMAND_CONNECT:
                // Transmit ACK
                SYS_USB_SendACK(uniUSBRecvFrame.stUSBFrame.ucCommand);
                break;

///////////////// COMMAND : USB_FRAME_COMMAND_UPDATE_RTC_TIME //////////////////
            case USB_FRAME_COMMAND_UPDATE_RTC_TIME:
                // Transmit ACK
                SYS_USB_SendACK(uniUSBRecvFrame.stUSBFrame.ucCommand);
                HAL_SetRTCYear((uniUSBRecvFrame.ucSingleByte[4] << 8) + uniUSBRecvFrame.ucSingleByte[5]);
                HAL_SetRTCMon(uniUSBRecvFrame.ucSingleByte[6]);
                HAL_SetRTCDay(uniUSBRecvFrame.ucSingleByte[7]);
                HAL_SetRTCHour(uniUSBRecvFrame.ucSingleByte[8]);
                HAL_SetRTCMin(uniUSBRecvFrame.ucSingleByte[9]);
                HAL_SetRTCSec(uniUSBRecvFrame.ucSingleByte[10]);
                break;
                
///////////////// COMMAND : USB_FRAME_COMMAND_UPDATE_TIME_TICK /////////////////               
            case USB_FRAME_COMMAND_UPDATE_TIME_TICK:
                // Transmit ACK
                SYS_USB_SendACK(uniUSBRecvFrame.stUSBFrame.ucCommand);
                uniSecondTimeTick.ucSingleByte[7] = uniUSBRecvFrame.ucSingleByte[11];
                uniSecondTimeTick.ucSingleByte[6] = uniUSBRecvFrame.ucSingleByte[10];
                uniSecondTimeTick.ucSingleByte[5] = uniUSBRecvFrame.ucSingleByte[9];
                uniSecondTimeTick.ucSingleByte[4] = uniUSBRecvFrame.ucSingleByte[8];
                uniSecondTimeTick.ucSingleByte[3] = uniUSBRecvFrame.ucSingleByte[7];
                uniSecondTimeTick.ucSingleByte[2] = uniUSBRecvFrame.ucSingleByte[6];
                uniSecondTimeTick.ucSingleByte[1] = uniUSBRecvFrame.ucSingleByte[5];
                uniSecondTimeTick.ucSingleByte[0] = uniUSBRecvFrame.ucSingleByte[4];
                
                // Wait until millisecond part becomes zero
                while(uniMillisecondTimeTick.ullMillisecondTimeTick != 0);
                // Read RTC time and save it to 'uniRTCTime'
                HAL_RTC_ReadRTCTime();
                // Store 'uniRTCTime (8 bytes)' to the first entry of internal FLASH bank 3 (0x038000) 
                // This information will later be used to restore system time 
                HAL_FLASH_Write(sizeof(uniRTCTime), (unsigned char *)&uniRTCTime, (unsigned char *)MSP430_INTERNAL_FLASH_BANK3_ADDRESS);
                // Store 'uniSecondTimeTick (8bytes)' next to 'uniRTCTime' stored to internal FLASH bank 3 (0x038008)
                // This information will later be used to restore system time
                HAL_FLASH_Write(sizeof(uniSecondTimeTick), (unsigned char *)&uniSecondTimeTick, (unsigned char *)(MSP430_INTERNAL_FLASH_BANK3_ADDRESS + sizeof(uniRTCTime)));
                
                if(SYS_MMC_IsMicroSDInserted() == _TRUE_){
                    if(ulNumberOfCLEONSamplesInAFileCount == 0){
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
                    }
                }
                
                // Now that time is synchronized, set flag 
                bFLAG_IsTimeSynced = _SET_;
                
                // Turn LED3 off 
                TIME_SYNC_ERROR_LED_OFF();
                break;
                
///////////////// COMMAND : USB_FRAME_COMMAND_UPDATE_SAMPLE_COUNT //////////////
            case USB_FRAME_COMMAND_UPDATE_SAMPLE_COUNT:
                // Transmit ACK
                SYS_USB_SendACK(uniUSBRecvFrame.stUSBFrame.ucCommand);
                ulSampleCount = ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[3] << 24) 
                                 + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[2] << 16) 
                                 + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[1] << 8) 
                                 + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[0]); 
                break;

///////////////// COMMAND : USB_FRAME_COMMAND_UPDATE_SAMPLE_GAP ////////////////
            case USB_FRAME_COMMAND_UPDATE_SAMPLE_GAP:
                // Transmit ACK
                SYS_USB_SendACK(uniUSBRecvFrame.stUSBFrame.ucCommand);
                ulSampleGap = ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[3] << 24) 
                               + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[2] << 16) 
                               + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[1] << 8) 
                               + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[0]); 
                break;

///////////////// COMMAND : USB_FRAME_COMMAND_UPDATE_CHUNK_COUNT ///////////////
            case USB_FRAME_COMMAND_UPDATE_CHUNK_COUNT:
                // Transmit ACK
                SYS_USB_SendACK(uniUSBRecvFrame.stUSBFrame.ucCommand);
                ulChunkCount = ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[3] << 24) 
                                + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[2] << 16) 
                                + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[1] << 8) 
                                + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[0]); 
                break;

///////////////// COMMAND : USB_FRAME_COMMAND_UPDATE_CHUNK_GAP //////////////
            case USB_FRAME_COMMAND_UPDATE_CHUNK_GAP:
                // Transmit ACK
                SYS_USB_SendACK(uniUSBRecvFrame.stUSBFrame.ucCommand);
                ulChunkGap = ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[3] << 24) 
                              + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[2] << 16) 
                              + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[1] << 8) 
                              + ((unsigned long)uniUSBRecvFrame.stUSBFrame.ucData[0]); 
                break;                
                
///////////////////////////////////// DEFAULT //////////////////////////////////
            default: break;
        }    
    }
}










