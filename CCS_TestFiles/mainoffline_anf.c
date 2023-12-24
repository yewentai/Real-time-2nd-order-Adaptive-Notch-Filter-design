/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*  main.c                                                                   */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5515 ANF-LMS implementation   				                     */
/*                                                                           */
/*****************************************************************************/

#include "stdio.h"
#include "anf.h"

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
int main( void ) 
{
	int y, e, tmp1;
	unsigned int index = 0;
	
	FILE  *fpIn;
	FILE  *fpOut;
	
	char  tempc[4];

	int s[3] = {0,0,0};
	int a[2] = {0,0};
	int rho[2] = {0x66666666,0x66666666}; // Fixed-point representation of 0.8 in 32q31 format
	// int rho[2] = {0, 0}; // rho adaptive {rho=?, rho_inf}

	fpIn = fopen("..\\data\\in.pcm", "rb");
	fpOut = fopen("..\\data\\out.pcm", "wb");

	if (fpIn == NULL || fpOut == NULL) {
	    printf("Can't open input or output file. Exiting. \n");
	    return 1;
	}

    //Begin filtering the data
    while (fread(tempc, sizeof(char), 4, fpIn) == 4) {
        // Convert 4 bytes to 32-bit integer assuming little-endian format
        y = (tempc[0] & 0xFF) | ((tempc[1] & 0xFF) << 8) | ((tempc[2] & 0xFF) << 16) | (tempc[3] << 24);
        
        // Call ANF function; ensure anf() is implemented for fixed-point
        e = anf(y, s, &a, &rho, &index); // Adaptive Notch Filter.
        
        // Convert 32-bit integer back to 4 bytes in little-endian format
        tempc[0] = e & 0xFF;
        tempc[1] = (e >> 8) & 0xFF;
        tempc[2] = (e >> 16) & 0xFF;
        tempc[3] = (e >> 24) & 0xFF;
        
        fwrite(tempc, sizeof(char), 4, fpOut);
    }

    fclose(fpIn);
    fclose(fpOut);
}

/*****************************************************************************/
/* End of main.c                                                             */
/*****************************************************************************/
