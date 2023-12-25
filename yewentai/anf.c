#include "anf.h"

/**
 * Implements an adaptive notch filter (ANF) for signal processing.
 *
 * @param y The newly captured sample (input signal).
 * @param x Pointer to a buffer holding the last three samples (circular buffer).
 * @param a Pointer to the adaptive filter coefficient.
 * @param rho Array containing the current and asymptotic values of rho.
 * @param index Pointer to the current index in the circular buffer x.
 *
 * @return The filtered output signal.
 *
 * The function processes the input signal using a recursive filtering algorithm.
 * It adapts the filter coefficient based on the input and updates the circular buffer.
 * The algorithm involves fixed-point arithmetic with careful attention to scaling
 * to prevent overflow and preserve precision.
 */
int anf(int y, int *x, int *a, int *rho, unsigned int* index)
{
    int a_i = *a;
    int e, k; // e: Filter output, k: Index for circular buffer
    long AC0, AC1; // AC0, AC1: Temporary variables for intermediate calculations

    // Processing starts here
    k = *index;

    // Modify rho
    // rho[0] is rho(t), rho[1] = rho(inf)
    AC0 = (long)lambda * rho[0];
    AC0 += 0x00004000; // Rounding
    AC0 >>= 15; // Right shift for normalization
    AC1 = (long)lambda2 * rho[1];
    AC1 += 0x00004000; // Rounding
    AC1 >>= 15; // Right shift for normalization
    rho[0] = (int)(AC0 + AC1);


    // Calculate the first intermediate value using the current rho and input sample x
    AC0 = (long)(*rho) * x[k]; // Multiply rho(t) with the current sample in x
    AC0 += 0x00000400; // Add rounding offset
    AC0 >>= 11; // Normalize the result to maintain fixed-point precision

    // Update AC0 based on filter coefficient a_i and the modified sample value
    AC0 = (long)a_i * (AC0 >> 2); // Multiply by adaptive filter coefficient
    AC0 += 0x00000400; // Add rounding offset
    AC0 >>= 11; // Normalize the result

    // Calculate AC1, another intermediate value for the filtering process
    AC1 = -(long)a_i * x[k]; // Multiply by negated filter coefficient
    AC1 += 0x00000100; // Add rounding offset
    AC1 >>= 9; // Normalize the result

    // Update index k for circular buffer manipulation
    k = (k + 1) % 3;

    // Perform intermediate calculations involving buffer values and rho
    AC1 += ((long)x[k] << 4); // Sum with next sample in the buffer

    // Calculate square of rho and use it in further processing
    long rho_sq = (long)(*rho) * *rho; // Square rho
    rho_sq += 0x00004000; // Add rounding offset
    rho_sq >>= 15; // Normalize
    long temp = ((long)x[k] * (short)rho_sq); // Multiply buffer sample by rho squared
    temp += 0x00000400; // Add rounding offset
    temp >>= 11; // Normalize
    AC0 -= temp; // Subtract from AC0 for final adjustment

    // Update buffer with the newly processed sample
    k = (k + 1) % 3;
    x[k] = (short)(((long)y + AC0) >> 4); // Update buffer with new value

    // Calculate final output e using buffer and intermediate value AC1
    e = (int)(((long)x[k] << 4) + (long)AC1); // Combine buffer value with AC1 for output

    // Update filter coefficient a_i based on the output e and buffer value
    k = (k + 1) % 3;
    AC0 = (long)mu * x[k]; // Multiply mu with current buffer sample
    AC0 += 0x00000400; // Add rounding offset
    AC0 >>= 11; // Normalize
    AC0 = (long)e * (short)(AC0); // Multiply by output signal
    AC0 += 0x00004000; // Add rounding offset
    AC0 >>= 15; // Normalize
    a_i += (short)((AC0 + 0x2) >> 2); // Update a_i
    k = (k + 2) % 3;

    // Check and limit the range of a_i to prevent overflow
    if (a_i < -0x4000)
        a_i = -0x4000;
    if (a_i > 0x4000)
        a_i = 0x4000;
    *a = a_i;
    
    // Update the index for the next call
    *index = k;

    // Return the filtered output
    return e;
}
