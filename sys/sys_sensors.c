/**
@file       sys_sensors.c

@brief      Sensors related functions
*/

#include "cleon_conf.h"
#include "app_define.h"
#include "sys_define.h"
#include "hal_define.h"
#include "fs_define.h"

////////////////////////////////////////////////////////////////////////////////
/////////       Light sensor : For visible range (S1087) (PAR)        //////////
////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------*/
/**
@brief    Initializing light sensor S1087 (PAR)

@return   void 

@param    void
*/
void SYS_SENSORS_S1087_Init(void)
{ 
    SENSOR_S1087_PORT(SEL) |=  BV(SENSOR_S1087_PIN);
    SENSOR_S1087_PORT(DIR) |=  BV(SENSOR_S1087_PIN);
    SENSOR_S1087_PORT(OUT) &= ~BV(SENSOR_S1087_PIN);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Read sensor value of S1087 (PAR)

@return   void 

@param    void
*/
unsigned int SYS_SENSORS_S1087_Read(void)
{ 
    double dLight = 0;
      
    // Sensor value (in 12-bit ADC-representation)
    dLight = (double) ADC12MEM0;

    // Voltage difference (in 12-bit ADC-representation)
    dLight = 4096 - dLight;
    
    // For S1087, it gives 1nA upon sensing 1 lux
    // It means that 0.5mV appears across 500k resistor
    // Therefore, light intensity (lux) = Vsense (V) * 1 (lux) / 0.5m (V)
    // That is, Vsense * 2000;
    dLight = dLight / 4096 * 2.88 * 2000;
    
#if (DEBUG_MODE) && (DUMP_SENSOR_VALUE_VIA_USB)
    SYS_USB_Printf("Light1(PAR): %.4d    \r\n", (unsigned int) dLight);
#endif
    
    return (unsigned int) dLight;
}

////////////////////////////////////////////////////////////////////////////////
/////////   Light sensor : For visible to IR range (S1087-01) (TSR)   //////////
////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------*/
/**
@brief    Initializing light sensor S1087-01 (TSR)

@return   void 

@param    void
*/
void SYS_SENSORS_S108701_Init(void)
{ 
    SENSOR_S108701_PORT(SEL) |=  BV(SENSOR_S108701_PIN);
    SENSOR_S108701_PORT(DIR) |=  BV(SENSOR_S108701_PIN);
    SENSOR_S108701_PORT(OUT) &= ~BV(SENSOR_S108701_PIN);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Read sensor value of S1087-01 (TSR)

@return   void 

@param    void
*/
unsigned int SYS_SENSORS_S108701_Read(void)
{ 
    double dLight = 0;
      
    // Sensor value (in 12-bit ADC-representation)
    dLight = (double) ADC12MEM1;

    // Voltage difference (in 12-bit ADC-representation) 3890
    dLight = 4096 - dLight;
    
    // For S1087-01, it gives 10nA upon sensing 1 lux
    // It means that 5mV appears across 500k resistor
    // Therefore, light intensity (lux) = Vsense (V) * 1 (lux) / 5m (V)
    // That is, Vsense * 200;
    dLight = dLight / 4096 * 2.88 * 200;
    
    // Manually found calibration factor
    dLight = dLight * 8;
    
#if (DEBUG_MODE) && (DUMP_SENSOR_VALUE_VIA_USB)
    SYS_USB_Printf("Light2(TSR): %.4d    \r\n", (unsigned int)dLight);
#endif
    
    return (unsigned int) dLight;
}


////////////////////////////////////////////////////////////////////////////////
/////////            SHT11 (Humidity & Temperature sensor)            //////////
////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------*/
/**
@brief    Initializing sensors

@return   void 

@param    void
*/
void SYS_SENSORS_Init(void)
{ 
    SYS_SENSORS_SHT11_Init();
    SYS_SENSORS_S1087_Init();
    SYS_SENSORS_S108701_Init();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Initializing SHT11 (Humidity & Temperature sensor)
          - SHT11 requires 11ms after power on

@return   void 

@param    void
*/
void SYS_SENSORS_SHT11_Init(void)
{ 
    SYS_SENSORS_SHT11_Off();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Read SHT11 temperature

@return   void 

@param    void
*/
unsigned int SYS_SENSORS_SHT11_ReadTemperature(void)
{
    unsigned int uiTemp = 0;
    
    // Read temperature in Fahrenheit
    uiTemp = (unsigned int) (-40.00 + 0.018 * SYS_SENSORS_SHT11_Command(MEASURE_TEMP));
    
#if (DEBUG_MODE) && (DUMP_SENSOR_VALUE_VIA_USB)
    SYS_USB_Printf("Temp(F)    : %.4d    \r\n", (unsigned int) uiTemp);
#endif    
    
    return uiTemp;
}

/*----------------------------------------------------------------------------*/
/**
@brief    Read SHT11 humidity

@return   void 

@param    void
*/
unsigned int SYS_SENSORS_SHT11_ReadHumidity(void)
{
    unsigned int uiRelativeHumidity = 0;
    unsigned int uiHumidity = 0;
            
    uiRelativeHumidity = SYS_SENSORS_SHT11_Command(MEASURE_HUMI);
    uiHumidity = (unsigned int)( -4 + 0.0405 * uiRelativeHumidity - 2.8e-6 * (uiRelativeHumidity * uiRelativeHumidity));
    
#if (DEBUG_MODE) && (DUMP_SENSOR_VALUE_VIA_USB)
    SYS_USB_Printf("Humidity   : %.4d    \r\n", (unsigned int) uiHumidity);
#endif        
        
    return uiHumidity;
}

/*----------------------------------------------------------------------------*/
/**
@brief    Turn SHT11 on

@return   void 

@param    void
*/
void SYS_SENSORS_SHT11_On(void)
{ 
    SENSOR_SHT11_PORT(OUT) |=   BV(SENSOR_SHT11_PWR_PIN);                           
    SENSOR_SHT11_PORT(OUT) &= ~(BV(SENSOR_SHT11_SDA_PIN) | BV(SENSOR_SHT11_SCL_PIN)); // Set pull-up register for DATA and SCK pins
    SENSOR_SHT11_PORT(DIR) |=   BV(SENSOR_SHT11_PWR_PIN) | BV(SENSOR_SHT11_SCL_PIN);  // SHT11 power on & Set SCK pin as output 
    
    SHT11_TIMING_DELAY(20);     // wait about 20ms for SHT11 stabilization
}

/*----------------------------------------------------------------------------*/
/**
@brief    Turn SHT11 off

@return   void 

@param    void
*/
void SYS_SENSORS_SHT11_Off(void)
{
    SENSOR_SHT11_PORT(OUT) &=  ~BV(SENSOR_SHT11_PWR_PIN);
    SENSOR_SHT11_PORT(OUT) &= ~(BV(SENSOR_SHT11_SDA_PIN) | BV(SENSOR_SHT11_SCL_PIN));
    SENSOR_SHT11_PORT(DIR) |=   BV(SENSOR_SHT11_PWR_PIN) | BV(SENSOR_SHT11_SCL_PIN);
}

/*----------------------------------------------------------------------------*/
/**
@brief    Start SHT11

@return   void 

@param    void
*/
void SYS_SENSORS_SHT11_Start(void)
{
    SHT11_SDA_SET_TO_INPUT_PIN(); 
    SHT11_SCL_LOW();
    SHT11_TIMING_DELAY(1);
    SHT11_SCL_HIGH();
    SHT11_TIMING_DELAY(1);
    SHT11_SDA_SET_TO_OUTPUT_PIN();
    SHT11_TIMING_DELAY(1);
    SHT11_SCL_LOW();
    SHT11_TIMING_DELAY(1);
    SHT11_SCL_HIGH();
    SHT11_TIMING_DELAY(1);
    SHT11_SDA_SET_TO_INPUT_PIN();
    SHT11_TIMING_DELAY(1);
    SHT11_SCL_LOW();
}

/*----------------------------------------------------------------------------*/
/**
@brief    Reset SHT11

@return   void 

@param    void
*/
void SYS_SENSORS_SHT11_Reset(void)
{
    SHT11_SDA_SET_TO_INPUT_PIN();
    SHT11_SCL_LOW();
    for(int i = 0; i < 9 ; i++) {
        SHT11_SCL_HIGH();
        SHT11_TIMING_DELAY(1);
        SHT11_SCL_LOW();
        SHT11_TIMING_DELAY(1);
    }
    SYS_SENSORS_SHT11_Start();	
}

/*----------------------------------------------------------------------------*/
/**
@brief    Write to SHT11

@return   bAck - return true if received OK otherwise false 

@param    ucCommand - command to be transmitted
*/
bool SYS_SENSORS_SHT11_Write(unsigned char ucCommand)
{
    bool bAck;

    for(int i = 0; i < 8; i++, ucCommand <<= 1) {
        if(ucCommand & 0x80) {
            SHT11_SDA_SET_TO_INPUT_PIN();
        }else{
            SHT11_SDA_SET_TO_OUTPUT_PIN();
        }
        SHT11_SCL_HIGH();
        SHT11_TIMING_DELAY(1);
        SHT11_SCL_LOW();
        SHT11_TIMING_DELAY(1);
    }

    SHT11_SDA_SET_TO_INPUT_PIN();
    SHT11_SCL_HIGH();
    SHT11_TIMING_DELAY(1);
    
    bAck = !SHT11_IS_SDA_HIGH;

    SHT11_SCL_LOW();

    return bAck;
}

/*----------------------------------------------------------------------------*/
/**
@brief    Read from SHT11

@return   ucReceivedData - data received from SHT11

@param    ucCommand - command to be transmitted
*/
unsigned char SYS_SENSORS_SHT11_Read(bool bAckRequested)
{
    unsigned char ucReceivedData = 0x00;

    SHT11_SDA_SET_TO_INPUT_PIN();
    
    for(int i = 0; i < 8; i++) {
        ucReceivedData <<= 1;
        SHT11_SCL_HIGH();
        SHT11_TIMING_DELAY(1);
        if(SHT11_IS_SDA_HIGH) {
            ucReceivedData |= 0x1;
        }
        SHT11_SCL_LOW();
        SHT11_TIMING_DELAY(1);
    }

    if(bAckRequested) {
        SHT11_SDA_SET_TO_OUTPUT_PIN();
    }
    
    SHT11_SCL_HIGH();
    SHT11_TIMING_DELAY(1);
    SHT11_SCL_LOW();

    SHT11_SDA_SET_TO_INPUT_PIN();		

    return ucReceivedData;
}

/*----------------------------------------------------------------------------*/
/**
@brief    Transmit command to SHT11

@return   uiReceivedData - data received from SHT11 or _FAIL_ if command is unsuccessful

@param    ucCommand - command to be transmitted
*/
unsigned int SYS_SENSORS_SHT11_Command(unsigned char ucCommand)
{
    unsigned long n;

    SYS_SENSORS_SHT11_Start();			
    if(!SYS_SENSORS_SHT11_Write(ucCommand)) {
        SYS_SENSORS_SHT11_Reset();
        return _FAIL_;
    }else{
        for(n = 0; n < 250000; n++) {
            if(!SHT11_IS_SDA_HIGH) {
#pragma diag_suppress = Pe550
                unsigned char t0, t1, rcrc;
                t0 = SYS_SENSORS_SHT11_Read(1);
                t1 = SYS_SENSORS_SHT11_Read(1);
                rcrc = SYS_SENSORS_SHT11_Read(0);

                return (t0 << 8) | t1;
            }
        }
    }
#pragma diag_suppress = Pe940
}

