/**
@file       sys_usb.c

@brief      USB related functions
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
bool bFLAG_USBFrameReceived = _CLEAR_;
bool bFLAG_USBConnected = _CLEAR_;

// USB frames
usb_frame_u uniUSBRecvFrame = {0};
usb_frame_u uniUSBSendFrame = {0};

// Counting received number of USB frame bytes for state transition
unsigned char ucUSBFrameByteCount = 0;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing USB

@return   void

@param    void
*/
void SYS_USB_Init()
{
    USB_PORT(SEL) |= BV(USB_TXD_PIN);
    USB_PORT(SEL) |= BV(USB_RXD_PIN);
    
    USB_STATUS_PORT(DIR) &= ~BV(USB_STATUS_PIN);
    USB_STATUS_PORT(IES) &= ~BV(USB_STATUS_PIN);
    USB_STATUS_PORT(IE)  |=  BV(USB_STATUS_PIN);
    
    if(SYS_USB_IsUSBConnected() == _TRUE_){
        bFLAG_USBConnected = _TRUE_;
    }
}

/*----------------------------------------------------------------------------*/
/**
@brief		Sending USB frame ACK

@return		void

@param		ucCommand - command to be sent
*/
void SYS_USB_SendACK(unsigned char ucCommand)
{
    memset(&uniUSBSendFrame, 0 , sizeof(uniUSBSendFrame));
    
    uniUSBSendFrame.stUSBFrame.ucHeader[0] = USB_FRAME_HEADER;
    uniUSBSendFrame.stUSBFrame.ucHeader[1] = USB_FRAME_HEADER;
    
    uniUSBSendFrame.stUSBFrame.ucLength = 0x02;
    
    uniUSBSendFrame.stUSBFrame.ucCommand = USB_FRAME_COMMAND_ACK;
    
    uniUSBSendFrame.stUSBFrame.ucData[0] = ucCommand;
    
    uniUSBSendFrame.stUSBFrame.ucCRC = SYS_USB_BuildCRC(&uniUSBSendFrame);
    
    uniUSBSendFrame.stUSBFrame.ucFooter[0] = USB_FRAME_FOOTER;
    uniUSBSendFrame.stUSBFrame.ucFooter[1] = USB_FRAME_FOOTER;
    
    for(int i = 0; i < USB_FRAME_LENGTH ; i++){
        SYS_USB_TransmitSingleByte(uniUSBSendFrame.ucSingleByte[i]);
    }
}

/*----------------------------------------------------------------------------*/
/**
@brief    Mimic the standard 'printf' with USB as output device
          - Sending a byte (stream) to USB

@return   index - number of bytes sent

@param    data  - a byte to be sent to USB
*/
int SYS_USB_Printf(char *format, ... )
{
    // Maximum length of data is 255
    char            buff[250];
    int             index = 0;
    unsigned char   data = 0;
    va_list         argptr;         // Argument list

    buff[0] = 0x00;
    va_start( argptr,format);       // Initializing the function
    vsprintf(buff,format,argptr );  // Copying buffer
    va_end( argptr );               // Finalizing the function

    data = buff[index++];               // Loading data
    
    while(data != 0x00){
        //USCI_A1 TX buffer ready?
        while (!(UCA1IFG&UCTXIFG));     // USCI_A1 TX buffer ready?
        UCA1TXBUF = data;               
        data = buff[index++];
    }
    return(index);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Transmit a byte vis USB

@return   void

@param    ucData - data to be transmitted
*/
void SYS_USB_TransmitSingleByte(unsigned char ucData)
{
    //USCI_A1 TX buffer ready?
    while (!(UCA1IFG&UCTXIFG));     // USCI_A1 TX buffer ready?
    UCA1TXBUF = ucData;
}

/*----------------------------------------------------------------------------*/
/**
@brief		Receiving a bayte and test whether correct frame has been received

@return		void

@param		ucData - recived single byte from USB
*/
void SYS_USB_ReceiveFrame(unsigned char ucData)
{
    if(bFLAG_USBFrameReceived == _CLEAR_){
        switch (ucUSBFrameByteCount){
            case 0: case 1:
                if(ucData == USB_FRAME_HEADER){
                    uniUSBRecvFrame.stUSBFrame.ucHeader[ucUSBFrameByteCount] = ucData;
                    ucUSBFrameByteCount++;
                }else{
                    ucUSBFrameByteCount = 0;
                    // request retransmission
                }
                break;
            case 2:
                if((uniUSBRecvFrame.stUSBFrame.ucHeader[0] == USB_FRAME_HEADER)&&(uniUSBRecvFrame.stUSBFrame.ucHeader[1] == USB_FRAME_HEADER) &&
                   (ucData > 0) && (ucData < USB_FRAME_COMMAND_AND_DATA_FIELD_LENGTH)){
                    uniUSBRecvFrame.stUSBFrame.ucLength = ucData;
                    ucUSBFrameByteCount++;
                }else{
                    ucUSBFrameByteCount = 0;
                    // request retransmission
                }
                break;
            case 3:
                uniUSBRecvFrame.stUSBFrame.ucCommand = ucData;
                ucUSBFrameByteCount++;
                
                break;
            case 4:  case 5:  case 6:  case 7:  case 8:  case 9:  case 10: case 11: case 12: case 13: 
            case 14: case 15: case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: 
            case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31: case 32:
                uniUSBRecvFrame.stUSBFrame.ucData[ucUSBFrameByteCount - 4] = ucData;
                ucUSBFrameByteCount++;
                break;
            case 33:
                if(ucData == SYS_USB_BuildCRC(&uniUSBRecvFrame)){
                    uniUSBRecvFrame.stUSBFrame.ucCRC = ucData;
                    ucUSBFrameByteCount++;
                }else{
                    ucUSBFrameByteCount = 0;
                    // request retransmission
                }
                break;
            case 34:
                if(ucData == USB_FRAME_FOOTER){
                    uniUSBRecvFrame.stUSBFrame.ucFooter[ucUSBFrameByteCount - 34] = ucData;
                    ucUSBFrameByteCount++;
                }else{
                    ucUSBFrameByteCount = 0;
                    // request retransmission
                }
                break;
            case 35:
                if(ucData == USB_FRAME_FOOTER){
                    uniUSBRecvFrame.stUSBFrame.ucFooter[ucUSBFrameByteCount - 34] = ucData;
                    bFLAG_USBFrameReceived = _SET_;
                    ucUSBFrameByteCount = 0;
                }else{
                    ucUSBFrameByteCount = 0;
                    // request retransmission
                }
                break;
            default:
                // request retransmission
                break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/**
@brief		Building CRC out of USB frame

@return		ucCheckOctet - generated CRC

@param		void
*/
unsigned char SYS_USB_BuildCRC(usb_frame_u *uniUSBFrame)
{	
    unsigned char ucCheckOctet = 0;
    
    for ( int i = 3 ; i < (USB_FRAME_LENGTH - 3) ; i++ ){
		if( i == 0 ){
			ucCheckOctet = (*uniUSBFrame).ucSingleByte[i];
		}
		if (i >= 1)
		{
			ucCheckOctet ^= (*uniUSBFrame).ucSingleByte[i];
		}
	}
        
    ucCheckOctet = ~ucCheckOctet;
        
	return ucCheckOctet;
}






























