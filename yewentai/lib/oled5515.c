/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*   oled.c                                                                  */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5515 USB Stick.                                                  */
/*   Send messages to 2 line OLED LCD display.                               */
/*                                                                           */
/* REVISION                                                                  */
/*   Revision: 1.00                                                          */
/*   Author  : Richard Sikora                                                */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* HISTORY                                                                   */
/*   Revision: 1.00                                                          */
/*   19th September 2010. Created from Spectrum Digital Code.                */
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


int character_set[] = {
	                       0x7C,0x09,0x0A,0x7C,  // A
	                       0x36,0x49,0x49,0x7F,  // B 
                           0x22,0x41,0x41,0x3E,  // C
                           0x3E,0x41,0x41,0x7F,  // D
                           0x41,0x49,0x49,0x7F,  // E
                           0x01,0x09,0x09,0x7F,  // F
                           0x32,0x51,0x41,0x3E,  // G
                           0x7F,0x08,0x08,0x7F,  // H
                           0x00,0x7F,0x00,0x00,  // I
                           0x3F,0x40,0x40,0x20,  // J                             
                           0x41,0x22,0x14,0x7F,  // K
                           0x40,0x40,0x40,0x7F,  // L
                           0x7F,0x06,0x06,0x7F,  // M
                           0x7F,0x18,0x0C,0x7F,  // N
                           0x3E,0x41,0x41,0x3E,  // O
                           0x06,0x09,0x09,0x7F,  // P
                           0x7E,0x61,0x41,0x3E,  // Q  
                           0x76,0x09,0x09,0x7F,  // R
                           0x32,0x49,0x49,0x26,  // S 
                           0x01,0x01,0x7F,0x01,  // T
                           0x3F,0x40,0x40,0x3F,  // U
                           0x0F,0x70,0x70,0x0F,  // V
                           0x7F,0x30,0x30,0x7F,  // W
                           0x63,0x1C,0x1C,0x63,  // X
                           0x03,0x0C,0x78,0x03,  // Y
                           0x43,0x4D,0x51,0x61   // Z
                            
                         };

int number_set [] = { 0x10,0x7C,0x10,0x10,  // +	                  
	                  0x00,0x60,0x80,0x00,  // ,
	                  0x10,0x10,0x10,0x10,  // -
	                  0x00,0x60,0x00,0x00,  // .
	                  0x03,0x0C,0x30,0x40,  // /
	                  0x3E,0x41,0x41,0x3E,  // 0
	                  0x00,0x7F,0x00,0x00,  // 1
	                  0x46,0x49,0x49,0x72,  // 2	                  
	                  0x36,0x49,0x49,0x22,  // 3
	                  0x08,0x7C,0x08,0x0F,  // 4
	                  0x31,0x49,0x49,0x2F,  // 5
                      0x33,0x49,0x49,0x3E,  // 6   
                      0x07,0x09,0x71,0x01,  // 7 
                      0x36,0x49,0x49,0x36,  // 8
                      0x3E,0x49,0x49,0x26,  // 9
					  0x00,0x6C,0x00,0x00,  // :
					  0x00,0x6C,0x80,0x00,  // ;
					  0x08,0x14,0x22,0x41,  // >
					  0x14,0x14,0x14,0x14,  // =
					  0x06,0x09,0x59,0x02,  // ?
					  0x7E,0x41,0x59,0x5E   // @
                    };
 
#include "usbstk5515.h"
#include "usbstk5515_i2c.h"
#include "lcd.h"
#include "usbstk5515_gpio.h"

#define OSD9616_I2C_ADDR 0x3C    // OSD9616 I2C address

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 OSD9616_send( Uint16 comdat, Uint16 data )                        *
 *                                                                          *
 *      Sends 2 bytes of data to the OSD9616                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 OSD9616_send( Uint16 comdat, Uint16 data )
{
    Uint8 cmd[2];
    cmd[0] = comdat & 0x00FF;     // Specifies whether data is Command or Data
    cmd[1] = data;                // Command / Data

    return USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 2 );
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 OSD9616_multiSend( Uint16 comdat, Uint16 data )                   *
 *                                                                          *
 *      Sends multiple bytes of data to the OSD9616                         *
 *                                                                          *
 * ------------------------------------------------------------------------ */
