/**
@file       hal_rtc.h

@brief      Initializing RTC
*/

#ifndef __HAL_RTC_H__
#define __HAL_RTC_H__

#define HAL_RTC_EnableRDYIE()   RTCCTL01 |=  RTCRDYIE;	        // enable rtc ready interrupt; 1sec
#define HAL_RTC_DisableRDYIE()  RTCCTL01 &= ~RTCRDYIE;	        // enable rtc ready interrupt; 1sec

#define HAL_RTC_Reset() do{                \
    RT0PS = 0;                             \
    RT1PS = 0;                             \
}while(0)

#define HAL_RTC_StartMillisecondTimer()     TA1CCR0 = 12000;    // Count 12000 to generate 1ms tick with 12MHz clock source
#define HAL_RTC_StopMillisecondTimer()      TA1CCR0 = 0;        // Stop millisecond timer by setting TA0CCR0 to 0

#define HAL_RTC_EnableSecondInterrupt()     RTCCTL01 |=  RTCRDYIE	        // Enable rtc ready interrupt; 1sec
#define HAL_RTC_DisableSecondInterrupt()    RTCCTL01 &= ~RTCRDYIE	        // Disable rtc ready interrupt; 1sec

// This is for a normal year, it should be adjusted if leap year, 
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_JAN      (0)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_FEB      (31)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_MAR      (31 + 28)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_APR      (31 + 28 + 31)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_MAY      (31 + 28 + 31 + 30)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_JUN      (31 + 28 + 31 + 30 + 31)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_JUL      (31 + 28 + 31 + 30 + 31 + 30)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_AUG      (31 + 28 + 31 + 30 + 31 + 30 + 31)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_SEP      (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_OCT      (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_NOV      (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31)
#define DAYS_IN_MONTHS_BEFORE_THE_FIRST_DAY_OF_DEC      (31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)

typedef union{
    unsigned long long ullSecondTimeTick;
    unsigned char      ucSingleByte[8];
}seoncd_time_tick_u;

typedef union{
    unsigned long long ullMillisecondTimeTick;
    unsigned char      ucSingleByte[8];
}milliseoncd_time_tick_u;

typedef struct{
    unsigned char ucYear;           // 1 bytes
    unsigned char ucMonth;          // 1 byte
    unsigned char ucDay;            // 1 byte
    unsigned char ucHour;           // 1 byte
    unsigned char ucMinute;         // 1 byte
    unsigned char ucSecond;         // 1 byte
    unsigned int  uiMillisecond;    // 2 byte
}rtc_time_s;    // 8 bytes

typedef union{
    rtc_time_s    stRTCTime;
    unsigned char ucSingleBytes[8];
}rtc_time_u;

#define TIME_MEASUREMENT                    _DISABLE_
#define NUMBER_OF_TIME_MEASUREMENT_TEST     10

typedef struct{
    unsigned long long ullTimeAtUserButtonPressed;
    unsigned long long ullTimeAtStartAPPLog; 
    unsigned long long ullTimeAtWriteHeaderToSDCard;
    unsigned long long ullTimeAtFirstChunk;
}time_measurement_s;

void HAL_RTC_Init(void);
void HAL_RTC_ReadRTCTime(void);
void HAL_RTC_RestoreTimeTick(void);

unsigned long long HAL_RTC_GetSecondConvertedFromStoredRTCTime(rtc_time_u *uniTempRTCTime);
    
unsigned int HAL_SetRTCYear(int year);
unsigned int HAL_SetRTCMon(int month);
unsigned int HAL_SetRTCDay(int day);
unsigned int HAL_SetRTCDow(int dow);
unsigned int HAL_SetRTCHour(int hour);
unsigned int HAL_SetRTCMin(int min);
unsigned int HAL_SetRTCSec(int sec);
unsigned int HAL_SetRTCMilisec(int millisec);

unsigned int HAL_GetRTCYear(void);
unsigned int HAL_GetRTCMon(void);
unsigned int HAL_GetRTCDow(void);
unsigned int HAL_GetRTCDay(void);
unsigned int HAL_GetRTCHour(void);
unsigned int HAL_GetRTCMin(void);
unsigned int HAL_GetRTCSec(void);
unsigned int HAL_GetRTCMilisec(void);

#endif
