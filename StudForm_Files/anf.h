#ifndef ANF_H
#define ANF_H

#define mu 0xC8 //0x0190       // Q15, 2 * MU = 400
#define lambda 0x4CCD   // Q15, lambda = 0.6
#define lambda2 0x3333  // Q15, lambda2 = 1 - lambda

int anf(int y, int *U , int *A, int *rho, unsigned int* index);

#endif
