/**
@file       cleon_conf.h

@brief      Configuration file for CLEON
*/

#ifndef __CLEON_CONF_H__
#define __CLEON_CONF_H__

// All the debug functions will be disabled if 'DEBUG_MODE' is set to '_DISABLE', or vice versa with '_ENABLE_'
#define DEBUG_MODE                          _DISABLE_   

#define DUMP_SYSTEM_INFO_VIA_USB            _ENABLE_
#define DUMP_SENSOR_VALUE_VIA_USB           _ENABLE_
#define DUMP_TIME_INFO_VIA_USB              _ENABLE_

////////////////////////////////////////////////////////////////////////////////
////////                  EXTERNAL FLASH configuration                //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define EXT_FLASH_MOSI_PORT(type)       P8##type    /* P8.5 - Output: External FLASH SPI MOSI */
#define EXT_FLASH_MOSI_PIN              5

#define EXT_FLASH_MISO_PORT(type)       P8##type    /* P8.6 - Input: External FLASH SPI MISO */
#define EXT_FLASH_MISO_PIN              6

#define EXT_FLASH_CLK_PORT(type)        P8##type    /* P8.4 - Output: External FLASH SPI CLK */
#define EXT_FLASH_CLK_PIN               4

#define EXT_FLASH_CS_PORT(type)         P1##type    /* P1.7 - Output: External FLASH chip select */
#define EXT_FLASH_CS_PIN                7

#define EXT_FLASH_WP_PORT(type)         P1##type    /* P1.4 - Output: External FLASH write protect */
#define EXT_FLASH_WP_PIN                4

#define EXT_FLASH_HOLD_PORT(type)       P8##type    /* P8.7 - Output: External FLASH hold */
#define EXT_FLASH_HOLD_PIN              7

////////////////////////////////////////////////////////////////////////////////
////////                     MICRO SD configuration                   //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define MICROSD_MOSI_PORT(type)         P8##type    /* P8.5 - Output: MicroSD SPI MOSI */
#define MICROSD_MOSI_PIN                5

#define MICROSD_MISO_PORT(type)         P8##type    /* P8.6 - Input: MicroSD SPI MISO */
#define MICROSD_MISO_PIN                6

#define MICROSD_CLK_PORT(type)          P8##type    /* P8.4 - Output: MicroSD SPI CLK */
#define MICROSD_CLK_PIN                 4

#define MICROSD_CS_PORT(type)           P1##type    /* P1.6 - Output: MicroSD chip select */
#define MICROSD_CS_PIN                  6

#define MICROSD_STATUS_PORT(type)       P1##type    /* P1.2 - Input: MicroSD status */
#define MICROSD_STATUS_PIN              2

////////////////////////////////////////////////////////////////////////////////
/////////                   Unified clock system                      //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define XT2_PORT(type)                  P7##type
#define XT2OUT_PIN                      3       /* P7.3 - Output: XT2 clock output */
#define XT2IN_PIN                       2       /* P7.2 - Input: XT2 clock input */

////////////////////////////////////////////////////////////////////////////////
/////////                        Power control                        //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define BOOST_SHDN_N_PORT(type)         P7##type
#define BOOST_SHDN_N_PIN                4       /* P7.4 - Output: Boost-up regulator shut down */
#define GPS_PWR_SHDN_N_PORT(type)       P9##type
#define GPS_PWR_SHDN_N_PIN              0       /* P9.0 - Output: GPS power shut down */

////////////////////////////////////////////////////////////////////////////////
/////////                        User switch                          //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define USER_SW_PORT(type)              P1##type
#define USER_SW_PIN                     0       /* P1.0 - Input: User switch */

////////////////////////////////////////////////////////////////////////////////
/////////                 Hum & Temp sensor (SHT11)                   //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define SENSOR_SHT11_PORT(type)         P2##type
#define SENSOR_SHT11_SDA_PIN            5       /* P2.5 - Input: SHT11 sensor value */
#define SENSOR_SHT11_SCL_PIN            6       /* P2.6 - Output: SHT11 clock */
#define SENSOR_SHT11_PWR_PIN            7       /* P2.7 - Output: SHT11 Power */

////////////////////////////////////////////////////////////////////////////////
/////////          Light sensor : For visible range (S1087)           //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define SENSOR_S1087_PORT(type)         P7##type
#define SENSOR_S1087_PIN                7       /* P7.7 - Input (ADC): S1087 sensor value */

////////////////////////////////////////////////////////////////////////////////
/////////      Light sensor : For visible to IR range (S1087-01)      //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define SENSOR_S108701_PORT(type)       P7##type
#define SENSOR_S108701_PIN              6       /* P7.6 - Input (ADC): S1087-01 sensor value */

