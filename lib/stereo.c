/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 stereo.c                                                                */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5505 USB Stick.                                                  */
/*   Conversion from stereo to mono.                                         */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Richard Sikora                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision: 1.00                                                          */
/*   5th March 2010. Created by Richard Sikora from TMS320C5510 DSK code.    */
/*                                                                           */
/*****************************************************************************/
/*
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


/*****************************************************************************/
/* stereo_to_mono()                                                          */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* INPUTS:  left and right channel (stereo) information.                     */
/*                                                                           */
/* RETURNS: Average of two input channels (mono)                             */
/*                                                                           */
/*****************************************************************************/

signed int stereo_to_mono(signed int left_channel, signed int right_channel)
{
 signed long temp;
 
 /* Take average of left and right channels. */
 
 temp = (signed long) left_channel + (signed long) right_channel;
 
 temp >>= 1;    /* Divide by 2 to prevent overload at output */
 
 return ((signed int) temp); /* Return mono value to calling function */
}

/*****************************************************************************/
/* End of stereo.c                                                           */
/*****************************************************************************/
