/** 
@file       power_consumption.c

@brief      Documentation for CLEON power consumption

@page       PWR         Power consumption

@section    PWR         Power consumption
                        - The software is fully interrupt-driven and system remains in low-power-mode almost all the time, except for sensing and USB communication
                            + STAGE 1: Low-power-mode (&asymp; 2.5mA)
                                - Almost all the time, CLEON remains in low-power-mode (LPM)
                            + STAGE 2: MSP430 in AM + GPS on (&asymp; 33mA)
                                - MSP430 is in active mode
                                - Temperature, Humidity, and light sensors are temporarily turned on, then off
                                - GPS is turned-on, stabilized, and in operation
                            + STAGE 3: MSP430 in AM + GPS on + MicroSD writing (&asymp; 73mA)
                                - MSP430 is in active mode
                                - GPS remains on
                                - Captured sensor data and GPS signal is written to MicroSD
                        - The figure below shows power consumption for 3 chunks of data with 50ms chunk gap
                        
                            @image html power_consumption.jpg 
*/
