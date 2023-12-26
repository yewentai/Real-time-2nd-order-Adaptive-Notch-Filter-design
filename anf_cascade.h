#ifndef ANF_CASCADE_H
#define ANF_CASCADE_H

#define lambda 0x4CCD   // Q15, lambda = 0.6
#define lambda2 0x3333  // Q15, lambda2 = 1 - lambda

int anf_cascade(int y, int *U , int *A, int *rho, unsigned int* index);

#endif
