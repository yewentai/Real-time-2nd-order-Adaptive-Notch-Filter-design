#include "anf.h"

#define RHO_FIXED 0xXXXX // Replace XXXX with the fixed 16q16 format value for rho

int anf(short y, short *s, unsigned short *a, int *index)
{
    /**
     * Simplified ANF using a fixed pole radius.
     * @param y The newly captured sample (input signal).
     * @param s Pointer to a buffer holding the last three samples.
     * @param a Pointer to the adaptive filter coefficient.
     * @param index Pointer to the current index in the circular buffer s.
     * @return The output signal after processing.
     */

    int e;         // Error signal
    long AC0, AC1; // Temporary 32-bit accumulator variables

    // Update the buffer index for the circular buffer implementation
    int k = *index; // k is now pointing in fact to m -1

    /**
     * Calculate rho squared to avoid overflow
     * rho_square = RHO_FIXED * RHO_FIXED
     */
    long rho_square = (long)RHO_FIXED * RHO_FIXED;
    rho_square >>= RHO_Q_FORMAT; // Normalize to 32q16

    // Calculate terms for s[m]
    // First term: RHO_FIXED * a[m - 1] * s[m - 1]
    AC0 = (long)RHO_FIXED * a[k];
    AC0 += 0x4000;
    AC0 >>= RHO_Q_FORMAT;
    AC0 *= s[k];
    AC0 >>= A_Q_FORMAT;

    // Second term: RHO_FIXED^2 * s[m - 2]
    AC1 = rho_square * s[(k + 2) % 3];
    AC1 >>= RHO_Q_FORMAT;

    // Update s[m]
    s[k] = (short)(((long)y + AC0 - AC1));

    // Update error e[m]
    e = (short)(s[k] - (long)a[k] * s[(k + 2) % 3] + s[(k + 1) % 3]);
    e <<= 4; // Normalize to 16q15

    // Update a[m]
    AC0 = (long)mu * e;
    AC0 <<= 1;
    AC0 >>= A_Q_FORMAT;
    k = (k + 1) % 3;
    AC0 *= s[k];
    AC0 >>= 9;
    a[k] = (short)((long)a[k] + AC0);

    // Check if |a| < 2
    if (a[k] < -0x07FFF)
        a[k] = -0x7FFF;
    if (a[k] > 0x7FFF)
        a[k] = 0x7FFF;

    // Update the index
    *index = (k + 2) % 3;

    return e; // Return the error signal
}