Int16 OSD9616_multiSend( Uint8* data, Uint16 len )
{
    Uint16 x;
    Uint8 cmd[10];
    for(x=0;x<len;x++)               // Command / Data
    {
    	cmd[x] = data[x];
    }
    return USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, len );
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 printCharacter( int * src )                                       *
 *                                                                          *
 *  Sends 1 ascii character to the OSD9616                                  *
 *                                                                          *
 * ------------------------------------------------------------------------ */

Int16 printCharacter(int * src)
{
 	OSD9616_send(0x40,src[0]);
    OSD9616_send(0x40,src[1]);
    OSD9616_send(0x40,src[2]);
    OSD9616_send(0x40,src[3]);
    OSD9616_send(0x40,0x00);
    return 0;
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  Int16 printMessage( int * src )                                         *
 *                                                                          *
 *  Checks ascii characters and sends 19 characters.                        *
 *                                                                          *
 * ------------------------------------------------------------------------ */

Int16 printMessage(char * src)
{
 int i,j;
 	
 OSD9616_send(0x40,0x00);
 OSD9616_send(0x40,0x00); // Two spaces at right for alignment	
 
 for ( i = 18 ; i >= 0; i--)
  {
  	  if ( src[i] >= 'A' && src[i] <= 'Z')
  	   {
  	    /* Upper case characters */	 
       printCharacter(&character_set[(src[i]-'A')*4]);
  	   }
   	  else if ( src[i] >= 'a' && src[i] <= 'z')
  	   { 
  	    /* Lower case characters. Print as upper case */	
       printCharacter(&character_set[(src[i]-'a')*4]);
  	   }
  	  else if ( src[i] >= '+' && src[i] <= '@')
  	   {
  	   	/* Numbers 0 to 9 */
  	    printCharacter(&number_set[(src[i]-'+')*4]);
  	   }  
  	  else
  	   {
  	    /* Default to blanks for unrecognised characters */
  	   	for ( j = 0 ; j < 5; j++)
  	   	 {
  	   	  OSD9616_send(0x40,0x00); /* Spaces */
  	   	 }  	    
  	   }  
  }
 return(0);
}

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  int oled_init(void)                                                     *
 *                                                                          *
 *      Testing function for the OSD9616 display                            *
 *                                                                          *
 * ------------------------------------------------------------------------ */
int oled_init(void)
{
	Int16 i2c_err;
	Uint8 cmd[10];    // For multibyte commands
	
	/* Initialize I2C */
    USBSTK5515_I2C_init( );
    
    /* Initialize LCD power */
    USBSTK5515_GPIO_setDirection( 12, 1 );  // Output
    USBSTK5515_GPIO_setOutput( 12, 1 );     // Enable 13V 
    
    /* Initialize OSD9616 display */
    i2c_err = OSD9616_send(0x00,0x2E);
    i2c_err = OSD9616_send(0x00,0x00); // Set low column address
    i2c_err = OSD9616_send(0x00,0x10); // Set high column address
    if(i2c_err)  // Don't setup display if not connected
        return 1;
    
    OSD9616_send(0x00,0x40); // Set start line address

    cmd[0] = 0x00 & 0x00FF;  // Set contrast control register
    cmd[1] = 0x81;
    cmd[2] = 0x7f;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xa1); // Set segment re-map 95 to 0
    OSD9616_send(0x00,0xa6); // Set normal display

    cmd[0] = 0x00 & 0x00FF;  // Set multiplex ratio(1 to 16)
    cmd[1] = 0xa8; 
    cmd[2] = 0x0f;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xd3); // Set display offset
    OSD9616_send(0x00,0x00); // Not offset
    OSD9616_send(0x00,0xd5); // Set display clock divide ratio/oscillator frequency
    OSD9616_send(0x00,0xf0); // Set divide ratio

    cmd[0] = 0x00 & 0x00FF;  // Set pre-charge period
    cmd[1] = 0xd9;
    cmd[2] = 0x22;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    cmd[0] = 0x00 & 0x00FF;  // Set com pins hardware configuration
    cmd[1] = 0xda;
    cmd[2] = 0x02;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xdb); // Set vcomh
    OSD9616_send(0x00,0x49); // 0.83*vref
    
    cmd[0] = 0x00 & 0x00FF;  //--set DC-DC enable
    cmd[1] = 0x8d;
    cmd[2] = 0x14;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xaf); // Turn on oled panel    
    
    /* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+0); // Set page for page 0 to page 5
  
	return 0;
}

/*****************************************************************************/
/* oled_display_message()                                                    */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Parameter 1:  Pointer to string containing ascii message for topline.     */
/* Parameter 2:  Pointer to string containing ascii message for bottomline   */
/*                                                                           */
/* RETURNS: Nothing                                                          */
/*                                                                           */
/*****************************************************************************/

