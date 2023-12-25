#include "anf.h"
#include "anf_cascade.h"

int anf_cascade(int y, int *x, int *a, int *rho, unsigned int* index)
{
    int a1 = a[0], a2 = a[1];
    int e1, e2, k1, k2; //16
    long AC0, AC1; //32

    //add your own code here
    k1 = index[0];

    AC0 = (long)lambda * rho[0];
    AC0 += 0x00004000;
    AC0 >>= 15;
    AC1 = (long)lambda2 * rho[1];
    AC1 += 0x00004000;
    AC1 >>= 15;
    rho[0] = (int)(AC0 + AC1);


    AC0 = (long)(rho[0]) * x[k1]; // 16q15 * 16q11 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11;
    /**
     * according to the matlab file, AC now swings between -3.+ and 3.+
     */
    AC0 = (long)a1 * (AC0 >> 2); // 32q13 * 16q13 = 32q26
    AC0 += 0x00002000;
    AC0 >>= 11;
    AC1 = -(long)a1 * x[k1]; // 32q13 * 32q11 = 32q24
    AC1 += 0x00000100;
    AC1 >>= 9;

    k1 = (k1 + 1) % 3;

    AC1 += ((long)x[k1] << 4); // 32q15 + 32q15

    long rho_sq = (long)(rho[0]) * rho[0];
    rho_sq += 0x00004000;
    rho_sq >>= 15;
    long temp = ((long)x[k1] * (short)rho_sq);
    temp += 0x00000400;
    temp >>= 11;
    AC0 -= temp;

    k1 = (k1 + 1) % 3;

    x[k1] = (short)(((long)y + AC0) >> 4); //v[i]
    //x_debug(i)= x[index];
    e1 = (int)(((long)x[k1] << 4) + (long)AC1);

    k1 = (k1 + 1) % 3;

    AC0 = (long)mu * x[k1]; // 16q15*16q11 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11; //32q15
    AC0 = (long)e1 * (short)(AC0); // 16q15 * 16q15 = 32q30
    AC0 += 0x00004000;
    AC0 >>= 15; //15 + 2
    a1 += (short)((AC0 + 0x2) >> 2);
    k1 = (k1 + 2) % 3;
    //not necessary
    if (a1 < -0x4000)
        a1 = -0x4000;
    if (a1 > 0x4000)
        a1 = 0x4000;
    a[0] = a1;
    index[0] = k1;
    
    //cascade part starts here
    //e is now our y
    k2 = index[1];

    AC0 = (long)(rho[0]) * x[k2 + 3]; // 16q15 * 16q11 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11;
    /**
     * according to the matlab file, AC now swings between -3.+ and 3.+
     */
    AC0 = (long)a2 * (AC0 >> 2); // 32q13 * 16q13 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11;
    AC1 = -(long)a2 * x[k2 + 3]; // 32q13 * 32q11 = 32q24
    AC1 += 0x00000100;
    AC1 >>= 9;

    k2 = (k2 + 1) % 3;

    AC1 += ((long)x[k2 + 3] << 4); // 32q15 + 32q15

    rho_sq = (long)(rho[0]) * rho[0];
    rho_sq += 0x00004000;
    rho_sq >>= 15;
    temp = ((long)x[k2 + 3] * (short)rho_sq);
    temp += 0x00000400;
    temp >>= 11;
    AC0 -= temp;

    k2 = (k2 + 1) % 3;

    x[k2 + 3] = (short)(((long)e1 + AC0) >> 4); //v[i]
    //x_debug(i)= x[index];
    e2 = (int)(((long)x[k2 + 3] << 4) + (long)AC1);

    k2 = (k2 + 1) % 3;

    AC0 = (long)mu * x[k2 + 3]; // 16q15*16q11 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11; //32q15
    AC0 = (long)e2 * (short)(AC0); // 16q15 * 16q15 = 32q30
    AC0 += 0x00004000;
    AC0 >>= 15; //15 + 2
    a2 += (short)((AC0 + 0x2) >> 2);
    k2 = (k2 + 2) % 3;
    //not necessary
    if (a2 < -0x4000)
        a2 = -0x4000;
    if (a2 > 0x4000)
        a2 = 0x4000;
    a[1] = a2;
    index[1] = k2;

    return e1;
}
