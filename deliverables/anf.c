#include "anf.h"

int anf(short y, short *s, unsigned short *a, unsigned short *rho, int *index)
{
    /**
     * Implements an adaptive notch filter (ANF) using the LMS algorithm.
     * @param y The newly captured sample (input signal).
     * @param s Pointer to a buffer holding the last three samples.
     * @param a Pointer to the adaptive filter coefficient.
     * @param rho Array containing the current and asymptotic values of rho.
     * @param index Pointer to the current index in the circular buffer s.
     * @return The output signal after processing.
     * This function processes the input signal through an ANF. The filter coefficient adapts based on the input
     * and updates its part of the circular buffer. The function returns the output of the filter.
     */

    int e;         // Error signal
    long AC0, AC1; // Temporay 32 bit accumulator variables to avoid overflow
    // Update the buffer index for the circular buffer implementation
    int k = *index; // k is now pointing in fact to m -1

    /** ANF-LMS Algorithm implementation */
    /**
     * Step 1: Update rho(m) = lambda * rho(m - 1) + one_minus_lambda * rho(inf)
     * rho[0] is rho(m-1), rho[1] = rho(+infinite)
     * compute the latest rho[m] using rho[0] since rho[m-1] is no longer needed
     */

    AC0 = (long)lambda * rho[0]; /* multiplication with long to reserve enough bits avoid overflow => 16q15 * 16q16 = 32q31 */
    /** no need adding 0x00004000 since it's desired to maintain all the fractional bits without rounding, 
     * as we're representing a number in the range 0 to 1 with no integer part.*/
    // AC0 += 0x00004000;  /** Round the part we'll be truncating (Carry bit of this sum will be the LSB after conversion)*/
    AC0 >>= L_Q_FORMAT;                    /** Shift q-foramt bits right to normalize fix-point => Now AC0 is 32q16 */
    AC1 = (long)one_minus_lambda * rho[1]; /* multiplication with long to reserve enough bits avoid overflow => 16q15 * 16q16 = 32q31 */
    // AC1 += 0x00004000;  /** Round the part we'll be truncating (Carry bit of this sum will be the LSB after conversion)*/
    AC1 >>= L_Q_FORMAT;                     /** Shift q-format bits right to fix-point => Now AC1 is 32q16 */
    rho[0] = (unsigned short)((AC0 + AC1)); /** update rho[m] => Convert back to 16q16*/

    /**
     * Step 2: Update s(m) = y(m) + rho(m)a(m - 1)s(m - 1) - rho(m)^2 s(m - 2)
     * compute rho[0] square
     * compute terms for s[m]*/
    // Calculate rho squared avoiding overflow
    long rho_square = (long)rho[0] * rho[0]; // 16q16 * 16q16 = 32q32
    rho_square >>= RHO_Q_FORMAT;             // Now rho_square is 32q16

    // Calculate the first term of s[m]: rho(m) * a(m - 1) * s(m - 1) with k = (m - 1)
    AC0 = (long)rho[0] * a[k]; // 16q16 * 16q15 = 32q31
    AC0 += 0x4000;             // // Add half (since we are shifting 15 bits) to round
    AC0 >>= RHO_Q_FORMAT;      // Now AC0 is 32q15
    AC0 *= s[k];               // 32q15 * 16q11 (assuming s is in q15 format) = 32q26
    AC0 >>= A_Q_FORMAT;        // Normalize to 32q11

    // Calculate the a(m - 1)s(m - 1) term in e computation at current instance
    long AC2 = -(long)a[k] * s[k]; // 16q15 * 16q11 = 32q26
    AC2 += 0x4000;
    AC2 >>= A_Q_FORMAT; // Normalize to 32q11

    k = (k + 1) % 3; // circulat buffer => k++ means one sample stored earlier k++ => (m - 2)

    // Calculate the second term of s[m]: rho(m)^2 * s(m - 2)
    AC1 = rho_square * s[k]; // 32q16 * 16q11 = 32q27
    AC1 >>= RHO_Q_FORMAT;    // Normalize to 32q11

    // Integrate the s(m-2) to the -(long)a(m-1) * s(m-1)
    e = (short)((long)s[k] + AC2); // 16q11 + 16q11

    k = (k + 1) % 3; // circulat buffer => k++ means one sample earlier k++ => m - 3 => m

    // Combine terms and update s[m], ensuring all values are in 16q15 format before addition
    s[k] = (short)(((long)y + AC0 - AC1)); // Convert y to 32q11, add/subtract, convert result back to 16q15

    /**
     * Step 3: Update error e(m) = s(m) - a(m - 1)s(m - 1) + s(m - 2)
     * compute terms for s[m]
     * */
    e += s[k];
    e <<= 4; // shift left 4 bits to normalize to 16q15

    /**
     * Step 4: Update a(m) = a(m - 1) + 2µe(m)s(m - 1)
     * compute terms for a[m]
     * */
    AC0 = (long)mu * e; // 16q15 * 16q15 = 32q30
    AC0 <<= 1;          // 2 * µ * e(m)
    AC0 >>= A_Q_FORMAT; // 32q15
    // Aadvance k to get to previous sample (from m to m -1 )
    k = (k + 1) % 3;
    AC0 = ((long)AC0 * s[k]);         // 32q15 * 16q11 = 32q26
    AC0 >>= 9;                        // 32q15
    a[k] = (short)((long)a[k] + AC0); // 16q15 + 32q15 = 16q15

    // Advance k back to m
    k = (k + 2) % 3;

    // necessary check to see if |a| < 2 (16q15)
    if (*a < -0x07FFF)
        *a = -0x7FFF;
    if (*a > 0x7FFF)
        *a = 0x7FFF;

    *index = k; // Update the circular buffer index

    return e; // Return the error signal
}