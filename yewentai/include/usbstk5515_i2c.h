/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 usbstk5515_i2c.h                                                        */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Functions for general purpose input output on TMS320C5515 USB Stick.    */
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

/*
 *  Copyright 2009 by Spectrum Digital Incorporated.
 *  All rights reserved. Property of Spectrum Digital Incorporated.
 */

/*
 *  I2C header file
 *
 */

#ifndef I2C_
#define I2C_

#include "usbstk5515.h"

#define MDR_STT			0x2000
#define MDR_TRX			0x0200
#define MDR_MST			0x0400
#define MDR_IRS			0x0020
#define MDR_FREE		0x4000
#define STR_XRDY		0x0010
#define STR_RRDY		0x0008
#define MDR_STP 		0x0800
/* ------------------------------------------------------------------------ *
 *  Prototypes                                                              *
 * ------------------------------------------------------------------------ */
Int16 USBSTK5515_I2C_init ( );
Int16 USBSTK5515_I2C_close( );
Int16 USBSTK5515_I2C_read( Uint16 i2c_addr, Uint8* data, Uint16 len );
Int16 USBSTK5515_I2C_write( Uint16 i2c_addr, Uint8* data, Uint16 len );

#endif