////////////////////////////////////////////////////////////////////////////////
/////////                            LEDs                             //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define LED1_PORT(type)                 P9##type
#define LED1_PIN                        7       /* P9.7 - Output: LED1 */
#define LED2_PORT(type)                 P9##type
#define LED2_PIN                        6       /* P9.6 - Output: LED2 */
#define LED3_PORT(type)                 P9##type
#define LED3_PIN                        5       /* P9.5 - Output: LED3 */
#define LED4_PORT(type)                 P8##type
#define LED4_PIN                        1       /* P8.1 - Output: LED4 */
#define LED5_PORT(type)                 P8##type
#define LED5_PIN                        0       /* P8.0 - Output: LED5 */
#define LED6_PORT(type)                 P1##type
#define LED6_PIN                        5       /* P1.5 - Output: LED6 */

#define MICRO_SD_ERROR_LED_ON()         LED1_ON()
#define MICRO_SD_ERROR_LED_OFF()        LED1_OFF()
#define MICRO_SD_ERROR_LED_TOGGLE()     LED1_TOGGLE()

#define SAMPLING_IN_ACTION_LED_ON()     LED2_ON()
#define SAMPLING_IN_ACTION_LED_OFF()    LED2_OFF()
#define SAMPLING_IN_ACTION_LED_TOGGLE() LED2_TOGGLE()

#define TIME_SYNC_ERROR_LED_ON()        LED3_ON()
#define TIME_SYNC_ERROR_LED_OFF()       LED3_OFF()
#define TIME_SYNC_ERROR_LED_TOGGLE()    LED3_TOGGLE()

#define GPS_ERROR_LED_ON()              LED6_ON()
#define GPS_ERROR_LED_OFF()             LED6_OFF()
#define GPS_ERROR_LED_TOGGLE()          LED6_TOGGLE()

////////////////////////////////////////////////////////////////////////////////
////////   USB configuration (Default Buad: 57600bps @ 12MHz SMCLK)   //////////
////////////////////////////////////////////////////////////////////////////////
//----------------------------- Port definition --------------------------------
#define USB_PORT(type)                  P8##type
#define USB_TXD_PIN                     2       /* P8.2 - Output: UCA1 TXD */
#define USB_RXD_PIN                     3       /* P8.3 - Input: UCA1 RXD */

#define USB_STATUS_PORT(type)           P1##type
#define USB_STATUS_PIN                  3       /* P1.3 - Input: USB status */

////////////////////////////////////////////////////////////////////////////////
////////                  GPS (MAX2769) configuration                 //////////
////////////////////////////////////////////////////////////////////////////////
//================================= GPS Data ===================================
//----------------------------- Port definition --------------------------------
/* DMA external trigger */
#define MAX2769_EXT_DMA_TRIG_PORT(type) P2##type
#define MAX2769_EXT_DMA_TRIG_PIN        1       /* P2.1 - Input: External DMA trigger for MAX2769 */

/* P3IN : 0x0220h */
#define MAX2769_Q0_PORT(type)           P3##type
#define MAX2769_Q0_1_PIN                0       /* P3.0 - Input: Q0 (1st Q0 bit) for MAX2769 */
#define MAX2769_Q0_2_PIN                1       /* P3.1 - Input: Q0 (2nd Q0 bit) for MAX2769 */
#define MAX2769_Q0_3_PIN                2       /* P3.2 - Input: Q0 (3rd Q0 bit) for MAX2769 */
#define MAX2769_Q0_4_PIN                3       /* P3.3 - Input: Q0 (4th Q0 bit) for MAX2769 */
#define MAX2769_Q0_5_PIN                4       /* P3.4 - Input: Q0 (5th Q0 bit) for MAX2769 */
#define MAX2769_Q0_6_PIN                5       /* P3.5 - Input: Q0 (6th Q0 bit) for MAX2769 */
#define MAX2769_Q0_7_PIN                6       /* P3.6 - Input: Q0 (7th Q0 bit) for MAX2769 */
#define MAX2769_Q0_8_PIN                7       /* P3.7 - Input: Q0 (8th Q0 bit) for MAX2769 */
                                          
