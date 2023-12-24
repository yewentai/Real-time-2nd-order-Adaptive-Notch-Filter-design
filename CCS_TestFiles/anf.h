#ifndef ANF_H
#define ANF_H

// Define the fixed-point format (Q31 in this case)
#define Q_FORMAT 31
// Define the step size, mu, properly scaled for the fixed-point format
#define mu 0xC8 // 2 * 100 / (2 ** 31)
#define lambda 0x73333333 // 0.9 * (2 ** 31)
#define one_minus_lambda 0xCCCCCCC // 1 - lambda = 0.1 * (2 ** 31)

int anf(int y, int *s , int *a, int *rho, unsigned int* index);

#endif