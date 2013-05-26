/** 
@file       hw_architecture.c

@brief      Documentation for CLEON hardware architecture

@page       HW_ARCH     Hardware architecture

@section    GPS_ARCH    GPS H/W architecture    
                        - CLEON embeds MAX2769 from MAXIM as GPS front-end
                        - MAX2769 generates 2bits of I (I1, I0) at a time with speed at 16.368MHz (sampling rate: 16.368MHz)
                        - The 2 bit data is parallelized by using two 8-bit serial-to-parallel IC in order to reduce the speed down to 2.046MHz (16.368MHz/8)
                        - Meanwhile, reference clock from MAX2769 running at 16.368MHz is being slowered down to 2.046MHz (16.368MHz/8) by using binary counter 
                        - MCU reads the parallelized I1 and I0 data with DMA support (DMA trigger: 2.046MHz (16.368MHz/8))
                        
                        @dot
                        digraph GPS_hw_architecture{
                                    graph       [bgcolor = white, resolution=128];
                                    node        [shape = box, fontname = Helvetica, fontsize = 8, style = filled];
                                    edge        [fontname = Helvetica, fontsize = 8, style = filled];    
                        
                                    max2769         [ label="GPS chip (MAX2769)" URL="http://datasheets.maxim-ic.com/en/ds/MAX2769.pdf"];
                                    binary_counter  [ label="Binary-counter" URL="http://www.ti.com/lit/ds/symlink/sn74lv161a.pdf"];
                                    inverter        [ label="Inverter" URL="http://www.ti.com/lit/ds/symlink/sn74lvc1g04.pdf"];
                                    sr_1            [ label="8-bit serial-to-parallel shift-register" URL="http://www.ti.com/lit/ds/symlink/sn74lv595a.pdf"];    
                                    sr_2            [ label="8-bit serial-to-parallel shift-register" URL="http://www.ti.com/lit/ds/symlink/sn74lv595a.pdf"];    
                                    mcu             [ label="MCU (MSP430F5338)" URL="http://www.ti.com/lit/ds/symlink/msp430f5338.pdf"];
                                    
                                    max2769 -> binary_counter [ label="16.368MHz (Ref. CLK)" ];
                                    max2769 -> inverter       [ label="16.368MHz (Ref. CLK)" ];
                                    inverter -> sr_1 [ label="Inverted_16.368MHz (SRCLK)" ];
                                    inverter -> sr_2 [ label="Inverted_16.368MHz (SRCLK)" ];
                                    binary_counter -> sr_1 [ label="2.046MHz (RCLK)" ];
                                    binary_counter -> sr_2 [ label="2.046MHz (RCLK)" ];
                                    
                                    binary_counter  -> mcu [ label="2.046MHz (DMA trigger)" ];
                                        
                                    max2769 -> sr_1 [ label="I1 (speed at 16.368MHz)" ];
                                    max2769 -> sr_2 [ label="I0 (speed at 16.368MHz)" ];

                                    sr_1 -> mcu [ label="I1_0"];    
                                    sr_1 -> mcu [ label="I1_1"];    
                                    sr_1 -> mcu [ label="I1_2"];    
                                    sr_1 -> mcu [ label="I1_3"];    
                                    sr_1 -> mcu [ label="I1_4"];    
                                    sr_1 -> mcu [ label="I1_5"];    
                                    sr_1 -> mcu [ label="I1_6"];    
                                    sr_1 -> mcu [ label="I1_7"];    
                                    
                                    sr_2 -> mcu [ label="I0_0"];    
                                    sr_2 -> mcu [ label="I0_1"];    
                                    sr_2 -> mcu [ label="I0_2"];    
                                    sr_2 -> mcu [ label="I0_3"];    
                                    sr_2 -> mcu [ label="I0_4"];    
                                    sr_2 -> mcu [ label="I0_5"];    
                                    sr_2 -> mcu [ label="I0_6"];    
                                    sr_2 -> mcu [ label="I0_7"];    
                        }
                        @enddot
*/