int oled_display_message(char * topline, char * bottomline)
{
	Uint8 cmd[10];    // For multibyte commands
	
	OSD9616_send(0x00,0x2E);           // Turn off scrolling
	
    /* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+0); // Set page for page 0 to page 5

    printMessage(&topline[0]);
    
    /* Write to page 1*/ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+1); // Set page for page 0 to page 5
   
    printMessage(&bottomline[0]);
    
        /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x29;  // Vertical and Right Horizontal Scroll
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x03;  // Set time interval between each scroll step
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x01;  // Vertical scrolling offset
    OSD9616_multiSend( cmd, 7 );
    OSD9616_send(0x00,0x2E);           // Turn off scrolling
    /* Keep first 8 rows from vertical scrolling  */
    cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Set Vertical Scroll Area
    cmd[2] = 0x08;  // Set No. of rows in top fixed area
    cmd[3] = 0x08;  // Set No. of rows in scroll area
    OSD9616_multiSend( cmd, 4 );

	return 0;
}


/*****************************************************************************/
/* amplitude2bargraph()                                                      */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Parameter 1:  Input value in range -32767 to - 32768.                     */
/*                                                                           */
/* RETURNS: Bargraph pixel value between 0 and FFFFh.                        */
/*                                                                           */
/*****************************************************************************/


static unsigned int amplitude2bargraph(int input)
{
 if ( input < 0 ) 
  {
    input = -input;
  }

 if ( input > 23166)
  {
    return (0xFFFF); /* All LEDs on */
  }
 else if (input > 16384)
  {
    return (0xFFFF);
  }
 else if ( input > 11583)
  {
    return (0xFFFF);
  }
 else if ( input > 8192)
  {
    return (0xFFFF);
  }
 else if ( input > 5791)
  {
    return (0xFFFF);
  }
 else if ( input > 4096)
  {
    return (0xFFFF);
  }
 else if (input > 2896)
  {
    return ( 0xFFFF);
  }
 else if ( input > 2048)
  {
    return (0xFFFF);
  }
 else if ( input > 1448)
  {
   return (0xFFFF);
  }
 else if ( input > 1024)
  {
   return (0xFFFE );
  }
 else if ( input > 724)
  {
   return (0xFFFC);
  }
 else if ( input > 512)
  {
   return (0xFFF8);
  }
 else if ( input > 362)
  {
   return (0xFFF0);
  }
 else if ( input > 256)
  {
   return (0xFFE0);
  }
 else if ( input > 181)
  {
   return (0xFFC0);
  }
 else if ( input > 128)
  {
   return (0xFF80);
  }
 else if ( input > 90)
  {
   return (0xFF00);
  } 
 else if ( input > 64)
  {
   return (0xFE00);
  } 
 else if ( input > 45)
  {
   return (0xFC00);
  } 
 else if ( input > 32)
  {
   return (0xF800);
  }  
 else if (input > 22)
  {
   return (0xF000);
  } 
 else if ( input > 16)
  {
   return(0xE000);
  } 
 else if ( input > 11)
  {
   return (0xC000);
  } 
 else if ( input > 8)
  {
   return (0x8000);
  } 
 else
  {
    return (0);
  }
}
/*****************************************************************************/
/* amplitude2linear()                                                        */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Parameter 1:  Input value in range -32767 to - 32768.                     */
/*                                                                           */
/* RETURNS: Bargraph pixel value between 0 and FFFFh.                        */
/*                                                                           */
/*****************************************************************************/

