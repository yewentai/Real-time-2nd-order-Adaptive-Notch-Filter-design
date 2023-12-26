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
	
	char  temp[2];

	int s[3] = {0};
	int a[2] = {0x2000};
	int rho[2] = {0x6666}; // Fixed-point representation of 0.8 in 32q31 format
	// int rho[2] = {0, 0}; // rho adaptive {rho=?, rho_inf}

	fpIn = fopen("..\\data\\input.pcm", "rb");
	fpOut = fopen("..\\data\\output.pcm", "wb");

	if (fpIn == NULL || fpOut == NULL) {
	    printf("Can't open input or output file. Exiting. \n");
	    return 1;
	}

    //Begin filtering the data
    while (fread(temp, sizeof(char), 2, fpIn) == 2) {
        // Convert 2 bytes to 16-bit integer assuming little-endian format
        y = (temp[0] & 0xFF) | (temp[1] << 8);
        
        // Call ANF function; ensure anf() is implemented for fixed-point
        e = anf(y, &s[0], &a[0], &rho[0], &index); // Adaptive Notch Filter.
        
        // Convert 16-bit integer back to 2 bytes in little-endian format
        temp[0] = (short) (e & 0x00FF);
        temp[1] = (short) (e & 0xFF00) >> 8;
        
        fwrite(temp, sizeof(char), 2, fpOut);
    }

    fclose(fpIn);
    fclose(fpOut);
}

/*****************************************************************************/
/* End of main.c                                                             */
/*****************************************************************************/
