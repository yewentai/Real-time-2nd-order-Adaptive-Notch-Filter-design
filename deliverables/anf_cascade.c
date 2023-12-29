#include "anf.h"
#include "anf_cascade.h"

/**
 * Implements a cascaded adaptive notch filter (ANF).
 *
 * @param y The newly captured sample (input signal).
 * @param s Pointer to a buffer holding the last six samples (two sets of three for each cascade stage).
 * @param a Array of two adaptive filter coefficients for each stage of the cascade.
 * @param rho Array containing the current and asymptotic values of rho.
 * @param index Array of two indices, each pointing to the current index in the circular buffer s for each stage.
 *
 * @return The output signal after processing through the first stage of the cascade.
 *
 * This function processes the input signal through two stages of ANF. Each stage adapts its filter
 * coefficient based on the input and updates its part of the circular buffer. The output of the first stage
 * is used as the input to the second stage. The function returns the output of the first stage.
 */
int anf_cascade(int y, int *s, int *a, int *rho, unsigned int *index)
{
    int a1 = a[0], a2 = a[1]; // Filter coefficients for the two stages
    int e1, e2, k1, k2;       // Output signals and indices for circular buffers
    long AC0, AC1;            // Temporary variables for intermediate calculations

    // Processing for the first stage of the cascade
    k1 = index[0];

    // Update rho based on lambda and lambda2 constants
    // This modification affects the response of the filter
    AC0 = (long)lambda * rho[0];
    AC0 += 0x00004000; // Rounding constant for fixed-point arithmetic
    AC0 >>= 15;        // Bit shift for normalization
    AC1 = (long)lambda2 * rho[1];
    AC1 += 0x00004000; // Rounding constant
    AC1 >>= 15;        // Bit shift for normalization
    rho[0] = (int)(AC0 + AC1);

    // Process the input sample y and update the circular buffer
    AC0 = (long)(rho[0]) * s[k1]; // Perform fixed-point multiplication
    AC0 += 0x00000400;            // Rounding constant
    AC0 >>= 11;                   // Bit shift for normalization
    AC0 = (long)a1 * (AC0 >> 2);  // Apply filter coefficient a1
    AC0 += 0x00002000;            // Rounding constant
    AC0 >>= 11;                   // Bit shift for normalization
    AC1 = -(long)a1 * s[k1];      // Negate and multiply by filter coefficient
    AC1 += 0x00000100;            // Rounding constant
    AC1 >>= 9;                    // Bit shift for normalization

    // Update circular buffer index for first stage
    k1 = (k1 + 1) % 3;

    // Additional processing steps using updated buffer value
    AC1 += ((long)s[k1] << 4);                 // Left shift to adjust fixed-point precision
    long rho_sq = (long)(rho[0]) * rho[0];     // Square rho value
    rho_sq += 0x00004000;                      // Rounding constant
    rho_sq >>= 15;                             // Bit shift for normalization
    long temp = ((long)s[k1] * (short)rho_sq); // Multiply buffer value by squared rho
    temp += 0x00000400;                        // Rounding constant
    temp >>= 11;                               // Bit shift for normalization
    AC0 -= temp;                               // Subtract to obtain new buffer value

    // Update the circular buffer with new value
    k1 = (k1 + 1) % 3;
    s[k1] = (short)(((long)y + AC0) >> 4); // Update buffer with new processed value

    // Calculate the output of the first stage
    e1 = (int)(((long)s[k1] << 4) + (long)AC1); // Combine buffer value with AC1

    // Continue processing for updating the filter coefficient
    k1 = (k1 + 1) % 3;
    AC0 = (long)mu * s[k1];        // Apply mu constant
    AC0 += 0x00000400;             // Rounding constant
    AC0 >>= 11;                    // Bit shift for normalization
    AC0 = (long)e1 * (short)(AC0); // Multiply by output signal
    AC0 += 0x00004000;             // Rounding constant
    AC0 >>= 15;                    // Bit shift for normalization
    // Update the adaptive filter coefficient for the first stage
    a1 += (short)((AC0 + 0x2) >> 2); // Adjust the filter coefficient
    // Ensure the coefficient stays within a specific range to avoid overflow
    if (a1 < -0x4000)
        a1 = -0x4000;
    if (a1 > 0x4000)
        a1 = 0x4000;
    a[0] = a1;     // Store the updated coefficient
    index[0] = k1; // Update the buffer index for the first stage

    // Begin processing for the second stage of the cascade
    // Use the output of the first stage (e1) as the input to the second stage
    k2 = index[1];

    // Process the signal similar to the first stage, but using second stage parameters
    // This involves similar steps of multiplication, rounding, and normalization
    AC0 = (long)(rho[0]) * s[k2 + 3]; // 16q15 * 16q11 = 32q26
    AC0 += 0x00000400;
    AC0 >>= 11;
    AC0 = (long)a2 * (AC0 >> 2); // Apply second filter coefficient a2
    AC0 += 0x00000400;
    AC0 >>= 11;
    AC1 = -(long)a2 * s[k2 + 3]; // Negate and apply a2
    AC1 += 0x00000100;
    AC1 >>= 9;

    // Update the circular buffer index for the second stage
    k2 = (k2 + 1) % 3;

    // Additional processing for the second stage
    AC1 += ((long)s[k2 + 3] << 4);    // Adjust fixed-point precision
    rho_sq = (long)(rho[0]) * rho[0]; // Square rho value for the second stage
    rho_sq += 0x00004000;
    rho_sq >>= 15;
    temp = ((long)s[k2 + 3] * (short)rho_sq); // Multiply buffer value by squared rho
    temp += 0x00000400;
    temp >>= 11;
    AC0 -= temp; // Obtain new buffer value for the second stage

    // Update the circular buffer with the new value for the second stage
    k2 = (k2 + 1) % 3;
    s[k2 + 3] = (short)(((long)e1 + AC0) >> 4); // Update buffer with the processed value

    // Calculate the output of the second stage
    e2 = (int)(((long)s[k2 + 3] << 4) + (long)AC1);

    // Update the filter coefficient for the second stage
    k2 = (k2 + 1) % 3;
    AC0 = (long)mu * s[k2 + 3];
    AC0 += 0x00000400;
    AC0 >>= 11;
    AC0 = (long)e2 * (short)(AC0);
    AC0 += 0x00004000;
    AC0 >>= 15;
    a2 += (short)((AC0 + 0x2) >> 2);

    // Ensure the coefficient stays within a specified range
    if (a2 < -0x4000)
        a2 = -0x4000;
    if (a2 > 0x4000)
        a2 = 0x4000;
    a[1] = a2;     // Store the updated coefficient
    index[1] = k2; // Update the buffer index for the second stage

    // Return the output of the first stage
    return e1;
}
