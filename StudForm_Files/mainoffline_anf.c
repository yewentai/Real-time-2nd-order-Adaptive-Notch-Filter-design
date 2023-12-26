///*****************************************************************************/
///*                                                                           */
///* FILENAME                                                                  */
///*  main.c                                                                   */
///*                                                                           */
///* DESCRIPTION                                                               */
///*   TMS320C5515 ANF-LMS implementation              				         */
///*   You should not have to edit this file                                   */
///*                                                                           */
///*****************************************************************************/
//
//#include "stdio.h"
////#include "anf.h"
//#include "anf_cascade.h"
//
//extern int anf(int v, int *X , int *A, int *rho, unsigned int* index);
//
//void writeToFile(FILE * fp, int x);
//
///* ------------------------------------------------------------------------ *
// *                                                                          *
// *  main()                                                                 *
// *                                                                          *
// * ------------------------------------------------------------------------ */
//void main(void)
//{
//	FILE  *fpi = NULL, *fpo = NULL; // File pointers
//	/*
//	 * non-cascade
//	int X[3]= {0,0,0};  // State vector X sways between [-16;15.999] << 4
//    int A[1] = {0x2000};     // Filter coefficient sways between [-4,3.9999] << 2
//    int rho[2] = {0x6666, 0x6666};  // Initial values for rho(t), rho(Inf)
//	 *
//	 */
//	//cascade
//	int X[6]= {0,0,0,0,0,0};  // State vector X sways between [-16;15.999] << 4
//	int A[2] = {0x2000, 0x2000};     // Filter coefficient sways between [-4,3.9999] << 2
//	int rho[2] = {0x6666, 0x6666};  // Initial values for rho(t), rho(Inf)
//
//    int y, e;  // input sample y, error signal sample e
//    unsigned int index = 0;
//
//    char tempc[2];  // Temporary variable to read 2 chars from a file.
//
//	fpi = fopen("..\\data\\input.pcm", "rb");
//	fpo = fopen("..\\data\\output.pcm", "wb");
//
//	if (fpi == NULL || fpo == NULL) {
//	    printf("Can't open input or output file. Exiting program. \n");
//	    return ;
//	}
//
//    int numRead = fread(tempc, sizeof(char), 2, fpi);  // Returns the number of chars read.
//
//	//  Begin filtering the data
//    //int sample = 0;
//    while (numRead == 2) {
//        y = (tempc[0] & 0xFF) | (tempc[1] << 8);  // Convert from little endian to big endian
//        // Later in your code
//        e = anf(y, &X[0], &A[0], &rho[0], &index); // Adaptive Notch Filter function.
//        //sample++;
//        writeToFile(fpo, e);  // Write output to file
//        numRead = fread(tempc, sizeof(char), 2, fpi);  // Read new sample
//    }
//
//    fclose(fpi);
//	fclose(fpo);
//    printf( "\n***Program has Terminated***\n" );
//}
//
//void writeToFile(FILE * fp, int x) {
//    char temp[2];
//    temp[0] = (x & 0xFF);
//    temp[1] = (x >> 8) & 0xFF;
//
//    fwrite(temp, sizeof(char), 2, fp);
//}
//
///*****************************************************************************/
///* End of main.c                                                             */
///*****************************************************************************/
//
