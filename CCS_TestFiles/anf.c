#include "anf.h"

int anf(short y, short *s, short *a, unsigned short* rho, unsigned int* index)
{
    /*
     y in Q : newly captured sample
     s in Q : x[3] databuffer - Hint: Reserve a sufficiently number of integer bits such that summing intermediate values does not cause overflow (so no shift is needed after summing numbers)
     a in  : the adaptive coefficient
     e in Q : output signal
     rho in Q : variable {rho, rho_inf} pole radius
     index : points to (t-1) sample (t current time index) in s -> circular buffer
     */

    short e; // Error signal
    long AC0, AC1; // Temporay 32 bit accumulator variables to avoid overflow
    // Update the buffer index for the circular buffer implementation
    short k = *index;

    /** ANF-LMS Algorithm implementation */ 
    /** 
     * Step 1: Update rho(m) = lambda * rho(m - 1) + one_minus_lambda * rho(inf)
     * modify rho section
     * rho[0] is rho(m-1), rho[1] = rho(+infinite)
     * compute the latest rho[m] using rho[0] since rho[m-1] is no longer needed
     */
    
    AC0 = (long)lambda * rho[0]; /* multiplication with long to reserve enough bits avoid overflow => 16q15 * 16q16 = 32q31 */
    /** no need adding 0x00004000 since it's desired to maintain all the fractional bits without rounding, as we're representing a number in the range 0 to 1 with no integer part.*/
    // AC0 += 0x00004000;  /** Round the part we'll be truncating (Carry bit of this sum will be the LSB after conversion)*/
    AC0 >>= RHO_Q_FORMAT; /** Shift q-foramt bits right to normalize fix-point => Now AC0 is 32q15 */
    AC1 = (long)one_minus_lambda * rho[1]; /* multiplication with long to reserve enough bits avoid overflow => 16q15 * 16q16 = 32q31 */
    // AC1 += 0x00004000;  /** Round the part we'll be truncating (Carry bit of this sum will be the LSB after conversion)*/
    AC1 >>= RHO_Q_FORMAT; /** Shift q-format bits right to fix-point => Now AC1 is 32q15 */
    rho[0] = (unsigned short) ((AC0 + AC1) >> 1); /** update rho[m] => Convert back to 16q16*/
 
    /** 
     * Step 2: Update s(m) = y(m) + rho(m)a(m - 1)s(m - 1) - rho(m)^2 s(m - 2)
     * modify s section 
     * compute rho[0] square
     * compute terms for s[m]*/ 
    // Calculate rho squared in 32q16 format to avoid overflow
    long rho_square = (long)rho[0] * rho[0]; // 16q16 * 16q16 = 32q32
    rho_square >>= RHO_Q_FORMAT; // Now rho_square is 32q16

    // Calculate the first term of s[m]: rho(m) * a(m - 1) * s(m - 1)
    AC0 = (long)rho[0] * a[k-1]; // 16q16 * 16q15 = 32q31
    AC0 += 0x4000; // // Add half (since we are shifting 15 bits) to round
    AC0 >>= A_Q_FORMAT; // Now AC0 is 32q16 to match with rho_square
    AC0 *= s[k-1]; // 32q16 * 16q15 (assuming s is in q15 format) = 32q31
    AC0 += 0x4000; // Add half to round
    AC0 >>= A_Q_FORMAT; // Normalize to 32q16

    // Calculate the second term of s[m]: rho(m)^2 * s(m - 2)
    AC1 = rho_square * s[k-2]; // 32q16 * 16q15 = 48q31
    AC1 += 0x4000; // Add half to round
    AC1 >>= A_Q_FORMAT; // Normalize to 32q16

    // Combine terms and update s[m], ensuring all values are in 16q15 format before addition
    s[k] = (short)(((long)y << 1) + AC0 - AC1); // Convert y to 32q16, add/subtract, convert result back to 16q15

    /** 
     * Step 3: Update error e(m) = s(m) - a(m - 1)s(m - 1) + s(m - 2)
     * modify e section 
     * compute terms for s[m]
     * */ 
    e = (short)(((long)s[k] - (a[k-1] * s[k-1]) >> A_Q_FORMAT) - s[k-2]); // Use 32-bit math for subtraction

    // Step 3: Update a[m]
    // Assuming mu is in 16q16 format
    AC0 = (long)mu * e; // 16q16 * 16q15 = 32q31
    AC0 = (AC0 * s[k-1]) >> A_Q_FORMAT; // 32q31 * 16q15 = 32q46, then normalize to 32q31
    a[k] = (short)(a[k-1] + (AC0 >> A_Q_FORMAT)); // Convert result back to 16q15
    
    // necessary check to see if |a| < 2
    if (a < -0x4000)
        a = -0x4000;
    if (a > 0x4000)
        a= 0x4000;

    *index = (k + 1) % 3; // Update the circular buffer index

    return e; // Return the error signal
}