/**
@file       sys_usb.h

@brief      USB related functions
*/

#ifndef __SYS_USB_H__
#define __SYS_USB_H__

////////////////////////////////////////////////////////////////////////////////
#define USB_FRAME_HEADER    0x55
#define USB_FRAME_FOOTER    0xAA

#define USB_FRAME_DATA_FIELD_LENGTH                 29
#define USB_FRAME_COMMAND_AND_DATA_FIELD_LENGTH     30
#define USB_FRAME_LENGTH                            36

#define USB_FRAME_COMMAND_CONNECT                               0x01
#define USB_FRAME_COMMAND_UPDATE_RTC_TIME                       0x02
#define USB_FRAME_COMMAND_UPDATE_TIME_TICK                      0x03   
#define USB_FRAME_COMMAND_UPDATE_SAMPLE_COUNT                   0x04
#define USB_FRAME_COMMAND_UPDATE_SAMPLE_GAP                     0x05
#define USB_FRAME_COMMAND_UPDATE_CHUNK_COUNT                    0x06
#define USB_FRAME_COMMAND_UPDATE_CHUNK_GAP                      0x07
#define USB_FRAME_COMMAND_ACK                                   0xFF
////////////////////////////////////////////////////////////////////////////////

typedef struct{
    unsigned char   ucHeader[2];                            // 2  bytes
    unsigned char   ucLength;                               // 1  bytes
    unsigned char   ucCommand;                              // 1  bytes
    unsigned char   ucData[USB_FRAME_DATA_FIELD_LENGTH];    // 29 bytes
    unsigned char   ucCRC;                                  // 1  byte
    unsigned char   ucFooter[2];                            // 2  bytes
}usb_frame_s;       // 36 bytes

typedef union{
    usb_frame_s     stUSBFrame;
    unsigned char   ucSingleByte[USB_FRAME_LENGTH];
}usb_frame_u;

#define SYS_USB_IsUSBConnected()  ((USB_STATUS_PORT(IN) & BV(USB_STATUS_PIN)) >> USB_STATUS_PIN)

void SYS_USB_Init(void);
void SYS_USB_SendACK(unsigned char ucCommand);
int  SYS_USB_Printf(char *format, ... );
void SYS_USB_ReceiveFrame(unsigned char ucData);
unsigned char SYS_USB_BuildCRC(usb_frame_u *uniUSBFrame);
void SYS_USB_TransmitSingleByte(unsigned char ucData);

#endif
