/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/* 	 bargraph.c                                                              */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5515 USB Stick.                                                  */
/*   Configures four user LEDs as a bargraph display.                        */
/*   Different formats of display for mono audio, stereo audio or power      */
/*   for use with Fast Fourier Transform (FFT).                              */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00	                                                         */
/*   Author  : Richard Sikora                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*                                                                           */
/*   Revision 1.00                                                           */
/*   19th September 2010. Created by Richard Sikora from TMS320C5510 code.   */
/*                                                                           */
/*****************************************************************************/


#include "usbstk5515_led.h"


/*****************************************************************************/
/* Reference thresholds                                                      */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* 0 dB reference is taken to be 50% output level. This allows 6dB headroom  */
/* Other thresholds are -6dB, -12dB and -18dB below the reference of 0dB.    */
/*                                                                           */ 
/*****************************************************************************/

#define LEVEL_0dB  0x4000
#define LEVEL_3dB  0x2D3F
#define LEVEL_6dB  0x2000
#define LEVEL_9dB  0x169F
#define LEVEL_12dB 0x1000
#define LEVEL_18dB 0x0800
#define LEVEL_24dB 0x0400
#define LEVEL_36dB 0x0100


/*****************************************************************************/
/* bargraph_3dB()                                                            */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Displays greater of two inputs as a bargraph using 3dB increments.        */
/* For mono use, use the single mono input for both the left and the right   */
/* channels.                                                                 */
/*                                                                           */
/*****************************************************************************/