#if 0

static unsigned int amplitude2linear(int input)
{
 if ( input < 0 ) 
  {
    input = -input;
  }

 if ( input > 30000 )
  {
    return (0xFFFFU); /* All LEDs on */
  }
 else if (input > 28000)
  {
    return (0xFFFEU);
  }
 else if ( input > 26000)
  {
    return (0xFFFCU);
  }
 else if ( input > 24000)
  {
    return (0xFFF8U);
  }
 else if ( input > 22000)
  {
    return (0xFFF0U);
  }
 else if ( input > 20000)
  {
    return (0xFFE0U);
  }
 else if (input > 18000)
  {
    return ( 0xFFC0U);
  }
 else if ( input > 16000)
  {
    return (0xFF80U);
  }
 else if ( input > 14000)
  {
   return (0xFF00U);
  }
 else if ( input > 12000)
  {
   return (0xFE00U);
  }
 else if ( input > 10000)
  {
   return (0xFC00U);
  }
 else if ( input > 8000)
  {
   return (0xF800U);
  }
 else if ( input > 6000)
  {
   return (0xF000U);
  }
 else if ( input > 4000)
  {
   return (0xE000U);
  }
 else if ( input > 2000)
  {
   return (0xC000U);
  }
 else if ( input > 1000)
  {
   return (0x8000U);
  }
 else
  {
    return (0);
  }
}

#endif

/*****************************************************************************/
/* amplitude2signal()                                                        */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Parameter 1:  Input value in range -32767 to - 32768.                     */
/*                                                                           */
/* RETURNS: Bargraph pixel value between 0 and FFFFh.                        */
/*                                                                           */
/*****************************************************************************/


static unsigned int amplitude2signal(int input)
{

 if ( input > 16000 )
  {
    return (0x0001); /* All LEDs on */
  }
 else if (input > 8000)
  {
    return (0x0002U);
  }
 else if ( input > 4000)
  {
    return (0x0004U);
  }
 else if ( input > 2000)
  {
    return (0x0008U);
  }
 else if ( input > 1000)
  {
    return (0x0010U);
  }
 else if ( input > 500)
  {
    return (0x0020U);
  }
 else if (input > 250)
  {
    return ( 0x0040U);
  }
 else if ( input >= 0)
  {
    return ( 0x0080U);
  } 
 else if ( input  < -16000)
  {
    return (0x8000U);
  }
 else if ( input < -8000)
  {
   return (0x4000U);
  }
 else if ( input < -4000)
  {
   return (0x2000U);
  }
 else if ( input < -2000)
  {
   return (0x1000U);
  }
 else if ( input < -1000)
  {
   return (0x0800U);
  }
 else if ( input < -500)
  {
   return (0x0400U);
  }
 else if ( input < -250)
  {
   return (0x0200U);
  }
 else
  {
    return (0x0100);
  }
}

/*****************************************************************************/
/* oled_display_bargraph()                                                   */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Parameter 1:  Pointer to array containing 96 16-bit input values.         */
/*                                                                           */
/* RETURNS: Nothing                                                          */
/*                                                                           */
/*****************************************************************************/

int oled_display_bargraph (int * message)
{
	Uint8 cmd[10];    // For multibyte commands
        int i;
        Uint16 input;
        Uint16 pixels;	

	OSD9616_send(0x00,0x2E);           // Turn off scrolling
	
    /* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+0); // Set page for page 0 to page 5

    /* Send topline from high to low */
    for ( i = 95/2 ; i >= 0 ; i--)
     {
       input = amplitude2bargraph(message[i]);       
     
       pixels = (input & 0xFF); /* Extract low byte */

       OSD9616_send(0x40,pixels);
       OSD9616_send(0x40,pixels);
          
     }
    
    /* Write to page 1*/ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+1); // Set page for page 0 to page 5
   

  /* Send bottomline */
    for ( i = 95/2 ; i >= 0 ; i-- )
     {
       input = amplitude2bargraph(message[i]);       
     
       pixels = (input >> 8 ); /* Extract high byte */
       
       OSD9616_send(0x40,pixels);
       OSD9616_send(0x40,pixels);
     }
    
        /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x29;  // Vertical and Right Horizontal Scroll
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x03;  // Set time interval between each scroll step
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x01;  // Vertical scrolling offset
    OSD9616_multiSend( cmd, 7 );
    OSD9616_send(0x00,0x2E);           // Turn off scrolling
    /* Keep first 8 rows from vertical scrolling  */
    cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Set Vertical Scroll Area
    cmd[2] = 0x08;  // Set No. of rows in top fixed area
    cmd[3] = 0x08;  // Set No. of rows in scroll area
    OSD9616_multiSend( cmd, 4 );

	return 0;
}