/* P4IN : 0x0221h */                      
#define MAX2769_Q1_PORT(type)           P4##type
#define MAX2769_Q1_1_PIN                0       /* P4.0 - Input: Q1 (1st Q1 bit) for MAX2769 */
#define MAX2769_Q1_2_PIN                1       /* P4.1 - Input: Q1 (2nd Q1 bit) for MAX2769 */
#define MAX2769_Q1_3_PIN                2       /* P4.2 - Input: Q1 (3rd Q1 bit) for MAX2769 */
#define MAX2769_Q1_4_PIN                3       /* P4.3 - Input: Q1 (4th Q1 bit) for MAX2769 */
#define MAX2769_Q1_5_PIN                4       /* P4.4 - Input: Q1 (5th Q1 bit) for MAX2769 */
#define MAX2769_Q1_6_PIN                5       /* P4.5 - Input: Q1 (6th Q1 bit) for MAX2769 */
#define MAX2769_Q1_7_PIN                6       /* P4.6 - Input: Q1 (7th Q1 bit) for MAX2769 */
#define MAX2769_Q1_8_PIN                7       /* P4.7 - Input: Q1 (8th Q1 bit) for MAX2769 */
                                        
/* P5IN : 0x0240h */                    
#define MAX2769_I0_PORT(type)           P5##type
#define MAX2769_I0_1_PIN                0       /* P5.0 - Input: I0 (1st I0 bit) for MAX2769 */
#define MAX2769_I0_2_PIN                1       /* P5.1 - Input: I0 (2nd I0 bit) for MAX2769 */
#define MAX2769_I0_3_PIN                2       /* P5.2 - Input: I0 (3rd I0 bit) for MAX2769 */
#define MAX2769_I0_4_PIN                3       /* P5.3 - Input: I0 (4th I0 bit) for MAX2769 */
#define MAX2769_I0_5_PIN                4       /* P5.4 - Input: I0 (5th I0 bit) for MAX2769 */
#define MAX2769_I0_6_PIN                5       /* P5.5 - Input: I0 (6th I0 bit) for MAX2769 */
#define MAX2769_I0_7_PIN                6       /* P5.6 - Input: I0 (7th I0 bit) for MAX2769 */
#define MAX2769_I0_8_PIN                7       /* P5.7 - Input: I0 (8th I0 bit) for MAX2769 */
                                          
/* P6IN : 0x0241h */                      
#define MAX2769_I1_PORT(type)           P6##type
#define MAX2769_I1_1_PIN                0       /* P6.0 - Input: I1 (1st I1 bit) for MAX2769 */
#define MAX2769_I1_2_PIN                1       /* P6.1 - Input: I1 (2nd I1 bit) for MAX2769 */
#define MAX2769_I1_3_PIN                2       /* P6.2 - Input: I1 (3rd I1 bit) for MAX2769 */
#define MAX2769_I1_4_PIN                3       /* P6.3 - Input: I1 (4th I1 bit) for MAX2769 */
#define MAX2769_I1_5_PIN                4       /* P6.4 - Input: I1 (5th I1 bit) for MAX2769 */
#define MAX2769_I1_6_PIN                5       /* P6.5 - Input: I1 (6th I1 bit) for MAX2769 */
#define MAX2769_I1_7_PIN                6       /* P6.6 - Input: I1 (7th I1 bit) for MAX2769 */
#define MAX2769_I1_8_PIN                7       /* P6.7 - Input: I1 (8th I1 bit) for MAX2769 */

//=============================== GPS Control ==================================
//----------------------------- Port definition --------------------------------
#define MAX2769_SHDN_N_PORT(type)       P9##type
#define MAX2769_SHDN_N_PIN              1       /* P9.1 - Output: /SHDN to MAX2769 */
                                          
#define MAX2769_IDLE_N_PORT(type)       P9##type
#define MAX2769_IDLE_N_PIN              4       /* P9.4 - Output: /IDLE to MAX2769 */
                                          
#define MAX2769_LD_PORT(type)           P2##type
#define MAX2769_LD_PIN                  4       /* P2.4 - Input: LD from MAX2769 */
//=============================== GPS program ==================================
//----------------------------- Port definition --------------------------------
#define MAX2769_PGM_EN_N_PORT(type)     P9##type
#define MAX2769_PGM_EN_N_PIN            3       /* P9.3 - Output: PGM_EN to MAX2769 */
                                        
#define MAX2769_PGM_CS_N_PORT(type)     P9##type
#define MAX2769_PGM_CS_N_PIN            2       /* P9.2 - Output: /CS to MAX2769 */
                                        
#define MAX2769_PGM_SCLK_PORT(type)     P2##type
#define MAX2769_PGM_SCLK_PIN            3       /* P2.3 (UCB0CLK) - SPI: SCLK to MAX2769 */

#define MAX2769_PGM_SDATA_PORT(type)    P2##type
#define MAX2769_PGM_SDATA_PIN           2       /* P2.2 (UCB0SIMO)- SPI: SDATA to MAX2769 */

#endif