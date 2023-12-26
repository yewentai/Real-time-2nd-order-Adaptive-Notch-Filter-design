#include "anf.h"

int anf(int y, int *s, int *a, int *rho, unsigned int* index)
{
    /*
     y in Q? : newly captured sample
     s in Q? : x[3] databuffer - Hint: Reserve a sufficiently number of integer bits such that summing intermediate values does not cause overflow (so no shift is needed after summing numbers)
     a in Q? : the adaptive coefficient
     e in Q? : output signal
     rho in Q? : fixed {rho, rho^2} or variable {rho, rho_inf} pole radius
     index : points to (t-1) sample (t current time index) in s -> circular buffer
     */

    int e, k;
    long AC0, AC1;

    k = *index;

	// TODO: add your own code here
    
    return e;
}
