/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 usbstk5505_gpio.c                                                       */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   Functions for general purpose input output on TMS320C5505 USB Stick.   */
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
 *  GPIO implementation
 *
 */

#include "usbstk5505_gpio.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_init( )                                                           *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 USBSTK5505_GPIO_init()
{
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_setDirection( number, direction )                                 *
 *                                                                          *
 *      number    <- GPIO#                                                  *
 *      direction <- 0:OUT 1:IN                                             *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 USBSTK5505_GPIO_setDirection( Uint16 number, Uint16 direction )
{

    Uint32 bank_id = ( number >> 4);
    Uint32 pin_id  = ( 1 << ( number & 0xF ) );
    
    if (bank_id == 0)
        if ((direction & 1) == GPIO_IN)
    	    SYS_GPIO_DIR0 &= ~pin_id;
        else
            SYS_GPIO_DIR0 |= pin_id;

    if (bank_id == 1)
        if ((direction & 1) == GPIO_IN)
    	    SYS_GPIO_DIR1 &= ~pin_id;
        else
            SYS_GPIO_DIR1 |= pin_id;

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_setOutput( number, output )                                       *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *      value    <- 0:LOW 1:HIGH                                            *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 USBSTK5505_GPIO_setOutput( Uint16 number, Uint16 output )
{
    Uint32 bank_id = ( number >> 4 );
    Uint32 pin_id  = ( 1 << ( number & 0xF ) );

    if (bank_id == 0)
        if ((output & 1) == 0)
    	    SYS_GPIO_DATAOUT0 &= ~pin_id;
        else
            SYS_GPIO_DATAOUT0 |= pin_id;

    if (bank_id == 1)
        if ((output & 1) == 0)
    	    SYS_GPIO_DATAOUT1 &= ~pin_id;
        else
            SYS_GPIO_DATAOUT1 |= pin_id;

    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  _GPIO_getInput( number )                                                *
 *                                                                          *
 *      number   <- GPIO#                                                   *
 *                                                                          *
 *      Returns:    0:LOW                                                   *
 *                  1:HIGH                                                  *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 USBSTK5505_GPIO_getInput( Uint16 number )
{
    Uint32 input;
    Uint32 bank_id = ( number >> 4 );
    Uint32 pin_id  = ( number & 0xF );

    if (bank_id == 0)
        input = (SYS_GPIO_DATAIN0 >> pin_id) & 1;
    if (bank_id == 1)
        input = (SYS_GPIO_DATAIN1 >> pin_id) & 1; 

    return input;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  End of usbstk5505_gpio.c                                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */
 


 
 
