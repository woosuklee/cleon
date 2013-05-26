/**
@file       sys_mmc.c

@brief      Micro SD card related functions
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

// File system variables
extern FATFS Fatfs;     
extern FIL   fileObject;      

// Flags
bool bFLAG_MicroSDInserted = _CLEAR_;

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing microcontroller

@return   void 

@param    void
*/
void SYS_MMC_Init(void)
{ 
    SYS_MMC_InitIO();
    
    // Check if MicroSD is inserted
    if(SYS_MMC_IsMicroSDInserted() == _SET_){
        bFLAG_MicroSDInserted = _SET_;
        
#if SYS_MMC_LED_INDICATION_FOR_ERROR == _ENABLE_      
        MICRO_SD_ERROR_LED_OFF();
#endif                
        
        // Initializing file system
        FS_Init();
        
        // Mount FatFs moudle
        FS_Mount(0, &Fatfs);
        
        // Create directory named 'CLEON'
        FS_MakeDir("CLEON");
        
        // Change directory to 'CLEON'
        FS_ChangeDir("/CLEON");
        
    }else{
#if SYS_MMC_LED_INDICATION_FOR_ERROR == _ENABLE_      
        MICRO_SD_ERROR_LED_ON();
#endif        
    }
}

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing Micro SD interface pins

@return   void 

@param    void
*/
void SYS_MMC_InitIO(void)
{
    MICROSD_MOSI_PORT(SEL)   |=  BV(MICROSD_MOSI_PIN);
    MICROSD_MISO_PORT(SEL)   |=  BV(MICROSD_MISO_PIN);
    MICROSD_CLK_PORT(SEL)    |=  BV(MICROSD_CLK_PIN);
    
    MICROSD_MOSI_PORT(DIR)   |=  BV(MICROSD_MOSI_PIN);
    MICROSD_CLK_PORT(DIR)    |=  BV(MICROSD_CLK_PIN);
    
    MICROSD_CS_PORT(SEL)     &= ~BV(MICROSD_CS_PIN);
    MICROSD_CS_PORT(OUT)     |=  BV(MICROSD_CS_PIN);
    MICROSD_CS_PORT(DIR)     |=  BV(MICROSD_CS_PIN);
    
    MICROSD_MOSI_PORT(REN)   |=  BV(MICROSD_MOSI_PIN);
    MICROSD_MISO_PORT(REN)   |=  BV(MICROSD_MISO_PIN);
    
    MICROSD_MOSI_PORT(OUT)   |=  BV(MICROSD_MOSI_PIN);
    MICROSD_MISO_PORT(OUT)   |=  BV(MICROSD_MISO_PIN);
    
    MICROSD_STATUS_PORT(DIR) &= ~BV(MICROSD_STATUS_PIN);
    MICROSD_STATUS_PORT(IES) &= ~BV(MICROSD_STATUS_PIN);
    MICROSD_STATUS_PORT(IE)  |=  BV(MICROSD_STATUS_PIN);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Enable fast SD card SPI transfer. 
          
@return   void 

@param    void
*/
void SYS_MMC_FastMode(void)
{
    UCB1CTL1 |= UCSWRST;    // Put state machine in reset
    UCB1BR0 = 2;            // f_UCxCLK = 12MHz/2 = 6MHz
    UCB1BR1 = 0;    
    UCB1CTL1 &= ~UCSWRST;   // Release USCI state machine
}

/*----------------------------------------------------------------------------*/
/**
@brief    Read a frame of bytes via SPI
          
@return   void 

@param    ptrucBuffer - Place to store the received bytes
@param    uiSize - Indicator of how many bytes to read
*/
void SYS_MMC_ReadFrame(unsigned char* ptrucBuffer, unsigned int uiSize)
{
    unsigned int uiGIE = __get_SR_register() & GIE;         //Store current GIE state

    __disable_interrupt();                                  //Make this operation atomic

    UCB1IFG &= ~UCRXIFG;                                    //Ensure RXIFG is clear

    //Clock the actual data transfer and receive the bytes
    while (uiSize--){
        while (!(UCB1IFG & UCTXIFG)) ;                      //Wait while not ready for TX
        UCB1TXBUF = 0xff;                                   //Write dummy byte
        while (!(UCB1IFG & UCRXIFG)) ;                      //Wait for RX buffer (full)
        *ptrucBuffer++ = UCB1RXBUF;
    }

    __bis_SR_register(uiGIE);                               //Restore original GIE state
}

/*----------------------------------------------------------------------------*/
/**
@brief    Send a frame of bytes via SPI
          
@return   void 

@param    ptrucBuffer - Place that holds the bytes to send
@param    uiSize - Indicator of how many bytes to send
*/
void SYS_MMC_SendFrame(unsigned char* ptrucBuffer, unsigned int uiSize)
{
    unsigned int uiGIE = __get_SR_register() & GIE;         //Store current GIE state

    __disable_interrupt();                                  //Make this operation atomic

    //Clock the actual data transfer and send the bytes. Note that we
    //intentionally not read out the receive buffer during frame transmission
    //in order to optimize transfer speed, however we need to take care of the
    //resulting overrun condition.
    while (uiSize--){
        while (!(UCB1IFG & UCTXIFG)) ;                      //Wait while not ready for TX
        UCB1TXBUF = *ptrucBuffer++;                         //Write byte
    }
    while (UCB1STAT & UCBUSY) ;                             //Wait for all TX/RX to finish

    UCB1RXBUF;                                              //Dummy read to empty RX buffer
                                                            //and clear any overrun conditions

    __bis_SR_register(uiGIE);                               //Restore original GIE state
}

/*----------------------------------------------------------------------------*/
/**
@brief    Set the SD Card's chip-select signal to high
          
@return   void 

@param    void
*/
void SYS_MMC_DeselectCard(void)
{
    MICROSD_CS_PORT(OUT) |=  BV(MICROSD_CS_PIN);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Set the SD Card's chip-select signal to low
          
@return   void 

@param    void
*/
void SYS_MMC_SelectCard(void)
{
    MICROSD_CS_PORT(OUT) &= ~BV(MICROSD_CS_PIN);
}