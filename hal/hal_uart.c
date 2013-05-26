/**
@file       hal_uart.c

@brief      UART configuration
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

// Flags
extern bool bFLAG_USBFrameReceived;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing UART

@return   void 

@param    void
*/
void HAL_UART_Init()
{ 
    USB_PORT(SEL) |= BV(USB_TXD_PIN);
    USB_PORT(SEL) |= BV(USB_RXD_PIN);

    // UART, which is used for USB, is set to 57600 bps
    // (see, pp909 of 'MSP430x5xx and MSP430x6xx family user's guide')    
    UCA1CTL1 |=  UCSWRST;       // **Put state machine in reset**
    UCA1CTL1 |=  UCSSEL_2;      // Set SMCLK as clock source
    UCA1BRW   =  208;           // 12MHz 57600 bps (see User's Guide)
    UCA1MCTL  =  UCBRS_2;       // Modln UCBRSx=2, UCBRFx=0,

    UCA1CTL1 &= ~UCSWRST;       // **Initialize USCI state machine**
    UCA1IE   |=  UCRXIE;        // Enable USCI_A1 RX interrupt
}

//------------------------------------------------------------------------------
// USCI_A1 Interrupt Service Routine
//------------------------------------------------------------------------------
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_VECTOR_ISR(void)
{
    unsigned char ucRxData;
    switch(__even_in_range(UCA1IV,12)){
        case  0: break;
        case  2: 
            ucRxData = UCA1RXBUF;
            SYS_USB_ReceiveFrame(ucRxData);
            // If a complete USB frame has received successfully, exit low-power-mode
            if(bFLAG_USBFrameReceived == _SET_){
                __low_power_mode_off_on_exit();
            }
            break;     
        case  4: break;
        case  6: break;
        case  8: break;
        case 10: break;
        case 12: break;
        default: break;
    }
}
