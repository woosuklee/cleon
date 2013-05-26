/**
@file       sys_gps.h

@brief      GPS related functions
*/

#ifndef __SYS_GPS_H__
#define __SYS_GPS_H__

#include <hal_define.h>

#define SIZE_OF_GPS_DATA_CHUNK_IN_BYTE     8184    // 2ms @ sampling frequency of 16.368MHz
#define MAX_NUMBER_OF_GPS_TEST_COUNT       3       // Maximum number of attempts for GPS test

#define MAX2769_SHDN_N_HIGH()   MAX2769_SHDN_N_PORT(OUT) |=  BV(MAX2769_SHDN_N_PIN);    
#define MAX2769_SHDN_N_LOW()    MAX2769_SHDN_N_PORT(OUT) &= ~BV(MAX2769_SHDN_N_PIN);    
    
#define MAX2769_IDLE_N_HIGH()   MAX2769_IDLE_N_PORT(OUT) |=  BV(MAX2769_IDLE_N_PIN);    
#define MAX2769_IDLE_N_LOW()    MAX2769_IDLE_N_PORT(OUT) &= ~BV(MAX2769_IDLE_N_PIN);    

// (n * 1ms) delay @ 12MHz
#define SYS_GPS_TimingDealy(n)  do{                                     \
    for(int z = 0 ; z < n ; z++){                                       \
        __delay_cycles(12000);                                          \
    }                                                                   \
}while(0)       

// Receiver setting
typedef union{
    unsigned char ucSingleByte[4];          // 4 bytes
    unsigned long ulOriginalVar;            // 4 bytes
}samples_per_ms_u;  // 4 bytes

typedef union{
    unsigned char ucSingleByte[8];          // 8 bytes
    double        dOriginalVar;             // 8 bytes
}intermediate_frequency_u;  // 8 bytes

typedef struct{
    samples_per_ms_u            uniSamplesPerMS;        // 4 bytes
    intermediate_frequency_u    uniIntermediateFreq;    // 8 bytes
}receiver_setting_s;    //  12 bytes

// Chunk count
typedef union{
    unsigned char ucSingleByte[4];          // 4 bytes
    unsigned long ulOriginalVar;            // 4 bytes
}chunk_count_u;     // 4 bytes

// Chunk size
typedef union{
    unsigned char ucSingleByte[4];          // 4 bytes
    unsigned long ulOriginalVar;            // 4 bytes
}chunk_size_ms_u;   // 4 bytes

// Chunk gap
typedef union{
    unsigned char ucSingleByte[4];          // 4 bytes
    unsigned long ulOriginalVar;            // 4 bytes
}chunk_gap_ms_u;    // 4 bytes

// Signal data format
typedef union{
        unsigned char ucSingleByte[4];      // 4 bytes
        unsigned long ulOriginalVar;        // 4 bytes
}gps_signal_data_format_u;  // 4 bytes

// GPS signal data
typedef union {
    unsigned char       ucSingleByte[SIZE_OF_GPS_DATA_CHUNK_IN_BYTE];   // 8184 bytes
    unsigned char       ucSignal[SIZE_OF_GPS_DATA_CHUNK_IN_BYTE];       // 8184 bytes
}cleon_gps_data_u;    // 8184 bytes

void SYS_GPS_Init(void);
void SYS_GPS_InitDataIO(void);
void SYS_GPS_InitControlIO(void);
void SYS_GPS_InitProgramIO(void);
void SYS_GPS_StartOperation(void);
void SYS_GPS_StopOperation(void);
bool SYS_GPS_TestGPS(unsigned int uiDelay);

#endif
