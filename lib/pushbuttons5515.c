/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 pushbuttons5515.c                                                       */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5515 ezDSP.                                                      */
/*   Read two pushbuttons SW1 and SW2 on the board.                          */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Richard Sikora                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision: 1.00                                                          */
/*   15th September 2010. Created by Richard Sikora from                     */
/*   Spectrum Digital Code.                                                  */
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

#include "usbstk5515.h"
#include "sar.h"

#define SAMPLING_FREQUENCY 48000
#define TIMEOUT_10ms      (SAMPLING_FREQUENCY/4000)


static unsigned int counter = 0;
static unsigned int buffer[] = {NoKey, NoKey, NoKey}; 
static unsigned int last_value = NoKey;
static unsigned int step = 1;
static enum {IDLE, UP, DOWN } state = IDLE;

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *   SAR_init(void)                                                         *
 *       Initialize SAR ADC                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */

void SAR_init(void)
{
    *SARCTRL    = 0x3C00;      // Select AIN3, which is GPAIN1
    *SARCLKCTRL = 0x0031;      // 100/50 = 2MHz 
    *SARPINCTRL = 0x7104;
    *SARGPOCTRL = 0; 
    return;
}


/*****************************************************************************/
/* pushbuttons_read()                                                        */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* INPUTS:  limit = maximum number of steps.                                 */
/*                                                                           */
/* RETURNS: Step between 1 and limit.                                        */
/*                                                                           */
/* ACTIONS:                                                                  */
/*          No buttons pressed = no action.                                  */
/*          SW1 pressed = decrement step.                                    */
/*          SW2 pressed = increment step.                                    */
/*          SW1 + SW2 pressed = set step to 1 (reset back to beginning).     */
/*                                                                           */
/*****************************************************************************/


unsigned int pushbuttons_read(unsigned int limit)
{
 unsigned int value;

 if ( counter == 0)
 {
    *SARCTRL = 0xB400;  /* Start SAR for channel 3. One conversion only */
	counter++;
 }
 else if ( counter >= TIMEOUT_10ms)
 {
   value = *SARDATA; /* Read ADC */  
   
   if ( value & 0x8000)
   {
    /* Still doing ADC convesion. Wait.  */
    /* Do not update counter             */
   }
   else
   {
    buffer[2] = buffer[1];
	buffer[1] = buffer[0]; /* Shuffle values along one place */

	value &= 0x3FF; /* 10-bit ADC value */

     /* Account for tolerance in measurement */
    if((value < SW1 + 12) && (value > SW1 - 12))
      {
        value = SW1;
      }  
    else if((value < SW2 + 12) && (value > SW2 - 12))
      {
        value = SW2;
      }  
    else if((value < SW12 + 12) && (value > SW12 - 12))
      {
        value = SW12;
      }  
    else if((value < NoKey + 12) && (value > NoKey - 12))
      {
        value = NoKey;
      }  
 	
	buffer[0] = value;
   
    if ( (buffer[0] == buffer[1]) && (buffer[1] == buffer[2]) )
	{
      /* Last three values were identical */
 
         last_value = value; /* Update with debounced new input */
	}
	else
    {
	  /* Switches have changed but not debounced. Default to last_value */
	}

	counter = 0; /* Start new conversion next time through. */
   }
 }
 else
 {
  /* Debounce time of 10ms has not yet elapsed. */
  counter++;
 }

switch (state)
{
 case IDLE:
  if ( last_value == SW2)
   {
   	step++;
   	
   	if ( step > limit)
   	 {
   	  step = 1;
   	 }
   	state = UP; 
   }
  else if ( last_value == SW1)
  {
   step--;
   
   if ( step == 0)
    {
     step = limit;	
    }
   state = DOWN;
  } 
  else if ( last_value == SW12)
  {
   	step = 1;
  }
  break;
 
 
 case UP:
 case DOWN:
   if ( last_value == NoKey)
    {
     state = IDLE;
    } 
 break;
}

 return (step);
}

/*****************************************************************************/
/* pushbuttons_read_raw()                                                        */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* INPUTS:  None                                                             */
/*                                                                           */
/* RETURNS: Value between 0 and 3.                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/


unsigned int pushbuttons_read_raw(void)
{
 unsigned int value;
 static unsigned int counter = 0;
 static unsigned int buffer[] = {NoKey, NoKey, NoKey}; 
 static unsigned int last_value = NoKey;

 if ( counter == 0)
 {
    *SARCTRL = 0xB400;  /* Start SAR for channel 3. One conversion only */
	counter++;
 }
 else if ( counter >= TIMEOUT_10ms)
 {
   value = *SARDATA; /* Read ADC */  
   
   if ( value & 0x8000)
   {
    /* Still doing ADC convesion. Wait.  */
    /* Do not update counter             */
   }
   else
   {
    buffer[2] = buffer[1];
	buffer[1] = buffer[0]; /* Shuffle values along one place */

	value &= 0x3FF; /* 10-bit ADC value */

     /* Account for tolerance in measurement */
    if((value < SW1 + 12) && (value > SW1 - 12))
      {
        value = SW1;
      }  
    else if((value < SW2 + 12) && (value > SW2 - 12))
      {
        value = SW2;
      }  
    else if((value < SW12 + 12) && (value > SW12 - 12))
      {
        value = SW12;
      }  
    else if((value < NoKey + 12) && (value > NoKey - 12))
      {
        value = NoKey;
      }  
 	
	buffer[0] = value;
   
    if ( (buffer[0] == buffer[1]) && (buffer[1] == buffer[2]) )
	{
      /* Last three values were identical */
 
         last_value = value; /* Update with debounced new input */
	}
	else
    {
	  /* Switches have changed but not debounced. Default to last_value */
	}

	counter = 0; /* Start new conversion next time through. */
   }
 }
 else
 {
  /* Debounce time of 10ms has not yet elapsed. */
  counter++;
 }


 if ( last_value == NoKey)
  {
   return (0);
  }
 else if ( last_value == SW1)
  {
   return (1);
  }
 else if ( last_value == SW2 )
  {
   return (2);
  } 
 else if ( last_value == SW12)
  {
   return (3);
  } 
 else 
  {
   return (0);
  } 

}


/*****************************************************************************/
/* End of pushbuttons.c                                                      */
/*****************************************************************************/





