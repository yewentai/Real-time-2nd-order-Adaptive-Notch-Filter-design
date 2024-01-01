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
// extern int anf(short y, short *X , unsigned short *A, unsigned short *rho, int* index);

/* ------------------------------------------------------------------------ *
 *                                                                          *
 *  main( )                                                                 *
 *                                                                          *
 * ------------------------------------------------------------------------ */
int main(void)
{
    short y, e;    // Filter input and error
    int index = 0; // Index for adaptive filter, initialized to 0

    FILE *fpIn;  // File pointer for input file
    FILE *fpOut; // File pointer for output file

    char tempc[2]; // Buffer to read input file

    short s[3] = {0, 0, 0};                   // Array for storing wanted signal, initialized to zero
    unsigned short a = {0x2000};              // Coefficient 'a' for the filter, initialized to a fixed value
    unsigned short rho[2] = {0xCCCC, 0xCCCC}; // Fixed-point representation of 0.8 (rho values for the filter)

    // Open input and output files in binary mode
    fpIn = fopen("..\\data\\input.pcm", "rb");
    fpOut = fopen("..\\data\\output_c_6.pcm", "wb");

    // Check if file pointers are null (indicating failure to open files)
    if (fpIn == NULL || fpOut == NULL)
    {
        printf("Can't open input or output file. Exiting. \n");
        return 1; // Exit the program if file opening fails
    }

    // Begin filtering the data
    while (fread(tempc, sizeof(char), 2, fpIn) == 2)
    { // Read 2 bytes from input file
        // Convert 2 bytes to 16-bit integer assuming little-endian format
        y = (tempc[0] & 0xFF) | (tempc[1] << 8);

        // Call ANF function; ensure anf() is implemented for fixed-point
        e = anf(y, &s[0], &a, &rho[0], &index); // Adaptive Notch Filter applied on input signal y

        // Convert 16-bit integer back to 2 bytes in little-endian format
        tempc[0] = (short)(e & 0x00FF);
        tempc[1] = (short)(e & 0xFF00) >> 8;

        fwrite(tempc, sizeof(char), 2, fpOut); // Write the filtered data to output file
    }

    // Close the input and output files
    fclose(fpIn);
    fclose(fpOut);

    return 0; // Return 0 to indicate successful execution
}

/*****************************************************************************/
/* End of main.c                                                             */
/*****************************************************************************/
