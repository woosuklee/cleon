/**
@file       app_log.h

@brief      Logging CLEON data
*/

#ifndef __APP_LOG_H__
#define __APP_LOG_H__

#define APP_LOG_SAMPLES_IN_A_FILE       100000  // Defined number of samples will be captured and written to a file; file size will be (one CLEON sample size * defined value)

//------------------------- Defining sensing feature ---------------------------
#define APP_LOG_LED_INDICATION          _ENABLE_
#define APP_LOG_TEMP_AND_HUM_SENSORS    _ENABLE_
#define APP_LOG_LIGHT_SENSORS           _ENABLE_
//------------------------------------------------------------------------------

void  APP_Log(void);

#endif
