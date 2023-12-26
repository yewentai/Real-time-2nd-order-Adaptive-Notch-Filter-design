#include "anf.h"

int anf(int y, int *x, int *a, int *rho, unsigned int* index)
{
    int a_i = *a;
    /*
     y in Q?: newly captured sample
     X in Q?: X[3] databuffer - Hint: Reserve a sufficient number of integer bits such that summing numbers does not cause overflow
     (Hence no shift is needed after summing numbers)
     A in Q?: the adaptive filter coefficients
     e in Q?: output signal
     rho in Q?: rho[2] = rho(t), rho(inf) }
     index points to t-1 sample (t current time index) in X -> circular buffer
     */

    int e, k; //16
    long AC0, AC1; //32

    //add your own code here
    k = *index;
    /*
     * modify rho section
     * rho[0] is rho(t), rho[1] = rho(+inf)
     */

    AC0 = (long)lambda * rho[0];
    AC0 += 0x00004000;
    AC0 >>= 15;
    AC1 = (long)lambda2 * rho[1];
    AC1 += 0x00004000;
    AC1 >>= 15;
    rho[0] = (int)(AC0 + AC1);


    AC0 = (long)(*rho) * x[k]; // 16q15 * 16q11 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11;
    /**
     * according to the matlab file, AC now swings between -3.+ and 3.+
     */
    AC0 = (long)a_i * (AC0 >> 2); // 32q13 * 16q13 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11;
    AC1 = -(long)a_i * x[k]; // 32q13 * 32q11 = 32q24
    AC1 += 0x00000100;
    AC1 >>= 9;

    k = (k + 1) % 3;

    AC1 += ((long)x[k] << 4); // 32q15 + 32q15

    long rho_sq = (long)(*rho) * *rho;
    rho_sq += 0x00004000;
    rho_sq >>= 15;
    long temp = ((long)x[k] * (short)rho_sq);
    temp += 0x00000400;
    temp >>= 11;
    AC0 -= temp;

    k = (k + 1) % 3;

    x[k] = (short)(((long)y + AC0) >> 4); //v[i]
    //x_debug(i)= x[index];
    e = (int)(((long)x[k] << 4) + (long)AC1);

    k = (k + 1) % 3;

    AC0 = (long)mu * x[k]; // 16q15*16q11 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11; //32q15
    AC0 = (long)e * (short)(AC0); // 16q15 * 16q15 = 32q30
    AC0 += 0x00004000;
    AC0 >>= 15; //15 + 2
    a_i += (short)((AC0 + 0x2) >> 2);
    k = (k + 2) % 3;
    // necessary check to see if |a| < 2
    if (a_i < -0x4000)
        a_i = -0x4000;
    if (a_i > 0x4000)
        a_i = 0x4000;
    *a = a_i;
    
    *index = k;
    return e;
}
