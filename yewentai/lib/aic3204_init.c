/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*   aic3204_init.c                                                          */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Initialisation of the AIC3204 Codec.                                    */
/*                                                                           */
/*   Sets up the codec for line inputs.                                      */
/*   Uses a sampling frequency of 48000 Hz.                                  */
/*   Note: "aic3204.h" needs to go after "usbstk5505.h"                      */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00                                                          */
/*   Author  : Richard Sikora                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*                                                                           */
/*   Revision 1.00                                                           */
/*   20th December 2009. Created by Richard Sikora from Texas Instruments    */
/*                       code. The crystal on the USB board has been changed */
/*                       from an earlier release and therefore some register */
/*                       values below also needed to be modified.            */
/*                                                                           */
/*****************************************************************************/
/*
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


#include "usbstk5505.h"
#include "aic3204.h" 
#include "stdio.h"        // For printf();          

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC3204 Initialisation.                                                 *
 *      Initialise both the registers and I2S                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 
 /* Setup: Line input. Fs = 48000 Hz */
 
void aic3204_init(void)
{
     
    /* Configure Serial Bus */
    SYS_EXBUSSEL |= 0x0100;  // Configure Serial bus 0 for I2S0
 
     
    /* Configure AIC3204 */

    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 1, 1 );      // Reset codec
    AIC3204_rset( 0, 1 );      // Point to page 1
    AIC3204_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
    AIC3204_rset( 0, 0 );
    /* PLL and Clocks config and Power Up  */
    AIC3204_rset( 27, 0x1d );  // BCLK and WCLK is set as o/p to AIC3204(Master)
    AIC3204_rset( 28, 0x00 );  // Data ofset = 0
    AIC3204_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6, 7 );      // PLL setting: J=7
    AIC3204_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D)
    AIC3204_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D)
    AIC3204_rset( 30, 0x88 );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5, 0x91 );   //PLL setting: Power up PLL, P=1 and R=1
    AIC3204_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x87 );  // Power up NDAC and set NDAC value to 7
    AIC3204_rset( 12, 0x82 );  // Power up MDAC and set MDAC value to 2
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2
    /* DAC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
    AIC3204_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 64, 2 );     // Left vol=right vol
    AIC3204_rset( 65, 0 );     // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x10, 10 );  // Unmute HPL , 10dB gain
    AIC3204_rset( 0x11, 10 );  // Unmute HPR , 10dB gain
    AIC3204_rset( 9, 0x30 );   // Power up HPL,HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    USBSTK5505_wait( 100 );    // wait
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x34, 0x30 );// STEREO 1 Jack
		                       // IN2_L to LADC_P through 40 kohm
    AIC3204_rset( 0x37, 0x30 );// IN2_R to RADC_P through 40 kohmm
    AIC3204_rset( 0x36, 3 );   // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3204_rset( 0x39, 0xc0 );// CM_1 (common mode) to RADC_M through 40 kohm
    AIC3204_rset( 0x3b, 0 );   // MIC_PGA_L unmute
    AIC3204_rset( 0x3c, 0 );   // MIC_PGA_R unmute
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 0x51, 0xc0 );// Powerup Left and Right ADC
    AIC3204_rset( 0x52, 0 );   // Unmute Left and Right ADC
    
    AIC3204_rset( 0, 0 );    
    USBSTK5505_wait( 100 );  // Wait
    
    /* I2S settings */
    I2S0_SRGR = 0x0;     
    I2S0_CR = 0x8010;    // 16-bit word, slave, enable I2C
    I2S0_ICMR = 0x3f;    // Enable interrupts

}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  AIC3204 Initialisation of both sampling frequency and gain in dB.       *
 *      Initialise both the registers and I2S                               *
 *                                                                          *
 * ------------------------------------------------------------------------ */

