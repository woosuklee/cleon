/**
@file       hal_rtc.c

@brief      RTC configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

#define GPS_TEST_ON_INITIALIZATION      _ENABLE_    

// CLEON data structure
extern cleon_header_u      uniCLEONHeader;
extern cleon_gps_data_u    uniCLEONGPSData;
extern cleon_sensor_data_u uniCLEONSensorData;

// Flags
extern bool bFLAG_CLEONSampleIntervalConuter;
extern bool bFLAG_ISLoggingRequested;
       bool bFLAG_IsTimeSynced = _CLEAR_;
       bool bFLAG_RTCTimeTickOfSecond = _CLEAR_;
       bool bFLAG_ISResetOccuredBySVSHPOR = _CLEAR_;

// Variable for storing SYSRSTIV
extern volatile unsigned int SysRstIv;

// User parameters
extern unsigned long   ulSampleCount;
extern unsigned long   ulSampleGap;
extern unsigned long   ulChunkCount;
extern unsigned long   ulChunkGap;

// RTC time
rtc_time_u  uniRTCTime  = {0};

// Second and millisecond time tick
milliseoncd_time_tick_u uniMillisecondTimeTick = {0};
seoncd_time_tick_u      uniSecondTimeTick = {0};

// Counting number of seconds
unsigned char ucRTCNumberOfSecondCounter = 0;

// Measuring time taken by each step of CLEON data logging
time_measurement_s  stTimeMeasurement[NUMBER_OF_TIME_MEASUREMENT_TEST] = {0};

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing RTC

@return   void 

@param    void
*/
void HAL_RTC_Init()
{ 
    // Check if reset has been occured by SVSH(POR)
    if((SysRstIv == 0x0E)||(SysRstIv == 0x02)){
        // Restore previous setting
        RTCCTL01 |=  RTCRDYIE;	        // enable rtc ready interrupt; 1sec
        RTCCTL01 &= ~RTCHOLD;	        // release rtchold, begin count
        // Restore time tick
        HAL_RTC_RestoreTimeTick();
    }else{
        RTCCTL01 |= RTCHOLD;            // hold rtc for setting; 
        RTCCTL01 |= RTCRDYIE;	        // enable rtc ready interrupt; 1sec
    
        // Example RTC setting
        RTCYEAR = 1980;                 // Year    = 1980
        RTCMON  = 1;                    // Month   = 1 (Jan)
        RTCDAY  = 1;                    // Day     = 1 (1st)
        RTCHOUR = 0;                    // Hour    = 0
        RTCMIN  = 0;                    // Minute  = 0
        RTCSEC  = 0;                    // Seconds = 0
    
        RTCCTL01 &= ~RTCHOLD;	        // release rtchold, begin count
        
        // Indicating that system time is not synchronized
        TIME_SYNC_ERROR_LED_ON();
    }
    
    // Change backup system's power source from backup battery to main power
    while(BAKCTL & LOCKBAK)	BAKCTL &= ~(LOCKBAK);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Read RTC time

@return   void 

@param    void
*/
void HAL_RTC_ReadRTCTime(void)
{
    // Only the value of 'Current-year - 1980' will be stored to 'ucYear' field to save memory
    uniRTCTime.stRTCTime.ucYear        = (unsigned char) (HAL_GetRTCYear() - 1980);     
    uniRTCTime.stRTCTime.ucMonth       = (unsigned char) HAL_GetRTCMon();
    uniRTCTime.stRTCTime.ucDay         = (unsigned char) HAL_GetRTCDay();
    uniRTCTime.stRTCTime.ucHour        = (unsigned char) HAL_GetRTCHour();
    uniRTCTime.stRTCTime.ucMinute      = (unsigned char) HAL_GetRTCMin();
    uniRTCTime.stRTCTime.ucSecond      = HAL_GetRTCSec();
    uniRTCTime.stRTCTime.uiMillisecond = HAL_GetRTCMilisec();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Restore time tick value out of RTC

@return   void 

@param    void
*/
void HAL_RTC_RestoreTimeTick(void)
{
    // Temporary variables
    rtc_time_u          uniTempRTCTime;
    seoncd_time_tick_u  uniTempSecondTimeTick;
    
    // Storing time difference
    unsigned long long ullRTCTimeDifferenceConvertedToTimeTick = 0;
    
    // Read stored time information, which was written to FLASH when time was synchronized
    memcpy(&uniTempRTCTime, (unsigned char *)MSP430_INTERNAL_FLASH_BANK3_ADDRESS, sizeof(uniTempRTCTime));
    memcpy(&uniTempSecondTimeTick, (unsigned char *)(MSP430_INTERNAL_FLASH_BANK3_ADDRESS + sizeof(uniTempRTCTime)), sizeof(uniTempSecondTimeTick));    
    
    // Read RTC time
    HAL_RTC_ReadRTCTime();

    // Validity check for restored time tick
    if(uniRTCTime.stRTCTime.ucYear < 32){
        // Note that 'uniRTCTime.stRTCTime.ucYear = 0' means current year of RTC is set to 1980 (See, HAL_RTC_ReadRTCTime())
        // Because this source is being written in year of 2012, 'ucYear' cannot be smaller than 32 (2012 - 1980 = 32)
        // If uniRTCTime.stRTCTime.ucYear is equal to zero, it means that current system RTC time has neither been synchronized nor restored correctly
        TIME_SYNC_ERROR_LED_ON();
    }else{    
        // If it is confirmed that RTC has been running even for the period of main-battery-failure, then, get current time tick out of RTC time
        // First, we get total seconds out of both current and stored RTC time
        // After that, get the differnce in seconds and add it to the previous time tick which has been kept in FLASH from last time synchronization
        ullRTCTimeDifferenceConvertedToTimeTick = HAL_RTC_GetSecondConvertedFromStoredRTCTime(&uniRTCTime) - HAL_RTC_GetSecondConvertedFromStoredRTCTime(&uniTempRTCTime);
        uniSecondTimeTick.ullSecondTimeTick = uniTempSecondTimeTick.ullSecondTimeTick + (ullRTCTimeDifferenceConvertedToTimeTick * 10000000);    
          
        // Set flag
        bFLAG_IsTimeSynced = _SET_;
        
        // Turn LED3 off 
        TIME_SYNC_ERROR_LED_OFF();
    }
}

/*----------------------------------------------------------------------------*/
/**
@brief    Get total seconds converted from RTC time

@return   llConvertedSecond - total time elapsed from year of '0', which is represented as seconds

@param    ptruniTempRTCTime - RTC time which has been kept in FLASH from last time synchronization
*/
unsigned long long HAL_RTC_GetSecondConvertedFromStoredRTCTime(rtc_time_u *ptruniTempRTCTime)
{
    // Total elapsed time represented in seconds
    unsigned long long ullConvertedSecond = 0;
    
    // Variable for checking leap year
    bool bIsLeapYear = _FALSE_;
    
    // 'ucYear' is regarded as leap year if it is divisible by 4
    // (See, pp. 567 of 'MSP430x5xx and MSP430x6xx Family User's Guide)
    if(ptruniTempRTCTime->stRTCTime.ucYear % 4 == 0) bIsLeapYear = _TRUE_;
    
    // Get total elapsed seconds from the first day of year 1 to the first day of RTC year
    ullConvertedSecond += (ptruniTempRTCTime->stRTCTime.ucYear - 1) * 365 * 24 * 60 * 60;
    // Compansate days in leap years
    ullConvertedSecond += (ptruniTempRTCTime->stRTCTime.ucYear / 4);
    
    // Get total elapsed seconds from the first day of this year to the end of previous month of RTC month
    switch(ptruniTempRTCTime->stRTCTime.ucMonth){
        case 1: // Jan
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_JAN * 24 * 60 * 60;
            break;
        case 2: // Feb
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_FEB * 24 * 60 * 60;
            break;
        case 3: // Mar
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_MAR * 24 * 60 * 60;
            break;
        case 4: // Apr
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_APR * 24 * 60 * 60;
            break;
        case 5: // May
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_MAY * 24 * 60 * 60;
            break;
        case 6: // Jun
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_JUN * 24 * 60 * 60;
            break;
        case 7: // Jul
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_JUL * 24 * 60 * 60;
            break;
        case 8: // Aug
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_AUG * 24 * 60 * 60;
            break;
        case 9: // Sep
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_SEP * 24 * 60 * 60;
            break;
        case 10:// Oct
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_OCT * 24 * 60 * 60;
            break;
        case 11:// Nov
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_NOV * 24 * 60 * 60;
            break;
        case 12:// Dec
            ullConvertedSecond += (unsigned long long)DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_DEC * 24 * 60 * 60;
            break;
        default:  
            break;
    }
    
    // If it is leap year, add one day
    if((bIsLeapYear == _TRUE_)&&(ptruniTempRTCTime->stRTCTime.ucMonth != 1)){
        ullConvertedSecond += (long long)24 * 60 * 60;
    }
    
    ullConvertedSecond += ((ptruniTempRTCTime->stRTCTime.ucDay)-1) * 24 * 60 * 60;
    ullConvertedSecond += ((ptruniTempRTCTime->stRTCTime.ucHour)-1) * 60 * 60;    
    ullConvertedSecond += ((ptruniTempRTCTime->stRTCTime.ucMinute)-1) * 60;
    ullConvertedSecond += ptruniTempRTCTime->stRTCTime.ucSecond;
    
    // Total elapsed time from the first day of year 1, which is represented in seconds
    return ullConvertedSecond;
}

//------------------------------------------------------------------------------
// RTC Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector=RTC_VECTOR
__interrupt void RTC_VECTOR_ISR(void)
{
    switch(__even_in_range(RTCIV,16))
    {
        case RTC_NONE:          // No interrupts
            break;
        case RTC_RTCRDYIFG:     // RTCRDYIFG
            bFLAG_RTCTimeTickOfSecond = _SET_;   
          
            // Reset millisecond to zero
            uniMillisecondTimeTick.ullMillisecondTimeTick = 0; 
            
            // Increase second time tick 
            uniSecondTimeTick.ullSecondTimeTick += 10000000;

            // Set flag if condition is met
            if(bFLAG_ISLoggingRequested == _SET_){ 
                if(ulSampleCount != 1){
                    // Check if defined time interval for logging has elapsed
                    if(ucRTCNumberOfSecondCounter == ((ulSampleGap/1000)-1)){
                        ucRTCNumberOfSecondCounter = 0;
                        bFLAG_CLEONSampleIntervalConuter = _SET_;
                        // If defined time interval for logging has elapsed, exit low-power-mode
                        __low_power_mode_off_on_exit();
                    }else{
                        if(bFLAG_ISLoggingRequested == _SET_){
                            ucRTCNumberOfSecondCounter++;
                        }
                    } 
                }
            }  
            break;
        case RTC_RTCTEVIFG:     // RTCEVIFG
            break;
        case RTC_RTCAIFG:       // RTCAIFG
            break;
        case RTC_RT0PSIFG:      // RT0PSIFG
            break;
        case RTC_RT1PSIFG:      // RT1PSIFG
            break;
        case 12: break;         // Reserved
        case 14: break;         // Reserved
        case 16: break;         // Reserved
        default: break;
    }
}

////////////////////////////// Set RTC values //////////////////////////////////
unsigned int HAL_SetRTCYear(int year)
{
    RTCYEAR = year;
    return year;
}

unsigned int HAL_SetRTCMon(int month)
{
    RTCMON = month;
    return month;
}

unsigned int HAL_SetRTCDay(int day)
{
    RTCDAY = day;
    return day;
}

unsigned int HAL_SetRTCDow(int dow)
{
    RTCDOW = dow;
    return dow;
}

unsigned int HAL_SetRTCHour(int hour)
{
    RTCHOUR = hour;
    return hour;
}

unsigned int HAL_SetRTCMin(int min)
{
    RTCMIN = min;
    return min;
}

unsigned int HAL_SetRTCSec(int sec)
{
    RTCSEC = sec;
    return sec;
}

unsigned int HAL_SetRTCMilisec(int millisec)
{
    uniRTCTime.stRTCTime.uiMillisecond = uniMillisecondTimeTick.ullMillisecondTimeTick/10000;
    return (uniRTCTime.stRTCTime.uiMillisecond);
}

////////////////////////////// HAL_Get RTC values //////////////////////////////////
unsigned int HAL_GetRTCYear(void)
{
    return RTCYEAR;
}

unsigned int HAL_GetRTCMon(void)
{
    return RTCMON;
}

unsigned int HAL_GetRTCDow(void)
{
    return RTCDOW;
}

unsigned int HAL_GetRTCDay(void)
{
    return RTCDAY;
}

unsigned int HAL_GetRTCHour(void)
{
    return RTCHOUR;
}

unsigned int HAL_GetRTCMin(void)
{
    return RTCMIN;
}

unsigned int HAL_GetRTCSec(void)
{
    return RTCSEC;
}

unsigned int HAL_GetRTCMilisec(void)
{
    unsigned int uiTemp = 0;
    uiTemp = (unsigned int)(uniMillisecondTimeTick.ullMillisecondTimeTick/10000);
    
    if(uiTemp > 999) uiTemp = 0;  // set value of ullMillisecondTick to 0, if it exceeds 999, to correct 1ms error
    return uiTemp;
}
