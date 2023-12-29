/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*  main_cascade_anf.c                                                       */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5515 Cascaded-ANF-LMS implementation   				             */
/*                                                                           */
/*****************************************************************************/

#include "stdio.h"
#include "anf.h"
extern int anf(short y, short *X, unsigned short *A, unsigned short *rho, int *index);

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
int main(void)
{
	short y, e, tmp1;
	int index = 0; // [-2,2]

	FILE *fpIn;
	FILE *fpOut;

	char tempc[2];

	short s[3] = {0, 0, 0};
	unsigned short a = {0x2000};
	unsigned short rho[2] = {0xCCCC, 0xCCCC}; // Fixed-point representation of 0.8 in 32q31 format

	fpIn = fopen("..\\data\\input.pcm", "rb");
	fpOut = fopen("..\\data\\output_off_assy_test.pcm", "wb");

	if (fpIn == NULL || fpOut == NULL)
	{
		printf("Can't open input or output file. Exiting. \n");
		return 1;
	}

	// Begin filtering the data
	while (fread(tempc, sizeof(char), 2, fpIn) == 2)
	{
		// Convert 2 bytes to 16-bit integer assuming little-endian format
		y = (tempc[0] & 0xFF) | (tempc[1] << 8);

		// Call ANF function; ensure anf() is implemented for fixed-point
		e = anf_cascade(y, &s[0], &a, &rho[0], &index); // Adaptive Notch Filter.

		// Convert 16-bit integer back to 2 bytes in little-endian format
		tempc[0] = (short)(e & 0x00FF);
		tempc[1] = (short)(e & 0xFF00) >> 8;

		fwrite(tempc, sizeof(char), 2, fpOut);
	}

	fclose(fpIn);
	fclose(fpOut);
}

/*****************************************************************************/
/* End of main.c                                                             */
/*****************************************************************************/