unsigned long set_sampling_frequency_and_gain(unsigned long SamplingFrequency, unsigned int ADCgain)
{
    unsigned int PLLPR = 0x91; // Default to 48000 Hz 	
    unsigned int gain;
    unsigned long output;

    if ( ADCgain >= 48)
     {
      gain = 95;      //  Limit gain to 47.5 dB
      ADCgain = 48;   // For display using printf()
     }
    else 
    {
     gain = (ADCgain << 1); // Convert 1dB steps to 0.5dB steps
    }
     
    switch (SamplingFrequency)
    {
     case 48000:
     	PLLPR = 0x91; // 1001 0001b. PLL on. P = 1, R = 1. 
     	printf("Sampling frequency 48000 Hz Gain = %2d dB\n", ADCgain);
     	output = 48000;
     break;
     
     case 24000:
      	PLLPR = 0xA1; // 1010 0001b. PLL on. P = 2, R = 1.
      	printf("Sampling frequency 24000 Hz Gain = %2d dB\n", ADCgain);
      	output = 24000;   
     break;
     
     case 16000:
     	PLLPR = 0xB1; // 1011 0001b. PLL on. P = 3, R = 1.
      	printf("Sampling frequency 16000 Hz Gain = %2d dB\n", ADCgain); 
      	output = 16000;      	
     break;
     
     case 12000:
        PLLPR = 0xC1; //1100 0001b. PLL on. P = 4, R = 1.
        printf("Sampling frequency 12000 Hz Gain = %2d dB\n", ADCgain);
        output = 12000;  
     break;
     
     case 9600:
     	PLLPR = 0xD1; //1101 0001b. PLL on. P = 5, R = 1.
       	printf("Sampling frequency 9600 Hz Gain = %2d dB\n", ADCgain); 
       	output = 9600; 
     break;
     
     case 8000:
     	PLLPR = 0xE1; //1110 0001b. PLL on. P = 6, R = 1.
     	printf("Sampling frequency 8000 Hz Gain = %2d dB\n", ADCgain);
     	output = 8000;  
     break;   	
     
     case 6857:
     	PLLPR = 0xF1; //1111 0001b. PLL on. P = 7, R = 1.
     	printf("Sampling frequency 6857 Hz Gain = %2d dB\n", ADCgain);  
     	output = 6857;    
     break;
     
     default:
     	PLLPR = 0x91; // 1001 0001b. PLL on. P = 1, R = 1. 
     	printf("Sampling frequency not recognised. Default to 48000 Hz Gain = %2d dB\n", ADCgain);
     	output = 48000;  
     break;
    } 
 

     
    /* Configure Serial Bus */
    SYS_EXBUSSEL |= 0x0100;  // Configure Serial bus 0 for I2S0
 
     
    /* Configure AIC3204 */

    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 1, 1 );      // Reset codec
    AIC3204_rset( 0, 1 );      // Point to page 1
    AIC3204_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
    AIC3204_rset( 0, 0 );
    /* PLL and Clocks config and Power Up  */
    AIC3204_rset( 27, 0x1d );  // BCLK and WCLK is set as o/p to AIC3204(Master)
    AIC3204_rset( 28, 0x00 );  // Data ofset = 0
    AIC3204_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6, 7 );      // PLL setting: J=7
    AIC3204_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D)
    AIC3204_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D)
    AIC3204_rset( 30, 0x88 );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5, PLLPR );   //PLL setting: Power up PLL, P=1 and R=1
    AIC3204_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x87 );  // Power up NDAC and set NDAC value to 7
    AIC3204_rset( 12, 0x82 );  // Power up MDAC and set MDAC value to 2
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2
    /* DAC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
    AIC3204_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 64, 2 );     // Left vol=right vol
    AIC3204_rset( 65, 0 );     // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x10, 10 );  // Unmute HPL , 10dB gain
    AIC3204_rset( 0x11, 10 );  // Unmute HPR , 10dB gain
    AIC3204_rset( 9, 0x30 );   // Power up HPL,HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    USBSTK5505_wait( 100 );    // wait
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x34, 0x10 );// STEREO 1 Jack
		                       // IN2_L to LADC_P through 0 kohm
    AIC3204_rset( 0x37, 0x10 );// IN2_R to RADC_P through 0 kohmm
    AIC3204_rset( 0x36, 1 );   // CM_1 (common mode) to LADC_M through 0 kohm
    AIC3204_rset( 0x39, 0x40 );// CM_1 (common mode) to RADC_M through 0 kohm
    AIC3204_rset( 0x3b, gain );   // MIC_PGA_L unmute
    AIC3204_rset( 0x3c, gain );   // MIC_PGA_R unmute
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 0x51, 0xc0 );// Powerup Left and Right ADC
    AIC3204_rset( 0x52, 0 );   // Unmute Left and Right ADC
    
    AIC3204_rset( 0, 0 );    
    USBSTK5505_wait( 100 );  // Wait
    
    /* I2S settings */
    I2S0_SRGR = 0x0;     
    I2S0_CR = 0x8010;    // 16-bit word, slave, enable I2C
    I2S0_ICMR = 0x3f;    // Enable interrupts

 	return(output);
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204_init.c                                                   *
 *                                                                          *
 * ------------------------------------------------------------------------ */