/*****************************************************************************/
/* oled_display_waveform()                                                   */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Parameter 1:  Pointer to array containing 96 16-bit input values.         */
/*                                                                           */
/* RETURNS: Nothing                                                          */
/*                                                                           */
/*****************************************************************************/

int oled_display_waveform (int * message)
{
	Uint8 cmd[10];    // For multibyte commands
        int i;
        Uint16 input;
        Uint16 pixels;	

	OSD9616_send(0x00,0x2E);           // Turn off scrolling
	
    /* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+0); // Set page for page 0 to page 5

    /* Send topline from high to low */
    for ( i = 95 ; i >= 0 ; i--)
     {
       input = amplitude2signal(message[i]);       
     
       pixels = (input & 0xFF); /* Extract low byte */

       OSD9616_send(0x40,pixels);
          
     }
    
    /* Write to page 1*/ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+1); // Set page for page 0 to page 5
   

  /* Send bottomline */
    for ( i = 95 ; i >= 0 ; i-- )
     {
       input = amplitude2signal(message[i]);       
     
       pixels = (input >> 8 ); /* Extract high byte */
       
       OSD9616_send(0x40,pixels);
     }
    
        /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x29;  // Vertical and Right Horizontal Scroll
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x03;  // Set time interval between each scroll step
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x01;  // Vertical scrolling offset
    OSD9616_multiSend( cmd, 7 );
    OSD9616_send(0x00,0x2E);           // Turn off scrolling
    /* Keep first 8 rows from vertical scrolling  */
    cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Set Vertical Scroll Area
    cmd[2] = 0x08;  // Set No. of rows in top fixed area
    cmd[3] = 0x08;  // Set No. of rows in scroll area
    OSD9616_multiSend( cmd, 4 );

	return 0;
}

/*****************************************************************************/
/* oled_display_picturew()                                                   */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* Parameter 1:  Pointer to array containing 96 16-bit input values.         */
/*                                                                           */
/* RETURNS: Nothing                                                          */
/*                                                                           */
/*****************************************************************************/

int oled_display_picture (int * message)
{
	Uint8 cmd[10];    // For multibyte commands
        int i;
        Uint16 input;
        Uint16 pixels;	

	OSD9616_send(0x00,0x2E);           // Turn off scrolling
	
    /* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+0); // Set page for page 0 to page 5

    /* Send topline from high to low */
    for ( i = 95 ; i >= 0 ; i--)
     {
       input = message[i];       
     
       pixels = (input & 0xFF); /* Extract low byte */

       OSD9616_send(0x40,pixels);
          
     }
    
    /* Write to page 1*/ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+1); // Set page for page 0 to page 5
   

  /* Send bottomline */
    for ( i = 95 ; i >= 0 ; i-- )
     {
       input = message[i];       
     
       pixels = (input >> 8 ); /* Extract high byte */
       
       OSD9616_send(0x40,pixels);
     }
    
        /* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x29;  // Vertical and Right Horizontal Scroll
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x03;  // Set time interval between each scroll step
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x01;  // Vertical scrolling offset
    OSD9616_multiSend( cmd, 7 );
    OSD9616_send(0x00,0x2E);           // Turn off scrolling
    /* Keep first 8 rows from vertical scrolling  */
    cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Set Vertical Scroll Area
    cmd[2] = 0x08;  // Set No. of rows in top fixed area
    cmd[3] = 0x08;  // Set No. of rows in scroll area
    OSD9616_multiSend( cmd, 4 );

	return 0;
}
/*****************************************************************************/
/* End of oled.c                                                             */
/*****************************************************************************/
