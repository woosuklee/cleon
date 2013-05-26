/**
@file       sys_init.h

@brief      Initializing the system
*/

#ifndef __SYS_INIT_H__
#define __SYS_INIT_H__

#define SIZE_OF_HEADER_IN_BYTE      44
#define SIZE_OF_GPS_SIGANL_IN_BYTE  8184
#define SIZE_OF_SENSOR_DATA_IN_BYTE 8

typedef union{
    unsigned char ucSingleByte[4];  // 4 bytes
    unsigned long ulOriginalVar;    // 4 bytes
}signature_u;   // 4 bytes

typedef union{  
    unsigned char ucSingleByte[4];  // 4 bytes
    unsigned long ulOriginalVar;    // 4 bytes
}version_u;     // 4 bytes

typedef struct{
    signature_u                 uniSignature;           // 4    bytes
    version_u                   uniVersion;             // 4    bytes
    milliseoncd_time_tick_u     uniMillisecondTimeTick; // 8    bytes
    receiver_setting_s          stReceiverSetting;      // 12   bytes 
    chunk_count_u               uniChunkCount;          // 4    bytes
    chunk_size_ms_u             uniChunkSizeMS;         // 4    bytes
    chunk_gap_ms_u              uniChunkGapMS;          // 4    bytes
    gps_signal_data_format_u    uniGPSSignalDataFormat; // 4    bytes
}cleon_header_s;// 44 bytes                                                            

typedef union{
    cleon_header_s      stCLEONHeader;
    unsigned char       ucSingleBytes[44];
}cleon_header_u;

void SYS_Init(void);
void SYS_InitCLEONDataStructure(void);

#endif
