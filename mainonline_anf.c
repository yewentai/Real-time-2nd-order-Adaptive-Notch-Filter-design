/*****************************************************************************/
/*                                                                           */
/* FILENAME                                                                  */
/*  main.c                                                                   */
/*                                                                           */
/* DESCRIPTION                                                               */
/*   TMS320C5515 ANF-LMS implementation              				         */
/*   You should not have to edit this file                                   */
/*                                                                           */
/*****************************************************************************/

#include "stdio.h"
#include "usbstk5515.h"
#include "usbstk5515_i2c.h"
#include "anf.h"
#include "AIC3204.h"

void writeToFile(FILE * fp, int x);

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main()                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
void main(void)
{
	FILE  *fpi = NULL, *fpo = NULL; // File pointers

    // int X[3]= {0,0,0};  // State vector X
    // int A[1] = {0x00008000};     // Filter coefficient 32Q15?????
    // int rho[2] = {0x6666, 0x6666};  // Initial values for rho(t), rho(Inf)

	int X[6]= {0,0,0,0,0,0};  // State vector X sways between [-16;15.999] << 4
	int A[2] = {0x2000, 0x2000};     // Filter coefficient sways between [-4,3.9999] << 2
	int rho[2] = {0x6666, 0x6666};  // Initial values for rho(t), rho(Inf)

    int y, e;  // input sample y, error signal sample e
    unsigned int index = 0;

    char tempc[2];  // Temporary variable to read 2 chars from a file.

    aic3204_init();

	fpi = fopen("..\\data\\input.pcm", "rb");
	fpo = fopen("..\\data\\output.pcm", "wb");

	if (fpi == NULL || fpo == NULL) {
	    printf("Can't open input or output file. Exiting program. \n");
	    return ;
	}

    int numRead = fread(tempc, sizeof(char), 2, fpi);  // Returns the number of chars read.

	//  Begin filtering the data
    while (numRead == 2) {
        y = (tempc[0] & 0xFF) | (tempc[1] << 8);  // Convert from little endian to big endian
        e = anf(y, &X[0], &A[0], &rho[0], &index); // Adaptive Notch Filter function.
        writeToFile(fpo, e);  // Write output to file
        numRead = fread(tempc, sizeof(char), 2, fpi);  // Read new sample
        aic3204_codec_write(e >> 8, e >> 8);
    }

    fclose(fpi);
	fclose(fpo);
    printf( "\n***Program has Terminated***\n" );
}

void writeToFile(FILE * fp, int x) {
    char temp[2];
    temp[0] = (x & 0xFF);
    temp[1] = (x >> 8) & 0xFF;

    fwrite(temp, sizeof(char), 2, fp);
}

/*****************************************************************************/
/* End of main.c                                                             */
/*****************************************************************************/

