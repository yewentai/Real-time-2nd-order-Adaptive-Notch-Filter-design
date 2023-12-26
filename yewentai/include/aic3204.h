/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 aic3204.h                                                               */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Header file for aic3204 codec on the TMS320C5505 USB Stick.             */
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


#ifndef AIC3204_H_
#define AIC3204_H_

 #define AIC3204_I2C_ADDR 0x18

 #define XmitL 0x10
 #define XmitR 0x20
 #define RcvR 0x08
 #define RcvL 0x04

 extern void aic3204_init(void);
 extern void aic3204_hardware_init(void);
 extern void aic3204_codec_read(Int16* left_input, Int16* right_input);
 extern void aic3204_codec_write(Int16 left_input, Int16 right_input);
 extern void aic3204_disable(void);

 extern Int16 AIC3204_rset( Uint16 regnum, Uint16 regval);


 extern unsigned long set_sampling_frequency_and_gain(unsigned long SamplingFrequency, unsigned int ADCgain);

#endif /*AIC3204_H_*/


/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of aic3204.h                                                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 
