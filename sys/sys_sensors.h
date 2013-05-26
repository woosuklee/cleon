/**
@file       sys_sensors.h

@brief      Sensors related functions
*/

#ifndef __SYS_SENSORS_H__
#define __SYS_SENSORS_H__

#define SIZE_OF_SENSOR_DATA_IN_BYTE      8

////////////////////////////////////////////////////////////////////////////////
/////////            SHT11 (Humidity & Temperature sensor)            //////////
////////////////////////////////////////////////////////////////////////////////
//------------------------------- DATA & SCK -----------------------------------
#define SHT11_SDA_SET_TO_OUTPUT_PIN()   (SENSOR_SHT11_PORT(DIR) |=  BV(SENSOR_SHT11_SDA_PIN))	/* SDA Output=0 */
#define SHT11_SDA_SET_TO_INPUT_PIN()   	(SENSOR_SHT11_PORT(DIR) &= ~BV(SENSOR_SHT11_SDA_PIN))	/* SDA Input */
#define SHT11_IS_SDA_HIGH  		(SENSOR_SHT11_PORT(IN)  &   BV(SENSOR_SHT11_SDA_PIN))

#define SHT11_SCL_LOW()    (SENSOR_SHT11_PORT(OUT) &= ~BV(SENSOR_SHT11_SCL_PIN))	/* SCL Output=0 */
#define SHT11_SCL_HIGH()   (SENSOR_SHT11_PORT(OUT) |=  BV(SENSOR_SHT11_SCL_PIN))	/* SCL Output=1 */

// (n * 1ms) delay @ 12MHz
#define SHT11_TIMING_DELAY(n)  do{                                      \
    for(int z = 0 ; z < n ; z++){                                       \
        __delay_cycles(12000);                                          \
    }                                                                   \
}while(0)    

//----------------------------- SHT11 commands ---------------------------------
#define STATUS_REG_W   0x06	
#define STATUS_REG_R   0x07	
#define MEASURE_TEMP   0x03	
#define MEASURE_HUMI   0x05	
#define RESET          0x1E	

typedef struct{
    unsigned int uiTemperature;     // 2 bytes
    unsigned int uiHumidity;        // 2 bytes
    unsigned int uiLightTSR;        // 2 bytes
    unsigned int uiLightPAR;        // 2 bytes
}sensors_s;     // 8 bytes

typedef union{
    sensors_s       stSensors;          // 8 bytes
    unsigned char   ucSingleByte[8];    // 8 bytes
}cleon_sensor_data_u;   // 8 bytes

void         SYS_SENSORS_Init(void);
void         SYS_SENSORS_SHT11_Init(void);
void         SYS_SENSORS_SHT11_On(void);
void         SYS_SENSORS_SHT11_Off(void);
unsigned int SYS_SENSORS_SHT11_ReadTemperature(void);
unsigned int SYS_SENSORS_SHT11_ReadHumidity(void);
unsigned int SYS_SENSORS_SHT11_Command(unsigned char ucCommand);

////////////////////////////////////////////////////////////////////////////////
/////////       Light sensor : For visible range (S1087) (PAR)          ////////
////////////////////////////////////////////////////////////////////////////////
void         SYS_SENSORS_S1087_Init(void);
unsigned int SYS_SENSORS_S1087_Read(void);

////////////////////////////////////////////////////////////////////////////////
/////////   Light sensor : For visible to IR range (S1087-01) (TSR)      ///////
////////////////////////////////////////////////////////////////////////////////
void         SYS_SENSORS_S108701_Init(void);
unsigned int SYS_SENSORS_S108701_Read(void);


#endif