void bargraph_3dB(signed int left_channel, signed int right_channel)
{   
 signed int input;
 
 /* If left channel is negative, then convert to positive */

 if ( 0x8000 == left_channel)  /* Special case where input = -32768 */
  { 
   left_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( left_channel < 0)
  {
   left_channel = - left_channel;     /* Make negative values positive */
  }                             
  
 /* If right channel is negative, then convert to positive */

 if ( 0x8000 == right_channel)  /* Special case where input = -32768 */
  { 
   right_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( right_channel < 0)
  {
   right_channel = - right_channel;     /* Make negative values positive */
  }    
                   
 /* Display the greater of the two channels */                  
                   
 if ( right_channel > left_channel)
  {
   input = right_channel;
  } 
 else
  {
   input = left_channel;
  }
  
 if ( input > LEVEL_0dB)
  {
    /* Large input signal. Turn on all 4 LEDs */
    USBSTK5515_ULED_setall(~0xF);
  }
 else if ( input > LEVEL_3dB)
  {
    /* Smaller signal. Turn on 3 LEDs */
    USBSTK5515_ULED_setall(~0xE);
  }           
 else if ( input > LEVEL_6dB)
  {
    /* Turn on 2 LEDs */
    USBSTK5515_ULED_setall(~0xC);
  }  
 else if ( input > LEVEL_9dB)
  { 
    /* Turn on 1 LED */
    USBSTK5515_ULED_setall(~0x8);
  }    
 else
  {
    /* Small or no input signal. Turn off all 4 LEDs */
    USBSTK5515_ULED_setall(~0x0);
  }   

}

/*****************************************************************************/
/* bargraph_6dB()                                                            */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Displays greater of two inputs as a bargraph. For mono use, use the       */
/* single mono input for both the left and the right channels.               */
/*                                                                           */
/*****************************************************************************/

void bargraph_6dB(signed int left_channel, signed int right_channel)
{   
 signed int input;
 
 /* If left channel is negative, then convert to positive */

 if ( 0x8000 == left_channel)  /* Special case where input = -32768 */
  { 
   left_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( left_channel < 0)
  {
   left_channel = - left_channel;     /* Make negative values positive */
  }                             
  
 /* If right channel is negative, then convert to positive */

 if ( 0x8000 == right_channel)  /* Special case where input = -32768 */
  { 
   right_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( right_channel < 0)
  {
   right_channel = - right_channel;     /* Make negative values positive */
  }    
                   
 /* Display the greater of the two channels */                  
                   
 if ( right_channel > left_channel)
  {
   input = right_channel;
  } 
 else
  {
   input = left_channel;
  }
  
 if ( input > LEVEL_0dB)
  {
    /* Large input signal. Turn on all 4 LEDs */
    USBSTK5515_ULED_setall(~0xF);
  }
 else if ( input > LEVEL_6dB)
  {
    /* Smaller signal. Turn on 3 LEDs */
    USBSTK5515_ULED_setall(~0xE);
  }           
 else if ( input > LEVEL_12dB)
  {
    /* Turn on 2 LEDs */
    USBSTK5515_ULED_setall(~0xC);
  }  
 else if ( input > LEVEL_18dB)
  { 
    /* Turn on 1 LED */
    USBSTK5515_ULED_setall(~0x8);
  }    
 else
  {
    /* Small or no input signal. Turn off all 4 LEDs */
    USBSTK5515_ULED_setall(~0x0);
  }   

}


/*****************************************************************************/
/* bargraph_12dB()                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Displays greater of two inputs as a bargraph using 3dB increments.        */
/* For mono use, use the single mono input for both the left and the right   */
/* channels.                                                                 */
/*                                                                           */
/*****************************************************************************/

void bargraph_12dB(signed int left_channel, signed int right_channel)
{   
 signed int input;
 
 /* If left channel is negative, then convert to positive */

 if ( 0x8000 == left_channel)  /* Special case where input = -32768 */
  { 
   left_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( left_channel < 0)
  {
   left_channel = - left_channel;     /* Make negative values positive */
  }                             
  
 /* If right channel is negative, then convert to positive */

 if ( 0x8000 == right_channel)  /* Special case where input = -32768 */
  { 
   right_channel= 0x7FFF;      /*  Make input +32767. There is no +32768! */ 
  } 
 else if ( right_channel < 0)
  {
   right_channel = - right_channel;     /* Make negative values positive */
  }    
                   
 /* Display the greater of the two channels */                  
                   
 if ( right_channel > left_channel)
  {
   input = right_channel;
  } 
 else
  {
   input = left_channel;
  }
  
 if ( input > LEVEL_0dB)
  {
    /* Large input signal. Turn on all 4 LEDs */
    USBSTK5515_ULED_setall(~0xF);
  }
 else if ( input > LEVEL_12dB)
  {
    /* Smaller signal. Turn on 3 LEDs */
    USBSTK5515_ULED_setall(~0xE);
  }           
 else if ( input > LEVEL_24dB)
  {
    /* Turn on 2 LEDs */
    USBSTK5515_ULED_setall(~0xC);
  }  
 else if ( input > LEVEL_36dB)
  { 
    /* Turn on 1 LED */
   USBSTK5515_ULED_setall(~0x8);
  }    
 else
  {
    /* Small or no input signal. Turn off all 4 LEDs */
    USBSTK5515_ULED_setall(~0x0);
  }   

}

/*****************************************************************************/
/* spectrum_analyser_display()                                               */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Each input represents the power in a particular frequency band, and       */
/* therefore can only be positive.                                           */
/*                                                                           */
/*****************************************************************************/

void spectrum_analyser_display( unsigned int input1,
                                unsigned int input2, 
                                unsigned int input3,
                                unsigned int input4 )
{
  /* Display each input on a different LED */
  
 if ( input1 > LEVEL_0dB)
  {
    USBSTK5515_ULED_setall(~0xF);
  } 
 else 
  {
    USBSTK5515_ULED_setall(~0xF);
  } 
  
 if ( input2 > LEVEL_0dB)
  {
    USBSTK5515_ULED_setall(~0xF);
  } 
 else 
  {
    USBSTK5515_ULED_setall(~0xF);
  }                           
  
 if ( input3 > LEVEL_0dB)
  {
    USBSTK5515_ULED_setall(~0xF);
  } 
 else 
  {
    USBSTK5515_ULED_setall(~0xF);
  } 
    
 if ( input4 > LEVEL_0dB)
  {
    USBSTK5515_ULED_setall(~0xF);
  } 
 else 
  {
    USBSTK5515_ULED_setall(~0xF);
  }   
 
}
 
/*****************************************************************************/
/* End of bargraph.c                                                         */
/*****************************************************************************/
